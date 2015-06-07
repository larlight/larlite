#ifndef ERTOOL_FINDRELATIONSHIP_CXX
#define ERTOOL_FINDRELATIONSHIP_CXX

#include "AlgoFindRelationship.h"

namespace ertool{

  AlgoFindRelationship::AlgoFindRelationship()
  {
    _debug  = false;

    //Track-particle masses (convert to MEV)
    _pi_mass = 1000.* TDatabasePDG().GetParticle(211)->Mass();
    _pr_mass = 1000.* TDatabasePDG().GetParticle(2212)->Mass();
    _mu_mass = 1000.* TDatabasePDG().GetParticle(13)->Mass();
    _ka_mass = 1000.* TDatabasePDG().GetParticle(311)->Mass();
    _uk_mass = 0.;
    _maxIP   = 1.;
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


  double AlgoFindRelationship::FindClosestApproach(const geoalgo::Trajectory_t& trk,
						   const geoalgo::HalfLine_t& shr,
						   geoalgo::Point_t& vtx) const
  {
    
    double IP = FindClosestApproach(shr,trk,vtx);
    
    return IP;
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

    //std::cout << "\t\t\t\tTrk A Start: " << trk1.front() << std::endl; 
    //std::cout << "\t\t\t\tTrk B Start: " << trk2.front() << std::endl; 

    // Search for IP looping through segments of one track
    // and using start-point of other track
    pttmp = _geoAlgo.ClosestPt(trk2.front(), trk1);
    vtxtmp = (trk2.front()+pttmp)/2.;
    IPtmp = pttmp.SqDist(trk2.front());
    //std::cout << "\t\t\t\tTrk A body and Trk B start. IP: " << IPtmp << " Vtx: " << vtxtmp << std::endl; 
    if (IPtmp < IP) { IP = IPtmp; vtx = vtxtmp; }
    // same but flip roles of tracks
    pttmp = _geoAlgo.ClosestPt(trk1.front(), trk2);
    vtxtmp = (trk1.front()+pttmp)/2.;
    IPtmp = pttmp.SqDist(trk1.front());
    //std::cout << "\t\t\t\tTrk B body and Trk A start. IP: " << IPtmp << " Vtx: " << vtxtmp <<  std::endl; 
    if (IPtmp < IP) { IP = IPtmp; vtx = vtxtmp; }
    // Search for IP looping through segments of one track
    // and using end point of other track
    pttmp = _geoAlgo.ClosestPt(trk2.back(), trk1);
    vtxtmp = (trk2.back()+pttmp)/2.;
    IPtmp = pttmp.SqDist(trk2.back());
    //std::cout << "\t\t\t\tTrk A body and Trk B end. IP: " << IPtmp << " Vtx: " << vtxtmp <<  std::endl; 
    if (IPtmp < IP) { IP = IPtmp; vtx = vtxtmp; }
    // same but flip roles of tracks
    pttmp = _geoAlgo.ClosestPt(trk1.back(), trk2);
    vtxtmp = (trk1.back()+pttmp)/2.;
    IPtmp = pttmp.SqDist(trk1.back());
    //std::cout << "\t\t\t\tTrk B body and Trk A end. IP: " << IPtmp << " Vtx: " << vtxtmp <<  std::endl; 
    if (IPtmp < IP) { IP = IPtmp; vtx = vtxtmp; }

    return sqrt(IP);
  }


  RelationType_t AlgoFindRelationship::FindRelation(const geoalgo::HalfLine_t& s1,
						    const geoalgo::HalfLine_t& s2,
						    geoalgo::Point_t& vtx,
						    double& score) const
  {
    
    double IP = FindClosestApproach(s1,s2,vtx);
    if (IP != 0)
      score = 1./IP;
    else
      score = 1e-10;

    if (IP < _maxIP){
      if (_debug) { std::cout << "\tshowers are siblings" << std::endl; }
      return RelationType_t::kSibling;
    }

    return RelationType_t::kUnrelated;
  }


  RelationType_t AlgoFindRelationship::FindRelation(const geoalgo::Trajectory_t& t,
						    const geoalgo::HalfLine_t& s,
						    geoalgo::Point_t& vtx,
						    double& score) const
  {
    
    double IP = FindClosestApproach(t,s,vtx);
    if (IP != 0)
      score = 1./IP;
    else
      score = 1e-10;

    if (IP < _maxIP){

      double vtx_to_t_start = vtx.Dist(t.front());
      double vtx_to_t_body  = sqrt(_geoAlgo.SqDist(vtx,t));
      
      if (_debug) { 
	std::cout << "\tShr Start: " << s.Start() << std::endl
		  << "\tTrk Start: " << t.front() << "\tEnd: " << t.back() << std::endl
		  << "\tVtx to Trk Start: " << vtx_to_t_start << "\t Vtx to Trk Body: " << vtx_to_t_body << std::endl;
      }
      
      // if vertex closer to track start point => siblings
      if ( (vtx_to_t_start <= vtx_to_t_body) || (vtx_to_t_start < 0.1) ){
	if (_debug) { std::cout << "\ttrk-shr are Siblings! HARDCODED NUMBER" << std::endl; }
	return RelationType_t::kSibling;
      }
      // otherwise assume track is parent
      if (_debug) { std::cout << "\tTrack is Shower's parent! HARDCODED NUMBER" << std::endl; }
      return RelationType_t::kParent;

    }
    
    return RelationType_t::kUnrelated;
  }
  
  RelationType_t AlgoFindRelationship::FindRelation(const geoalgo::HalfLine_t& s,
						    const geoalgo::Trajectory_t& t,
						    geoalgo::Point_t& vtx,
						    double& score) const
  {
  
    auto const& rel = FindRelation(t,s,vtx,score);

    if (rel == RelationType_t::kSibling)
      return rel;
    // if track is shower's parent flip
    // order to reflect the fact that
    // return should show relation of
    // 1st object to second
    if (rel == RelationType_t::kParent)
      return RelationType_t::kChild;
    if (rel == RelationType_t::kChild)
      return RelationType_t::kParent;

    return RelationType_t::kUnrelated;
  }
  
  
  RelationType_t AlgoFindRelationship::FindRelation(const geoalgo::Trajectory_t& t1,
						    const geoalgo::Trajectory_t& t2,
						    geoalgo::Point_t& vtx,
						    double& score) const
  {

    double IP = FindClosestApproach(t1,t2,vtx);
    if (IP != 0)
      score = 1./IP;
    else
      score = 1e-10;

    if (IP < _maxIP){

      // if vertex closer to body than to start point
      // of one shower => that shower is parent
      double vtx_to_t1_start = vtx.Dist(t1.front());
      double vtx_to_t1_body  = sqrt(_geoAlgo.SqDist(vtx,t1));
      double vtx_to_t2_start = vtx.Dist(t2.front());
      double vtx_to_t2_body  = sqrt(_geoAlgo.SqDist(vtx,t2));
      
      if (_debug) { 
	std::cout << "\tTrk1 Start: " << t1.front() << "\tEnd: " << t1.back() << std::endl
		  << "\tTrk2 Start: " << t2.front() << "\tEnd: " << t2.back() << std::endl
		  << "\tVtx to Trk1 Start: " << vtx_to_t1_start << "\t Vtx to Trk1 Body: " << vtx_to_t1_body << std::endl
		  << "\tVtx to Trk2 Start: " << vtx_to_t2_start << "\t Vtx to Trk2 Body: " << vtx_to_t2_body << std::endl;
      }
      
      // if vertex very very close to start points
      if ( (vtx_to_t1_start <= 0.1) and (vtx_to_t2_start <= 0.1) ){
	if (_debug) { std::cout << "\ttracks are siblings - HARDCODED NUMBER" << std::endl; }
	return RelationType_t::kSibling;
      }
      // if vertex closer to track start point => siblings
      if ( (vtx_to_t1_start <= vtx_to_t1_body) and (vtx_to_t2_start <= vtx_to_t2_body) ){
	if (_debug) { std::cout << "\ttracks are siblings" << std::endl; }
	return RelationType_t::kSibling;
      }
      else if (vtx_to_t1_start < vtx_to_t2_start){
	if (_debug) { std::cout << "\ttrk1 is child of trk2" << std::endl; }
	return RelationType_t::kParent; // t1 is parent to t2
      }
      if (_debug) { std::cout << "\ttrk2 is child of trk1" << std::endl; }
      return RelationType_t::kChild;
    }// if IP passes cut
      
    return RelationType_t::kUnrelated;
  }


  double AlgoFindRelationship::GetPDG(const Track &trk)
  {

    switch(trk._pid){
    case Track::TrackPartID_t::kUnknown : return -1;
    case Track::TrackPartID_t::kPIDA    : return -1;
    case Track::TrackPartID_t::kProton  : return 2212;
    case Track::TrackPartID_t::kKaon    : return 311;
    case Track::TrackPartID_t::kPion    : return 211;
    case Track::TrackPartID_t::kMuon    : return 13;
    case Track::TrackPartID_t::kTrackPartIDMax : return -1;
    }

    return -1;
  }

};

#endif
