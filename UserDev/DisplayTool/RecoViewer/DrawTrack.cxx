#ifndef DRAWTRACK_CXX
#define DRAWTRACK_CXX

#include "DrawTrack.h"
#include "DataFormat/track.h"
#include "LArUtil/DetectorProperties.h"

namespace evd {


  DrawTrack::DrawTrack(){
    _name="DrawTrack";
    _fout=0;
    // std::cout << "Constructing!" << std::endl;
    trackVectorByPlane = new std::vector<std::vector<track2d> >;

  }

  bool DrawTrack::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //

    // Initialize the geoService object:
    geoService = larutil::Geometry::GetME();
    geoUtils = larutil::GeometryUtilities::GetME();

    // Resize data holder to accomodate planes and wires:
    if (trackVectorByPlane -> size() != geoService -> Nviews()){
      trackVectorByPlane     -> resize(geoService -> Nviews());
    }
    return true;

  }
  
  bool DrawTrack::analyze(larlite::storage_manager* storage) {
  
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

    // Initialize the geoService object:
    if (!geoService)
      geoService = larutil::Geometry::GetME();



    // Resize data holder to accomodate planes and wires:
    if (trackVectorByPlane->size() == 0){
      trackVectorByPlane     -> resize(geoService -> Nviews());
    }


    // get a handle to the showers
    auto trackHandle = storage->get_data<larlite::event_track>(producer);

    // Clear out the hit data but reserve some space for the showers
    for (unsigned int p = 0; p < geoService -> Nviews(); p ++){
      trackVectorByPlane     ->at(p).clear();

      trackVectorByPlane     ->at(p).reserve(trackHandle -> size());

    }
    

    // Populate the shower vector:
    for (auto & shower: *trackHandle){
      for (unsigned int view = 0; view < geoService -> Nviews(); view++){
        trackVectorByPlane->at(view).push_back(getTrack2d(shower,view));
      }
    }


    return true;
  }

  bool DrawTrack::finalize() {

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

  DrawTrack::~DrawTrack(){
    delete trackVectorByPlane;    

  }
  
  const std::vector< track2d > & DrawTrack::getTracksByPlane(unsigned int p) const{
    static std::vector<track2d> returnNull;
    if (p >= geoService->Nviews() ){
      std::cerr << "ERROR: Request for nonexistant plane " << p << std::endl;
      return returnNull;
    }
    else{
      if (trackVectorByPlane !=0){
        return trackVectorByPlane->at(p);
      }
      else{
        return returnNull;
      }
    }
  }

  track2d DrawTrack::getTrack2d(larlite::track track, unsigned int plane){
    track2d result;
    auto detProp = larutil::DetectorProperties::GetME();
    for (unsigned int i = 0; i < track.NumberTrajectoryPoints(); i++){
      // project a point into 2D:
      try {
        float _wire = geoService->NearestWire(track.LocationAtPoint(i), plane);
        float _time = track.LocationAtPoint(i).X() / geoUtils -> TimeToCm();
        _time += detProp -> TriggerOffset();
        if (plane == 0){
          _time += detProp -> TimeOffsetU();
        }
        if (plane == 1){
          _time += detProp -> TimeOffsetV();
        }
        result.push_back(std::make_pair(_wire,_time));   
      }
      catch(...) {
        continue;
      }

    }

    return result;

    // result._plane = plane;
    // // Fill out the parameters of the 2d shower
    // float _wire = 0;
    // try{
    //   _wire = geoService->NearestWire(shower.ShowerStart(), plane);
    // }
    // catch(...){
    //   std::cerr << "Caught exception trying to find nearest shower.  There is a junk shower.\n";
    //   return result;
    // }

    // float _time = shower.ShowerStart().X();
    // // Convert wire to cm:
    // _wire *= geoUtils->WireToCm();
    // result._startPoint = TVector2(_wire,_time);
    // // std::cout << "3D start point: (" << shower.ShowerStart().X() 
    // //           << ", " << shower.ShowerStart().Y()  
    // //           << ", " << shower.ShowerStart().Z()  
    // //           << ") " << std::endl;
    // // std::cout << "Start point in plane " << plane << " (" << _wire << ", " << _time << ")\n";

    // // Next get the direction:
    // result._angleInPlane = geoUtils->Get2DangleFrom3D(plane,shower.Direction());

    // // Get the opening Angle:
    // result._openingAngle = shower.OpeningAngle();

    // // Get the length:
    // float length = shower.Length();
    // auto secondPoint = shower.ShowerStart() + length*shower.Direction();

    // // project the second point into the plane:

    // float _second_wire =0; 
    // try {
    //   _second_wire = geoUtils->WireToCm() * geoService->NearestWire(secondPoint,plane);
    // }
    // catch(...) {
    //   std::cerr << "another exception" << '\n';
    // }
    // float _second_time = geoUtils->TimeToCm() * secondPoint.X();

    // result._length = sqrt(pow(_wire - _second_wire, 2) + pow(_time - _second_time, 2));
    return result;
  }


} // larlite

#endif
