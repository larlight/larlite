#ifndef LARLITE_EFFPI0_CXX
#define LARLITE_EFFPI0_CXX

#include "effpi0.h"
#include "DataFormat/mcshower.h"

namespace larlite {

  bool effpi0::initialize() {

        _pi0_eff_tree 	= nullptr ;

        _total_events 	= 0  ;
        _passed_events  = 0  ;
	_misID 		= 0  ;
	_fidLoss 	= 0  ;
	
	_nNCpi0		= 0  ;
	_nSingleNCpi0   = 0  ;
	_nCCpi0		= 0  ;
	_event		= 0  ;
	_n_E		= 0  ;

	PrepareTTree() ;
    return true;
  }
 



  void effpi0::PrepareTTree(){
  
      if (_pi0_eff_tree) { delete _pi0_eff_tree; }
        _pi0_eff_tree = new TTree("_pi0_eff_tree","Pi0 Tree");

	_pi0_eff_tree->Branch("totalpi0",&totalpi0,"totalpi0/I");
	_pi0_eff_tree->Branch("cmeson",&cmeson,"cmeson/I");
	_pi0_eff_tree->Branch("mu",&mu,"mu/I");
	_pi0_eff_tree->Branch("nShowers",&nShowers,"nShowers/I");

	_pi0_eff_tree->Branch("pi0Inside",&pi0Inside,"pi0Inside/B");
	_pi0_eff_tree->Branch("lowEnergy",&lowEnergy,"lowEnergy/B");

	_pi0_eff_tree->Branch("_containment",&_containment,"_containment/F");
	_pi0_eff_tree->Branch("contained",&contained,"contained/B");


  }

  void effpi0::Reset(){

	totalpi0    = 0;
        cmeson      = 0;
        mu          = 0;
        nShowers    = 0;

        pi0Inside     = 1;  
        lowEnergy     = 0;
            
        _containment  = 1;
	contained    = 1;

	}

  bool effpi0::analyze(storage_manager* storage) {

	Reset() ;
	_event++;
        // Bring in the info for the event
        auto mctruth = storage->get_data<event_mctruth>("generator");
            if(!mctruth) {
                print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mctruth!"));
                return false;}// if no mctruth
        auto mcpart = mctruth->at(0).GetParticles();
        auto mcnu = mctruth->at(0).GetNeutrino();
	auto ev_mcshower = storage->get_data<event_mcshower>("mcreco");

        _total_events++;

//        int totalpi0 	= 0;
//        int cmeson 	= 0;
//	int mu 		= 0;
//
//	bool inside        = 1; 
//	bool pi0Inside     = 1; 
//	bool contained	   = 1;
//	bool lowEnergy     = 0;

	// If it is ccnc 
//	std::cout<<" 1 means interaction is NC: "<<mcnu.CCNC() <<std::endl ;
	if(_ccnc != mcnu.CCNC()) return false;
	
//       std::cout << "\n\n\n\n\nNew event: "<<std::endl; 
	//loop over all particles to look topology 1 pi0 and no pi+/-
	for(auto const& p : mcpart){
	    auto x = p.Trajectory().at(0).X() ;
	    auto y = p.Trajectory().at(0).Y() ;
	    auto z = p.Trajectory().at(0).Z() ;

	    if( p.PdgCode() == 111 && p.StatusCode() == 1 ){ 

		totalpi0++; 
		if( x < _fv || x > (256 - _fv) || y < (-116 + _fv) || y > (116 - _fv) || z < _fv || z > (1036 - _fv) )//{ //|| length < _length){ 
		//if( x < 0 || x > (256) || y < (-116) 
		  //          || y > (116) || z < 0 || z > (1036) ) 
		    pi0Inside = 0 ;  
		}
	    if( abs( p.PdgCode()) == 13 && p.StatusCode() == 1 ) mu    ++ ;  
	    if( abs( p.PdgCode()) == 211 && p.StatusCode() == 1 ) cmeson++ ;

	    }
	


        event_mcshower selected;
	
        for (auto const& s : *ev_mcshower){
	    nShowers++;
	    if ( s.Start().E() > _energy ) //MeV
		selected.push_back(s);

	    if ( totalpi0 == 1 ){

		if( s.MotherPdgCode() == 111 && s.PdgCode() == 22){
		  //  auto x = s.DetProfile().X();
	    	  //  auto y = s.DetProfile().Y();
	    	  //  auto z = s.DetProfile().Z();

		    _containment = s.DetProfile().E() / s.Start().E() ; 

		    if( _containment < _con ) 
			contained = 0 ;

		  //  auto xEnd = s.End().X();
		  //  auto yEnd = s.End().Y();
		  //  auto zEnd = s.End().Z();

		  //  std::cout<<"Start and end of showeR: \n"<<x<<", "<<y<<", "<<z 
		  //      			    <<"\n"<<xEnd<<", "<<yEnd<<", "<<zEnd<<std::endl ;
		  //  auto length = pow( pow( x - xEnd,2 ) + pow( y - yEnd,2 ) + pow( z - zEnd,2 ),0.5) ;
		  //  std::cout<<"len cut, fv cut, length : "<<length <<std::endl ;

		//    if( x < _fv || x > (256 - _fv) || y < (-116 + _fv) || y > (116 - _fv) || z < _fv || z > (1036 - _fv) ){ //|| length < _length){ 
		//    
		//        inside = 0 ;  
		//        break ; 
		//        }
		    } 
		}
	    }   
        if (selected.size() < 2 && totalpi0 == 1)
	    lowEnergy = 1 ;

	if ( totalpi0  == 1 && cmeson == 0 ){ 
	    _n_E++ ;
	    _pi0_eff_tree->Fill() ;
	    }

	// This will return a bool for signal events
	if( _isSignal ){
	    // Fill in the topo you want to look at -- Exclusive refers to single pi0, no pi+/-
	    if( _isExclusive == 0 ){ 
		if( totalpi0 != 1 ) 
		    return false;
	//	if (  !pi0Inside || !contained || lowEnergy || nShowers != 2){ 
		if (nShowers !=2 ){
		    _fidLoss++;
		    return false;
		    }
		 _nNCpi0++;
		}

	    if( _isExclusive == 1 ){ 
		if( totalpi0 != 1 || cmeson != 0 ) 
		    return false;
//		if ( !pi0Inside || !contained || lowEnergy || nShowers != 2 ){//|| lowEnergy){
		if ( nShowers != 2){
		    _fidLoss++;
		    return false;
		    }

		if( totalpi0 == 1){
		    _nSingleNCpi0++;
		    _nNCpi0++ ;
//		    std::cout<<"Eventzz: "<<_event <<std::endl ;
		    }

		}
	    _passed_events++;
	    return true;
	    }


	// Repeat with negatives if !_isSignal
	//	if( _isExclusive == 0 ){if(totalpi0!=1) return true;}
	//	if( _isExclusive == 1 ){if(totalpi0!=1 || cmeson!=0) return true;}
	//	_misID ++ ;
	//	_passed_events++;
	    return false;

  }

  bool effpi0::finalize() {

       std::cout<<"\n =====================\n Summary of Generator Events \n ====================="<<std::endl;
        std::cout<<" Total Events generated  " << _total_events<<std::endl;
	std::cout<<" With ccnc = "<<_ccnc
		 <<" and total Single pi0 Topology "<<_isExclusive
//		 <<" \nWe're left with:  " << _passed_events<<std::endl;
		 <<" \nLost this many to FV  :  " << _fidLoss
		 <<" \n*******SHOULD BE CORRECT NUMNER :  " << _n_E
		 <<" \nWe're left with NC Inc:  " << _nNCpi0
		 <<" \nWe're left with NC Exc:  " << _nSingleNCpi0
		 <<" \nWe're left with CC ?  :  " << _nCCpi0<<std::endl;
	std::cout<<" Percent NC Inc     :  " << float(_nSingleNCpi0 )/ _n_E * 100
		 <<" \nPercent NC Exc     :  " << float( _nNCpi0 ) / _n_E * 100<<std::endl; 
//	std::cout<<" Percent NC Inc     :  " << float(_nSingleNCpi0 )/ _total_events * 100
//		 <<" \nPercent NC Exc     :  " << float( _nNCpi0 ) / _total_events * 100<<std::endl; 
    //    std::cout<<" Percent of sample " << (double) _passed_events/_total_events<<std::endl;
       std::cout<<"\n =====================\n End  of Generator Events \n ========================"<<std::endl;


    if(_fout) {
           _fout->cd();
           _pi0_eff_tree->Write();
         }

  
    return true;
  }

}
#endif
