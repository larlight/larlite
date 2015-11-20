#ifndef OPT0FINDER_QWEIGHTPOINT_CXX
#define OPT0FINDER_QWEIGHTPOINT_CXX

#include "QWeightPoint.h"
#include "OpT0Finder/Base/OpT0FinderException.h"
#include "OpT0Finder/PhotonLibrary/PhotonVisibilityService.h"
#include <cmath>
#include <sstream>
#include <numeric>
namespace flashana {

  QWeightPoint::QWeightPoint(const std::string name)
    : BaseFlashMatch(name)
    , _x_step_size ( 0.5   )
    , _zdiff_max   ( 50*50 )
  {}

  void QWeightPoint::Configure(const ::fcllite::PSet &pset)
  {
    _x_step_size = pset.get<double>("XStepSize");
    _zdiff_max   = pset.get<double>("ZDiffMax" );
    _zdiff_max *= _zdiff_max; 
  }
  
  FlashMatch_t QWeightPoint::Match(const QCluster_t& pt_v, const Flash_t& flash)
  {

    if(_vis_array.pe_v.empty())
      _vis_array.pe_v.resize(OpDetXArray().size());

    // Prepare the return values (Mostly QWeightPoint)
    FlashMatch_t f;
    if(pt_v.empty()){
      std::cout<<"Not enough points!"<<std::endl;
      return f;
    }
    
    _tpc_qcluster.resize(pt_v.size());

    // Get min & max x value
    double x_max = 0;
    double x_min = 1e12;

    for(auto const& pt : pt_v) {
      if(pt.x > x_max) x_max = pt.x;
      if(pt.x < x_min) x_min = pt.x;
    }

    double min_dz = 1e9;
    for(double x_offset=0;
	x_offset<(250.-(x_max-x_min));
	x_offset+=_x_step_size) {
      
      // Create QCluster_t with this offset

      for(size_t i=0; i<_tpc_qcluster.size(); ++i) {
	_tpc_qcluster[i].x = pt_v[i].x + x_offset - x_min;
	_tpc_qcluster[i].y = pt_v[i].y;
	_tpc_qcluster[i].z = pt_v[i].z;
	_tpc_qcluster[i].q = pt_v[i].q;
      }
            
      FillEstimate(_tpc_qcluster,_vis_array);

      // Calculate amplitudes corresponding to max opdet amplitudes
      double vis_pe_sum = std::accumulate(std::begin(_vis_array.pe_v),
					  std::end(_vis_array.pe_v),
					  0.0);

      double weighted_z = 0;
      for(size_t pmt_index=0; pmt_index<NOpDets(); ++pmt_index)

	weighted_z += OpDetZ(pmt_index) * _vis_array.pe_v[pmt_index] / vis_pe_sum;

      double dz = std::fabs(weighted_z - flash.z);
      
      if(dz < min_dz) {

	min_dz = dz;

	f.score = 1./min_dz;
	f.tpc_point.x = f.tpc_point.y = 0;
	f.tpc_point.q = vis_pe_sum;

	for(size_t pmt_index=0; pmt_index<NOpDets(); ++pmt_index) {
	  
	  f.tpc_point.x += OpDetX(pmt_index) * _vis_array.pe_v[pmt_index] / vis_pe_sum;
	  f.tpc_point.y += OpDetY(pmt_index) * _vis_array.pe_v[pmt_index] / vis_pe_sum;
	}

	f.tpc_point.z = weighted_z;
      }
    }
    
    if(_verbosity <= msg::kINFO) {
      std::stringstream ss1;
      ss1 << "Best match Hypothesis: "
	  << f.tpc_point.x << " : "
	  << f.tpc_point.y << " : "
	  << f.tpc_point.z << " ... min dist : " << min_dz;
      Print(msg::kINFO,__FUNCTION__,ss1.str());
    }
  
    // If min-diff is bigger than assigned max, return default match (score<0)
    if( min_dz > _zdiff_max ) {
      
      f.tpc_point.x = f.tpc_point.y = f.tpc_point.z = -1;
      f.tpc_point.q = -1;
      f.score = -1;
      return f;
    }

    return f;

  }


}
#endif
