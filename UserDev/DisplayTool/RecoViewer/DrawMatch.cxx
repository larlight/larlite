#ifndef LARLITE_DRAWMATCH_CXX
#define LARLITE_DRAWMATCH_CXX

#include "DrawMatch.h"

namespace evd {

DrawMatch::DrawMatch() {
  _name = "DrawMatch";
  _fout = 0;
  _time_ratio_cut  = 0.001;
  _start_time_cut  = 10 ;
  _require_3planes = true ;
}

bool DrawMatch::initialize() {

  // Resize data holder
  if (_dataByPlane.size() != geoService -> Nviews()) {
    _dataByPlane.resize(geoService -> Nviews());
  }

  return true;
}

bool DrawMatch::analyze(larlite::storage_manager* storage) {

  auto ev_clus = storage->get_data<larlite::event_cluster>(_producer);
  if (!ev_clus)
    return false;
  if (!ev_clus->size()) {
    print(larlite::msg::kWARNING, __FUNCTION__,
          Form("Skipping event %d since no cluster found...", ev_clus->event_id()));
    return false;
  }

  // Clear out the hit data but reserve some space for the hits
  for (unsigned int p = 0; p < geoService -> Nviews(); p ++) {
    _dataByPlane.at(p).clear();
    _dataByPlane.at(p).reserve(ev_clus -> size());
    _wireRange.at(p).first  = 99999;
    _timeRange.at(p).first  = 99999;
    _timeRange.at(p).second = -1.0;
    _wireRange.at(p).second = -1.0;

  }

  ::larlite::event_hit* ev_hit = nullptr;
  auto const& hit_index_v = storage->find_one_ass(ev_clus->id(), ev_hit, _producer);

  // std::cout << "Hit _producer is " << ev_hit -> name() << std::endl;

  if (!ev_hit) {
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

  cluster::DefaultParamsAlg params_alg ;
  cluster::cluster_params params;
  params_alg.SetVerbose(false);
  params_alg.SetDebug(false);
  params_alg.SetMinHits(10);

  for (auto const& hit_indices : hit_index_v) {
    view = ev_hit->at(hit_indices[0]).View();

    // Make a new cluster in the data:
    _dataByPlane.at(view).push_back(Cluster2d());
    _dataByPlane.at(view).back()._is_good = true;
    
    // Fill the cluster params alg
    _cru_helper.GenerateParams( hit_indices, ev_hit, params);
    params_alg.FillParams(params);

    // Set the params:
    _dataByPlane.at(view).back()._params = params;

    for (auto const& hit_index : hit_indices) {

      auto & hit = ev_hit->at(hit_index);
      // if (view == 0){
      //   std::cout << "Got a hit, seems to be view " << view
      //             << " and cluster " << cluster_index[view]
      //             << " at " << ev_hit->at(hit_index).WireID().Wire
      //             << ", " << ev_hit->at(hit_index).PeakTime()
      //             << std::endl;
      // }
    // Hit(float w, float t, float c, float r) :

      _dataByPlane.at(view).back().emplace_back(Hit(hit.WireID().Wire,
                                                    hit.PeakTime(),
                                                    hit.PeakAmplitude(),
                                                    hit.RMS()));


      // Determine if this hit should change the view range:
      if (hit.WireID().Wire > _wireRange.at(view).second)
        _wireRange.at(view).second = hit.WireID().Wire;
      if (hit.WireID().Wire < _wireRange.at(view).first)
        _wireRange.at(view).first = hit.WireID().Wire;
      if (hit.PeakTime() > _timeRange.at(view).second)
        _timeRange.at(view).second = hit.PeakTime();
      if (hit.PeakTime() < _timeRange.at(view).first)
        _timeRange.at(view).first = hit.PeakTime();

    }

    cluster_index[view] ++;

  }
    //
    // THUS IT IS HERE THAT MATCHING SHALL BEGIN.
    // Use information stored in dataByPlane to match 
    // clusters with similar start/end times and similar
    // time spans.
    //
    std::vector<int> best_clusters;
    double high_score = 0 ;
    _match_scores.clear() ; 

    for (int i = 0; i < _dataByPlane.at(0).size(); i++){
	
	auto c0 = _dataByPlane.at(0)[i].params(); 

      if (( c0.start_point.t == 0 && c0.end_point.t == 0) || c0.N_Hits < 20) continue; 

      std::cout<<"\n\nPlane 0 " <<std::endl ; 
      double max_time_difference = 0;
      double max_charge          = 0;
      double ratio               = 1;
      double start_t      = 0;
      double end_t        = 0;
      std::vector<int> clusters ;

      //Make start_t always smaller
      c0.start_point.t > c0.end_point.t ? start_t = c0.end_point.t, end_t = c0.start_point.t : start_t = c0.start_point.t, end_t = c0.end_point.t;
	
      double prev_start_t = start_t;
      double prev_end_t   = end_t;

      for (int j = 0; j < _dataByPlane.at(1).size(); j++){
	
	auto c1 = _dataByPlane.at(1)[j].params(); 
        if ( (c1.start_point.t == 0 && c1.end_point.t == 0 ) || c1.N_Hits < 20 ) continue; 

	for (int k = 0; k < _dataByPlane.at(2).size(); k++){

	    //auto charge0 = c0.sum_charge ;    
	    //auto charge1 = c1.sum_charge ;    
	    //auto charge2 = c2.sum_charge ;    
	    ratio               = 1;
	    auto c2 = _dataByPlane.at(2)[k].params() ;
	    if ( (c2.start_point.t == 0 && c2.end_point.t == 0) || c2.N_Hits < 20) continue;

	    //Calculate ratio using time differences. 
	    auto c0_time_diff  = fabs( c0.start_point.t - c0.end_point.t ); 
	    auto c1_time_diff  = fabs( c1.start_point.t - c1.end_point.t ); 
	    auto c2_time_diff  = fabs( c2.start_point.t - c2.end_point.t); 

	    c1_time_diff > c0_time_diff ? max_time_difference = c1_time_diff : max_time_difference = c0_time_diff ; 
	    c2_time_diff > max_time_difference ? max_time_difference = c2_time_diff : true ; 
	    
	    ratio *= (c0_time_diff )/max_time_difference ;
	    ratio *= (c1_time_diff )/max_time_difference ;
      	    ratio *= (c2_time_diff )/max_time_difference ;



            //Make start_t always smaller
	    if(c2.start_point.t > c2.end_point.t){
              start_t = c2.end_point.t   ;
              end_t   = c2.start_point.t ;
                }
            else{
              start_t = c2.start_point.t ;
              end_t   = c2.end_point.t   ;   
	     }

	    double length = c2.length ;
      	    //If current cluster's start time is not within some range of the previous cluster's start time,
      	    //modify ratio to disallow matching
      	    if( (start_t > (prev_start_t - _start_time_cut) && start_t < (prev_start_t + _start_time_cut))
      	        || (end_t > (prev_end_t - _start_time_cut) && end_t < (prev_end_t + _start_time_cut) )
      	        || (length >25 && start_t >(prev_start_t - 2*_start_time_cut) && start_t < (prev_start_t + 2*_start_time_cut) ) )
      	      ratio *= 1;
      	    else
      	      ratio *= 0.001;

	    
	//    if(ratio < 0.001){
	//	std::cout<<"\nLess than."<<std::endl;
	//    	std::cout<<"Ratio is: "<<ratio
	//    	         <<"\nStart time: \n"<<c0.start_point.t <<", "<<c0.end_point.t<<", \n"
	//    	    			<<c1.start_point.t<<", "<<c1.end_point.t<<", \n"
	//    	    			<<c2.start_point.t<<", "<<c2.end_point.t<<std::endl;
	//    	std::cout<<"i j k: "<<i <<", "<<j<<", "<<k<<std::endl ;
	//	}

	    clusters = {i,j,k} ;

	    if(ratio > high_score){
		high_score = ratio; 
		best_clusters = clusters;
		}

	    if(ratio > 0.001){
		_match_scores.push_back(std::make_pair(clusters,ratio));
	//	std::cout<<"What's the ratio???" <<ratio<<std::endl ;
	//	std::cout<<"Hits : "<<c0.N_Hits<<", "<<c1.N_Hits<<", "<<c2.N_Hits ;
	//	std::cout<<"\nHits : "<<_dataByPlane.at(0)[i].params().N_Hits<<", "<<_dataByPlane.at(1)[j].params().N_Hits<<", "<<_dataByPlane.at(2)[k].params().N_Hits << std::endl; 
	//	std::cout<<"i j k: "<<i <<", "<<j<<", "<<k<<std::endl ;
		}
	}
      }
    }
    // End of matching algorithm
    
    std::vector<std::pair<std::vector<int>,double>> new_matches ;

    //
    // Take group of clusters with best score. Remove all
    // other pairs that share those clusters because 
    // their scores are worse and they probably suck.
    //
    bool keep_going = true; 
    //std::cout<<"\n\nMatch stuff : \nBest cluster pts: " <<std::endl;
    while(keep_going){

	new_matches.push_back(std::make_pair(best_clusters,high_score)) ;
	//for ( int i=0; i < best_clusters.size(); i++) std::cout<<best_clusters[i]<<"  ";
	//std::cout<<"\nHigh score??"<<high_score<<std::endl;
	
    if( _match_scores.size() > 1 ){
    for ( int i=0; i <_match_scores.size(); i++ ){
	if(_match_scores.at(i).first ==best_clusters)
	    _match_scores.erase(_match_scores.begin()+i);

	if ( _match_scores.at(i).first != best_clusters && (_match_scores.at(i).first[0] == best_clusters[0] || _match_scores.at(i).first[1] == best_clusters[1] || _match_scores.at(i).first[2] == best_clusters[2]) ){
	    _match_scores.erase(_match_scores.begin()+i);
	    i--;
	    }
	}

    high_score = 0;
    keep_going = false ;

    for ( int i=0; i <_match_scores.size(); i++ ){
	
	if(_match_scores.at(i).second > high_score ){
	    high_score = _match_scores.at(i).second ;
	    best_clusters = _match_scores.at(i).first ;
	    keep_going = true; 
	    }
	}
    }
    else
	keep_going = false; 

    }
    // End of shitty cluster pairs removal

    // 
    // Now insert the matched pairs at the beginning of dataByPlane
    // Then, remove all clusters at the end not part of the match stuff
    //
    int a = 0;
    for ( auto & m : new_matches ){
	_dataByPlane.at(0).insert(_dataByPlane.at(0).begin(),_dataByPlane.at(0)[m.first[0]+a]); 
    	_dataByPlane.at(1).insert(_dataByPlane.at(1).begin(),_dataByPlane.at(1)[m.first[1]+a]); 
    	_dataByPlane.at(2).insert(_dataByPlane.at(2).begin(),_dataByPlane.at(2)[m.first[2]+a]); 
	a++; 
       }

    _dataByPlane.at(0).erase(_dataByPlane.at(0).begin()+new_matches.size(),_dataByPlane.at(0).end()) ;
    _dataByPlane.at(1).erase(_dataByPlane.at(1).begin()+new_matches.size(),_dataByPlane.at(1).end()) ;
    _dataByPlane.at(2).erase(_dataByPlane.at(2).begin()+new_matches.size(),_dataByPlane.at(2).end()) ;


  // Now that clusters are done filling, go through and pad out the rest of the data
  // Just in case they aren't the same length:
  int max_val = cluster_index.front();
  for (auto & val : cluster_index){
    if (val > max_val ){
      max_val = val;
    }
  }

  for (auto & planeOfClusters : _dataByPlane){
    if (planeOfClusters.size() < max_val){
      planeOfClusters.resize(max_val);
    }
  }


  return true;
}

bool DrawMatch::finalize() {

  // This function is called at the end of event loop.
  // Do all variable finalization you wish to do here.
  // If you need, you can store your ROOT class instance in the outputmake
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

DrawMatch::~DrawMatch() {
}

}
#endif
