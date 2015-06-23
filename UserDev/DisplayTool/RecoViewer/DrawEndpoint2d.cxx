#ifndef LARLITE_DRAWENDPOINT2D_CXX
#define LARLITE_DRAWENDPOINT2D_CXX

#include "DrawEndpoint2d.h"
#include "DataFormat/endpoint2d.h"
// #include "DataFormat/cluster.h"
#include "LArUtil/DetectorProperties.h"
#include "LArUtil/GeometryUtilities.h"

namespace larlite {

  DrawEndpoint2d::DrawEndpoint2d(){
    _name="DrawEndpoint2d";
    _fout=0;

    wireByPlane = new std::vector<std::vector<int>>;
    timeByPlane = new std::vector<std::vector<float>>;
  }

  bool DrawEndpoint2d::initialize() {

    //
    // This function is called in the beginning of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //

    // Initialize the geoService object:
    geoService = larutil::Geometry::GetME();

    wireRange.resize(geoService -> Nviews());
    timeRange.resize(geoService -> Nviews());

    // Resize data holder to accomodate planes and wires:
    if (wireByPlane -> size() != geoService -> Nviews()){
      // std::cout << "resizing vectors to: " << geoService -> Nviews() << std::endl;
      wireByPlane  -> resize(geoService -> Nviews());
      timeByPlane  -> resize(geoService -> Nviews());
    }

    return true;
  }
  
  bool DrawEndpoint2d::analyze(storage_manager* storage) {
  
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

    // clear the spots that hold the data:
  // Obtain event-wise data object pointers
    //
   
    auto detProp = larutil::DetectorProperties::GetME();
    // auto geoUtil = larutil::GeometryUtilities::GetME();

    // Clear out the hit data but reserve some space for the hits
    for (unsigned int p = 0; p < geoService -> Nviews(); p ++){

      wireByPlane  -> at(p).clear();
      timeByPlane  -> at(p).clear();
      
      wireRange.at(p).resize(2);
      timeRange.at(p).resize(2);

      // Reset the bounding items for this event:
      wireRange.at(p).at(0) = geoService -> Nwires(p);
      wireRange.at(p).at(1) = 0;
      timeRange.at(p).at(0) = detProp -> NumberTimeSamples();
      timeRange.at(p).at(1) = 0;
    
    }

    auto ev_end2d = storage->get_data<event_endpoint2d>(producer);
    if(!ev_end2d)
      return false;
    if(!ev_end2d->size()) {
      print(msg::kWARNING,__FUNCTION__,
      Form("Skipping event %d since no endpoint2d found...",ev_end2d->event_id()));
      return false;
    }
    
    for (unsigned int p = 0; p < geoService -> Nviews(); p ++){
      wireByPlane  -> at(p).reserve(ev_end2d->size());
      timeByPlane  -> at(p).reserve(ev_end2d->size());
    }


    // Loop over the vertices and fill the necessary vectors.  

    // larutil::PxPoint point;
    // double * xyz = new double[3];

    for (auto & endpoint2d : * ev_end2d){
      // for (unsigned int p = 0; p < geoService -> Nviews(); p ++){
        int p = endpoint2d.View();
        wireByPlane -> at(p).push_back(endpoint2d.Wire());
        timeByPlane -> at(p).push_back(endpoint2d.DriftTime());

        // Determine if this hit should change the view range:
        if (endpoint2d.Wire() > wireRange.at(p).at(1))
          wireRange.at(p).at(1) = endpoint2d.Wire();
        if (endpoint2d.Wire() < wireRange.at(p).at(0))
          wireRange.at(p).at(0) = endpoint2d.Wire();
        if (endpoint2d.DriftTime() > timeRange.at(p).at(1))
          timeRange.at(p).at(1) = endpoint2d.DriftTime();
        if (endpoint2d.DriftTime() < timeRange.at(p).at(0))
          timeRange.at(p).at(0) = endpoint2d.DriftTime();
      // }
    }
    

    return true;
  }

  bool DrawEndpoint2d::finalize() {

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
      //
    

    return true;
  }

  DrawEndpoint2d::~DrawEndpoint2d(){
    delete wireByPlane;
    delete timeByPlane;
  }


  const std::vector<int> & DrawEndpoint2d::getWireByPlane(unsigned int p) const{
    static std::vector<int> returnNull;
    if (p >= geoService->Nviews() ){
      std::cerr << "ERROR: Request for nonexistant plane " << p << std::endl;
      return returnNull;
    }
    else{
      if (wireByPlane !=0){
        return wireByPlane->at(p);
      }
      else{
        return returnNull;
      }
    }
  }

  const std::vector<float> & DrawEndpoint2d::getTimeByPlane(unsigned int p) const{
    static std::vector<float> returnNull;
    if (p >= geoService->Nviews() ){
      std::cerr << "ERROR: Request for nonexistant plane " << p << std::endl;
      return returnNull;
    }
    else{
      if (timeByPlane !=0){
        return timeByPlane->at(p);
      }
      else{
        return returnNull;
      }
    }
  }


  std::vector<float> DrawEndpoint2d::GetWireRange(unsigned int p){
    static std::vector<float> returnNull;
    if (p >= geoService->Nviews() ){
      std::cerr << "ERROR: Request for nonexistent plane " << p << std::endl;
      return returnNull;
    }
    else 
      return wireRange.at(p);
  }
  std::vector<float> DrawEndpoint2d::GetTimeRange(unsigned int p){
    static std::vector<float> returnNull;
    if (p >= geoService->Nviews() ){
      std::cerr << "ERROR: Request for nonexistent plane " << p << std::endl;
      return returnNull;
    }
    else 
      return timeRange.at(p);
  }

}
#endif
