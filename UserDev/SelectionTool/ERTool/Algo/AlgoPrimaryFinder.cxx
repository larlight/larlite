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

    // Find primary showers
    for (auto const& p : graph.GetParticleNodes(RecoType_t::kShower)){

      auto const& thisID = graph.GetParticle(p).RecoID();
      auto const& thisShower = datacpy.Shower(thisID);
      
      // Ok, we have a shower.
      // make sure it does not:
      // 1) come from another shower
      // 2) come from a track

      if (_verbose) { std::cout << "This shower: (" << thisID << ")" << "\tE: " << thisShower._energy << std::endl; }
      bool primary = true;
      // loop over other showers and check 1) and 2)
      for (auto const& p2 : graph.GetParticleNodes(RecoType_t::kShower)){

	auto const& thatID = graph.GetParticle(p2).RecoID();

	// make sure we don't use the same shower or repeat search
	if (thatID <= thisID) continue;
	auto const& thatShower = datacpy.Shower(thatID);
	geoalgo::Point_t vtx(3);

	if (_verbose) { std::cout << "Comparing with shower (" << thisID << ")" << std::endl; }
	// compare the two showers -> make sure thisShower does not come from thatShower
	// if thisShower comes from somewhere along the second shower's trunk
	// then it is not primary
	double thatShowerLength = thatShower.Length();
	// create a segment starting at thatShower start
	// and going for the entire length in shower dir
	::geoalgo::LineSegment_t thatShowerTrunk(thatShower.Start(),thatShower.Start()+thatShower.Dir()*thatShowerLength);
	_IP = _findRel.FindClosestApproach(thisShower,thatShower,vtx);
	_IPthisStart = vtx.Dist(thisShower.Start());
	_IPthatStart = vtx.Dist(thatShower.Start());
	_IPthatTrunk = sqrt(_geoAlgo.SqDist(vtx,thatShowerTrunk));
	if (_verbose)
	  std::cout << "\tImpact Parameter      : " << _IP << std::endl
		    << "\tIP to other Shr Start : " << _IPthatStart << std::endl
		    << "\tIP to other Shr Trunk : " << _IPthatTrunk << std::endl
		    << "\tIP to this Shr Start  : " << _IPthisStart << std::endl;
	if ( (_IP < _maxIP)                            // good correlation
	     && ( _IPthisStart < _vtxToTrkDist)        // this shower's start point close to IP
	     && ( _IPthatTrunk < _vtxToTrkDist) )      // vtx close to thatShower's trunk
	  {
	    primary = false;
	    if (_verbose) { std::cout << "shower from shower: NOT primary" << std::endl; }
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
	geoalgo::Point_t vtx(3);
	// compare the two tracks
	_IP =  _findRel.FindClosestApproach(thisShower,thatTrack,vtx);
	_IPthisStart = vtx.Dist(thisShower.Start());
	_IPthatStart = vtx.Dist(thatTrack.front());
	_IPtrkBody = sqrt(_geoAlgo.SqDist(vtx,thatTrack));
	if (_verbose)
	  std::cout << "\tImpact Parameter: " << _IP << std::endl
		    << "\tIP to Trk Start : " << _IPthatStart << std::endl
		    << "\tIP to Trk Body  : " << _IPtrkBody << std::endl
		    << "\tIP to Shr Start : " << _IPthisStart << std::endl;
	primary = true;
	if ( (_IP < _maxIP)                             // good correlation
	     && (_IPthatStart > _vtxToTrkStartDist)     // vertex far enough away from track start
	     && (_IPtrkBody < _vtxToTrkDist)            // vertex close to track body
	     && (_IPthisStart < _vtxToShrStartDist) )   // vertex not unreasonably far from shower start
	  {
	    // our shower comes from t -> not interested
	    primary = false;
	    if (_verbose) { std::cout << "shower from track: NOT primary" << std::endl; }
	    break;
	  }
      }// for all tracks
      
      // if still primary append to list of primaries
      // create new particle
      if (primary){
	// set particle as primary
	graph.SetPrimary(thisID);
      }
      
    }// end loop through all showers


    // Find primary tracks
    for (auto const& p : graph.GetParticleNodes(RecoType_t::kTrack)){

      auto const& thisID = graph.GetParticle(p).RecoID();
      auto const& thisTrack = datacpy.Track(thisID);    

      if (thisTrack.size() < 2)
	continue;
      
      if (_verbose) { std::cout << "This Track: (" << thisID << ")" << "\tE: " << thisTrack._energy << std::endl; }
      bool primary = true;
      

      // loop over other showers and check 1) and 2)
      for (auto const& p2 : graph.GetParticleNodes(RecoType_t::kShower)){

	auto const& thatID = graph.GetParticle(p2).RecoID();

	// make sure we don't use the same shower or repeat search
	if (thatID <= thisID) continue;

	auto const& thatShower = datacpy.Shower(thatID);
	geoalgo::Point_t vtx(3);
	if (_verbose) { std::cout << "Comparing with shower (" << thatID << ")" << std::endl; }
	// compare the two showers -> make sure thisTrack does not come from thatShower
	// if thisTrack comes from somewhere along the second shower's trunk
	// then it is not primary
	double thatShowerLength = thatShower.Length();
	// create a segment starting at thatShower start
	// and going for the entire length in shower dir
	::geoalgo::LineSegment_t thatShowerTrunk(thatShower.Start(),thatShower.Start()+thatShower.Dir()*thatShowerLength);
	_IP = _findRel.FindClosestApproach(thatShower,thisTrack,vtx);
	_IPthisStart = vtx.Dist(thisTrack.front());
	_IPthatStart = vtx.Dist(thatShower.Start());
	_IPthatTrunk = sqrt(_geoAlgo.SqDist(vtx,thatShowerTrunk));
	if (_verbose)
	  std::cout << "\tImpact Parameter : " << _IP << std::endl
		    << "\tIP to Shr Start  : " << _IPthatStart << std::endl
		    << "\tIP to Shr Trunk  : " << _IPthatTrunk << std::endl
		    << "\tIP to Trk Start  : " << _IPthisStart << std::endl;
	if ( (_IP < _maxIP)                            // good correlation
	     && ( _IPthisStart < _vtxToTrkDist)        // this shower's start point close to IP
	     && ( _IPthatTrunk < _vtxToTrkDist) )      // vtx close to thatShower's trunk
	  {
	    primary = false;
	    if (_verbose) { std::cout << "track from shower: NOT primary" << std::endl; }
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
	if (_verbose) { std::cout << "Comparing with track (" << t << ")" << std::endl; }
	geoalgo::Point_t vtx(3);
	// compare the two tracks
	_IP =  _findRel.FindClosestApproach(thisTrack,thatTrack,vtx);
	_IPthisStart = vtx.Dist(thisTrack.front());
	_IPthatStart = vtx.Dist(thatTrack.front());
	_IPtrkBody = sqrt(_geoAlgo.SqDist(vtx,thatTrack));
	if (_verbose)
	  std::cout << "\tImpact Parameter     : " << _IP << std::endl
		    << "\tIP to this Trk Start : " << _IPthatStart << std::endl
		    << "\tIP to that Trk Body  : " << _IPtrkBody << std::endl
		    << "\tIP to this Trk Start : " << _IPthisStart << std::endl;
	primary = true;
	if ( (_IP < _maxIP)                          // good correlation
	     && (_IPthatStart > _vtxToTrkStartDist)  // vertex far enough away from track start
	     && (_IPtrkBody < _vtxToTrkDist) )       // vertex close to track body
	  {
	    // our shower comes from t -> not interested
	    primary = false;
	    if (_verbose) { std::cout << "track from track: NOT primary" << std::endl; }
	    break;
	  }
      }// for all tracks

      // if still primary append to list of primaries
      // create new particle
      if (primary){
	// set particle as primary
	graph.SetPrimary(thisID);
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

}

#endif
