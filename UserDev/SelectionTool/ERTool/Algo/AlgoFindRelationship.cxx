#ifndef ERTOOL_FINDRELATIONSHIP_CXX
#define ERTOOL_FINDRELATIONSHIP_CXX

#include "AlgoFindRelationship.h"

namespace ertool{

  AlgoFindRelationship::AlgoFindRelationship() : AlgoBase()
  {
    _name   = "AlgoFindRelationship";
    _debug  = false;
  }
  
  void AlgoFindRelationship::Reset()
  {}


  double AlgoFindRelationship::FindClosestApproach(const geoalgo::HalfLine_t& shr1,
						   const geoalgo::HalfLine_t& shr2,
						   geoalgo::Point_t& vtx) const
  {
    // Find mininum impact parameter between a two showers
    // flip their directions and look backwards
    
    // Create a half-line pointing backwards from the shower
    geoalgo::HalfLine_t shr1Bkwd(shr1.Start(), shr1.Dir()*(-1));
    geoalgo::HalfLine_t shr2Bkwd(shr2.Start(), shr2.Dir()*(-1));
    
    // coordinates for closest points on the two objects
    geoalgo::Point_t PtShr1(3);
    geoalgo::Point_t PtShr2(3);
    double IP = _geoAlgo.SqDist(shr1Bkwd, shr2Bkwd, PtShr1, PtShr2);
    // build candidate vertex
    vtx = (PtShr1+PtShr2)/2.;
    
    return sqrt(IP);
  }


  double AlgoFindRelationship::FindClosestApproach(const geoalgo::HalfLine_t& shr,
						   const geoalgo::Trajectory_t& trk,
						   geoalgo::Point_t& vtx) const
  {
    // Find mininum impact parameter between a shower and a track
    // Search for a point of origin, either common to the two objects
    // or on one of the two objects (that gave rise to the second)
    // along the shower direction, starting from the start point and
    // going backwards along the shower direction
    // *** ADD ALONG SHOWER DIR FOR ITS LENGTH - NOT IMPLEMENTED NOW ***
    // and along the entire segment track as well as backwards along the
    // track direction
    
    // Create a half-line pointing backwards from the shower
    geoalgo::HalfLine_t shrBkwd(shr.Start(), shr.Dir()*(-1));
    
    // 1) Search for whether the shower might originate
    // from the track
    // get IP for this shower and the entire track
    // but constrain the search along the track
    // coordinates for closest points on the two objects
    geoalgo::Point_t PtShr(3);
    geoalgo::Point_t PtTrk(3);
    double IP = _geoAlgo.SqDist(shrBkwd, trk, PtShr, PtTrk);
    // build candidate vertex
    vtx = (PtShr+PtTrk)/2.;
    
    return sqrt(IP);
  }



  double AlgoFindRelationship::FindClosestApproach(const geoalgo::Trajectory_t& trk2,
						   const geoalgo::Trajectory_t& trk1,
						   geoalgo::Point_t& vtx) const
  {
    // Find mininum impact parameter between two tracks
    // Search for a point of origin, either common to the two objects
    // or on one of the two objects (that gave rise to the second)
    // compare the points on one track to the backwards direction of
    // the other searching if A came from B
    // then flip the roles and search if B comes from A

    // holders for closest approach points
    geoalgo::Point_t pt1(3);
    geoalgo::Point_t pt2(3);
    
    // keep track of smallest quantity
    double IP = 1036.;
    // and temporary holders
    double IPtmp = 0;
    geoalgo::Point_t vtxtmp(3);
    geoalgo::Point_t pttmp(3);

    // Search for IP looping through segments of one track
    // and using start-point of other track
    pttmp = _geoAlgo.ClosestPt(trk2.front(), trk1);
    vtxtmp = (trk2.back()+pttmp)/2.;
    IPtmp = pttmp.SqDist(trk2.front());
    if (IPtmp < IP) { IP = IPtmp; vtx = vtxtmp; }
    // same but flip roles of tracks
    pttmp = _geoAlgo.ClosestPt(trk1.front(), trk2);
    vtxtmp = (trk1.back()+pttmp)/2.;
    IPtmp = pttmp.SqDist(trk1.front());
    if (IPtmp < IP) { IP = IPtmp; vtx = vtxtmp; }
    // Search for IP looping through segments of one track
    // and using end point of other track
    pttmp = _geoAlgo.ClosestPt(trk2.back(), trk1);
    vtxtmp = (trk2.back()+pttmp)/2.;
    IPtmp = pttmp.SqDist(trk2.back());
    if (IPtmp < IP) { IP = IPtmp; vtx = vtxtmp; }
    // same but flip roles of tracks
    pttmp = _geoAlgo.ClosestPt(trk1.back(), trk2);
    vtxtmp = (trk1.back()+pttmp)/2.;
    IPtmp = pttmp.SqDist(trk1.back());
    if (IPtmp < IP) { IP = IPtmp; vtx = vtxtmp; }

    return sqrt(IP);
  }


};

#endif
