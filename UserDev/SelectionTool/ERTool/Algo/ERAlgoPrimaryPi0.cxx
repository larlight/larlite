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

    //At this point,
    //1)Graph has gone through AlgoPi0, so graph contains pi0's. 
    //2)Graph has gone through AlgoPrimaryFinder, so graph also
    //  contains primary tracks and showers. 

    auto datacpy = data ;
    auto const& primaryTrks = graph.GetPrimaryNodes(RecoType_t::kTrack);
    bool primaryPi0 = false ; 

    //First, look only at events with pi0s
    for ( auto const & p : graph.GetParticleArray()){
	if (p.PdgCode() == 111){
	    auto pi0 = p;

	    //Were pi0's children showers tagged primary by the Finder?
	    for(auto const & s : pi0.Children()){
		primaryPi0 = false ;
		if(graph.GetParticle(s).Primary()){
		    primaryPi0 = true ;
		    break ;
		  }
		}
		    
	    if ( primaryPi0 ){
	    	    graph.SetPrimary(pi0.ID());
    
		    //Loop over primary tracks and compare vertices with primary pi0 
	    	    for( auto const & primID : primaryTrks) {
    
	    	        auto trk = graph.GetParticle(primID) ;
	    	        double _vtxDist = pow( pow(pi0.Vertex()[0] - trk.Vertex()[0],2) 
	    	        		     + pow(pi0.Vertex()[1] - trk.Vertex()[1],2)
	    	            		     + pow(pi0.Vertex()[2] - trk.Vertex()[2],2) ,2) ;
	    	                
	    	        if( _vtxDist < _minDist){

	    	            //If pi0 is close enough to a track's vertex, 
	    	            //make those guys siblings
	    	            trk.SetParticleInfo( trk.PdgCode(), trk.Mass(),
						   trk.Vertex() , trk.Momentum());

	    	            graph.SetSiblings(pi0.ID(),primID);
			  }

	    	        }//Loop over primary tracks
	    	    }//if pi0 is primary
	    }//if pi0
	}//Loop over particle array
	
    return true; 
    }


  void ERAlgoPrimaryPi0::ProcessEnd(TFile* fout)
  {}

}

#endif
