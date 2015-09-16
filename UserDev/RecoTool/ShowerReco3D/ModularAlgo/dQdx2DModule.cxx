#ifndef DQDX2DMODULE_CXX
#define DQDX2DMODULE_CXX

#include <iomanip>

#include "dQdx2DModule.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"

namespace showerreco{

  void dQdx2DModule::do_reconstruction(const ShowerClusterSet_t & inputShowers, Shower_t & resultShower){
    // This function takes the shower cluster set and computes the dQdx in the beginning of the shower
    // and then assigns it to the shower

    auto geomHelper = larutil::GeometryHelper::GetME();
    
    if(inputShowers.size() > 3){std::cout << "da fuck" << std::endl;}
    
    for(auto const & cluster : inputShowers){
      
      // measure the distance between the start point and showering point
      double dist = 8;//sqrt(geomHelper->Get2DDistanceSqrd(cluster.start_point,cluster.showering_point));      
      
      std::cout << "Dist " << dist << std::endl;
      
      // get the plane associated with this cluster
      auto const& pl = cluster.plane_id.Plane;
      
      // set the energy for this plane
      resultShower.fShoweringLength[pl] = dist;
      
      
    }
    
    
    
    return;
    
  }


} //showerreco

#endif
