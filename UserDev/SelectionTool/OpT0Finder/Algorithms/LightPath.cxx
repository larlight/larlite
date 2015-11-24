#ifndef LIGHTPATH_CXX
#define LIGHTPATH_CXX

#include "LightPath.h"

namespace flashana{

  LightPath::LightPath(const std::string name)
    : BaseAlgorithm(kCustomAlgo,name)
    , _start_bool  ( true   )
    , _end_bool    ( true   )
    , _gap         ( 0.5    )
    , _light_yield ( 29000. )
    , _dEdxMIP     ( 2.3    )
  {}

  void LightPath::Configure(const ::fcllite::PSet &pset)
  {
    _start_bool  = pset.get< bool   > ( "ExtendStart" );
    _end_bool    = pset.get< bool   > ( "ExtendEnd"   );
    _gap         = pset.get< double > ( "SegmentSize" );
    _light_yield = pset.get< double > ( "LightYield"  );
    _dEdxMIP     = pset.get< double > ( "MIPdEdx"     );
  }

  void LightPath::QCluster(const ::geoalgo::Vector& pt_1,
                           const ::geoalgo::Vector& pt_2,
                           QCluster_t& Q_cluster)  const {
    
    double dist = pt_1.Dist(pt_2);
    QPoint_t q_pt;
    
      if(dist<_gap){
	::geoalgo::Vector mid_pt((pt_1+pt_2)/2.);
	q_pt.x = mid_pt[0];
	q_pt.y = mid_pt[1];
	q_pt.z = mid_pt[2];
	q_pt.q = _dEdxMIP * _light_yield * dist/2.;
	Q_cluster.emplace_back(q_pt);
	return;
      }
      
      int num_div = int(dist/_gap);
      ::geoalgo::Vector direction = pt_1 - pt_2;
      ::geoalgo::Vector direct = direction.Dir();
      
      Q_cluster.reserve(Q_cluster.size() + num_div);
      
      for(int div_index = 0; div_index <num_div+1; div_index++){
	if(div_index<num_div){
	  auto const mid_pt = pt_2 + direct * (_gap * div_index + _gap/2.);
	  q_pt.x = mid_pt[0];
	  q_pt.y = mid_pt[1];
	  q_pt.z = mid_pt[2];
	  q_pt.q = _gap * _dEdxMIP * _light_yield;
	  Q_cluster.emplace_back(q_pt);
	}
	else{
	  double weight = (dist - int(dist / _gap) * _gap);
	  auto const mid_pt = pt_2 + direct * (_gap * div_index + weight/2.);
	  q_pt.x = mid_pt[0];
	  q_pt.y = mid_pt[1];
	  q_pt.z = mid_pt[2];
	  q_pt.q = _gap * _dEdxMIP * _light_yield;
	  Q_cluster.emplace_back(q_pt);
	}//Last segment less than gap
	
    }
  }

  QCluster_t LightPath::FlashHypothesis(const ::geoalgo::Trajectory& trj) const {
    
    QCluster_t result;
    result.clear();
    
    auto const& _vfiducial = ActiveVolume();
    if(_start_bool){
      double length = 0;
      int sample_index = 1;
      while (length < 10 && sample_index<trj.size()){
        length = trj.Length(0,sample_index);
	sample_index++;
      }
      ::geoalgo::HalfLine prj_start(trj[sample_index-1],trj[0]-trj[sample_index-1]);
      auto const& x_pt_v = _geoAlgo.Intersection(_vfiducial,prj_start);
      if (x_pt_v.size()==1){
	
	auto const& this_loc(trj[0]);
        auto const& last_loc(x_pt_v.front());
	
	LightPath::QCluster(this_loc, last_loc, result);
	
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
        
	auto const& this_loc(trj[trj.size()-1]);
        auto const& last_loc(x_pt_v.front());
	
	LightPath::QCluster(this_loc, last_loc, result);
      }
    }
    
    for (int i = 0; i < trj.size()-2;i++){
      auto const& this_loc(trj[i]);
      auto const& last_loc(trj[i+1]);
      
      LightPath::QCluster(this_loc, last_loc, result);
    }
    return result;
  }
}


#endif
