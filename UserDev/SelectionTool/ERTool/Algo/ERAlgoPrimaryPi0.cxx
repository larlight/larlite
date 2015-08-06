#ifndef ERTOOL_ERALGOPRIMARYPI0_CXX
#define ERTOOL_ERALGOPRIMARYPI0_CXX

#include "ERAlgoPrimaryPi0.h"

namespace ertool {

  ERAlgoPrimaryPi0::ERAlgoPrimaryPi0(const std::string& name) : AlgoBase(name)
  {}

  void ERAlgoPrimaryPi0::Reset()
  {}

  void ERAlgoPrimaryPi0::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAlgoPrimaryPi0::ProcessBegin()
  {}

  bool ERAlgoPrimaryPi0::Reconstruct(const EventData &data, ParticleGraph& graph)
  {

///    std::cout<<"\n\nStart!"<<std::endl ;
    if(graph.GetParticleNodes(RecoType_t::kShower).size() < 2) return true;	

    //At this point,
    //1)Graph has gone through AlgoPi0, so graph contains pi0's. 
    //2)Graph has gone through AlgoPrimaryFinder, so graph also
    //  contains primary tracks and showers. 
    //3)Using 1) and 2), look for pi0s originating from neutrino vertex

    auto datacpy = data ;
    auto const& primaryTrks = graph.GetPrimaryNodes(RecoType_t::kTrack);
    int ctr = 0;

    //First, look only at events with pi0s
    for ( auto const & p : graph.GetParticleArray()){
	if (p.PdgCode() == 111){
	    ctr++ ;
	    auto & pi0 = p;

	    //  Event cases:
	    //	0) prim pi0, no primary tracks
	    //	1) prim pi0, primary tracks at vertex
	    //	2) non-prim pi0 
	    //	3) prim pi0, primary tracks from cosmics or some other unrelated interaction in event somewhere
	    
	    // case 0)
	    //If there are no primary tracks, it's a NC pi0
	    if ( primaryTrks.size() == 0 ){
		graph.SetPrimary(pi0.ID());
		if ( _verbose )
		    std::cout<<"SINGLE NC pi0 "<<std::endl ;
		}

	    if ( _verbose )
		std::cout<<primaryTrks.size()<<" primary track(s) with pi0 " <<pi0.ID()<<std::endl;

	    int primTrks = graph.GetPrimaryNodes(RecoType_t::kTrack).size() ;

	    // cases 1,2,3 involve track-pi0 correlations
	    // Loop over primary tracks and compare vertices with primary pi0 
	    for ( auto const& primID : graph.GetPrimaryNodes(RecoType_t::kTrack) ){

		//This "primTrks" counter is for case 3; if at the end of the primary track loop, 
		//no tracks(front or back) in the event have been associated with the pi0, the pi0 is
		//primary (and trackless).
		primTrks-- ;
		if ( _verbose ){
		    std::cout<<"New primary track! "<<std::endl ;
		    std::cout<<"Pdg of prim track: "<<graph.GetParticle(primID).PdgCode() <<std::endl ;
		    }

		auto trkParticle = graph.GetParticle(primID) ;
		auto const& thisTrack = datacpy.Track( graph.GetParticle(primID).RecoID() );

		//Dist of pi0 vtx to track start point
		double _vtxDist = pow( pow(pi0.Vertex()[0] - thisTrack.at(0)[0],2) //Vertex()[0],2) 
		    		 + pow(pi0.Vertex()[1] -     thisTrack.at(0)[1],2) //Vertex()[1],2)
		    		 + pow(pi0.Vertex()[2] -     thisTrack.at(0)[2],2), 0.5 ); //Vertex()[2],2) ,0.5) ;
		//Dist of pi0 vtx to track end -- want to make sure we're not looking at non-primary pi0s 
		double _pi0vtxToTrkEnd = pow( pow(pi0.Vertex()[0] - thisTrack.at(thisTrack.size()-1)[0],2) 
				            + pow(pi0.Vertex()[1] - thisTrack.at(thisTrack.size()-1)[1],2)
				            + pow(pi0.Vertex()[2] - thisTrack.at(thisTrack.size()-1)[2],2) ,0.5) ;


		if ( _verbose ){

		    std::cout<<"Vtx dist is: "		<<_vtxDist <<std::endl ; 
		    std::cout<<"End to vtx dist is: "	<<_pi0vtxToTrkEnd<<std::endl;
		    std::cout<<"pi0 vertex: "		<<pi0.Vertex()[0]<<", "<<pi0.Vertex()[1]
							<<", "<<pi0.Vertex()[2]<<std::endl ;
		    std::cout<<"Track start: "		<<thisTrack.at(0)[0]
							<<", "<<thisTrack.at(0)[1]
							<<", "<<thisTrack.at(0)[2]<<std::endl ;
		    std::cout<<"Track end: "		<<thisTrack.at(thisTrack.size()-1)[0]
							<<", "<<thisTrack.at(thisTrack.size()-1)[1]
							<<", "<<thisTrack.at(thisTrack.size()-1)[2]<<std::endl ;
		    }
    	                
    	        //case 1) 
		//If pi0 is close enough to a track's vertex, make those guys siblings
    	        if( _vtxDist < _minDistVtx){
		    
		    if ( _verbose )
			std::cout<<"Make these guys sibs: "<<pi0.ID()<<", "<<primID <<std::endl; 

		    graph.SetPrimary(pi0.ID()) ;
    	            graph.SetSiblings(pi0.ID(),primID);
//		    std::cout<<"Pi0 should  be primary! "<<pi0.Primary() <<std::endl ;
//		    std::cout<<"How many? "<<graph.GetSiblingNodes(p.ID()).size() <<std::endl ;
		    if(abs( graph.GetParticle(primID).PdgCode()) == 13 ){
			std::cout<<"Muon sib"<<std::endl;
			continue ;
			}
		   }

		//case 2)
		//If pi0 not from vertex, but pi0 is close to the end of track, non-primary pi0
		else if( _vtxDist >= _minDistVtx && _pi0vtxToTrkEnd < _minDistEnd ){

		    if ( _verbose )
			std::cout<<"Set pi0's parent to track "<<std::endl ;

    	            graph.SetParentage(primID,pi0.ID());
		    break; 
		    }
		
		//case 3)
		//If pi0 is in event with primary tracks, but after loop seems to not
		//be associated with any of them, it is a trackless primary pi0
		else if( _vtxDist > _minDistVtx && _pi0vtxToTrkEnd > _minDistEnd && primTrks == 0 ){ 
		    graph.SetPrimary( pi0.ID() ) ;
		    if ( _verbose )
			std::cout<<"*******************Setting pi0 to primary ! "<<std::endl ;
		    }

	    	}//Loop over primary tracks

	    break ;
	    }//if pi0
	}//Loop over particle array

	if ( ctr > 1 && _verbose ) 
	    std::cout<<"We foudn >= 2!****************************************** "<<ctr<<std::endl ;
	
    return true; 
    }


  void ERAlgoPrimaryPi0::ProcessEnd(TFile* fout)
  {}

}

#endif
