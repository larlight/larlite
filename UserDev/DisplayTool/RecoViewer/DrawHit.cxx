#ifndef DRAWHIT_CXX
#define DRAWHIT_CXX

#include "DrawHit.h"
#include "DataFormat/hit.h"

namespace evd {


  DrawHit::DrawHit(){
    _name="DrawCluster";
    _fout=0;
    // std::cout << "Constructing!" << std::endl;
    wireByPlane = new std::vector<std::vector<int>>;
    hitStartByPlane = new std::vector<std::vector<float>>;
    hitEndByPlane = new std::vector<std::vector<float>>;
  }

  bool DrawHit::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //

    // Initialize the geoService object:
    geoService = larutil::Geometry::GetME();


    // Resize data holder to accomodate planes and wires:
    if (wireByPlane -> size() != geoService -> Nviews()){
      // std::cout << "resizing vectors to: " << geoService -> Nviews() << std::endl;
      wireByPlane     -> resize(geoService -> Nviews());
      hitStartByPlane -> resize(geoService -> Nviews());
      hitEndByPlane   -> resize(geoService -> Nviews());
    }
    return true;

  }
  
  bool DrawHit::analyze(larlite::storage_manager* storage) {
  
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
    if (wireByPlane->size() == 0){
      wireByPlane     -> resize(geoService -> Nviews());
      hitStartByPlane -> resize(geoService -> Nviews());
      hitEndByPlane   -> resize(geoService -> Nviews());
    }


    // get a handle to the hits
    auto hitHandle = storage->get_data<larlite::event_hit>(producer);

    // Clear out the hit data but reserve some space for the hits
    for (unsigned int p = 0; p < geoService -> Nviews(); p ++){
      wireByPlane     ->at(p).clear();
      hitStartByPlane ->at(p).clear();
      hitEndByPlane   ->at(p).clear();

      wireByPlane     ->at(p).reserve(hitHandle -> size());
      hitStartByPlane ->at(p).reserve(hitHandle -> size());
      hitEndByPlane   ->at(p).reserve(hitHandle -> size());
    }
    

    
    for (auto & hit: *hitHandle){
        unsigned int view = hit.View();
        wireByPlane -> at(view).push_back(hit.WireID().Wire);
        hitStartByPlane -> at(view).push_back(hit.PeakTime() - hit.RMS());
        hitEndByPlane -> at(view).push_back(hit.PeakTime() + hit.RMS());
    }


    return true;
  }

  bool DrawHit::finalize() {

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

  DrawHit::~DrawHit(){
    delete wireByPlane;    
    delete hitStartByPlane;
    delete hitEndByPlane;  

  }
  
  const std::vector<int> & DrawHit::getWireByPlane(unsigned int p) const{
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

  const std::vector<float> & DrawHit::getHitStartByPlane(unsigned int p) const{
    static std::vector<float> returnNull;
    if (p >= geoService->Nviews() ){
      std::cerr << "ERROR: Request for nonexistant plane " << p << std::endl;
      return returnNull;
    }
    else{
      if (hitStartByPlane !=0){
        return hitStartByPlane->at(p);
      }
      else{
        return returnNull;
      }
    }
  }

  const std::vector<float> & DrawHit::getHitEndByPlane(unsigned int p) const{
    static std::vector<float> returnNull;
    if (p >= geoService->Nviews() ){
      std::cerr << "ERROR: Request for nonexistant plane " << p << std::endl;
      return returnNull;
    }
    else{
      if (hitEndByPlane !=0){
        return hitEndByPlane->at(p);
      }
      else{
        return returnNull;
      }
    }
  }


} // larlite

#endif
