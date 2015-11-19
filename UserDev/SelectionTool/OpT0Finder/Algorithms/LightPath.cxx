#ifndef LIGHTPATH_CXX
#define LIGHTPATH_CXX

#include "LightPath.h"

namespace flashana{

  LightPath::LightPath()
    : _start_bool (true)
    , _end_bool   (true)
    , _gap        (0.5   )
    , _xmax       (256.35)
    , _xmin       (0.0   )
    , _ymax       (116.5 )
    , _ymin       (-116.5)
    , _zmax       (1036.8)
    , _zmin       (0.0   )
  {}

  void LightPath::QCluster(::geoalgo::Vector pt_1,
                           ::geoalgo::Vector pt_2,
                           QCluster_t& Q_cluster)  const {

    double dist = pt_1.Dist(pt_2);
    QPoint_t q_pt;
    if(dist<_gap){
      ::geoalgo::Vector mid_pt((pt_1+pt_2)/2.);
      q_pt.x = mid_pt[0];
      q_pt.y = mid_pt[1];
      q_pt.z = mid_pt[2];
      q_pt.q = _gap * 2.3 * 29000.;
      Q_cluster.emplace_back(q_pt);
    }
    else{
      int num_div = int(dist/_gap);
      ::geoalgo::Vector direction = pt_1 - pt_2;
      ::geoalgo::Vector direct = direction.Dir();
      for(int div_index = 0; div_index <num_div+1; div_index++){
        if(div_index<num_div){
          ::geoalgo::Vector mid_pt(pt_2 + direct * (_gap*div_index + _gap/2.));
          q_pt.x = mid_pt[0];
          q_pt.y = mid_pt[1];
          q_pt.z = mid_pt[2];
          q_pt.q = _gap * 2.3 * 29000.;
          Q_cluster.emplace_back(q_pt);
        }
        else{
          double weight = (dist - int(dist / _gap) * _gap);
          ::geoalgo::Vector mid_pt(pt_2 + direction * (div_index * _gap + weight/2.));
          q_pt.x = mid_pt[0];
          q_pt.y = mid_pt[1];
          q_pt.z = mid_pt[2];
          q_pt.q = _gap * 2.3 * 29000.;
          Q_cluster.emplace_back(q_pt);
        }//Last segment less than gap
      }
    }
  }


  std::vector<double> LightPath::PhotonLibrary(::geoalgo::Vector pt_1, ::geoalgo::Vector pt_2, std::vector<double> pe)const {

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

  QCluster_t LightPath::FlashHypothesis(::geoalgo::Trajectory trj) const {
    ::geoalgo::GeoAlgo _geoAlgo;
    std::vector<double> PE_PL;
    QCluster_t result;
    PE_PL.resize(32, 0.0);
    result.clear();
    ::geoalgo::AABox _vfiducial(_xmin, _ymin, _zmin,
                                _xmax, _ymax, _zmax);
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
        PE_PL = LightPath::PhotonLibrary(this_loc, last_loc,PE_PL);
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
        ::geoalgo::Vector this_loc(trj[sample_index+1]);
        ::geoalgo::Vector last_loc(x_pt_v.front());
        PE_PL = LightPath::PhotonLibrary(this_loc, last_loc,PE_PL);
	LightPath::QCluster(this_loc, last_loc, result);
      }
    }

    for (int i = 0; i < trj.size()-1;i++){
      ::geoalgo::Vector this_loc(trj[i]);
      ::geoalgo::Vector last_loc(trj[i+1]);
      PE_PL = LightPath::PhotonLibrary(this_loc, last_loc,PE_PL);
      LightPath::QCluster(this_loc, last_loc, result);
      //std::cout<<PE_PL[0];
    }
    //return PE_PL;
    return result;
  }
}


#endif
