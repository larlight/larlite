#ifndef STARTPOINT3DMODULE_CXX
#define STARTPOINT3DMODULE_CXX

#include "StartPoint3DModule.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"
#include "LArUtil/GeometryUtilities.h"
#include "LArUtil/DetectorProperties.h"

namespace showerreco {

  void StartPoint3DModule::do_reconstruction(const ShowerClusterSet_t & inputShowers, Shower_t & resultShower) {
    
//    if (_verbose)
      std::cout<<"\n\nIn do_reco of 3D start.... "<<std::endl ;
    
    // This function takes the matched shower cluster set and computes the best fit 3D start point
	// and then assigns it to the shower
	auto geomHelper = larutil::GeometryHelper::GetME();
	auto geom       = larutil::Geometry::GetME();
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
      *  2*abs(sY) = length of detector in Y.  _res < 1-- /_res divides detector up into 
      *  _res sized steps.
      */

	for ( int i = 0 ; i < 2*116.5/_res ; i++ ){

	    sY += _res ;
//	    std::cout<<"Y coord: "<<sY<<std::endl;
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

	    std::cout<<"Cluster distance: "<<distTemp<<std::endl ;

	    planeTemp ++ ;
	    }


//	double sX = 0 ;
//	double sY ; 
//	double sZ ; 
//
//	std::vector<int> wireStarts(0) ;
//	std::vector<int> planes(0) ;
//	
//	for( auto const c : inputShowers ){
//	    
//	    if(c.plane_id.Plane != worstPlane){
//		wireStarts.emplace_back( int(c.start_point.w/0.3) ) ;
//		planes.emplace_back( c.plane_id.Plane ) ;
//		sX += c.start_point.t;	
//		}
//	}
//    
//	//if ( 
//	geom->IntersectionPoint(wireStarts[0],wireStarts[1],planes[0],planes[1],sY,sZ ); 
//	    anchorCoords.push_back({sX/2,sY,sZ}); 
	    
//	else
//	    anchorCoords.push_back({sX/2,0,0});



	std::cout<<"Worst Plane: "<<worstPlane<<std::endl ;

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

	    if ( c.plane_id.Plane != worstPlane ){
		std::cout<<"\nPlane: "<<c.plane_id.Plane
			 <<"\nTimeS: "<<c.start_point.t
			 <<"\nTimeE: "<<c.end_point.t
			 <<"\nWireS: "<<c.start_point.w
			 <<"\nWireE: "<<c.end_point.w<<std::endl ;
		} 

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
  //  resultShower.fXYZStart = anchorCoords[0] ;
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
