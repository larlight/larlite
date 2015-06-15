/**
 * \file ConicalProjections.h
 *
 * \ingroup NCfilter
 * 
 * \brief Class def header for a class ForceRegions
 *
 * @author ryan
 */

///** \addtogroup Pi0Enhancer

#ifndef RECOTOOL_GEOCONIC_H
#define RECOTOOL_GEOCONIC_H

#include <iostream>
#include <vector>
#include "LArUtil/LArUtilManager.h"
#include "LArUtil/LArUtilBase.h"
#include "LArUtil/PxUtils.h"
#include "LArUtil/LArUtil-TypeDef.h"
#include "LArUtil/TimeService.h"

#include <math.h>       /* Tan */
#define PI 3.14159265358979323846264338327950288419

#include "TVector.h"

#include "DataFormat/hit.h"





namespace larlite {
  /**
     \class geoconic 
     Describe me!
   */
  class geoconic{

  public:

    /// Default constructor
    geoconic(){};

    /// Default destructor
    virtual ~geoconic(){};

	//$ Is a 3D point Contained in TPC
	bool TPCContained( const TLorentzVector& pos);

	//$ Are the 3d Cone Edges Contained in the tpc?
	bool ConeInTPC(const TLorentzVector& Pos, const TLorentzVector& dir, double Length, double OpeningAngle, int smoothness);

	//$ This returns Polygon edges for the cone If you already have the cone points
	std::vector<larutil::PxPoint> ConicalEdge(std::vector<larutil::PxPoint> incone);

	//$ This returns Polygon edges for the cone If With input as
                // 1 Position of vertex 
                // 2 Direction of vertex 
                // 3 Length of cone 
                // 4 Opening Angle 
                // 5 Plane
                // 6 smoothness 
        std::vector<larutil::PxPoint> ConicalFeatures(const TLorentzVector& Pos, const TLorentzVector& dir, double Length, double OpeningAngle, int plane , int smoothness);

        bool ConicalOverlap(const std::vector<larutil::PxPoint> conea,const std::vector<larutil::PxPoint> coneb);
	//$ This returns the hits contained in the polygon
	std::vector<larutil::PxHit> PolyContain(std::vector<larutil::PxHit> hits, std::vector<larutil::PxPoint> polygon );

	//$ This returns the larlite::hits contained in the polygon
	//std::vector<larlite::hit> PolyContainHit(std::vector<larlite::hit> hits, std::vector<larutil::PxPoint> polygon );

	//$ This returns the larlite::hits contained in the polygon
	std::vector<unsigned int> PolyContainHit(std::vector<larlite::hit> hits, std::vector<larutil::PxPoint> polygon , unsigned int plane);

	//$ This tells you if the walk is possible with given params
	bool walkable(const TLorentzVector& Pos, const TLorentzVector& dir, double Length, double OpeningAngle, int plane, int smoothness);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int orientation(larutil::PxPoint p,larutil::PxPoint q, larutil::PxPoint r);
bool onSegment(larutil::PxPoint p, larutil::PxPoint q, larutil::PxPoint r);
bool doIntersect(larutil::PxPoint p1, larutil::PxPoint q1, larutil::PxPoint p2, larutil::PxPoint q2);








	};

}
#endif


