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
					   geoalgo::Point_t& vtx,
					   double& distShr, double& distTrk) const
  {
    
    double dot = _geoAlgo.commonOrigin(shr,trk,vtx);

    // Distance from candidate vertex and shower start
    double distShrStart = vtx.Dist(shr.Start());
    // Distance from candidate vertex and track start
    double distTrkStart = vtx.Dist(trk.front());
    // Distance from candidate vertex and track body
    distTrk = sqrt(_geoAlgo.SqDist(trk,vtx));
    

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
    if ( (dot > -0.2) && (dot < 0.2) && ( distTrk < 5) && (distTrkStart > 3) && (distShrStart < 100))
      return true;

    return false;
  }


  bool AlgoFindRelationship::isShowerFromShower(const geoalgo::HalfLine_t& shr1, 
					    const geoalgo::HalfLine_t& shr2,
					    geoalgo::Point_t& vtx,
					    double& distShr1, double& distShr2) const
  {

    double dot = _geoAlgo.commonOrigin(shr1,shr2,vtx);

    distShr1 = vtx.Dist(shr1.Start());
    distShr2 = vtx.Dist(shr2.Start());

    if (_debug) { 
      std::cout << "Shr vs. Trk. Sum of dot products: " << dot << std::endl;
      std::cout << "Shr 1 Start to Vertex: " << distShr1 << std::endl;
      std::cout << "Shr 2 start to Vertex: " << distShr2 << std::endl;
    }    

    return false;
  }


  bool AlgoFindRelationship::isTrackFromTrack(const geoalgo::Trajectory_t& trk1, 
					  const geoalgo::Trajectory_t& trk2,
					  geoalgo::Point_t& vtx,
					  double& distTrk1, double& distTrk2) const
  {

    double dot = _geoAlgo.commonOrigin(trk1,trk2,vtx);
    
    distTrk1 = vtx.Dist(trk1.front());
    distTrk2 = vtx.Dist(trk2.front());

    if (_debug) { 
      std::cout << "Trk vs. Trk. Sum of dot products: " << dot << std::endl;
      std::cout << "Trk 1 Start to Vertex: " << distTrk1 << std::endl;
      std::cout << "Trk 2 start to Vertex: " << distTrk2 << std::endl;
    }    

    return false;
  }


  bool AlgoFindRelationship::haveCommonOrigin(const geoalgo::HalfLine_t& shr1, 
					      const geoalgo::HalfLine_t& shr2,
					      geoalgo::Point_t& vtx,
					      double& distShr1, double& distShr2) const
  {

    // Because we want to see if they have a common origin, the geo objects
    // should be projected only backwards (hence the last "true" argument)
    double dot = _geoAlgo.commonOrigin(shr1,shr2,vtx,true);

    distShr1 = vtx.Dist(shr1.Start());
    distShr2 = vtx.Dist(shr2.Start());

    if (_debug) {
      std::cout << "Shr vs. Trk. Sum of dot products: " << dot << std::endl;
      std::cout << "Shr 1 Start to Vertex: " << distShr1 << std::endl;
      std::cout << "Shr 2 start to Vertex: " << distShr2 << std::endl;
    }

    // We want the dot value to be close to 2.
    // Since we are dealing with showers, the vertex should be reasonably close
    // to the shower start points (a few radiation lengths away)
    if ( (dot > 1.8) && (distShr1 < 100) && (distShr2 < 100) )
      return true;

    return false;
  }


  bool AlgoFindRelationship::haveCommonOrigin(const geoalgo::Trajectory_t& trk1, 
					      const geoalgo::Trajectory_t& trk2,
					      geoalgo::Point_t& vtx,
					      double& distTrk1, double& distTrk2) const
  {

    // Because we want to see if they have a common origin, the geo objects
    // should be projected only backwards (hence the last "true" argument)
    double dot = _geoAlgo.commonOrigin(trk1,trk2,vtx,true);

    distTrk1 = vtx.Dist(trk1.front());
    distTrk2 = vtx.Dist(trk2.front());

    if (_debug) {
      std::cout << "Shr vs. Trk. Sum of dot products: " << dot << std::endl;
      std::cout << "Trk 1 Start to Vertex: " << distTrk1 << std::endl;
      std::cout << "Trk 2 start to Vertex: " << distTrk2 << std::endl;
    }

    // We want the dot value to be close to 2.
    // Since we are dealing with showers, the vertex should be close
    // to the start point of both tracks
    if ( (dot > 1.8) && (distTrk1 < 5) && (distTrk2 < 5) )
      return true;

    return false;
  }


  bool AlgoFindRelationship::haveCommonOrigin(const geoalgo::Trajectory_t& trk, 
					      const geoalgo::HalfLine_t&   shr,
					      geoalgo::Point_t& vtx,
					      double& distShr, double& distTrk) const
  {

    // Because we want to see if they have a common origin, the geo objects
    // should be projected only backwards (hence the last "true" argument)
    double dot = _geoAlgo.commonOrigin(trk,shr,vtx,true);

    distTrk = vtx.Dist(trk.front());
    distShr = vtx.Dist(shr.Start());

    if (_debug) {
      std::cout << "Shr vs. Trk. Sum of dot products: " << dot << std::endl;
      std::cout << "Trk Start to Vertex: " << distTrk << std::endl;
      std::cout << "Shr start to Vertex: " << distShr << std::endl;
    }

    // We want the dot value to be close to 2.
    // Since we are dealing with showers, the vertex should be close
    // to the start point of the track and within a few radiation
    // lengths away from the shower start
    if ( (dot > 1.8) && (distTrk < 5) && (distShr < 100) )
      return true;

    return false;
  }

};

#endif
