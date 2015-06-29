#ifndef ERTOOL_ERANAPI0ALL_CXX
#define ERTOOL_ERANAPI0ALL_CXX

#include "ERAnaPi0All.h"

namespace ertool {

  ERAnaPi0All::ERAnaPi0All(const std::string& name) 
    : AnaBase(name)
    ,_pi0_tree(nullptr)
  { 

    PrepareTTree() ;
    _CC_ctr 	= 0;
    _NC_ctr 	= 0;
    _NC1pi0_ctr = 0;
    _eff 	= 0;
    _nEvents 	= 0    ;

  }

  void ERAnaPi0All::Reset()
  {
        _x_pi0 		= -1000 ; 
        _y_pi0 		= -1000 ;
        _z_pi0 		= -1000 ;
        _x_track	= -1000 ; 
        _y_track	= -1000 ;
        _z_track 	= -1000 ;
        _vtx_dist 	= 10000 ;
        _nCCPi0 	= 0    ;
        _nNCIncPi0 	= 0    ;
        _nNC1Pi0   	= 0    ;


    }

  void ERAnaPi0All::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAnaPi0All::ProcessBegin()
  {}

  void ERAnaPi0All::PrepareTTree(){
  
      if (_pi0_tree) { delete _pi0_tree; }
        _pi0_tree = new TTree("_pi0_tree","Pi0 Tree");

        _pi0_tree->Branch("_x_pi0",&_x_pi0,"x_pi0/D");
        _pi0_tree->Branch("_y_pi0",&_y_pi0,"y_pi0/D");
        _pi0_tree->Branch("_z_pi0",&_z_pi0,"z_pi0/D");
        _pi0_tree->Branch("_x_track",&_x_track,"x_track/D");
        _pi0_tree->Branch("_y_track",&_y_track,"y_track/D");
        _pi0_tree->Branch("_z_track",&_z_track,"z_track/D");
        _pi0_tree->Branch("_vtx_dist",&_vtx_dist,"vtx_dist/D");
        _pi0_tree->Branch("_nCCPi0",&_nCCPi0,"nCCPi0/I");
        _pi0_tree->Branch("_nNCIncPi0",&_nNCIncPi0,"nNCIncPi0/I");
        _pi0_tree->Branch("_nNC1Pi0",&_nNC1Pi0,"nNC1Pi0/I");

  }

  bool ERAnaPi0All::Analyze(const EventData &data, const ParticleGraph &graph)
  { 

    _nEvents++; 

    if(graph.GetParticleNodes(RecoType_t::kShower).size() < 2)
	return true; 

    //Fill a bunch of variables

    //How many pi0s per event? 
   // for (auto const & p : particles){

	


  //auto primNodes = graph.GetPrimaryNodes(RecoType_t::kTrack) ;
    auto particles = graph.GetParticleArray() ;
    for( auto const & p : particles){

	Reset() ; 

        if( p.PdgCode() == 111 ){
	  //  _nEvents++ ;
	    if ( _verbose )
		std::cout<<"In Ana: found a pi0: "<<std::endl;
	    }
	else 
	   continue ;

	    auto pi0 = p ;

	    _x_pi0 = pi0.Vertex()[0];
	    _y_pi0 = pi0.Vertex()[1];
	    _z_pi0 = pi0.Vertex()[2];

	    //if ( graph.GetSiblingNodes(p.ID()).size() == 0  ){

	    if ( pi0.Primary() ) { 

		_nNC1Pi0++;
		_NC1pi0_ctr++;

		_nNCIncPi0++;
		_NC_ctr++;

		if ( _verbose ){
		    std::cout<<"Graph particles : "<<graph.Diagram()<<std::endl ;
		    std::cout<<_NC1pi0_ctr<<"****************SINGLE NC pi0~!!!!!! ********************"<<std::endl;
		    }

	//	std::cout<<"EVENT: "<<_nEvents<<std::endl ;
		continue ;

		}
	    
	    _vtx_dist = 10000 ;

	    for( auto const & sib : graph.GetSiblingNodes(pi0.ID())){ 

		auto trk = graph.GetParticle(sib) ;

		double minVtx = pow( pow(pi0.Vertex()[0] - trk.Vertex()[0],2)
                                 + pow(pi0.Vertex()[1] - trk.Vertex()[1],2)
                                 + pow(pi0.Vertex()[2] - trk.Vertex()[2],2) ,0.5) ;

		
//		if ( _vtx_dist > minVtx ){

		    if ( _verbose ) 
			std::cout<<"sib PDG and vertex dist : "<<trk.PdgCode()<<" , "<<minVtx<<std::endl ;

		    _vtx_dist = minVtx ;
		    _x_track = trk.Vertex()[0];  
		    _y_track = trk.Vertex()[1];  
		    _z_track = trk.Vertex()[2];  

        	    if( abs( trk.PdgCode() ) == 13 )
			{
			if( _nNCIncPi0 > 0 ){
			    _nNCIncPi0 = 0 ;
			    _NC_ctr-- ;
			    }
			if( _nNC1Pi0 > 0 ){
			    _nNC1Pi0 = 0 ;
			    _NC1pi0_ctr-- ;
			    }
        	       _nCCPi0++; 
		       _CC_ctr ++ ;
			if ( _verbose )
			    std::cout<<_nEvents<<"******************************This is a CC event "<<std::endl ;
		       break;
			}
		    else if( abs( trk.PdgCode() ) == 2212 ){
			if (_nNC1Pi0 == 0 && _nNCIncPi0 == 0){
			    _nNC1Pi0  = 1;
			    _NC1pi0_ctr++;
			    if (_verbose)
				std::cout<<_NC1pi0_ctr<<"****************SINGLE NC pi0~!!!!!! ********************"<<std::endl;

			    _nNCIncPi0++ ;
			    _NC_ctr++ ;
			    }
			}
		    
		    else if( abs( trk.PdgCode() ) == 211 ) 
			{
			if(_nNCIncPi0 == 0 ){
			    _nNCIncPi0++ ;
			    _NC_ctr++ ;
			    }
			if( _nNC1Pi0 > 0 ){
			    _nNC1Pi0 = 0 ;
			    _NC1pi0_ctr-- ;
			    }
			if ( _verbose ){
			    std::cout<<"Graph particles : "<<graph.Diagram()<<std::endl ;
			    std::cout<<"******************************This is a NC event "<<std::endl ;
			    }
		       continue;
			}
		    //}


        	} 

		_pi0_tree->Fill();


    }
  

  return true; 
  }


  void ERAnaPi0All::ProcessEnd(TFile* fout)
  {

//  if ( _nEvents != 0 )
    _eff = double( _NC1pi0_ctr) /_nEvents * 100 ;

     std::cout << "RESULTS: " << std::endl
               << "CC Events           : " << _CC_ctr << std::endl
               << "NC Events           : " << _NC_ctr << std::endl
               << "Tot Events          : " << _nEvents << std::endl
               << "Single NC pi0 Events: " << _NC1pi0_ctr << std::endl
               << "NC Eff              : " << double ( _NC_ctr )/ _nEvents *100 << " %" << std::endl
               << "CC Eff              : " << double ( _CC_ctr )/ _nEvents *100 << " %" << std::endl
               << "Single NC Eff       : " << _eff << " %" << std::endl;



      if(fout) {
           fout->cd();
           _pi0_tree->Write();
         }
        
    return; 

    }

}

#endif
