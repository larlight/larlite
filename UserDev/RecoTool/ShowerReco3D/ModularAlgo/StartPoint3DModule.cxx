#ifndef STARTPOINT3DMODULE_CXX
#define STARTPOINT3DMODULE_CXX

#include "StartPoint3DModule.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"
#include "LArUtil/GeometryUtilities.h"
#include "LArUtil/DetectorProperties.h"

namespace showerreco {

  void StartPoint3DModule::do_reconstruction(const ShowerClusterSet_t & inputShowers, Shower_t & resultShower) {
    
    if (_verbose)
      std::cout<<"\n\nIn do_reco of 3D start.... "<<std::endl ;
    
    // This function takes the matched shower cluster set and computes the best fit 3D start point
	// and then assigns it to the shower
	auto geomHelper = larutil::GeometryHelper::GetME();
	auto detProp = larutil::DetectorProperties::GetME() ;

	//Inside axis calculation, stored a point on the axis line as the start point for reference/adjustment 
	auto pointOnAxis = resultShower.fXYZStart ;
	auto axis 	 = resultShower.fDCosStart ; 

	double sW;
	double sT;

	/**
	*  Let's try to get close to 3D start by using 2D start info.
	*  Let's do this by first using the info from plane2,Y 
	*/
	for( auto const c : inputShowers ){
	    if ( c.plane_id.Plane == 2 ){
		sW = c.start_point.w ; 
		sT = c.start_point.t ; 
		}
	    }

	double sY = -116.5 ;
	std::vector<TVector3> anchorCoords ;
	TVector3 Temp ;
	
	/**
	*  Create List of N points up y across the detector using this time, Z info.  
	*  Loop over all showers in event, build up list of start points for each	
	*/
	for ( int i = 0 ; i < 464; i++ ){

	    sY += 0.5 ;
	    Temp.SetX(sT) ; 
	    Temp.SetY(sY) 	 ; 
	    Temp.SetZ(sW) ; 

	    anchorCoords.emplace_back(Temp) ;
	    }

	/**
	*  Once anchor points are stored, loop over matched clusters, pick 2 best
	*  planes by finding worse plane. Will use 2 best planes as 3D->2D projection
	*  comparisons.
	*/
	float minClusDist      =  999999 ;
	int worstPlane  = -1      ; 
	int planeTemp   =  0      ;

	for( auto const c : inputShowers ){
	    float distTemp = abs ( c.start_point.w - c.end_point.w );

	    if( distTemp < minClusDist ){
		minClusDist = distTemp ;
		worstPlane = planeTemp;
		}

	    planeTemp ++ ;
	    }

	std::vector<TVector3> pointsVector ;

	int plane = -1;
	int minDistIt = -1 ; //Keep track of the iterator with smallest error 
	double minDist   =  99999999. ; 

	/**
	*  Look at the projection of each 3D point onto the 2 best planes in 2D
	*  Loop through the line of points in anchorCoords and test each roughly. 
	*  Dist is the distance from the cluster's 2D start to this new projection point
	*  Use this distance to calculate an error in the chosen start point
	*/
	for( auto const & c : inputShowers ){

	    float dist = 0 ; 

	    if ( c.plane_id.Plane != worstPlane && c.plane_id.Plane != 2){
		for( int i = 0 ; i < anchorCoords.size() ; i++ ){

		    plane = c.plane_id.Plane ;
		    //testPoint is a PxPoint, 2D
		    auto testPoint = geomHelper->Point_3Dto2D(anchorCoords[i],plane) ;

		    dist = sqrt ( pow(c.start_point.t - testPoint.t,2) + 
			    	  pow(c.start_point.w - testPoint.w,2) ) ;

		    if ( dist < minDist ){
			minDistIt = i ;
		    	minDist   = dist ; 
		    	}
		    else
			break;
		    }
		}
	    }

    resultShower.fXYZStart = anchorCoords[minDistIt] ;
//    std::cout<<"min dist : "<<minDist<<std::endl ;

}

bool StartPoint3DModule::inTPC(const TVector3 & pt ){

    if ( pt[0] < 0      || pt[0] > 256.35 || 
	 pt[1] < -116.5 || pt[1] > 116.5  ||
	 pt[2] < 0      || pt[2] > 1036.8  )	

	 return false ;

    else 
	return true; 

    }

/**
*  Not used right now-- leaving this here just in case I need it again
*/
void StartPoint3DModule::generatePoints(TVector3 & seedPoint, int nSteps, const Shower_t & s, std::vector<TVector3> & result){

    //create vector of ordered points. Doesn't matter which direction vector moves in, so long as 
    //the points are ordered, things should work
    auto axis = s.fDCosStart ;
    auto growPoint3D   = seedPoint ;
    auto shrinkPoint3D = seedPoint ;
    bool grow(true), shrink(true) ;

    for ( int i = 0; i < nSteps ; i++ ){ 

	//Get rid of hardcoded 10.
	growPoint3D   += 10*axis ;
	shrinkPoint3D -= 10*axis ;

	if ( inTPC( growPoint3D ) )
	    result.emplace_back( growPoint3D ) ;
	else grow = false; 

	if ( inTPC( shrinkPoint3D ) )
	    result.insert( result.begin(), shrinkPoint3D ) ;
	else shrink = false; 

        if( !grow && !shrink ) 
	    break ;
	}
    }

} //showerreco

#endif
