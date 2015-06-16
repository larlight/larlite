#ifndef ERTOOL_ERALGOPRIMARYPI0_CXX
#define ERTOOL_ERALGOPRIMARYPI0_CXX

#include "ERAlgoPrimaryPi0.h"

namespace ertool {

  ERAlgoPrimaryPi0::ERAlgoPrimaryPi0(const std::string& name) : AlgoBase(name)
  { 
    SetVerbose(true);
    SetMinDist(3);
    }

  void ERAlgoPrimaryPi0::Reset()
  {}

  void ERAlgoPrimaryPi0::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAlgoPrimaryPi0::ProcessBegin()
  {}

  bool ERAlgoPrimaryPi0::Reconstruct(const EventData &data, ParticleGraph& graph)
  {
    if(graph.GetParticleNodes(RecoType_t::kShower).size() < 2) return true;	

    //Assume the things in graph are being passed after AlgoPi0, so graph
    //contains pi0's. Now compare the vertex of the pi0 with the vertex 
    //of primary tracks. So, find primary tracks. Some code borrowed from DC's
    //AlgoPrimaryFinder
    auto datacpy = data ;
 
    // Find primary tracks (from DC's PrimaryFinder)
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

      // Check that no tracks are coming from showers 
      for (auto const& p2 : graph.GetParticleNodes(RecoType_t::kShower)){

        auto const& thatID = graph.GetParticle(p2).RecoID();
        auto const& thatShower = datacpy.Shower(thatID);
        if (_verbose) { std::cout << "Comparing with shower (" << thatID << ")" << std::endl; }

        if (_primary_alg.From(thisTrack,thatShower)){
          if (_verbose) { std::cout << "track not primary" << std::endl; }
          primary = false;
          break;
        }
      }// loop over all showers

      // loop over other tracks
      for (auto const& t : graph.GetParticleNodes(RecoType_t::kTrack)){

        auto const& thatID = graph.GetParticle(t).RecoID();
        auto const& thatTrack = datacpy.Track(thatID);
        if (thatTrack.size() < 2)
          continue;
        if (thatID == thisID)
          continue;

        if (_verbose) { std::cout << "Comparing with track (" << t << ")" << std::endl; }
       
	 if (_primary_alg.From(thisTrack,thatTrack)){
          if (_verbose) { std::cout << "track not primary" << std::endl; }
          primary = false;
          // exit the loop...no need to search further
          break;
        }
      }// for all tracks

      // if still primary append to list of primaries
      if (primary){
        // set particle as primary
        if (_verbose) { std::cout << "track is primary!" << std::endl; }
        graph.SetPrimary(t);
        graph.GetParticle(t).SetParticleInfo(graph.GetParticle(t).PdgCode(),
                                             graph.GetParticle(t).Mass(),
                                             thisTrack.front(),
                                             graph.GetParticle(t).Momentum());

	return true;

	}
      }//find all primaries

    auto const& primaryTracks = graph.GetPrimaryNodes();
 
    //Now we have all primary tracks and pi0s from AlgoPi0. 
    //We're interested in pi0s, whether or not there are tracks.  
    //If pi0 is associated with a track, make those suckers siblings

    for ( auto const & p : graph.GetParticleArray()){
	if (p.PdgCode() == 111){
	    auto pi0 = p;
    
	    //If no primary tracks, must be single NC pi0	    
	    if ( primaryTracks.size() == 0 )
		graph.SetPrimary(pi0.ID());

	    for( auto const & primID : primaryTracks) {
    
		auto track = graph.GetParticle(primID) ;
		double _vtxDist = pow( pow(pi0.Vertex()[0] - track.Vertex()[0],2) 
				     + pow(pi0.Vertex()[1] - track.Vertex()[1],2)
		    		     + pow(pi0.Vertex()[2] - track.Vertex()[2],2) ,2) ;
		        
		if( _vtxDist < _minDist){

		    auto pdg    = track.PdgCode();
		    auto mass   = track.Mass();
		    auto vtx    = track.Vertex();
		    auto mom    = track.Momentum();
	     
		    //If pi0 is close enough to a vertex, make
		    //it a primary particle
		    track.SetParticleInfo(pdg,mass,vtx,mom);
		    graph.SetPrimary(pi0.ID());

		    // Add relationship
		    graph.SetSiblings(pi0.ID(),primID);

		    }

	        }//If a pi0 is found in this event

	    }
    }
	
    return true; 
    }


  void ERAlgoPrimaryPi0::ProcessEnd(TFile* fout)
  {}

}

#endif
