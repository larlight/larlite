#ifndef DQDXMODULE_CXX
#define DQDXMODULE_CXX

#include "dQdxModule.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"

namespace showerreco{

  dQdxModule::dQdxModule()
  {
    _name = "dQdxModule";
  }

  void dQdxModule::do_reconstruction(const ShowerClusterSet_t & inputShowers, Shower_t & resultShower){
    

    auto geom = larutil::Geometry::GetME();
    auto geomHelper = larutil::GeometryHelper::GetME();
    
    // get the 3D direction reconstructed hopefully in a previous step
    auto const& dir3D = resultShower.fDCosStart;

    // loop over all input cluster -> calculate a dQdx per plane
    for (size_t n=0; n < inputShowers.size(); n++){
      
      // get the hits associated with this cluster
      auto const& hits = inputShowers.at(n).hit_vector;
      
      // get the plane associated with this cluster
      auto const& pl = inputShowers.at(n).plane_id.Plane;

      // get the start point for this cluster
      auto const& start = inputShowers.at(n).start_point;

      // get the showering point for this cluster
      auto const& shr_start = inputShowers.at(n).showering_point;

      // start direction
      auto const& start_dir = inputShowers.at(n).start_dir;

      //*** we need to calculate a pitch for this plane
      double pitch = geomHelper->GetPitch(dir3D,(int)pl);

      // BEGIN ALGORITHM DEVELOPMENT
      
      // STEP 1 : find list of hits that are along the trunk of
      // the shower. Find these hits by looking around the segment
      // connecting the start point and the beginning of the
      // showering point along the start_dir direction

      double trunk_length = sqrt ( (shr_start.w - start.w) * (shr_start.w - start.w) + 
				   (shr_start.t - start.t) * (shr_start.t - start.t) );

    }

    return;
  }


} //showerreco

#endif
