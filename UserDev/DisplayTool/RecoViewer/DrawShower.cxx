#ifndef EVD_DRAWSHOWER_CXX
#define EVD_DRAWSHOWER_CXX

#include "DrawShower.h"
#include "DataFormat/shower.h"
#include "LArUtil/DetectorProperties.h"

namespace evd {


DrawShower::DrawShower()
  : RecoBase<Shower2D>()
{
  _name = "DrawShower";
  _fout = 0;
  // showerVectorByPlane = new std::vector<std::vector<Shower2D> >;
}

bool DrawShower::initialize() {

  //
  // This function is called in the beggining of event loop
  // Do all variable initialization you wish to do here.
  // If you have a histogram to fill in the event loop, for example,
  // here is a good place to create one on the heap (i.e. "new TH1D").
  //


  // // Resize data holder to accommodate planes and wires:
  if (_dataByPlane.size() != geoService -> Nviews()) {
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
  for (unsigned int p = 0; p < geoService -> Nviews(); p ++) {
    _dataByPlane.at(p).clear();
    _dataByPlane.at(p).reserve(showerHandle -> size());
    _wireRange.at(p).first  = 99999;
    _timeRange.at(p).first  = 99999;
    _timeRange.at(p).second = -1.0;
    _wireRange.at(p).second = -1.0;
  }


  // Populate the shower vector:
  for (auto & shower : *showerHandle) {
    for (unsigned int view = 0; view < geoService -> Nviews(); view++) {
      _dataByPlane.at(view).push_back(getShower2d(shower, view));
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



Shower2D DrawShower::getShower2d(larlite::shower shower, unsigned int plane) {

  auto detProp = larutil::DetectorProperties::GetME();


  Shower2D result;
  result._is_good = false;
  result._plane = plane;
  // Fill out the parameters of the 2d shower
  result._startPoint
    = geoHelper -> Point_3Dto2D(shower.ShowerStart(), plane);

  // std::cout << "3D Start point is (" << shower.ShowerStart().X()
  //           << ", " << shower.ShowerStart().Y()
  //           << ", " << shower.ShowerStart().Z() << ")\n";
  // std::cout << "3D Direction is (" << shower.Direction().X()
  //           << ", " << shower.Direction().Y()
  //           << ", " << shower.Direction().Z() << ")\n";
  // std::cout << "2D Start point is (" << result._startPoint.w
  //           << ", " << result._startPoint.t << ")\n";

  // std::cout << "dE/dx is: \n"
  //           << "\tPlane 0: " << shower.dEdx_v().at(0)
  //           << "\tPlane 1: " << shower.dEdx_v().at(1)
  //           << std::endl;

  // Next get the direction:
  result._angleInPlane = geoHelper->Slope_3Dto2D(shower.Direction(), plane);

  // Get the opening Angle:
  result._openingAngle = shower.OpeningAngle();


  auto secondPoint = shower.ShowerStart() + shower.Length() * shower.Direction();


  result._endPoint
    = geoHelper -> Point_3Dto2D(secondPoint, plane);

  result._length = sqrt(pow(result.startPoint().w - result.endPoint().w, 2) +
                        pow(result.startPoint().t - result.endPoint().t, 2));

  result._dedx = shower.dEdx();

  result._energy = shower.Energy();

  result._is_good = true;
  return result;
}



} // larlite

#endif
