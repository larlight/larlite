#ifndef ERTOOL_ERALGOPRIMARYPI0_CXX
#define ERTOOL_ERALGOPRIMARYPI0_CXX

#include "ERAlgoPrimaryPi0.h"

namespace ertool {

  ERAlgoPrimaryPi0::ERAlgoPrimaryPi0(const std::string& name) : AlgoBase(name)
  { 
    SetVerbose(true);
    }

  void ERAlgoPrimaryPi0::Reset()
  {}

  void ERAlgoPrimaryPi0::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAlgoPrimaryPi0::ProcessBegin()
  {}

  bool ERAlgoPrimaryPi0::Reconstruct(const EventData &data, ParticleGraph& graph)
  {

    std::cout<<"\n\n\n NEW EVENT! "<<std::endl ;

    if(graph.GetParticleNodes(RecoType_t::kShower).size() < 2) return true;	

    //At this point,
    //1)Graph has gone through AlgoPi0, so graph contains pi0's. 
    //2)Graph has gone through AlgoPrimaryFinder, so graph also
    //  contains primary tracks and showers. 

    auto datacpy = data ;
    auto const& primaryTrks = graph.GetPrimaryNodes(RecoType_t::kTrack);
    bool primaryPi0 = true ; 

//    auto mc = MCParticleGraph() ;
//
//    for ( auto const & m : mc.GetParticleArray()){
//	std::cout<<"MCParts : "<< m.PdgCode()<<std::endl ;
//    
//	}

    std::cout<<"graph diagram : "<<graph.Diagram() <<std::endl;


    //First, look only at events with pi0s
    for ( auto const & p : graph.GetParticleArray()){
	if (p.PdgCode() == 111){
	    auto pi0 = p;

	    //  Event cases:
	    //	1) prim pi0, no primary tracks
	    //	2) prim pi0, primary tracks at vertex
	    //	3) prim pi0, primary tracks from cosmics or some other unrelated interaction in event somewhere
	    //	4) non-prim pi0 

	    //If there are no primary tracks, it's a NC pi0
	    if ( primaryTrks.size() == 0 ){
//		graph.SetPrimary(pi0.ID());
		std::cout<<"ERAlgoPriamary NC pi0 "<<std::endl ;
		}

	    int nPrimTracks = primaryTrks.size() ;

	    //std::cout<<primaryTrks.size()<<" primary track(s) with pi0 "<<pi0.ID()<<std::endl;
	    //Loop over primary tracks and compare vertices with primary pi0 
	    for ( auto const& primID : graph.GetPrimaryNodes(RecoType_t::kTrack) ){

//		auto const& thatID = graph.GetParticle(primID).RecoID();
//		auto thatTrack  = data.Track(thatID) ;

    	//	double IP = _findRel.FindClosestApproach(thisShower,thatTrack,vtx);
//	//	std::cout<<"\nPrimary track pdgcode : " <<graph.GetParticle(primID).PdgCode() <<std::endl ;

		auto trkParticle = graph.GetParticle(primID) ;
		double _vtxDist = pow( pow(pi0.Vertex()[0] - trkParticle.Vertex()[0],2) 
		    		 + pow(pi0.Vertex()[1] - trkParticle.Vertex()[1],2)
		    		 + pow(pi0.Vertex()[2] - trkParticle.Vertex()[2],2) ,0.5) ;

		auto const& thisTrack = datacpy.Track( graph.GetParticle(primID).RecoID() );
		double _pi0vtxToTrkEnd = pow( pow(pi0.Vertex()[0] - thisTrack.at(thisTrack.size()-1)[0],2) 
				            + pow(pi0.Vertex()[1] - thisTrack.at(thisTrack.size()-1)[1],2)
				            + pow(pi0.Vertex()[2] - thisTrack.at(thisTrack.size()-1)[2],2) ,0.5) ;

//		std::cout<<"Min dist is: "<<_vtxDist <<std::endl ;//_pi0vtxToTrkEnd<<", and vtxDist is: "<<_vtxDist<<std::endl;
//		std::cout<<"Min dist set at: "<<_minDistVtx<<std::endl; 
//		std::cout<<"pi0 vertex: "<<pi0.Vertex()[0]<<", "<<pi0.Vertex()[1]<<", "<<pi0.Vertex()[2]<<std::endl ;
//		std::cout<<"track vtx: "<<trkParticle.Vertex()[0]<<", "<<trkParticle.Vertex()[1]<<", "<<trkParticle.Vertex()[2]<<std::endl ;
	//	std::cout<<"Track end: "<<thisTrack.at(thisTrack.size()-1)[0]
	//				<<", "<<thisTrack.at(thisTrack.size()-1)[1]
	//				<<", "<<thisTrack.at(thisTrack.size()-1)[2]<<std::endl ;
    	                
    	        if( _vtxDist < _minDistVtx){

//		    graph.SetPrimary(pi0.ID());

    	            //If pi0 is close enough to a track's vertex, 
    	            //make those guys siblings
    	            trkParticle.SetParticleInfo( trkParticle.PdgCode(), trkParticle.Mass(),
					         trkParticle.Vertex() , trkParticle.Momentum());
		    std::cout<<"Make these guys sibs: "<<pi0.ID()<<", "<<primID <<std::endl; 
    	            graph.SetSiblings(pi0.ID(),primID);
		   }

		//Non-primary pi0
		if( _vtxDist >= _minDistVtx && _pi0vtxToTrkEnd < _minDistEnd ){

    	            trkParticle.SetParticleInfo( trkParticle.PdgCode(), trkParticle.Mass(),
					   trkParticle.Vertex() , trkParticle.Momentum());
//    	            graph.SetParentage(primID,pi0.ID());
		    //std::cout<<"Non primary pi0" <<std::endl ;
		    }
		//case 3) 
		if( _vtxDist >= 2*_minDistVtx && _pi0vtxToTrkEnd >= 2*_minDistEnd){
		    nPrimTracks -= 1; 
		 //   std::cout<<"Not a primary pi0-track relation."<<std::endl;    
		    } 


	    	}//Loop over primary tracks

	    if (pi0.Primary() ) {

		auto pi0ID = pi0.RecoID() ;
	    	auto pi0Shr = data.Shower ( pi0ID ) ;

		for (auto const & s : graph.GetParticleNodes(RecoType_t::kShower) ) {
		
		    auto thisID = graph.GetParticle(s).RecoID() ;
	    	    auto thisShr = data.Shower ( thisID ) ;
		    //Does thisShr come from pi0? 
		    auto related = _primary_alg.From ( thisShr, pi0Shr ) ;

		    if ( related ) 
			graph.SetParentage(pi0.ID(),thisID) ;
			
		    }
		}

	//	std::cout<<"Finished looping over tracks"<<std::endl ;
		//If prim tracks variable = 0, at least one primary track had "relation" to pi0.
		//Otherwise, this pi0 walks alone.  
	//	if(nPrimTracks == 0) 
	//	    graph.SetPrimary(pi0.ID());
		    
	    }//if pi0
	}//Loop over particle array



	
    return true; 
    }


  void ERAlgoPrimaryPi0::ProcessEnd(TFile* fout)
  {}

}

#endif
