#ifndef RECOTOOL_CLUSTERVIEWER_CXX
#define RECOTOOL_CLUSTERVIEWER_CXX

#include "ClusterViewer.h"
#include "ClusterRecoUtil/DefaultParamsAlg.h"

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
    if(!ev_clus->size()) {
      print(msg::kWARNING,__FUNCTION__,
	    Form("Skipping event %d since no cluster found...",ev_clus->event_id()));
      return false;
    }

    event_hit* ev_hit = nullptr;
    auto const& ass_hit_v = storage->find_one_ass(ev_clus->id(),ev_hit,ev_clus->name());
    if (!ev_hit)
      throw ::cluster::ViewerException("Did not find associated hits!");
    if (!ass_hit_v.size())
      throw ::cluster::ViewerException("Did not find associated hits!");

    /*
    auto associated_hit_producers = ev_clus->association_keys(data::kHit);
    
    if(!(associated_hit_producers.size()))

      throw ::cluster::ViewerException("Did not find associated hits!");

    auto ev_hit  = storage->get_data<event_hit>(associated_hit_producers[0]);

    if(!ev_hit)
      throw ::cluster::ViewerException(Form("Did not find hit data product by %s!",
					    associated_hit_producers[0].c_str()
					    )
				       );
    */
    std::vector<float> hit_charge_frac;
    std::vector<UInt_t> MCShower_indices;
    int n_showers = 0;

    // Find hit range & fill all-hits vector
    std::vector<std::pair<double,double> > xrange(nplanes,std::pair<double,double>(1e9,0));
    std::vector<std::pair<double,double> > yrange(nplanes,std::pair<double,double>(1e9,0));
    std::vector<std::vector<std::pair<double,double> > > hits_xy(nplanes,std::vector<std::pair<double,double> >());
    std::vector<std::vector<double> > hits_charge(nplanes,std::vector<double>());
    std::vector<std::vector<std::vector<std::pair<double,double> > > > shower_xy(nplanes,
										 std::vector<std::vector<std::pair<double,double> > >(n_showers,std::vector<std::pair<double,double> >()));

    for(auto const &h : *ev_hit) {
      
      UChar_t plane = geo->ChannelToPlane(h.Channel());
      double x = h.WireID().Wire * wire2cm;
      double y = h.PeakTime() * time2cm;
      
      if(xrange.at(plane).first > x ) xrange.at(plane).first = x;
      if(xrange.at(plane).second < x ) xrange.at(plane).second = x;

      if(yrange.at(plane).first > y ) yrange.at(plane).first = y;
      if(yrange.at(plane).second < y ) yrange.at(plane).second = y;
      
      hits_xy.at(plane).push_back(std::pair<double,double>(x,y));

      hits_charge.at(plane).push_back(h.Integral());

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

    // Find hits-per-cluster
    //auto ass_hit_v = ev_clus->association(data::kHit,associated_hit_producers[0]);
    for(size_t i=0; i<ev_clus->size(); ++i) {

      UChar_t plane = nplanes;

      std::vector<std::pair<double,double> > cluster_hits;
      
      for(auto const& index : ass_hit_v[i]) {

	cluster_hits.push_back(std::pair<double,double>( ev_hit->at(index).WireID().Wire * wire2cm,
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
	::cluster::DefaultParamsAlg _params_alg;
  ::cluster::cluster_params _params;
	_cru_helper.GenerateParams( hit_index, ev_hit, _params);

	_params_alg.FillParams(_params);

	std::pair<double,double> cluster_start  ( _params.start_point.w, _params.start_point.t );
	std::pair<double,double> cluster_end    ( _params.end_point.w,   _params.end_point.t     );
	
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

