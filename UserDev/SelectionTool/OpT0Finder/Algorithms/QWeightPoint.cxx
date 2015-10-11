#ifndef OPT0FINDER_QWEIGHTPOINT_CXX
#define OPT0FINDER_QWEIGHTPOINT_CXX

#include "QWeightPoint.h"
#include "OpT0Finder/Base/OpT0FinderException.h"
#include "OpT0Finder/PhotonLibrary/PhotonVisibilityService.h"
#include <cmath>
#include <sstream>
namespace flashana {

  QWeightPoint::QWeightPoint(const double x_step_size)
    : BaseFlashMatch()
    , _pos_x()
    , _pos_y()
    , _pos_z()
    , _zdiff_max( 50*50 )
    , _use_library( false )
  {
    _x_step_size=x_step_size;
  }
    
  QWeightPoint::QWeightPoint(const std::vector<double>& pos_x,
			     const std::vector<double>& pos_y,
			     const std::vector<double>& pos_z,
			     const double x_step_size)
    : _pos_x(pos_x)
    , _pos_y(pos_y)
    , _pos_z(pos_z)
    , _zdiff_max( 50*50 )
  {
    if(_pos_x.size() != _pos_y.size() || _pos_x.size() != _pos_z.size() )
      throw OpT0FinderException("Unmatching optical detector position array length!");
    _x_step_size = x_step_size;
  }

  FlashMatch_t QWeightPoint::Match(const QCluster_t& pt_v, const Flash_t& flash)
  {
    // Prepare the return
    FlashMatch_t f;
    QCluster_t flash_hypothesis_v;
    if(pt_v.empty()) return f;

    for(double x_step_size=1; x_step_size<250; x_step_size+=_x_step_size) {
      QPoint_t pt;

      pt.x = pt.y = pt.z = pt.q = 0;
      double weight_tot = 0;
      QPoint_t min_pt = pt_v[0];

      for(auto const& tpc_pt : pt_v)
	
	if(min_pt.x < tpc_pt.x) min_pt = tpc_pt;

      // if we should use the photon library
      if(_use_library) {

	auto const& lib = ::phot::PhotonVisibilityService::GetME();
	double weight = 0;
	for(auto const& tpc_pt : pt_v) {

	  for(size_t i=0; i<lib.NOpChannels(); ++i)
	    weight += tpc_pt.q * lib.GetVisibility(tpc_pt.x,tpc_pt.y,tpc_pt.z,i);

	  pt.x += (tpc_pt.x - min_pt.x + x_step_size) * weight;
	  pt.y += (tpc_pt.y * weight);
	  pt.z += (tpc_pt.z * weight);
	  weight_tot += weight;
	}
	pt.x /= weight_tot;
	pt.y /= weight_tot;
	pt.z /= weight_tot;
	
      }
      // default
      else if(_pos_x.empty()) {

	for(auto const& tpc_pt : pt_v) {
	  
	  double weight = 0;

	  weight = tpc_pt.q / pow(tpc_pt.x - min_pt.x + x_step_size,2);
	  //pt.x += (tpc_pt.x - min_pt.x + x_step_size) * weight;
	  pt.y += (tpc_pt.y * weight);
	  pt.z += (tpc_pt.z * weight);
	  weight_tot += weight;


	}

	if(_verbosity <= msg::kINFO) {
	  std::stringstream ss;
	  ss << "@ step : " << x_step_size << " weight tot : " << weight_tot;
	  Print(msg::kINFO,__FUNCTION__,ss.str());
	}
	
	//pt.x /= weight_tot;
	pt.x = x_step_size;
	pt.y /= weight_tot;
	pt.z /= weight_tot;
	
      }
      else{
	
	for(auto const& tpc_pt : pt_v) {
	  
	  double weight = 0;
	  for(size_t i=0; i<_pos_x.size(); ++i) {
	    
	    double r2 = 0;
	    r2 += pow(tpc_pt.x - min_pt.x + x_step_size,2);
	    r2 += pow(tpc_pt.y - _pos_y[i],2);
	    r2 += pow(tpc_pt.z - _pos_z[i],2);
	    
	    double angle = (tpc_pt.x - min_pt.x + x_step_size) / r2;
	    if(angle<0) angle *= -1;
	    
	    weight += tpc_pt.q * angle / r2;
	    //weight += tpc_pt.q / r2;
	  }

	  pt.x += (tpc_pt.x - min_pt.x + x_step_size) * weight;
	  pt.y += (tpc_pt.y * weight);
	  pt.z += (tpc_pt.z * weight);
	  weight_tot += weight;
	}
	pt.x /= weight_tot;
	pt.y /= weight_tot;
	pt.z /= weight_tot;
      }
      if(_verbosity <= msg::kINFO) {
	std::stringstream ss;
	ss << "Flash Hypothesis Point: " << pt.x << " : " << pt.y << " : " << pt.z;
	Print(msg::kINFO,__FUNCTION__,ss.str());
      }
      flash_hypothesis_v.push_back(pt);
    }

    // (re)-evaluate flash position if possible
    double flash_y = flash.y;
    double flash_z = flash.z;
    if(_pos_x.size()) {
      double qsum = 0;
      for(size_t i=0; i<_pos_x.size()&&i<flash.pe_v.size(); ++i) {
	flash_y += flash.pe_v[i] * _pos_y[i];
	flash_z += flash.pe_v[i] * _pos_z[i];
	qsum += flash.pe_v[i];
      }
      flash_y /= qsum;
      flash_z /= qsum;
    }
    
    // Compute the minimum z-position difference


    
    double min_dist = 1e12;
    ID_t min_id=kINVALID_ID;
    for(ID_t id=0; id<flash_hypothesis_v.size(); ++id) {
      auto const& pt = flash_hypothesis_v[id];
      // (re)-estimate position
      double dist = pow(pt.z - flash_z,2);
      if(dist < min_dist) {
	min_dist = dist;
	min_id = id;
      }
    }

    if(_verbosity <= msg::kINFO) {
      std::stringstream ss1;
      ss1 << "Best match Hypothesis: "
	  << flash_hypothesis_v[min_id].x << " : "
	  << flash_hypothesis_v[min_id].y << " : "
	  << flash_hypothesis_v[min_id].z;
      Print(msg::kINFO,__FUNCTION__,ss1.str());
      std::stringstream ss2;
      ss2 << "Flash point: " << flash.x << " : " << flash.y << " : " << flash.z
	  << "\t w/ dist : " << min_dist << std::endl;
      Print(msg::kINFO,__FUNCTION__,ss2.str());
    }

    // If min-diff is bigger than assigned max, return default match (score<0)
    if(min_dist > _zdiff_max) return f;

    // Assign the score, return
    f.score = 1./min_dist;
    f.tpc_point = flash_hypothesis_v[min_id];

    return f;
  }


}
#endif
