#ifndef ERTOOL_ERALGOCOSMICTAGGER_CXX
#define ERTOOL_ERALGOCOSMICTAGGER_CXX

#include "ERAlgoCosmicTagger.h"

namespace ertool {

  ERAlgoCosmicTagger::ERAlgoCosmicTagger(const std::string& name) 
  : AlgoBase(name)
  {
    
    
  }

  void ERAlgoCosmicTagger::Reset()
  {
    std::cout<<"\t\t"<<__FUNCTION__<<" Reset " << std::endl;

  }

  void ERAlgoCosmicTagger::AcceptPSet(const ::fcllite::PSet& cfg)
  { return; }

  void ERAlgoCosmicTagger::ProcessBegin()
  { return; }

  bool ERAlgoCosmicTagger::Reconstruct(const EventData &data, ParticleGraph& graph)
  {

    //Step 1: Find Track
    //Step 2: Find Showers that are not close to the start or end
    //Step 3: Assign the Parent as Muon
    //Step 4: Profit.

    // Iterate through all Tracks
    
    for(auto const& t : graph.GetParticleNodes(RecoType_t::kTrack)){

      //      if(graph.GetParticle(t).Descendant()) continue;
            
      auto const& trk = data.Track(graph.GetParticle(t).RecoID());
      
      if(trk.Length() < 0.3) continue;


      // Vector of Daughters gets reset
      daughters.clear();
      
      geoalgo::Point_t trkStart(3);
      geoalgo::Point_t trkEnd(3);
      
      trkStart = trk.front();
      trkEnd = trk.back();
      
      //Iterate through all the showers
      for(auto const& s : graph.GetParticleNodes(RecoType_t::kShower)){

	if(graph.GetParticle(s).Descendant()) continue;
	
	auto const& show = data.Shower(graph.GetParticle(s).RecoID());	
	// Find Showers that aren't too far from a muon
	// Find Showers that aren't near the front
	// Find Showers that aren't near the back
	
	geoalgo::Point_t showStart(3);
	showStart = show.Start();

	
	if(_verbose){
	  
	  if(_geoAlgo.SqDist(showStart, trk) > (_minDist * _minDist) && _geoAlgo.SqDist(showStart, trk) < 1.1*(_minDist * _minDist)) 
	    std::cout << "\t\t Shower-Track Dist : "<< sqrt(_geoAlgo.SqDist(showStart, trk)) << std::endl;
	  if(showStart.SqDist(trkStart) > (_strtDist*_strtDist) && showStart.SqDist(trkStart) < 1.1*(_strtDist*_strtDist))
	    std::cout << "\t\t Shower-TrkSt Dist : "<< sqrt(showStart.SqDist(trkStart))      << std::endl;
	  if(showStart.SqDist(trkEnd)   > (_endDist * _endDist) && showStart.SqDist(trkEnd)   < 1.1*(_endDist * _endDist))
	    std::cout << "\t\t Shower-TrkEd Dist : "<< sqrt(showStart.SqDist(trkEnd))        << std::endl;
	  
	}
	
	if(_geoAlgo.SqDist(showStart, trk) > (_minDist * _minDist) || 
	   showStart.SqDist(trkStart) < (_strtDist*_strtDist)      ||
	   showStart.SqDist(trkStart) < (_strtDist*_strtDist)      ) continue;
	
	if(_geoAlgo.SqDist(showStart, trk) < (_deltaDist *_deltaDist) ){	  
	  graph.SetParentage(graph.GetParticle(t).ID(), graph.GetParticle(s).ID());
	}
	
      } // iterate through all Showers
      
    }//iterate through tracks
    
    return true;}

  void ERAlgoCosmicTagger::ProcessEnd(TFile* fout)
  {}

}

#endif
