#ifndef STARTPOINT3DMODULE_CXX
#define STARTPOINT3DMODULE_CXX

#include "StartPoint3DModule.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"

namespace showerreco {

void StartPoint3DModule::do_reconstruction(const ShowerClusterSet_t & inputShowers, Shower_t & resultShower) {

	std::cout<<"Is anything happeing? "<<std::endl ;

	// This function takes the shower cluster set and computes the best fit 3D start point
	// and then assigns it to the shower

//	auto geom = larutil::Geometry::GetME();
	auto geomHelper = larutil::GeometryHelper::GetME();

	//Inside axis calculation, stored a point on the axis line as the start point for reference/adjustment 
	auto pointOnAxis = resultShower.fXYZStart ;
	auto axis 	 = resultShower.fDCosStart ; 



	//Loop over matched clusters, pick 2 best planes by finding worse plane 
	float dist      =  999999 ;
	int worstPlane  = -1      ; 
	int planeTemp   =  0      ;

	for( auto const c : inputShowers ){
	    float distTemp = abs ( c.start_point.w - c.end_point.w );

	    if( distTemp < dist ){
		dist = distTemp ;
		worstPlane = planeTemp;
		}

	    planeTemp ++ ;
	    }


	std::vector<TVector3> pointsVector ;
	std::vector<float>    errors ;

	int minErrorIt = -1 ; //Keep track of the iterator with smallest error 
	int minError   =  99999999 ; 

        generatePoints(pointOnAxis, 100, resultShower,pointsVector);

	//Look at the projection of the point onto the 2 best planes in 2D
	for( int i = 0 ; i < pointsVector.size() ; i++ ){

	    int multError  =  0 ;

	    for( auto const c : inputShowers ){
		if ( c.plane_id.Plane != worstPlane ){

		    auto testPoint = geomHelper->Point_3Dto2D(pointsVector[i],c.plane_id.Plane) ;

		    //Dist is the distance from the cluster's 2D start to this new projection point
		    //Use this distance to calculate an error in the chosen start point
		    float dist = sqrt ( pow(c.start_point.t - testPoint.t,2) + 
					pow(c.start_point.w - testPoint.w,2) ) ;


		    multError += dist ;
		    }
		
		}

		multError /= 2; 
		errors.emplace_back(multError) ;

		if ( multError < minError ){
		    minErrorIt = i ;
		    minError   = multError ;
		    }
	
	    }

    resultShower.fXYZStart = pointsVector[minErrorIt] ;
    std::cout<<"This is the error! "<<minError<<" , "<<minErrorIt<<std::endl ;
    
    // Temporary: Make sure I can store the result appropriately
    //resultShower.fXYZStart = TVector3(1., 2., 3.);

    }

bool StartPoint3DModule::inTPC(const TVector3 & pt ){

    if ( pt[0] < 0      || pt[0] > 256.35 || 
	 pt[1] < -116.5 || pt[1] > 116.5  ||
	 pt[2] < 0      || pt[2] > 1036.8  )	

	 return false ;

    else 
	return true; 

    }

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
