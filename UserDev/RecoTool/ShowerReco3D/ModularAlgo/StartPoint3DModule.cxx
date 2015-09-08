#ifndef STARTPOINT3DMODULE_CXX
#define STARTPOINT3DMODULE_CXX

#include "StartPoint3DModule.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"
#include "LArUtil/GeometryUtilities.h"
#include "LArUtil/DetectorProperties.h"

namespace showerreco {

void StartPoint3DModule::do_reconstruction(const ShowerClusterSet_t & inputShowers, Shower_t & resultShower) {

	std::cout<<"\n\nIn do_reco of 3D start.... "<<std::endl ;

	// This function takes the matched shower cluster set and computes the best fit 3D start point
	// and then assigns it to the shower

//	auto geom = larutil::Geometry::GetME();
	auto geomHelper = larutil::GeometryHelper::GetME();
//	auto w2cm = larutil::GeometryUtilities::GetME()->WireToCm();
//	auto t2cm = larutil::GeometryUtilities::GetME()->TimeToCm();
	auto detProp = larutil::DetectorProperties::GetME() ;

	//Inside axis calculation, stored a point on the axis line as the start point for reference/adjustment 
	auto pointOnAxis = resultShower.fXYZStart ;
	auto axis 	 = resultShower.fDCosStart ; 

	double sW(0.), sT(0.) ;
	

	for( auto const c : inputShowers ){
	    if ( c.plane_id.Plane == 2 ){
		//Let's try to get close to start point on axis.
		//Let's do this by first using the info from plane2,Y 
		sW = c.start_point.w ; // / w2cm ;
		sT = c.start_point.t ; // / t2cm ;

		std::cout<<"*******************sW, sT: "<<sW<<", "<<sT<<std::endl ;
		}
	    }

	double sY = -116.5 ;
	std::vector<TVector3> anchorCoords ;
	TVector3 Temp ;

	for ( int i = 0 ; i < 232 ; i++ ){
	    sY += 1 ;
	    Temp.SetX(sT) ; 
	    Temp.SetY(sY) ; 
	    Temp.SetZ(sW) ; 

	    anchorCoords.emplace_back(Temp) ;
	    }
	
	//To be done once some anchor point is had. 
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
	std::vector<float>    errors (0) ;

	//Create List of N points up y across the detector using this time, Z info.  
       // generatePoints(pointOnAxis, 100, resultShower,pointsVector);

	//Store plane of interest
	int plane = -1;
	bool forward   = false ;
	bool backwards = false ; 
	Point2D projStart, projDir ;
	Point2D projStartNeg, projDirNeg ;

	int minErrorIt = -1 ; //Keep track of the iterator with smallest error 
	int minError   =  99999999 ; 

	//Look at the projection of the point onto the 2 best planes in 2D
	//Loop through the line of points in anchorCoords and test each roughly. 
	for( int i = 0 ; i < anchorCoords.size() ; i++ ){

	    float multError  =  0 ;
	    float minAngle = 999999 ; 
	    float dist = 0 ; 


	    for( auto const c : inputShowers ){
		if ( c.plane_id.Plane != worstPlane && c.plane_id.Plane != 2){
		    
		    plane = c.plane_id.Plane ;

		    //testPoint is a PxPoint, 2D
		    auto testPoint = geomHelper->Point_3Dto2D(anchorCoords[i],c.plane_id.Plane) ;


		    //Dist is the distance from the cluster's 2D start to this new projection point
		    //Use this distance to calculate an error in the chosen start point
		    dist = sqrt ( pow(c.start_point.t - testPoint.t,2) + 
			    	  pow(c.start_point.w - testPoint.w,2) ) ;

		    //std::cout<< " In TPC? "<<inTPC(anchorCoords[i])<<", "<<anchorCoords[i].X()
		    //	     <<", "<<anchorCoords[i].Y()<<", "<<anchorCoords[i].Z()<<std::endl;
		    geomHelper->Line_3Dto2D(anchorCoords[i],axis,c.plane_id.Plane,projStart,projDir) ;
		    geomHelper->Line_3Dto2D(anchorCoords[i],-axis,c.plane_id.Plane,projStartNeg,projDirNeg) ;
		    auto clusDir = c.principal_dir ;
		    clusDir[0] /= sqrt ( pow(clusDir[0],2) + pow(clusDir[1],2) ) ;
		    clusDir[1] /= sqrt ( pow(clusDir[0],2) + pow(clusDir[1],2) ) ;

		    //Both vectors are normalized-- the angle between them is thus the dot product
		    auto cosAngle = clusDir[0] * projDir.t + clusDir[1] * projDir.w ;
		    auto cosAngleNeg = clusDir[0] * projDirNeg.t + clusDir[1] * projDirNeg.w ;
    
		    if ( cosAngle < minAngle ){
			minAngle = cosAngle ;
			forward   = true  ;
			backwards = false ;
			}

		    if ( cosAngleNeg < minAngle ){
			minAngle = cosAngleNeg ;
			forward   = false ;
			backwards = true  ;
			}
		    }

		}
		multError = sqrt ( pow(minAngle,2) + pow(dist,2) ) ;

//		errors.emplace_back(multError) ;
//
		if ( multError < minError ){
		    minErrorIt = i ;
		    minError   = multError ;
		    }
	
	    }


    resultShower.fXYZStart = anchorCoords[minErrorIt] ;
//    std::cout<<"This is the error! "<<minError<<" , "<<minErrorIt<<std::endl ;
    
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
