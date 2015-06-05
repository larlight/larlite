#ifndef ERTOOL_ALGOPRIMARYFINDER_CXX
#define ERTOOL_ALGOPRIMARYFINDER_CXX

#include "AlgoPrimaryFinder.h"

namespace ertool {

  AlgoPrimaryFinder::AlgoPrimaryFinder() : AlgoBase()
  {

    _name       = "AlgoPrimaryFinder";
    _verbose = false;
    _useRadLength = false;

  }

  void AlgoPrimaryFinder::Reset(){}
  
  void AlgoPrimaryFinder::ProcessBegin()
  {
    return;
  }

  void AlgoPrimaryFinder::LoadParams(std::string fname, size_t version)
  {
    return;
  }

  bool AlgoPrimaryFinder::Reconstruct(const EventData &data, ParticleGraph& graph){
    
    if (_verbose) { 
      std::cout << "*********** BEGIN PrimaryFinder RECONSTRUCTION ************" << std::endl;
    }

    // This Reconstruction stage is divided in two independent parts:
    // 1) Reconstruct primary showers
    // 2) Reconstruct primary tracks
    // Stages are separated due to the different conditions required
    // to determine if a shower/track is primary

    // make a copy of EventData -> to fix!!!
    auto datacpy = data;

    // get node combinations
    //combos = graph.GetNodeCombinations(2,
    

    // Find primary showers
    for (auto const& s : graph.GetParticleNodes(RecoType_t::kShower)){
      
      // default: the shower is primary
      // if we find an indication of the
      // contrary change state accordingly
      bool primary = true;

      auto const& thisID = graph.GetParticle(s).RecoID();
      auto const& thisShower = datacpy.Shower(thisID);
      
      // Ok, we have a shower.
      // make sure it does not:
      // 1) come from another shower
      // 2) come from a track

      if (_verbose) { std::cout << "This shower: (" << thisID << ")" << "\tE: " << thisShower._energy << std::endl; }
      // loop over other showers and check 1) and 2)
      for (auto const& p2 : graph.GetParticleNodes(RecoType_t::kShower)){

	auto const& thatID = graph.GetParticle(p2).RecoID();

	// make sure we don't use the same shower or repeat search
	if (thatID == thisID) continue;

	if (_verbose) { std::cout << "Comparing with shower (" << thisID << ")" << std::endl; }
	auto const& thatShower = datacpy.Shower(thatID);

	if (From(thisShower,thatShower)){
	  if (_verbose) { std::cout << "shower not primary" << std::endl; }
	  primary = false;
	  // exit the loop...no need to search further
	  break;
	}
      }//loop over showers
      
      // now loop over tracks if still primary
      if (!primary)
	continue;

      // loop over other tracks
      for (auto const& t : graph.GetParticleNodes(RecoType_t::kTrack)){

	auto const& thatID = graph.GetParticle(t).RecoID();
	auto const& thatTrack = datacpy.Track(thatID);

	if (thatTrack.size() < 2)
	  continue;
	if (_verbose) { std::cout << "Comparing with track (" << t << ")" << std::endl; }

	if (From(thisShower,thatTrack)){
	  if (_verbose) { std::cout << "shower not primary" << std::endl; }
	  primary = false;
	  // exit the loop...no need to search further
	  break;
	}
      }// for all tracks
      
      // if still primary append to list of primaries
      // create new particle
      if (primary){
	// set particle as primary
	if (_verbose) { std::cout << "shower is primary!" << std::endl; }
	graph.SetPrimary(s);
	graph.GetParticle(s).SetParticleInfo(graph.GetParticle(s).PdgCode(),
					     graph.GetParticle(s).Mass(),
					     thisShower.Start(),
					     graph.GetParticle(s).Momentum());
      }
      
    }// end loop through all showers


    // Find primary tracks
    for (auto const& t : graph.GetParticleNodes(RecoType_t::kTrack)){

      // default: the track is primary
      // if we find an indication of the
      // contrary change state accordingly
      bool primary = true;

      auto const& thisID = graph.GetParticle(t).RecoID();
      auto const& thisTrack = datacpy.Track(thisID);    

      if (thisTrack.size() < 2)
	continue;
      
      if (_verbose) { std::cout << "This Track: (" << thisID << ")" << "\tE: " << thisTrack._energy << std::endl; }

      // loop over other showers and check 1) and 2)
      for (auto const& p2 : graph.GetParticleNodes(RecoType_t::kShower)){

	auto const& thatID = graph.GetParticle(p2).RecoID();
	auto const& thatShower = datacpy.Shower(thatID);
	if (_verbose) { std::cout << "Comparing with shower (" << thatID << ")" << std::endl; }

	if (From(thisTrack,thatShower)){
	  if (_verbose) { std::cout << "track not primary" << std::endl; }
	  primary = false;
	  // exit the loop...no need to search further
	  break;
	}	  
      }// loop over all showers
      
      // now loop over tracks if still primary
      if (!primary)
	continue;
      
      // loop over other tracks
      for (auto const& t : graph.GetParticleNodes(RecoType_t::kTrack)){

	auto const& thatID = graph.GetParticle(t).RecoID();
	auto const& thatTrack = datacpy.Track(thatID);
	if (thatTrack.size() < 2)
	  continue;
	if (thatID == thisID)
	  continue;
	
	if (_verbose) { std::cout << "Comparing with track (" << t << ")" << std::endl; }

	if (From(thisTrack,thatTrack)){
	  if (_verbose) { std::cout << "track not primary" << std::endl; }
	  primary = false;
	  // exit the loop...no need to search further
	  break;
	}	  
      }// for all tracks
      
      // if still primary append to list of primaries
      // create new particle
      if (primary){
	// set particle as primary
	if (_verbose) { std::cout << "track is primary!" << std::endl; }
	graph.SetPrimary(t);
	graph.GetParticle(t).SetParticleInfo(graph.GetParticle(t).PdgCode(),
					     graph.GetParticle(t).Mass(),
					     thisTrack.front(),
					     graph.GetParticle(t).Momentum());
      }
      
    }// loop over all track to find primaries
    
    return true;
  }

  void AlgoPrimaryFinder::ProcessEnd(TFile* fout){
    
    if(fout){
      fout->cd();
    }

    return;
  }

  bool AlgoPrimaryFinder::From(const ::geoalgo::Cone& thisShower,
			       const ::geoalgo::Cone& thatShower) const
  {
    geoalgo::Point_t vtx(3);    
    double thatShowerLength = thatShower.Length();
    // create a segment starting at thatShower start
    // and going for the entire length in shower dir
    ::geoalgo::LineSegment_t thatShowerTrunk(thatShower.Start(),thatShower.Start()+thatShower.Dir()*thatShowerLength);
    double IP = _findRel.FindClosestApproach(thisShower,thatShower,vtx);
    double IPthisStart = vtx.Dist(thisShower.Start());
    double IPthatStart = vtx.Dist(thatShower.Start());
    double IPthatTrunk = sqrt(_geoAlgo.SqDist(vtx,thatShowerTrunk));
    if (_verbose)
      std::cout << "\tImpact Parameter      : " << IP << std::endl
		<< "\tIP to other Shr Start : " << IPthatStart << std::endl
		    << "\tIP to other Shr Trunk : " << IPthatTrunk << std::endl
		<< "\tIP to this Shr Start  : " << IPthisStart << std::endl;
    if ( (IP < _maxIP)                            // good correlation
	 && ( IPthisStart < _vtxToTrkDist)        // this shower's start point close to IP
	 && ( IPthatTrunk < _vtxToTrkDist) )      // vtx close to thatShower's trunk
      {
	// then thisShower comes from thatShower
	return true;
      }
    return false;
  }

  bool AlgoPrimaryFinder::From(const ::geoalgo::Trajectory& thisTrack,
			       const ::geoalgo::Cone& thatShower) const
  {
    geoalgo::Point_t vtx(3);
    double thatShowerLength = thatShower.Length();
    // create a segment starting at thatShower start
    // and going for the entire length in shower dir
    ::geoalgo::LineSegment_t thatShowerTrunk(thatShower.Start(),thatShower.Start()+thatShower.Dir()*thatShowerLength);
    double IP = _findRel.FindClosestApproach(thatShower,thisTrack,vtx);
    double IPthisStart = vtx.Dist(thisTrack.front());
    double IPthatStart = vtx.Dist(thatShower.Start());
    double IPthatTrunk = sqrt(_geoAlgo.SqDist(vtx,thatShowerTrunk));
    if (_verbose)
      std::cout << "\tImpact Parameter : " << IP << std::endl
		<< "\tIP to Shr Start  : " << IPthatStart << std::endl
		<< "\tIP to Shr Trunk  : " << IPthatTrunk << std::endl
		<< "\tIP to Trk Start  : " << IPthisStart << std::endl;
    if ( (IP < _maxIP)                            // good correlation
	 && ( IPthisStart < _vtxToTrkDist)        // this shower's start point close to IP
	 && ( IPthatTrunk < _vtxToTrkDist) )      // vtx close to thatShower's trunk
      return true;
    
    return false;
  }


  bool AlgoPrimaryFinder::From(const ::geoalgo::Cone& thisShower,
			       const ::geoalgo::Trajectory& thatTrack) const
  {
    geoalgo::Point_t vtx(3);
    double IP =  _findRel.FindClosestApproach(thisShower,thatTrack,vtx);
    double IPthisStart = vtx.Dist(thisShower.Start());
    double IPthatStart = vtx.Dist(thatTrack.front());
    double IPtrkBody = sqrt(_geoAlgo.SqDist(vtx,thatTrack));
    if (_verbose)
      std::cout << "\tImpact Parameter: " << IP << std::endl
		<< "\tIP to Trk Start : " << IPthatStart << std::endl
		<< "\tIP to Trk Body  : " << IPtrkBody << std::endl
		<< "\tIP to Shr Start : " << IPthisStart << std::endl;

    if ( (IP < _maxIP)                             // good correlation
	 && (IPthatStart > _vtxToTrkStartDist)     // vertex far enough away from track start
	 && (IPtrkBody < _vtxToTrkDist)            // vertex close to track body
	 && (_IPthisStart < _vtxToShrStartDist) )   // vertex not unreasonably far from shower start
      {
	// our shower comes from t -> not interested
	return true;
      }
    return false;
  }

  bool AlgoPrimaryFinder::From(const ::geoalgo::Trajectory& thisTrack,
			       const ::geoalgo::Trajectory& thatTrack) const
  {
    geoalgo::Point_t vtx(3);
    double IP =  _findRel.FindClosestApproach(thisTrack,thatTrack,vtx);
    double IPthisStart = vtx.Dist(thisTrack.front());
    double IPthatStart = vtx.Dist(thatTrack.front());
    double IPtrkBody = sqrt(_geoAlgo.SqDist(vtx,thatTrack));
    if (_verbose)
      std::cout << "\tImpact Parameter     : " << IP << std::endl
		<< "\tIP to this Trk Start : " << IPthatStart << std::endl
		<< "\tIP to that Trk Body  : " << IPtrkBody << std::endl
		<< "\tIP to this Trk Start : " << IPthisStart << std::endl;
    if ( (IP < _maxIP)                          // good correlation
	 && (IPthatStart > _vtxToTrkStartDist)  // vertex far enough away from track start
	 && (IPtrkBody < _vtxToTrkDist) )       // vertex close to track body
      {
	// our shower comes from t -> not interested
	return true;
      }
    
    return false;
  }

  /// Function that based on primaries in event
  /// returns potential vertices.
  /// Takes start point of each object
  /// and groups together other start points
  /// within _IP of the start point
  /// Start points are this way grouped
  /// into vertices
  std::vector<::geoalgo::Point_t> AlgoPrimaryFinder::GetVertices(const ParticleGraph& graph,
								 const int minObjectsAtVertex) const
  {
    // make list to hold vertices
    std::vector<::geoalgo::Point_t> vertices;
    // vector keeping track of how many objects
    // share the same  vertex
    std::vector<int> numObjects;

    // get primaries only
    auto const& primaries = graph.GetPrimaryNodes();
    
    // for each primary, get the start point
    // check against all already found vertices
    // if any one is within the _IP of the
    // current start point -> add object to that vertex
    for (auto const& nodeID : primaries){
      bool newvtx = true;
      auto const start = graph.GetParticle(nodeID).Vertex();
      for (size_t v = 0; v < vertices.size(); v++){
	if (start.Dist(vertices[v]) < _IP){
	  newvtx = false;
	  // add to this vertex
	  numObjects[v] += 1;
	}
      }
      if (newvtx){
	// this vertex is new!
	// add it
	vertices.push_back(start);
	numObjects.push_back(1);
      }
    }// for all primaries

    // we only want vertices with a minimum number of objects
    // starting from this vertex
    std::vector<::geoalgo::Point_t> vertices_afterCut;
    for (size_t i=0; i < vertices.size(); i++){
      if (numObjects[i] >= minObjectsAtVertex)
	vertices_afterCut.push_back(vertices[i]);
    }

    return vertices_afterCut;
  }


}

#endif
