#ifndef LARLITE_DRAWCLUSTER_CXX
#define LARLITE_DRAWCLUSTER_CXX

#include "DrawCluster.h"

namespace evd {

  DrawCluster::DrawCluster(){
    _name="DrawCluster";
    _fout=0;



    wireByPlaneByCluster     
      = new std::vector<std::vector<std::vector<int > > >;
    hitStartByPlaneByCluster 
      = new std::vector<std::vector<std::vector<float > > >;
    hitEndByPlaneByCluster   
      = new std::vector<std::vector<std::vector<float > > >;
    clusterParamsByPlane 
      = new std::vector<std::vector<::cluster::cluster_params > > ;

  }

  bool DrawCluster::initialize() {

    //
    // This function is called in the beginning of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //

    // Initialize the geoService object:
    geoService = larutil::Geometry::GetME();
    geoUtil    = larutil::GeometryUtilities::GetME();

    wireRange.resize(geoService -> Nviews());
    timeRange.resize(geoService -> Nviews());

    // Resize data holder to accomodate planes and wires:
    if (wireByPlaneByCluster -> size() != geoService -> Nviews()){
      // std::cout << "resizing vectors to: " << geoService -> Nviews() << std::endl;
      wireByPlaneByCluster     -> resize(geoService -> Nviews());
      hitStartByPlaneByCluster -> resize(geoService -> Nviews());
      hitEndByPlaneByCluster   -> resize(geoService -> Nviews());
      clusterParamsByPlane     -> resize(geoService -> Nviews());
    }

    return true;
  }
  
  bool DrawCluster::analyze(larlite::storage_manager* storage) {
  
    clusters = 0 ;
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
    

    // Clear out the hit data but reserve some space for the hits
    for (unsigned int p = 0; p < geoService -> Nviews(); p ++){

      wireByPlaneByCluster     ->at(p).clear();
      hitStartByPlaneByCluster ->at(p).clear();
      hitEndByPlaneByCluster   ->at(p).clear();
      clusterParamsByPlane ->at(p).clear();

      
      wireRange.at(p).resize(2);
      timeRange.at(p).resize(2);

      // Reset the bounding items for this event:
      wireRange.at(p).at(0) = geoService -> Nwires(p);
      wireRange.at(p).at(1) = 0;
      timeRange.at(p).at(0) = detProp -> NumberTimeSamples();
      timeRange.at(p).at(1) = 0;
    
    }

    auto ev_clus = storage->get_data<larlite::event_cluster>(producer);
    if(!ev_clus)
      return false;
    if(!ev_clus->size()) {
      print(larlite::msg::kWARNING,__FUNCTION__,
      Form("Skipping event %d since no cluster found...",ev_clus->event_id()));
      return false;
    }

    clusters = ev_clus->size() ;

    
    for (unsigned int p = 0; p < geoService -> Nviews(); p ++){

      wireByPlaneByCluster        ->at(p).reserve(ev_clus->size());
      hitStartByPlaneByCluster    ->at(p).reserve(ev_clus->size());
      hitEndByPlaneByCluster      ->at(p).reserve(ev_clus->size());

    }
            

    ::larlite::event_hit* ev_hit = nullptr;
    auto const& hit_index_v = storage->find_one_ass(ev_clus->id(),ev_hit,producer);

    // std::cout << "Hit producer is " << ev_hit -> name() << std::endl;

    if(!ev_hit){
      std::cout << "Did not find hit data product"
                << "!" << std::endl;
      return false;
    }


    if (!hit_index_v.size())
      return false;

    // Loop over the clusters and fill the necessary vectors.  
    // I don't know how clusters are stored so I'm taking a conservative
    // approach to packaging them for drawing
    std::vector<int> cluster_index;
    cluster_index.resize(geoService -> Nviews());

    int view = ev_hit->at(hit_index_v.front()[0]).View();
    std::vector<int>  nullIntVec;
    std::vector<float>  nullFltVec;
    ::cluster::cluster_params  nullCPVec;

    cluster::DefaultParamsAlg params_alg ;
    cluster::cluster_params params;
    params_alg.SetVerbose(false);
    params_alg.SetDebug(false);
    params_alg.SetMinHits(10);
    
    for(auto const& hit_indices : hit_index_v) {
      view = ev_hit->at(hit_indices[0]).View();
      // Fill the cluster params alg
      _cru_helper.GenerateParams( hit_indices, ev_hit, params);
      params_alg.FillParams(params);


      // if ( (int) clusterParamsByPlane->at(view).size() != cluster_index[view] -1){
      //   clusterParamsByPlane->at(view).push_back(nullCPVec);
      // }
      clusterParamsByPlane->at(view).push_back( params );

      for(auto const& hit_index : hit_indices){



        // if (view == 0){
        //   std::cout << "Got a hit, seems to be view " << view
        //             << " and cluster " << cluster_index[view] 
        //             << " at " << ev_hit->at(hit_index).WireID().Wire
        //             << ", " << ev_hit->at(hit_index).PeakTime()
        //             << std::endl;
        // }



        if ((int)wireByPlaneByCluster -> at(view).size() != cluster_index[view]-1){
          wireByPlaneByCluster -> at(view).push_back(nullIntVec);
        }
        wireByPlaneByCluster 
          -> at(view).at(cluster_index[view]).push_back(
            ev_hit->at(hit_index).WireID().Wire);
        
        if ((int)hitStartByPlaneByCluster -> at(view).size() != cluster_index[view]-1){
          hitStartByPlaneByCluster -> at(view).push_back(nullFltVec);
        }
        hitStartByPlaneByCluster 
          -> at(view).at(cluster_index[view]).push_back(ev_hit->at(hit_index).PeakTime() - ev_hit->at(hit_index).RMS());
        
        if ((int)hitEndByPlaneByCluster -> at(view).size() != cluster_index[view]-1){
          hitEndByPlaneByCluster -> at(view).push_back(nullFltVec);
        }
        hitEndByPlaneByCluster 
          -> at(view).at(cluster_index[view]).push_back(ev_hit->at(hit_index).PeakTime() + ev_hit->at(hit_index).RMS());

        // Determine if this hit should change the view range:
        if (ev_hit->at(hit_index).WireID().Wire > wireRange.at(view).at(1))
          wireRange.at(view).at(1) = ev_hit->at(hit_index).WireID().Wire;
        if (ev_hit->at(hit_index).WireID().Wire < wireRange.at(view).at(0))
          wireRange.at(view).at(0) = ev_hit->at(hit_index).WireID().Wire;
        if (ev_hit->at(hit_index).PeakTime() > timeRange.at(view).at(1))
          timeRange.at(view).at(1) = ev_hit->at(hit_index).PeakTime();
        if (ev_hit->at(hit_index).PeakTime() < timeRange.at(view).at(0))
          timeRange.at(view).at(0) = ev_hit->at(hit_index).PeakTime();

      }



      cluster_index[view] ++;

    }
    

    return true;
  }

  bool DrawCluster::finalize() {

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

  DrawCluster::~DrawCluster(){
    delete wireByPlaneByCluster;
    delete hitStartByPlaneByCluster;
    delete hitEndByPlaneByCluster;
  }

  int DrawCluster::getClusters(unsigned int p) const{

        std::cout<<"Number of clus and p "<<p<<", "<< clusterParamsByPlane->at(p).size() <<std::endl  ;
        return clusterParamsByPlane->at(p).size() ;
    }

  int DrawCluster::getNClustersByPlane(unsigned int p) const{
      if (p >= geoService->Nviews() ){
        std::cerr << "ERROR: Request for nonexistent plane " << p << std::endl;
        return 0;
      }
      else if (wireByPlaneByCluster !=0){
        return wireByPlaneByCluster->at(p).size();
      }
      else{
        return 0;
      }
  }

  const std::vector<::cluster::cluster_params> & DrawCluster::getParamsByPlane(unsigned int p) const{         
      static std::vector<::cluster::cluster_params> returnNull; 
        if (clusterParamsByPlane !=0){
        //  if (c >= clusterParamsByPlane->at(p).size()){
        //    std::cerr << "ERROR: Request for nonexistent cluster " << c << std::endl;
        //    return returnNull;
        //  } 

          return clusterParamsByPlane->at(p);
        }
        else{
          return returnNull;
        }
        

    }

  const std::vector<int>   & 
    DrawCluster::getWireByPlaneAndCluster(unsigned int p, unsigned int c) const{
      static std::vector<int> returnNull;
      if (p >= geoService->Nviews() ){
        std::cerr << "ERROR: Request for nonexistent plane " << p << std::endl;
        return returnNull;
      }
      else{
        if (wireByPlaneByCluster !=0){
          if (c >= wireByPlaneByCluster->at(p).size()){
            std::cerr << "ERROR: Request for nonexistent cluster " << c << std::endl;
            return returnNull;
          }
          return wireByPlaneByCluster->at(p).at(c);
        }
        else{
          return returnNull;
        }
      }
    }
  const std::vector<float> & 
    DrawCluster::getHitStartByPlaneAndCluster(unsigned int p, unsigned int c) const{
      static std::vector<float> returnNull;
      if (p >= geoService->Nviews() ){
        std::cerr << "ERROR: Request for nonexistent plane " << p << std::endl;
        return returnNull;
      }
      else{
        if (hitStartByPlaneByCluster !=0){
          if (c >= hitStartByPlaneByCluster->at(p).size()){
            std::cerr << "ERROR: Request for nonexistent cluster " << c << std::endl;
            return returnNull;
          }
          return hitStartByPlaneByCluster->at(p).at(c);
        }
        else{
          return returnNull;
        }
      }
    }
  const std::vector<float> & 
    DrawCluster::getHitEndByPlaneAndCluster(unsigned int p, unsigned int c) const{
      static std::vector<float> returnNull;
      if (p >= geoService->Nviews() ){
        std::cerr << "ERROR: Request for nonexistent plane " << p << std::endl;
        return returnNull;
      }
      else{
        if (hitEndByPlaneByCluster !=0){
          if (c >= hitEndByPlaneByCluster->at(p).size()){
            std::cerr << "ERROR: Request for nonexistent cluster " << c << std::endl;
            return returnNull;
          }
          return hitEndByPlaneByCluster->at(p).at(c);
        }
        else{
          return returnNull;
        }
      }
    }

    std::vector<float> DrawCluster::GetWireRange(unsigned int p){
      static std::vector<float> returnNull;
      if (p >= geoService->Nviews() ){
        std::cerr << "ERROR: Request for nonexistent plane " << p << std::endl;
        return returnNull;
      }
      else 
        return wireRange.at(p);
    }
    std::vector<float> DrawCluster::GetTimeRange(unsigned int p){
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
