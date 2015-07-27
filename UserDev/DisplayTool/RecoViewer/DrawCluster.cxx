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
    clusterStartByPlaneByCluster 
      = new std::vector<std::vector<std::vector<float> > > ;
//      = new std::vector<std::vector<::cluster::cluster_params > >;

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
      clusterStartByPlaneByCluster -> resize(geoService -> Nviews());
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
//      clusterStartByPlaneByCluster ->at(p).clear();

      
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

    int plane0Clus = 0;
    int plane1Clus = 0;
    int plane2Clus = 0; 

    for( auto const c: * ev_clus ){
	if ( c.Plane().Plane == 0 )
	    plane0Clus++;
	else if ( c.Plane().Plane == 1)
	    plane1Clus++;
	else if ( c.Plane().Plane == 2 )
	    plane2Clus++ ;

	}

	std::cout<<"\n\n\nPLANE CONTAINS: "<<plane0Clus<<", "<<plane1Clus<<", "<<plane2Clus<<std::endl ;
    
    for (unsigned int p = 0; p < geoService -> Nviews(); p ++){

      wireByPlaneByCluster     ->at(p).reserve(ev_clus->size());
      hitStartByPlaneByCluster ->at(p).reserve(ev_clus->size());
      hitEndByPlaneByCluster   ->at(p).reserve(ev_clus->size());


          if ( p == 0  )
	     clusterStartByPlaneByCluster->at(p).resize(plane0Clus);
    
          if ( p == 1 )
    	     clusterStartByPlaneByCluster->at(p).resize(plane1Clus);
    
          if ( p == 2 )
    	     clusterStartByPlaneByCluster->at(p).resize(plane2Clus);
	}
	    


    auto w2cm = geoUtil->WireToCm();
    auto t2cm = geoUtil->TimeToCm();
//    std::cout<<"\n\n\n\nWIRE TO CM: "<<w2cm<<std::endl;
    
    for (unsigned int p = 0; p < geoService -> Nviews(); p ++){
      std::cout<<"Size of planes piece: "<<clusterStartByPlaneByCluster->at(p).size()<<std::endl;
      for(int i=0; i< clusterStartByPlaneByCluster->at(p).size(); i++){
	    
	clusterStartByPlaneByCluster ->at(p).at(i).reserve(2);
//	std::cout<<"Size of clusters piece: "<<clusterStartByPlaneByCluster->at(p).at(i).size()<<std::endl ;
	    }
	}
    


    ::larlite::event_hit* ev_hit = nullptr;
    auto const& hit_index_v = storage->find_one_ass(ev_clus->id(),ev_hit,producer);


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

    std::vector<larutil::PxHit> pxhits;
    pxhits.clear();

    cluster::ClusterParamsAlg cpan ;
 
//  auto totalClus = clusterStartByPlaneByCluster->at(0).size() 
//		 + clusterStartByPlaneByCluster->at(1).size() 
//		 + clusterStartByPlaneByCluster->at(2).size() ;
//

 std::cout<<"Get here"<<std::endl ;
  for (unsigned int p = 0; p < geoService -> Nviews(); p ++){
    int j =0;
    std::cout<<"Size of clusters in this plane: "<<clusterStartByPlaneByCluster->at(p).size()<<std::endl ;
    for( int i=0; i< ev_clus->size(); i++){
//    for ( auto const & c : *ev_clus ) {
	
//	std::cout<<"Size of clusters in this plane: "<<clusterStartByPlaneByCluster->at(p).size()<<std::endl ;

	auto hit_index = hit_index_v[i];
	::cluster::ClusterParamsAlg bestclus_CPAN;
	_cru_helper.GenerateCPAN( hit_index, ev_hit, cpan);
//
////    cpan.SetHits(pxhits);
    cpan.DisableFANN();
    cpan.SetVerbose(false);
    cpan.FillParams(true,true,true,true,true,true);
//    std::cout<<"CPAN Plane: "<<cpan.Plane()<<std::endl ;
    if(cpan.Plane () == p){
	clusterStartByPlaneByCluster->at(p).at(j).push_back(int (cpan.GetParams().start_point.w / w2cm));
	clusterStartByPlaneByCluster->at(p).at(j).push_back(int (cpan.GetParams().start_point.t / t2cm));
	clusterStartByPlaneByCluster->at(p).at(j).push_back(int (cpan.GetParams().end_point.w / w2cm));
	clusterStartByPlaneByCluster->at(p).at(j).push_back(int (cpan.GetParams().end_point.t / t2cm));
	j++ ;
	}
  //  clusterStartByPlaneByCluster->at(p).at(c.ID()-1).push_back(c.StartWire());
  //  clusterStartByPlaneByCluster->at(p).at(c.ID()-1).push_back(int(c.StartTick()));

//    std::cout<<"Start time and wire: "<<c.StartWire()<<", "<<c.StartTick()<<std::endl ;
//    std::cout<<"Size of clusters piece: "<<clusterStartByPlaneByCluster->at(p).at(i).size()<<std::endl ;
//    std::cout<<"Are we seeing things? "<<cpan.GetParams().start_point.w<<std::endl;
	}
}

    
    for(auto const& hit_indices : hit_index_v) {
      view = ev_hit->at(hit_indices[0]).View();

      for(auto const& hit_index : hit_indices){

        // if (view == 0){
        //   std::cout << "Got a hit, seems to be view " << view
        //             << " and cluster " << cluster_index[view] 
        //             << " at " << ev_hit->at(hit_index).WireID().Wire
        //             << ", " << ev_hit->at(hit_index).PeakTime()
        //             << std::endl;
        // }

//	pxhits.reserve(hit_index_v.size()); 
//
//    	auto const& hit = ev_hit->at(hit_index);
//
//    	::larutil::PxHit h;
//
//    	h.t = hit.PeakTime()    * geoUtil->TimeToCm();
//    	h.w = hit.WireID().Wire * geoUtil->WireToCm();
//
//    	h.charge = hit.Integral();
//    	h.peak   = hit.PeakAmplitude();
//    	h.plane  = view;
//
//    	pxhits.push_back(h);


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

	std::cout<<"Number of clus and p "<<p<<", "<< clusterStartByPlaneByCluster->at(p).size() <<std::endl  ;
	return clusterStartByPlaneByCluster->at(p).size() ;
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

  const std::vector<float> & DrawCluster::getParamsByPlane(unsigned int p, unsigned int c) const{	 
      static std::vector<float> returnNull; 
        if (clusterStartByPlaneByCluster !=0){
        //  if (c >= clusterStartByPlaneByCluster->at(p).size()){
        //    std::cerr << "ERROR: Request for nonexistent cluster " << c << std::endl;
        //    return returnNull;
        //  }
//	  std::cout<<"WHAT WE GOT HERE????? "<<clusterStartByPlaneByCluster->at(p).size()<<std::endl ;
          return clusterStartByPlaneByCluster->at(p).at(c);
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
