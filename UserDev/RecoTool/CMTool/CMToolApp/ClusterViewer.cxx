#ifndef RECOTOOL_CLUSTERVIEWER_CXX
#define RECOTOOL_CLUSTERVIEWER_CXX

#include "ClusterViewer.h"

namespace larlite {

  //################################################################
  ClusterViewer::ClusterViewer() : ana_base(),
				   _algo("ClusterViewer")
  //################################################################
  {
    // Class name
    _name = "ClusterViewer";
    _cluster_producer="fuzzycluster";
    //default is to use hits viewer (need to hadd files first for MC showerviewer)
    SetDrawShowers(false);
    //default is now to use log-z scale for hits viewer
    SetHitsLogZ(true);
    SetDrawStartEnd(false);
  }

  //#######################################
  void ClusterViewer::SetDrawShowers(bool on)
  //#######################################
  {
    _showerColor = on;
    _algo.ShowShowers(_showerColor);
  }


  //#######################################
  void ClusterViewer::SetHitsLogZ(bool flag)
  //#######################################
  {
    _algo.SetHitsLogZ(flag);
  }

  //################################################################
  bool ClusterViewer::initialize()
  //################################################################
  {
    _algo.Reset();
    return true;
  }
  
  //################################################################
  bool ClusterViewer::analyze(storage_manager* storage)
  //################################################################
  {

    _algo.Reset();
    
    const ::larutil::Geometry* geo = ::larutil::Geometry::GetME();
    const ::larutil::GeometryUtilities* geo_util = ::larutil::GeometryUtilities::GetME();
    
    UChar_t nplanes = geo->Nplanes();
    double  wire2cm = geo_util->WireToCm();
    double  time2cm = geo_util->TimeToCm();

    //
    // Obtain event-wise data object pointers
    //
    auto ev_clus = storage->get_data<event_cluster>(_cluster_producer);
    if(!ev_clus)
      throw ::cluster::ViewerException(Form("Did not find cluster data product by %s!",
					    _cluster_producer.c_str()
					    )
				       );

    auto associated_hit_producers = ev_clus->association_keys(data::kHit);
    
    if(!(associated_hit_producers.size()))

      throw ::cluster::ViewerException("Did not find associated hits!");

    auto ev_hit  = storage->get_data<event_hit>(associated_hit_producers[0]);

    if(!ev_hit)
      throw ::cluster::ViewerException(Form("Did not find hit data product by %s!",
					    associated_hit_producers[0].c_str()
					    )
				       );

    std::vector<float> hit_charge_frac;
    std::vector<UInt_t> MCShower_indices;
    int n_showers = 0;

    //DavidC
    /*
    if ( _showerColor ) {
      
      //grab the MC showers
      larlite::event_mcshower* ev_mcshower = (larlite::event_mcshower*) ( storage->get_data(larlite::DATA::MCShower));
      if(!ev_mcshower) {
	print(larlite::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, MCShower!"));
	return false;
      }
      
      //grab the simchannels
      larlite::event_simch* ev_simch = (larlite::event_simch*)( storage->get_data(larlite::DATA::SimChannel));
      if(!ev_simch) {
	print(larlite::MSG::ERROR,__FUNCTION__,Form("Did not find specified data product, SimChannel!"));
	return false;
      }

      _mcslb.SetMinEnergyCut(0.02);
      _mcslb.SetMaxEnergyCut(10.0);
      _shower_idmap.clear();
      _mcslb.FillShowerMap(ev_mcshower,_shower_idmap);
      _simch_map.clear();
      _mcslb.FillSimchMap(ev_simch,_simch_map);
      n_showers = ev_mcshower->size();
      for(UInt_t i=0; i < ev_mcshower->size(); ++i)
	MCShower_indices.push_back(i);
    }
    */
    // Find hit range & fill all-hits vector
    std::vector<std::pair<double,double> > xrange(nplanes,std::pair<double,double>(1e9,0));
    std::vector<std::pair<double,double> > yrange(nplanes,std::pair<double,double>(1e9,0));
    std::vector<std::vector<std::pair<double,double> > > hits_xy(nplanes,std::vector<std::pair<double,double> >());
    std::vector<std::vector<double> > hits_charge(nplanes,std::vector<double>());
    std::vector<std::vector<std::vector<std::pair<double,double> > > > shower_xy(nplanes,
										 std::vector<std::vector<std::pair<double,double> > >(n_showers,std::vector<std::pair<double,double> >()));

    for(auto const &h : *ev_hit) {
      
      UChar_t plane = geo->ChannelToPlane(h.Channel());
      double x = h.Wire() * wire2cm;
      double y = h.PeakTime() * time2cm;
      
      if(xrange.at(plane).first > x ) xrange.at(plane).first = x;
      if(xrange.at(plane).second < x ) xrange.at(plane).second = x;

      if(yrange.at(plane).first > y ) yrange.at(plane).first = y;
      if(yrange.at(plane).second < y ) yrange.at(plane).second = y;
      
      hits_xy.at(plane).push_back(std::pair<double,double>(x,y));


      //DavidC
      /*
      if ( _showerColor ){
	hit_charge_frac.clear();
	
	hit_charge_frac = 
	  _mcslb.MatchHitsAll(h,
			      _simch_map,
			      _shower_idmap,
			      MCShower_indices);
	//find item with largest fraction
	int   max_item = 0;
	float max_frac = 0.;
	for ( size_t i=0; i < hit_charge_frac.size(); i++){
	  if ( hit_charge_frac.at(i) > max_frac ) { 
	    max_item = i;
	    max_frac = hit_charge_frac.at(i);
	  }
	}
	if ( max_item < n_showers ){
	  shower_xy.at(plane).at(max_item).push_back(std::pair<double,double>(x,y));
	}
      }
      else {
	hits_charge.at(plane).push_back(h.Charge());
      }
      */
      hits_charge.at(plane).push_back(h.Charge());

    }//for all hits

    // Inform the algorithm about the range
    for(size_t i=0; i<nplanes; ++i)
      _algo.SetRange(i,xrange.at(i),yrange.at(i));

    // Provide plane-hits to the algorithm
    for(size_t i=0; i<nplanes; ++i)
      if ( _showerColor ){
	for(int s=0; s < n_showers; ++s){
	  _algo.AddShowers(i,shower_xy.at(i).at(s));
	}
      }
      else
	_algo.AddHits(i,hits_xy.at(i),hits_charge.at(i));
    //DavidC--placeholder--instead of hits_charge.at(i)
    //use color dependent on _mcslb return value

    // Find hits-per-cluster
    auto ass_hit_v = ev_clus->association(data::kHit,associated_hit_producers[0]);
    for(size_t i=0; i<ev_clus->size(); ++i) {

      UChar_t plane = nplanes;

      std::vector<std::pair<double,double> > cluster_hits;
      
      for(auto const& index : ass_hit_v[i]) {

	cluster_hits.push_back(std::pair<double,double>( ev_hit->at(index).Wire() * wire2cm,
							 ev_hit->at(index).PeakTime() * time2cm )
			       );
	if(plane == nplanes)

	  plane = geo->ChannelToPlane(ev_hit->at(index).Channel());
      
      }

      if(plane == nplanes) {

	throw ::cluster::ViewerException("No associated hits found!");

	return true;
      }
      
      if ( _showStartEnd ){

	//for start/end point, need to calculate CPAN
       	auto hit_index = ass_hit_v[i];
	::cluster::ClusterParamsAlg bestclus_CPAN;
	_cru_helper.GenerateCPAN( hit_index, ev_hit, bestclus_CPAN);
	bestclus_CPAN.DisableFANN();
	bestclus_CPAN.SetVerbose(false);
	bestclus_CPAN.FillParams(true,true,true,true,true,true);

	std::pair<double,double> cluster_start  ( bestclus_CPAN.GetParams().start_point.w, bestclus_CPAN.GetParams().start_point.t );
	std::pair<double,double> cluster_end    ( bestclus_CPAN.GetParams().end_point.w,   bestclus_CPAN.GetParams().end_point.t     );
	
	_algo.AddCluster(plane,
			 cluster_hits,
			 cluster_start,
			 cluster_end);
      }
      else
	_algo.AddCluster(plane,
			 cluster_hits);
      
    }

    return true;
  };
  
  bool ClusterViewer::finalize() {
    
    return true;
  }

}
#endif

