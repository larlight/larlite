#ifndef DRAWSHOWER_CXX
#define DRAWSHOWER_CXX

#include "DrawShower.h"
#include "DataFormat/shower.h"
#include "LArUtil/DetectorProperties.h"

namespace evd {


DrawShower::DrawShower()
  : RecoBase<Shower2d>()
{
  _name = "DrawShower";
  _fout = 0;
  // showerVectorByPlane = new std::vector<std::vector<Shower2d> >;
}

bool DrawShower::initialize() {

  //
  // This function is called in the beggining of event loop
  // Do all variable initialization you wish to do here.
  // If you have a histogram to fill in the event loop, for example,
  // here is a good place to create one on the heap (i.e. "new TH1D").
  //


  // // Resize data holder to accommodate planes and wires:
  if (_dataByPlane.size() != geoService -> Nviews()){
    // std::cout << "resizing vectors to: " << geoService -> Nviews() << std::endl;
    _dataByPlane.resize(geoService -> Nviews());
  }
  return true;

}

bool DrawShower::analyze(larlite::storage_manager* storage) {

  //
  // Do your event-by-event analysis here. This function is called for
  // each event in the loop. You have "storage" pointer which contains
  // event-wise data. To see what is available, check the "Manual.pdf":
  //
  // http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
  //
  // Or you can refer to Base/DataFormatConstants.hh for available data type
  // enum values. Here is one example of getting PMT waveform collection.
  //
  // event_fifo* my_pmtfifo_v = (event_fifo*)(storage->get_data(DATA::PMFIFO));
  //
  // if( event_fifo )
  //
  //   std::cout << "Event ID: " << my_pmtfifo_v->event_id() << std::endl;
  //

  // get a handle to the showers
  auto showerHandle = storage->get_data<larlite::event_shower>(_producer);

  // Clear out the hit data but reserve some space for the showers
  for (unsigned int p = 0; p < geoService -> Nviews(); p ++){
    _dataByPlane.at(p).clear();
    _dataByPlane.at(p).reserve(showerHandle -> size());
  }


  // Populate the shower vector:
  for (auto & shower: *showerHandle){
    for (unsigned int view = 0; view < geoService -> Nviews(); view++){
      _dataByPlane.at(view).push_back(getShower2d(shower,view));
    }
  }


  return true;
}

bool DrawShower::finalize() {

  // This function is called at the end of event loop.
  // Do all variable finalization you wish to do here.
  // If you need, you can store your ROOT class instance in the output
  // file. You have an access to the output file through "_fout" pointer.
  //
  // Say you made a histogram pointer h1 to store. You can do this:
  //
  // if(_fout) { _fout->cd(); h1->Write(); }
  //
  // else
  //   print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
  //
  return true;
}



Shower2d DrawShower::getShower2d(larlite::shower shower, unsigned int plane){

  auto detProp = larutil::DetectorProperties::GetME();


  Shower2d result;
  result._is_good = false;
  result._plane = plane;
  // Fill out the parameters of the 2d shower
  float _wire = 0;
  try {
    _wire = geoService->NearestWire(shower.ShowerStart(), plane);
  }
  catch (...) {
    // std::cerr << "Caught exception trying to find nearest shower.  There is a junk shower.\n";
    return result;
  }

  float _time = shower.ShowerStart().X();
  _time += detProp -> TriggerOffset() * geoHelper -> TimeToCm();
  if (plane == 0) {
    _time += detProp -> TimeOffsetU() * geoHelper -> TimeToCm();
  }
  if (plane == 1) {
    _time += detProp -> TimeOffsetV() * geoHelper -> TimeToCm();
  }



  // Convert wire to cm:
  _wire *= geoHelper->WireToCm();
  result._startPoint = larutil::Point2D();
  result._startPoint.w = _wire;
  result._startPoint.t = _time;
  // std::cout << "3D start point: (" << shower.ShowerStart().X()
  //           << ", " << shower.ShowerStart().Y()
  //           << ", " << shower.ShowerStart().Z()
  //           << ") " << std::endl;
  // std::cout << "Start point in plane " << plane << " (" << _wire << ", " << _time << ")\n";

  // Next get the direction:
  result._angleInPlane = geoHelper->Slope_3Dto2D(shower.Direction(),plane);

  // Get the opening Angle:
  result._openingAngle = shower.OpeningAngle();

  // Get the length:
  float length = shower.Length();
  auto secondPoint = shower.ShowerStart() + length * shower.Direction();

  // project the second point into the plane:

  float _second_wire = 0;
  try {
    _second_wire = geoHelper->WireToCm() * geoService->NearestWire(secondPoint, plane);
  }
  catch (...) {
    // std::cerr << "another exception" << '\n';
    return result;
  }
  float _second_time = geoHelper->TimeToCm() * secondPoint.X();
  _second_time += detProp -> TriggerOffset() * geoHelper -> TimeToCm();
  if (plane == 0) {
    _second_time += detProp -> TimeOffsetU() * geoHelper -> TimeToCm();
  }
  if (plane == 1) {
    _second_time += detProp -> TimeOffsetV() * geoHelper -> TimeToCm();
  }
  result._length = sqrt(pow(_wire - _second_wire, 2) + pow(_time - _second_time, 2));

  result._dedx = shower.dEdx();

  result._is_good = true;
  return result;
}



} // larlite

#endif
