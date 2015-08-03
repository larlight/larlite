#ifndef ERTOOL_ERANAPI0ALL_CXX
#define ERTOOL_ERANAPI0ALL_CXX

#include "ERAnaPi0All.h"

namespace ertool {

  ERAnaPi0All::ERAnaPi0All(const std::string& name) 
    : AnaBase(name)
    , _pi0_tree(nullptr)
    , fTPC(0.,-115.5,0.,254.8,117.5,1036.92)
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
        _x_pi0 		= -10000 ; 
        _y_pi0 		= -10000 ;
        _z_pi0 		= -10000 ;
        _px_pi0 	= -10000 ; 
        _py_pi0 	= -10000 ;
        _pz_pi0 	= -10000 ;
        _p_tot_pi0 	= -10000 ;
	_n_pi0_mc 	= -1 ;
        _e_pi0 		= -10000 ;
        _x_pi0_mc 	= -10000 ; 
        _y_pi0_mc 	= -10000 ;
        _z_pi0_mc 	= -10000 ;
        _e_pi0_mc 	= -10000 ;
        _x_track	= -10000 ; 
        _y_track	= -10000 ;
        _z_track 	= -10000 ;
        _vtx_dist 	= 10000 ;
	_vtx_mc_reco	= 10000 ;
        _nCCPi0 	= 0     ;
        _nNCIncPi0 	= 0     ;
        _nNC1Pi0   	= 0     ;
	_angle 		= -10000 ;
	_mass		= -10000 ;
	_manyPi0	= 0 ;

	_distToWall 	= -10000 ;
	_distAlongTraj  = 0 ;
	_combinedE	= 0 ;

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
        _pi0_tree->Branch("_px_pi0",&_px_pi0,"px_pi0/D");
        _pi0_tree->Branch("_py_pi0",&_py_pi0,"py_pi0/D");
        _pi0_tree->Branch("_pz_pi0",&_pz_pi0,"pz_pi0/D");
        _pi0_tree->Branch("_p_tot_pi0",&_p_tot_pi0,"p_tot_pi0/D");
        _pi0_tree->Branch("_e_pi0",&_e_pi0,"e_pi0/D");
        _pi0_tree->Branch("_x_pi0_mc",&_x_pi0_mc,"x_pi0_mc/D");
        _pi0_tree->Branch("_y_pi0_mc",&_y_pi0_mc,"y_pi0_mc/D");
        _pi0_tree->Branch("_z_pi0_mc",&_z_pi0_mc,"z_pi0_mc/D");
        _pi0_tree->Branch("_e_pi0_mc",&_e_pi0_mc,"e_pi0_mc/D");
        _pi0_tree->Branch("_n_pi0_mc",&_n_pi0_mc,"n_pi0_mc/I");
        _pi0_tree->Branch("_angle",&_angle,"angle/D");
        _pi0_tree->Branch("_mass",&_mass,"mass/D");
        _pi0_tree->Branch("_x_track",&_x_track,"x_track/D");
        _pi0_tree->Branch("_y_track",&_y_track,"y_track/D");
        _pi0_tree->Branch("_z_track",&_z_track,"z_track/D");
        _pi0_tree->Branch("_vtx_dist",&_vtx_dist,"vtx_dist/D");
        _pi0_tree->Branch("_vtx_mc_reco",&_vtx_mc_reco,"vtx_mc_reco/D");
        _pi0_tree->Branch("_nCCPi0",&_nCCPi0,"nCCPi0/I");
        _pi0_tree->Branch("_nNCIncPi0",&_nNCIncPi0,"nNCIncPi0/I");
        _pi0_tree->Branch("_nNC1Pi0",&_nNC1Pi0,"nNC1Pi0/I");
        _pi0_tree->Branch("_manyPi0",&_manyPi0,"manyPi0/I");
        _pi0_tree->Branch("_distToWall",&_distToWall,"distToWall/D");
        _pi0_tree->Branch("_distAlongTraj",&_distAlongTraj,"distAlongTraj/D");
	_pi0_tree->Branch("_x_shr",&_x_shr,"x_shr/D");
	_pi0_tree->Branch("_y_shr",&_y_shr,"y_shr/D");
	_pi0_tree->Branch("_z_shr",&_z_shr,"z_shr/D");
	_pi0_tree->Branch("_combinedE",&_combinedE,"combinedE/D");

  }

  bool ERAnaPi0All::Analyze(const EventData &data, const ParticleGraph &graph)
  { 

    int pi0_mc = 0 ;
    int pi0_reco = 0;
    double xMC(0.), yMC(0.), zMC(0.), eMC(0.), mMC(0.);


    auto const& mc_graph = MCParticleGraph();
 //   std::cout << "\n\nMC Particle Diagram: " << std::endl
 //          << mc_graph.Diagram() << std::endl;
//    std::cout << "Reg Particle Diagram: " << std::endl
//           << graph.Diagram() << std::endl;

    _nEvents++; 
    std::cout<<"Number of events thus far: "<<_nEvents<<std::endl ;

    //Don't want to look at any events with less than 2 showers-- pi0 event
    //will have at least 2
    if(graph.GetParticleNodes(RecoType_t::kShower).size() < 2){
	return true; 
	}

    //Fill some mc pi0 vertex info
    for( auto const & mcp : mc_graph.GetParticleArray()){

	if( mcp.PdgCode()==111){
	    pi0_mc = 1 ;
	    xMC = mcp.Vertex()[0];
	    yMC = mcp.Vertex()[1];
	    zMC = mcp.Vertex()[2];
	    eMC = mcp.Energy() ;
	    mMC = mcp.Mass() ;

	    }
	}

    //Fill a bunch of variables
    auto particles = graph.GetParticleArray() ;
    for( auto const & p : particles){

	Reset() ; 

        if( p.PdgCode() == 111 ){
	  //  _nEvents++ ;
	    pi0_reco++;
	    if ( _verbose )
	    	std::cout<<"In Ana: found a pi0: "<<std::endl;
	    }
	else 
	   continue ;

	    auto pi0 = p ;

            _distToWall = sqrt(_geoAlgo.SqDist(pi0.Vertex(),fTPC));

	    _mass = pi0.Mass() ; //sqrt(4 * energy_0 * energy_1 * pow(sin(_angle/2.),2));

	    _x_pi0 = pi0.Vertex()[0];
	    _y_pi0 = pi0.Vertex()[1];
	    _z_pi0 = pi0.Vertex()[2];
	    _px_pi0 = pi0.Momentum()[0];
	    _py_pi0 = pi0.Momentum()[1];
	    _pz_pi0 = pi0.Momentum()[2];
	    _p_tot_pi0 = sqrt( _px_pi0 * _px_pi0 +
			       _py_pi0 * _py_pi0 +
			       _pz_pi0 * _pz_pi0 );
	    _e_pi0 = pi0.Energy() ;
	    

	    _x_pi0_mc = xMC ;
	    _y_pi0_mc = yMC ;
	    _z_pi0_mc = zMC ;
	    _e_pi0_mc = eMC ;
	    _m_pi0_mc = mMC ;

	    if(pi0_mc > 0 )
		_n_pi0_mc = 1;

//	    std::cout<<"Vertex points: "<<_x_pi0_mc<<", "<<_y_pi0_mc<<std::endl ;
	    //Loop over daighters of pi0 and extract parameters like containment, length
//	    for ( auto const & n : pi0.Children()){
//
//		auto s = graph.GetParticle(n) ;
//		_x_shr = s.Vertex()[0];
//                _y_shr = s.Vertex()[1];
//		_z_shr = s.Vertex()[2];
//
//	      ::geoalgo::HalfLine shr(s.Vertex(),s.Momentum());
//
//              if(_geoAlgo.Intersection(fTPC,shr,false).size() > 0){
//		
//                _distAlongTraj = sqrt(s.Vertex().SqDist(_geoAlgo.Intersection(fTPC,shr,false)[0])) ;
//		_combinedE = s.Energy() ;
//		}
//              else
//                _distAlongTraj = -999; 
//
//		}


	    if (pi0_mc > 1 )
		_manyPi0++; 

	    if ( pi0.Primary() && graph.GetSiblingNodes(p.ID()).size() == 0 ) { 

		_nNC1Pi0++;
		_NC1pi0_ctr++;

		_nNCIncPi0++;
		_NC_ctr++;

		if ( _verbose ){

		    std::cout << "MC Particle Diagram: " << std::endl
		              << mc_graph.Diagram() << std::endl;

		    std::cout<<"Graph particles : "<<graph.Diagram()<<std::endl ;
		    std::cout<<_NC1pi0_ctr<<"****************SINGLE primaryNC pi0~!!!!!! ********************"<<std::endl;
		    }

	//	std::cout<<"EVENT: "<<_nEvents<<std::endl ;
	//	std::cout<< " Status of CC, NC, NC Single: "<<_nCCPi0<<", "<<_nNCIncPi0<<", "<<_nNC1Pi0<<std::endl ;
		std::cout<<"Fill single NC pi0..."<<std::endl ;
		_pi0_tree->Fill();
		break;

		}
	    
	    _vtx_dist = 10000 ;


	    _vtx_mc_reco = pow( pow(pi0.Vertex()[0] - xMC,2)
			      + pow(pi0.Vertex()[1] - yMC,2)
			      + pow(pi0.Vertex()[2] - zMC,2) ,0.5) ;


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
			std::cout << "MC Particle Diagram: " << std::endl
			              << mc_graph.Diagram() << std::endl;
			std::cout<<"Graph particles :\n "<<graph.Diagram()<<std::endl ;
        	       _nCCPi0++; 
		       _CC_ctr ++ ;
		//	if ( _verbose )
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


			    std::cout << "MC Particle Diagram: " << std::endl
			              << mc_graph.Diagram() << std::endl;

			    std::cout<<"Graph particles : "<<graph.Diagram()<<std::endl ;
			    std::cout<<"******************************This is a NC event "<<std::endl ;
			    }

//			    std::cout<<"******************************This is a NC event "<<std::endl ;
		       continue;
			}
		    //}


        	} 

//		std::cout<< " Status of CC, NC, NC Single: "<<_nCCPi0<<", "<<_nNCIncPi0<<", "<<_nNC1Pi0<<std::endl ;


	    std::cout<<"Fill everything else pi0..."<<std::endl ;
	    _pi0_tree->Fill();


    }


  //080315, Adding this condition so that the tree is filled for every event, even when no pi0 is found
  if ( pi0_reco == 0 )
     _pi0_tree->Fill();
  

  return true; 
  }


  void ERAnaPi0All::ProcessEnd(TFile* fout)
  {

  if ( _nEvents != 0 )
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
