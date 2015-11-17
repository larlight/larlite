#ifndef FLASHHYPO_CXX
#define FLASHHYPO_CXX

#include "FlashHypo.h"

namespace flashana{
  std::vector<std::vector<double>> FlashHypo::QCluster(::geoalgo::Vector pt_1, ::geoalgo::Vector pt_2, std::vector<std::vector<double>> Q_cluster)  const {
    
    double dist = pt_1.Dist(pt_2);
    std::vector<double> q_pt;
    if(dist<_gap){
      ::geoalgo::Vector mid_pt((pt_1+pt_2)/2.);
      q_pt = {mid_pt[0],mid_pt[1],mid_pt[2],_gap*2.3*29000};
      Q_cluster.push_back(q_pt);
    }
    else{
      int num_div = int(dist/_gap);
      ::geoalgo::Vector direction = pt_1 - pt_2;
      ::geoalgo::Vector direct = direction.Dir();
      for(int div_index = 0; div_index <num_div+1; div_index++){
	if(div_index<num_div){
	  ::geoalgo::Vector mid_pt(pt_2 + direct * (_gap*div_index + _gap/2.));
	  q_pt = {mid_pt[0],mid_pt[1],mid_pt[2],_gap*2.3*29000};
	  Q_cluster.push_back(q_pt);
	}
	else{
	  double weight = (dist - int(dist / _gap) * _gap);
	  ::geoalgo::Vector mid_pt(pt_2 + direction * (div_index * _gap + weight/2.));
	  q_pt = {mid_pt[0],mid_pt[1],mid_pt[2],_gap*2.3*29000};
	  Q_cluster.push_back(q_pt);
	}//Last segment less than gap
      }
    }
    return Q_cluster;
  }

  
  std::vector<double> FlashHypo::PhotonLibrary(::geoalgo::Vector pt_1, ::geoalgo::Vector pt_2, std::vector<double> pe)const {
    
    double dist = pt_1.Dist(pt_2);
    if(dist<_gap){
      ::geoalgo::Vector mid_pt((pt_1+pt_2)/2.);
      for(size_t pmt_id = 0; pmt_id<32; pmt_id++){
	pe[pmt_id] += ::phot::PhotonVisibilityService::GetME().GetVisibility(mid_pt[0],mid_pt[1],mid_pt[2],pmt_id);
      }
    }
    else{
      int num_div = int(dist/_gap);//no need for plus 1
      ::geoalgo::Vector direction = pt_1 - pt_2;
      ::geoalgo::Vector direct = direction.Dir();
      for(int div_index = 0; div_index <num_div+1; div_index++){
	if(div_index<num_div){
	  ::geoalgo::Vector mid_pt(pt_2 + direct * (_gap*div_index + _gap/2.));
	  for(size_t pmt_id = 0; pmt_id<32; pmt_id++){
	    pe[pmt_id] += ::phot::PhotonVisibilityService::GetME().GetVisibility(mid_pt[0],mid_pt[1],mid_pt[2],pmt_id);
	  }
	}
	else{
	  double weight = (dist - int(dist / _gap) * _gap);
	  ::geoalgo::Vector mid_pt(pt_2 + direction * (div_index * _gap + weight/2.));
	  for(size_t pmt_id = 0; pmt_id<32; pmt_id++){
	    pe[pmt_id] += ::phot::PhotonVisibilityService::GetME().GetVisibility(mid_pt[0],mid_pt[1],mid_pt[2],pmt_id);
	  }//Fill each PMT
	}//Last segment less than gap
      }
    }
    return pe;
  }
  
  std::vector<std::vector<double>> FlashHypo::FlashHypothesis(::geoalgo::Trajectory trj) const {
    ::geoalgo::GeoAlgo _geoAlgo;
    std::vector<double> PE_PL;
    std::vector<std::vector<double>> Q_cluster;
    PE_PL.resize(32, 0.0);
    Q_cluster.clear();
    double _length_xfiducial = larutil::Geometry::GetME()->DetHalfWidth();
    double _length_yfiducial = larutil::Geometry::GetME()->DetHalfHeight();
    double _length_zfiducial = larutil::Geometry::GetME()->DetLength();
    ::geoalgo::AABox _vfiducial(0, -_length_yfiducial, 0, 2 * _length_xfiducial, _length_yfiducial,_length_zfiducial);
    if(_start_bool){
      double length = 0;
      int sample_index = 1;
      while (length < 10 && sample_index<trj.size()){
	length = trj.Length(0,sample_index);
	sample_index++;
      }
      ::geoalgo::HalfLine prj_start(trj[sample_index-1],trj[sample_index-1]-trj[0]);
      auto const& x_pt_v = _geoAlgo.Intersection(_vfiducial,prj_start);
      if (x_pt_v.size()==1){
	::geoalgo::Vector this_loc(trj[sample_index-1]);
	::geoalgo::Vector last_loc(x_pt_v.front());
	PE_PL = FlashHypo::PhotonLibrary(this_loc, last_loc,PE_PL);
	Q_cluster = FlashHypo::QCluster(this_loc, last_loc, Q_cluster);
      }
    }
    if(_end_bool){
      double length = 0;
      int sample_index = trj.size()-2;
      while (length < 10 && sample_index >-1){
	length = trj.Length(sample_index,trj.size()-1);
	sample_index--;
      }
      ::geoalgo::HalfLine prj_end(trj[sample_index+1],trj[trj.size()-1]-trj[sample_index+1]);
      auto const& x_pt_v = _geoAlgo.Intersection(_vfiducial,prj_end);
      if (x_pt_v.size()==1){
	::geoalgo::Vector this_loc(trj[sample_index+1]);
	::geoalgo::Vector last_loc(x_pt_v.front());
	PE_PL = FlashHypo::PhotonLibrary(this_loc, last_loc,PE_PL);
	Q_cluster = FlashHypo::QCluster(this_loc, last_loc, Q_cluster);
      }
    }
    
    for (int i = 0; i < trj.size()-1;i++){
      ::geoalgo::Vector this_loc(trj[i]);
      ::geoalgo::Vector last_loc(trj[i+1]);
      PE_PL = FlashHypo::PhotonLibrary(this_loc, last_loc,PE_PL);
      Q_cluster = FlashHypo::QCluster(this_loc, last_loc, Q_cluster);
      //std::cout<<PE_PL[0];
    }
    //return PE_PL;
    return Q_cluster;
  }
}
#endif
