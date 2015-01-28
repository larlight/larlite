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


  bool AlgoFindRelationship::isShowerFromTrack(const geoalgo::HalfLine_t& shr, 
					   const geoalgo::Trajectory_t& trk,
					       geoalgo::Point_t& vtx) const
  {
    
    double dot = _geoAlgo.commonOrigin(shr,trk,vtx);

    // Distance from candidate vertex and shower start
    double distShrStart = vtx.Dist(shr.Start());
    // Distance from candidate vertex and track start
    double distTrkStart = vtx.Dist(trk.front());
    // Distance from candidate vertex and track body
    double distTrk = sqrt(_geoAlgo.SqDist(trk,vtx));
    

    if (_debug) { 
      std::cout << "Shr vs. Trk. Sum of dot products: " << dot << std::endl;
      std::cout << "Shr start to Vertex: " << distShrStart << std::endl;
      std::cout << "Trk start to Vertex: " << distTrkStart << std::endl;
      std::cout << "Trk body to Vertex : " << distTrk << std::endl;
    }    

    // Let's decide!
    // We want the dot product between the shr direction & the vtx -> shr start == 1  (means shower is coming from vertex)
    // We want the dot product between the trk direction & the vtx -> trk start == -1 (means vtx somewhere on trk body)
    // this means the dot parameter should be close to 0
    // We also want the vertex to be near to the track, but far from the track start point
    // If the vertex is too close to the track start point it may be that these are sister particles!
    // Finally place a gentle cut on shr to vertex distance...just so that we don't get too far from the event
    if ( (dot > -0.2) && (dot < 0.2) && ( distTrk < 5) && (distTrkStart > 3) && (distShrStart < 100)){
      if (_debug) { std::cout << "Shower is from Track" << std::endl << std::endl; }
      return true;
    }
    if (_debug) { std::cout << "Shower is not from Track" << std::endl << std::endl; }
    return false;
  }


  bool AlgoFindRelationship::isShowerFromShower(const geoalgo::HalfLine_t& shr1, 
						const geoalgo::HalfLine_t& shr2,
						geoalgo::Point_t& vtx) const
  {
    // Figure out if shower1 comes from shower2
    // if so return true

    double dot = _geoAlgo.commonOrigin(shr1,shr2,vtx);

    double distShr1Start = vtx.Dist(shr1.Start());
    double distShr2Start = vtx.Dist(shr2.Start());
    // calculate distance from shower trunk
    // create a segment that goes up to the shower length
    geoalgo::LineSegment_t shr2Seg(shr2.Start(),shr2.Start()+shr2.Dir()*100);
    double distShr2 = sqrt( _geoAlgo.SqDist(vtx,shr2Seg) );

    if (_debug) { 
      std::cout << "Sum of dot products               : " << dot << std::endl;
      std::cout << "Candidate Daughter Start to Vertex: " << distShr1Start << std::endl;
      std::cout << "Candidate Mother start to Vertex  : " << distShr2Start << std::endl;
      std::cout << "Candidate Mother trunk to Vertex  : " << distShr2 << std::endl;
    }    

    // Require a dot-value close to zero. If so, figure out which shower
    // comes from which
    // Also, require that the vertex not be far from shower 2 (the 
    // candidate mother shower)
    // but not too close to the shower start. Otherwise we might
    // mix up a case in which the showers are siblings (common vertex)
    // And, if vtx closer to start than to trunk -> likely vtx behind. So exclude thse cases
    // Finally, shower1 should not be too far from the vertex
    // (a few radiation lengths)
    if ( (dot > -0.2) && (dot < 0.2) && (distShr2 < 10) && (distShr2Start > 3) && (distShr1Start < 100) && (distShr2Start > distShr2) ){
      // the "mother" shower is the one that is anti-aligned with the
      // reconstructed vertex (the vertex is somewhere along the shower
      // body.
      geoalgo::Vector_t vtx_to_shr2(vtx-shr2.Start());
      vtx_to_shr2.Normalize();
      double dot2 = vtx_to_shr2.Dot(shr2.Dir());
      if (_debug) { std::cout << "Candidate Mother dot-product: " << dot2 << std::endl; }
      // dot2 better be close to -1 (anti-aligned)
      // if so, then because dot is close to 0
      // this automatically checks that the dot product
      // for shower 1 is close to 1, as we would like!
      if ( dot2 < -0.8 ){
	if (_debug) { std::cout << "Shower 1 comes from Shower 2!" << std::endl << std::endl; }
	return true;
      }
    }
    
    if (_debug) { std::cout << "Shower 1 not from Shower 2" << std::endl << std::endl; }
    return false;
  }


  bool AlgoFindRelationship::isTrackFromTrack(const geoalgo::Trajectory_t& trk1, 
					      const geoalgo::Trajectory_t& trk2,
					      geoalgo::Point_t& vtx) const 
  {

    double dot = _geoAlgo.commonOrigin(trk1,trk2,vtx);
    
    double distTrk1Start = vtx.Dist(trk1.front());
    double distTrk2Start = vtx.Dist(trk2.front());
    // Distance from candidate vertex and candidate mother body
    double distTrk2 = sqrt(_geoAlgo.SqDist(trk2,vtx));

    if (_debug) { 
      std::cout << "Trk vs. Trk. Sum of dot products      : " << dot << std::endl;
      std::cout << "Trk Candidate Daughter Start to Vertex: " << distTrk1Start << std::endl;
      std::cout << "Trk Candidate Mother start to Vertex  : " << distTrk2Start << std::endl;
      std::cout << "Trk Candidate Mother trunk to Vertex  : " << distTrk2 << std::endl;
    }    

    // Require a dot-value close to zero. If so, figure out which track
    // comes from which
    // Also, require that the vertex not be far from track 2 (the 
    // candidate mother track)
    // but far enough away from candidate mother start
    // Finally, track1 should be close to the vertex also
    if ( (dot > -0.2) && (dot < 0.2) && (distTrk2 < 10) && (distTrk2Start > 5) && (distTrk1Start < 10) ){
      // the "mother" shower is the one that is anti-aligned with the
      // reconstructed vertex (the vertex is somewhere along the shower
      // body.
      geoalgo::Vector_t vtx_to_trk2(vtx-trk2.front());
      vtx_to_trk2.Normalize();
      double dot2 = vtx_to_trk2.Dot(trk2.Dir());
      if (_debug) { std::cout << "Candidate Mother dot-product: " << vtx_to_trk2 << std::endl; }
      // dot2 better be close to -1 (anti-aligned)
      // if so, then because dot is close to 0
      // this automatically checks that the dot product
      // for shower 1 is close to 1, as we would like!
      if ( dot2 < -0.8 ){
	if (_debug) { std::cout << "Track 1 comes from Track 2!" << std::endl << std::endl; }
	return true;
      }
    }
    
    if (_debug) { std::cout << "Track 1 not from Track 2" << std::endl << std::endl; }
    return false;
  }


  bool AlgoFindRelationship::haveCommonOrigin(const geoalgo::HalfLine_t& shr1, 
					      const geoalgo::HalfLine_t& shr2,
					      geoalgo::Point_t& vtx) const
  {

    // Because we want to see if they have a common origin, the geo objects
    // should be projected only backwards (hence the last "true" argument)
    double dot = _geoAlgo.commonOrigin(shr1,shr2,vtx,true);

    double distShr1 = vtx.Dist(shr1.Start());
    double distShr2 = vtx.Dist(shr2.Start());

    if (_debug) {
      std::cout << "Shr vs. Shr. Sum of dot products: " << dot << std::endl;
      std::cout << "Shr 1 Start to Vertex: " << distShr1 << std::endl;
      std::cout << "Shr 2 start to Vertex: " << distShr2 << std::endl;
    }

    // We want the dot value to be close to 2.
    // Since we are dealing with showers, the vertex should be reasonably close
    // to the shower start points (a few radiation lengths away)
    if ( (dot > 1.8) && (distShr1 < 100) && (distShr2 < 100) ){
      if (_debug) { std::cout << "Showers have common origin!" << std::endl << std::endl; }
      return true;
    }

      if (_debug) { std::cout << "Showers do not have common origin" << std::endl << std::endl; }
    return false;
  }


  bool AlgoFindRelationship::haveCommonOrigin(const geoalgo::Trajectory_t& trk1, 
					      const geoalgo::Trajectory_t& trk2,
					      geoalgo::Point_t& vtx) const
  {

    // Because we want to see if they have a common origin, the geo objects
    // should be projected only backwards (hence the last "true" argument)
    double dot = _geoAlgo.commonOrigin(trk1,trk2,vtx,true);

    double distTrk1 = vtx.Dist(trk1.front());
    double distTrk2 = vtx.Dist(trk2.front());

    if (_debug) {
      std::cout << "Trk. vs. Trk. Sum of dot products: " << dot << std::endl;
      std::cout << "Trk 1 Start to Vertex: " << distTrk1 << std::endl;
      std::cout << "Trk 2 start to Vertex: " << distTrk2 << std::endl;
    }

    // We want the dot value to be close to 2.
    // Since we are dealing with showers, the vertex should be close
    // to the start point of both tracks
    if ( (dot > 1.8) && (distTrk1 < 5) && (distTrk2 < 5) ){
      if (_debug) { std::cout << "Tracks have common origin!" << std::endl << std::endl; }
      return true;
    }

    if (_debug) { std::cout << "Tracks do not have common origin" << std::endl << std::endl; }
    return false;
  }


  bool AlgoFindRelationship::haveCommonOrigin(const geoalgo::Trajectory_t& trk, 
					      const geoalgo::HalfLine_t&   shr,
					      geoalgo::Point_t& vtx) const
  {

    // Because we want to see if they have a common origin, the geo objects
    // should be projected only backwards (hence the last "true" argument)
    double dot = _geoAlgo.commonOrigin(trk,shr,vtx,true);

    double distTrk = vtx.Dist(trk.front());
    double distShr = vtx.Dist(shr.Start());

    if (_debug) {
      std::cout << "Shr vs. Trk. Sum of dot products: " << dot << std::endl;
      std::cout << "Trk Start to Vertex: " << distTrk << std::endl;
      std::cout << "Shr start to Vertex: " << distShr << std::endl;
    }

    // We want the dot value to be close to 2.
    // Since we are dealing with showers, the vertex should be close
    // to the start point of the track and within a few radiation
    // lengths away from the shower start
    if ( (dot > 1.8) && (distTrk < 5) && (distShr < 100) ){
      if (_debug) { std::cout << "Have common origin!" << std::endl << std::endl; }
      return true;
    }

    if (_debug) { std::cout << "Do not have common origin" << std::endl << std::endl; }
    return false;
  }

};

#endif
