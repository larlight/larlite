#ifndef OPT0FINDER_COMMONAMPS_CXX
#define OPT0FINDER_COMMONAMPS_CXX

#include "CommonAmps.h"
#include "OpT0Finder/Base/OpT0FinderException.h"
#include "OpT0Finder/PhotonLibrary/PhotonVisibilityService.h"
#include <cmath>
#include <sstream>
namespace flashana {

  CommonAmps::CommonAmps(const double x_step_size)
    : BaseFlashMatch()
    , _pos_x()
    , _pos_y()
    , _pos_z()
    , _use_library( false )
  {
    _x_step_size=x_step_size;
    _percent = 0.5;
    _score   = 0.8;
  }
    
  CommonAmps::CommonAmps(const std::vector<double>& pos_x,
			     const std::vector<double>& pos_y,
			     const std::vector<double>& pos_z,
			     const double x_step_size)
    : _pos_x(pos_x)
    , _pos_y(pos_y)
    , _pos_z(pos_z)
  {
    if(_pos_x.size() != _pos_y.size() || _pos_x.size() != _pos_z.size() )
      throw OpT0FinderException("Unmatching optical detector position array length!");
    _x_step_size = x_step_size;
    _percent = 0.5 ;
    _score   = 0.8 ;

  }

  FlashMatch_t CommonAmps::Match(const QCluster_t& pt_v, const Flash_t& flash)
  {

    double integral_vis = 0;
    double integral_op  = 0;
    double maxRatio 	= -1; 
    int    maxID 	= -1;

    for(size_t k=0; k<32; k++)
	integral_op += flash.pe_v[k]; 

    // Create multimap to hold the largest amplitudes in the first slots of map
    // Normalize each PE bin to the total # of PEs-- 1/x to put highest amps in front
    std::multimap<double,int> ampToOpDet ;

    for(size_t k=0; k<32; k++)
        ampToOpDet.emplace(1/(flash.pe_v[k]/integral_op),k);

    // Now that opdet hits are normalized and ordered, store indices for PMTs which 
    // had the greatest hit amplitudes up to _percent of total PMT hits
    double opAmpTotal = 0;
    std::vector<int> ids ;
    ids.resize(0);

    for( auto const & e : ampToOpDet ){
        opAmpTotal += 1/e.first ;
        ids.push_back(e.second) ;
        if( opAmpTotal > _percent )
            break;
        }

    // Prepare the return values (Mostly QWeightPoint)
    FlashMatch_t f;
    QCluster_t flash_hypothesis_v;
    if(pt_v.empty()){ std::cout<<"Not enough points!" <<std::endl ;  return f;}
    
    _vis_array = {0};
    _vis_array.resize(32,0);

   for(double x_step_size=1; x_step_size<250; x_step_size+=_x_step_size) {
      QPoint_t pt; 

      pt.x = pt.y = pt.z = pt.q = 0;
      double weight_tot = 0;
      QPoint_t min_pt = pt_v[0];

      for(auto const& tpc_pt : pt_v)
        if(min_pt.x < tpc_pt.x) min_pt = tpc_pt;

      if(_use_library) {

        auto const& lib = ::phot::PhotonVisibilityService::GetME();
        double weight = 0;
        for(auto const& tpc_pt : pt_v) {

	    for(size_t i=0; i<lib.NOpChannels(); ++i){
	      weight += tpc_pt.q * lib.GetVisibility(tpc_pt.x,tpc_pt.y,tpc_pt.z,i);
	      _vis_array[i] += tpc_pt.q * lib.GetVisibility(tpc_pt.x,tpc_pt.y,tpc_pt.z,i) ;
	      }

          pt.x += (tpc_pt.x - min_pt.x + x_step_size) * weight;
          pt.y += (tpc_pt.y * weight);
          pt.z += (tpc_pt.z * weight);
          weight_tot += weight;
	  }

        pt.x /= weight_tot;
        pt.y /= weight_tot;
        pt.z /= weight_tot;

	flash_hypothesis_v.push_back(pt); 

	// Normalize by the sum of all hits, weight
	for(size_t i=0; i<lib.NOpChannels(); i++)
	    _vis_array[i] /= weight; 
	
	// Calculate amplitudes corresponding to max opdet amplitudes
	double visAmpTotal = 0;
	for(size_t i=0; i<ids.size(); i++)
	    visAmpTotal += _vis_array[ids[i]] ;

        double ratio = 0;
        if(opAmpTotal > visAmpTotal )
            ratio = visAmpTotal/opAmpTotal ;
        else
            ratio = opAmpTotal/visAmpTotal ;
    
        if(ratio > maxRatio ){	
	   maxRatio = ratio;
    	   maxID    = x_step_size-1 ;
    	   }

	  }
      }//x loop

    // If min-diff is bigger than assigned max, return default match (score<0)
    if( maxRatio < _score ) 
	return f;

    // Assign the score, return
    f.score = maxRatio;
    f.tpc_point = flash_hypothesis_v[maxID];

    return f;
  }


}
#endif
