#ifndef STARTPOINT3DMODULE_CXX
#define STARTPOINT3DMODULE_CXX

#include "StartPoint3DModule.h"

#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"
#include "LArUtil/DetectorProperties.h"

namespace showerreco {

  void StartPoint3DModule::do_reconstruction( const ::protoshower::ProtoShower & proto_shower,
					      Shower_t& resultShower)
{

    //if the module does not have 2D cluster info -> fail the reconstruction
    if (!proto_shower.hasCluster2D()) {
        std::stringstream ss;
        ss << "Fail @ algo " << this->name() << " due to missing 2D cluster";
        throw ShowerRecoException(ss.str());
    }

    auto & clusters = proto_shower.params();

    /**
    *  Use 2 best planes to calculate Y, Z of start point.
    *  Use their time average to calculate X.
    *  First calculate the worst plane and don't use that guy.
    */
    float minClusDist      =  999999 ;
    int worstPlane  = -1      ;
    int planeTemp   =  0      ;

    if ( clusters.size() > 2 ) {
        for ( auto const c : clusters ) {
            float distTemp = abs ( c.start_point.w - c.end_point.w );

            if ( distTemp < minClusDist ) {
                minClusDist = distTemp ;
                worstPlane = planeTemp;
            }

            planeTemp ++ ;
        }
    }

    /**
    *  Store planes and wire start, calculate average time for best 2 clusters.
    */
    double sX = 0 ;
    double sY ;
    double sZ ;

    std::vector<int> wireStarts(0) ;
    std::vector<int> planes(0) ;
    
    auto geom  = larutil::Geometry::GetME();
    auto geomH = larutil::GeometryHelper::GetME();

    for ( auto const c : clusters ) {

        if ((int) c.plane_id.Plane != worstPlane) {
            wireStarts.emplace_back(
                int(c.start_point.w / geom -> WirePitch(0) ) ) ;
            planes.emplace_back( c.plane_id.Plane ) ;
            sX += c.start_point.t;
        }
    }

    /**
    *  Caluclate intersection point in Y,Z of the 2 best planes.
    *  Calculate their average start time
    */
    if (planes.size() < 2){
      std::stringstream ss;
      ss << "Fail @ algo " << this->name() << " due to too few (1) 2D start points";
      throw ShowerRecoException(ss.str());
    }

    // check start/end point range
    if ( (wireStarts.at(0) < 0) or (wireStarts.at(0) > geom->Nwires(planes.at(0))) or
	 (wireStarts.at(1) < 0) or (wireStarts.at(1) > geom->Nwires(planes.at(1))) ) {
      std::stringstream ss;
      ss << "Fail @ algo " << this->name() << " due to wires out of range";
      throw ShowerRecoException(ss.str());
    } 

    geom->IntersectionPoint(wireStarts.at(0) , wireStarts.at(1) , planes.at(0), planes.at(1), sY, sZ );

    // check if reconstructed start point is outside of TPC volume    
    if (geomH->ContainedYZ(sY, sZ) == false){
      std::stringstream ss;
      ss << "Fail @ algo " << this->name() << " due to start point reconstructed out of TPC bouns";
      throw ShowerRecoException(ss.str());
    }

    resultShower.fXYZStart = {sX / 2, sY, sZ} ;

}

} //showerreco

#endif
