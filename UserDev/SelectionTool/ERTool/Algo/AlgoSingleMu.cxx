#ifndef ERTOOL_ALGOSINGLEMU_CXX
#define ERTOOL_ALGOSINGLEMU_CXX

#include "AlgoSinglemu.h"
#include <sstream>
namespace ertool {

  AlgoSinglemu::AlgoSinglemu(const std::string& name)
    : AlgoBase(name)
    , fTPC(-10., -126., -10., 292., 136., 1150.)
  {
    
  }
  
  void AlgoSinglemu::Reset()
  {
    
  }
  
  void AlgoSinglemu::ProcessBegin()
  {
    return;
  }
  
  void AlgoSinglemu::AcceptPSet(const ::fcllite::PSet& cfg) {
    
    return;
  }
  
  bool AlgoSinglemu::Reconstruct(const EventData &data, ParticleGraph& graph) {
    
    if (Debug()) {
      Debug(__FUNCTION__, "*********** BEGIN Singlemu RECONSTRUCTION ************");
      std::stringstream ss;
      ss << "Tracks in event  : " << data.Track().size();
      Debug(__FUNCTION__, ss.str());
    }
    
    
    std::vector <int> MuonCandidates; 
    std::vector <int> AllTracks;

    // Iterate through all primary non-cosmic tracks and roughly sort based on length 
    for (auto const& p : graph.GetPrimaryNodes(RecoType_t::kTrack)) {

      // SKIP IF COSMIC      
      if (isCosmic(p, graph)) {if (Debug()) Debug(__FUNCTION__, "Cosmic Shower"); continue;}

      auto const& thisTrack = data.Track(graph.GetParticle(p).RecoID());
      AllTracks.push_back(graph.GetParticle(p).RecoID());
      
      if(MuonCandidates.size() == 0) MuonCandidates.push_back(graph.GetParticle(p).RecoID());
      else{
	auto const& thatTrack = data.Track(MuonCandidates[0]);
	if(thisTrack.Length() > thatTrack.Length() ){
	  MuonCandidates.insert(MuonCandidates.begin(), graph.GetParticle(p).RecoID());
	}
	else{MuonCandidates.push_back(graph.GetParticle(p).RecoID());}
      }

    }//Iterate through tracks 


    //Select the Muon Candidates
    //   1.  Shorter than 20cm reject
    //
    //   2.  Iterate through the track list look for tracks longer than 
    //       _min_trk_length originating from the end of the "test track"
    // 
    //  [NOT IMPLEMENTED] 3. Check the dE/dx of track 
    //

    for (auto const& trk :  MuonCandidates){
      
      auto const& thisTrack = data.Track(trk);

      // 1. 
      if(thisTrack.Length() < 20){ 
	MuonCandidates.erase(std::remove(MuonCandidates.begin(),  MuonCandidates.end(), trk),  MuonCandidates.end());
	continue;
      }
     
      // 2.
      for(auto const& test : AllTracks){
      
	auto const& thatTrack = data.Track(test);
	if(thatTrack.Length() < 5){continue;}

	auto const& end = thisTrack.back();

	if(thatTrack.front().Dist(end) < 3){ 
	  MuonCandidates.erase(std::remove(MuonCandidates.begin(),  MuonCandidates.end(), trk),  MuonCandidates.end());
	  continue;
	}
	      
      }
    }


    // Remove double counting in AllTracks 
    for(auto const& item : MuonCandidates){
      AllTracks.erase(std::remove(AllTracks.begin(),  AllTracks.end(), item),  AllTracks.end());     
    }


    //Build Candidate Interactions
    //  1. Collect all the tracks at the start of a Muon Candidate and build an "interaction" 
    //       a. Check for two muon canidates originating together
    //       b. Check for any other tracks originating from same spot
    //       c. Build a particle which is parent of muon and all its siblings
    //
    //  2. Check all against flash
    //       --> Particle with best match is named neutrino
    //
    //
    for(auto const& trk :  MuonCandidates){
 
      std::vector<int> muon_siblings; 

      auto const& thisTrack = data.Track(trk);

      // a.
      for(auto const& test : MuonCandidates){
	
	if(trk == test) continue;

	auto const& thatTrack = data.Track(test);
	
	if(thisTrack.front().Dist(thatTrack.front()) < 5){
	  muon_siblings.push_back(test);
	  MuonCandidates.erase(std::remove(MuonCandidates.begin(),  MuonCandidates.end(), test),  MuonCandidates.end());
	  continue;
	}
      }
      
      // b.
      for(auto const& test : AllTracks){
	
	auto const& thatTrack = data.Track(test);
	
	if(thisTrack.front().Dist(thatTrack.front()) < 5){
	  muon_siblings.push_back(test);
	  AllTracks.erase(std::remove(AllTracks.begin(),  AllTracks.end(), test),  AllTracks.end());
	  continue;	  	  
	}	
      }
      
      // c.
      Particle& NeutrinoCand = graph.CreateParticle();
      
      
      
      
    }
    

    return true;
    
  }

 
  bool AlgoSinglemu::isCosmic(NodeID_t node, ParticleGraph& graph){

    if (graph.GetParticle(node).ProcessType() == ::ertool::ProcessType_t::kCosmic ||
	graph.GetParticle(graph.GetParticle(node).Ancestor()).ProcessType() == ::ertool::ProcessType_t::kCosmic) {
      return true;
    }
    else{
      return false;
    }
    
  }

  void AlgoSinglemu::ProcessEnd(TFile* fout) {
    
    std::cout << "Num. of neutrinos found: " << _neutrinos << std::endl;
    
    if (fout) {
      fout->cd();
      
    }
    
    return;
  }


  void AlgoSinglemu::ClearTree() {
    
    
    return;
  }
  
  
}

#endif
