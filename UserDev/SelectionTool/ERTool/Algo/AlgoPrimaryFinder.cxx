#ifndef ERTOOL_ALGOPRIMARYFINDER_CXX
#define ERTOOL_ALGOPRIMARYFINDER_CXX

#include "AlgoPrimaryFinder.h"
#include <sstream>
namespace ertool {

  AlgoPrimaryFinder::AlgoPrimaryFinder(const std::string& name) : AlgoBase(name)
  {
    _useRadLength = false;
  }

  void AlgoPrimaryFinder::Reset(){}
  
  void AlgoPrimaryFinder::ProcessBegin()
  {
    return;
  }

  void AlgoPrimaryFinder::AcceptPSet(const ::fcllite::PSet& cfg)
  {
    return;
  }

  bool AlgoPrimaryFinder::Reconstruct(const EventData &data, ParticleGraph& graph){
    
    if (Debug()) 
      Debug(__FUNCTION__,"*********** BEGIN PrimaryFinder RECONSTRUCTION ************");

    // This Reconstruction stage is divided in two independent parts:
    // 1) Reconstruct primary showers
    // 2) Reconstruct primary tracks
    // Stages are separated due to the different conditions required
    // to determine if a shower/track is primary

    // get node combinations
    //combos = graph.GetNodeCombinations(2,
    

    // Find primary showers
    for (auto const& s : graph.GetParticleNodes(RecoType_t::kShower)){

      auto const& part = graph.GetParticle(s);
      if(part.Descendant()) continue;

      auto const& parent_part = graph.GetParticle(part.Parent());
      auto const& ancestor_part = graph.GetParticle(part.Ancestor());
      
      if( parent_part.RecoType() == RecoType_t::kTrack) {
        
	if(Debug()) Debug(__FUNCTION__,"\t\t Track mama");

        continue;
      }
	
      if(part.ProcessType() == kCosmic || ancestor_part.ProcessType() == kCosmic){
	if(Debug()) Debug("\t\t Cosmic Shower");
        continue;	
      }

      // default: the shower is primary
      // if we find an indication of the
      // contrary change state accordingly
      bool primary = true;

      auto const& thisID = part.RecoID();
      auto const& thisShower = data.Shower(thisID);
      
      // Ok, we have a shower.
      // make sure it does not:
      // 1) come from another shower
      // 2) come from a track

      if (Debug()) {
	std::stringstream ss;
	ss << "This shower: (" << thisID << ")" << "\tE: " << thisShower._energy;
	Debug(__FUNCTION__,ss.str());
      }
      // loop over other showers and check 1) and 2)
      for (auto const& p2 : graph.GetParticleNodes(RecoType_t::kShower)){

	if( graph.GetParticle(graph.GetParticle(p2).Parent()).RecoType() == RecoType_t::kTrack){

	  if(Debug()) Debug("\t\t Joseph mama");

	  continue;
	}


	if(graph.GetParticle(p2).ProcessType() == ::ertool::ProcessType_t::kCosmic|| 
	 graph.GetParticle(graph.GetParticle(p2).Ancestor()).ProcessType() == ::ertool::ProcessType_t::kCosmic){
	  if(Debug()) Debug("\t\t Cosmic Shower");
	
        continue;
	
	}
	auto const& thatID = graph.GetParticle(p2).RecoID();
	
	// make sure we don't use the same shower or repeat search
	if (thatID == thisID) continue;

	if (Debug()) {
	  std::stringstream ss;
	  ss << "Comparing with shower (" << thisID << ")";
	  Debug(__FUNCTION__,ss.str());
	}
	auto const& thatShower = data.Shower(thatID);

	if (From(thisShower,thatShower)){
	  if (Debug()) { Debug(__FUNCTION__,"shower not primary"); }
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

	if(graph.GetParticle(t).ProcessType() == ::ertool::ProcessType_t::kCosmic|| 
	 graph.GetParticle(graph.GetParticle(t).Ancestor()).ProcessType() == ::ertool::ProcessType_t::kCosmic){
	  if(Debug()) Debug(__FUNCTION__,"\t\t Cosmic Track");
	  
	  continue;
	  
	}
	
	auto const& thatID = graph.GetParticle(t).RecoID();
	auto const& thatTrack = data.Track(thatID);
	
	if (thatTrack.size() < 2)
	  continue;
	if (Debug()) {
	  std::stringstream ss;
	  ss << "Comparing with track (" << t << ")";
	  Debug(__FUNCTION__,ss.str());
	}
	
	if (From(thisShower,thatTrack)){
	  if (Debug()) { Debug(__FUNCTION__,"shower not primary"); }
	  primary = false;
	  // exit the loop...no need to search further
	  break;
	}
      }// for all tracks
      
      // if still primary append to list of primaries
      // create new particle
      if (primary){
	// set particle as primary
	if (Debug())
	  Debug("shower is primary!");
	graph.SetPrimary(s);
	graph.GetParticle(s).SetParticleInfo(graph.GetParticle(s).PdgCode(),
					     graph.GetParticle(s).Mass(),
					     thisShower.Start(),
					     graph.GetParticle(s).Momentum());
      }
      
    }// end loop through all showers


    // Find primary tracks
    for (auto const& t : graph.GetParticleNodes(RecoType_t::kTrack)){

      auto const& part = graph.GetParticle(t);
      if(part.Descendant()) continue;

      auto const& parent_part = graph.GetParticle(part.Parent());
      auto const& ancestor_part = graph.GetParticle(part.Ancestor());
      
      if(part.ProcessType() == kCosmic || ancestor_part.ProcessType() == kCosmic) {
	if(Debug()) Debug("Cosmic Shower");
        continue;	
      }

      // default: the track is primary
      // if we find an indication of the
      // contrary change state accordingly
      bool primary = true;

      auto const& thisID = graph.GetParticle(t).RecoID();
      auto const& thisTrack = data.Track(thisID);    

      if (thisTrack.size() < 2)
	continue;
      
      if (Debug()) {
	std::stringstream ss;
	ss << "This Track: (" << thisID << ")" << "\tE: " << thisTrack._energy;
	Debug(__FUNCTION__,ss.str());
      }

      // loop over other showers and check 1) and 2)
      for (auto const& p2 : graph.GetParticleNodes(RecoType_t::kShower)){

	if( graph.GetParticle(graph.GetParticle(p2).Parent()).RecoType() == RecoType_t::kTrack){
	  if(Debug()) Debug("Muon mama");
	  continue;
	}
	
	if(graph.GetParticle(p2).ProcessType() == ::ertool::ProcessType_t::kCosmic|| 
	   graph.GetParticle(graph.GetParticle(p2).Ancestor()).ProcessType() == ::ertool::ProcessType_t::kCosmic){
	  if(Debug()) Debug("Cosmic Shower");
	  continue;	
	}

	

	auto const& thatID = graph.GetParticle(p2).RecoID();
	auto const& thatShower = data.Shower(thatID);
	if (Debug()) {
	  std::stringstream ss;
	  ss << "Comparing with shower (" << thatID << ")";
	  Debug(__FUNCTION__,ss.str());
	}

	if (From(thisTrack,thatShower)){
	  if (Debug()) Debug("track not primary");
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

	if(graph.GetParticle(t).ProcessType() == ::ertool::ProcessType_t::kCosmic|| 
	 graph.GetParticle(graph.GetParticle(t).Ancestor()).ProcessType() == ::ertool::ProcessType_t::kCosmic){
	  if(Debug()) Debug("Cosmic Shower");
	  continue;	
	}

	auto const& thatID = graph.GetParticle(t).RecoID();
	auto const& thatTrack = data.Track(thatID);
	if (thatTrack.size() < 2)
	  continue;
	if (thatID == thisID)
	  continue;
	
	if (Debug()) {
	  std::stringstream ss;
	  ss << "Comparing with track (" << t << ")";
	  Debug(__FUNCTION__,ss.str());
	}

	if (From(thisTrack,thatTrack)){
	  if (Debug()) Debug("track not primary");
	  primary = false;
	  // exit the loop...no need to search further
	  break;
	}	  
      }// for all tracks
      
      // if still primary append to list of primaries
      // create new particle
      if (primary){
	// set particle as primary
	if (Debug()) Debug("track is primary!");
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
    if (Debug()) {
      std::stringstream ss;
      ss << "\tImpact Parameter      : " << IP << std::endl
	 << "\tIP to other Shr Start : " << IPthatStart << std::endl
	 << "\tIP to other Shr Trunk : " << IPthatTrunk << std::endl
	 << "\tIP to this Shr Start  : " << IPthisStart << std::endl;
      Debug(__FUNCTION__,ss.str());
    }
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
    if (Debug()) {
      std::stringstream ss;
      ss << "\tImpact Parameter : " << IP << std::endl
	 << "\tIP to Shr Start  : " << IPthatStart << std::endl
	 << "\tIP to Shr Trunk  : " << IPthatTrunk << std::endl
	 << "\tIP to Trk Start  : " << IPthisStart;
      Debug(__FUNCTION__,ss.str());
    }
      
    if ( (IP < _maxIP)                       // good correlation
	 && ( IPthisStart < _vtxToTrkDist )  // this shower's start point close to IP
	 && ( IPthatTrunk < _vtxToTrkDist )  // vtx close to thatShower's trunk
	 && ( IPthatStart > _vtxToTrkDist ) )// vtx far from shower start
      
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
    if (Debug()) {
      std::stringstream ss;
      ss << "\tImpact Parameter: " << IP << std::endl
	 << "\tIP to Trk Start : " << IPthatStart << std::endl
	 << "\tIP to Trk Body  : " << IPtrkBody << std::endl
	 << "\tIP to Shr Start : " << IPthisStart << std::endl;
      Debug(__FUNCTION__,ss.str());
    }

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
    if (Debug()) {
      std::stringstream ss;
      ss << "\tImpact Parameter     : " << IP << std::endl
	 << "\tIP to this Trk Start : " << IPthatStart << std::endl
	 << "\tIP to that Trk Body  : " << IPtrkBody << std::endl
	 << "\tIP to this Trk Start : " << IPthisStart << std::endl;
      Debug(__FUNCTION__,ss.str());
    }
    if ( (IP < _maxIP)                          // good correlation
	 && (IPthatStart > _vtxToTrkStartDist)  // vertex far enough away from track start
	 && (IPtrkBody < _vtxToTrkDist) )       // vertex close to track body
      {
	// our shower comes from t -> not interested
	return true;
      }
      
    return false;
  }

}

#endif
