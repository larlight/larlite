#ifndef LARLITE_EFFPI0_CXX
#define LARLITE_EFFPI0_CXX

#include "effpi0.h"
#include "DataFormat/mcshower.h"

namespace larlite {

  bool effpi0::initialize() {

	_energy 	= 20 ;
        _total_events 	= 0  ;
        _passed_events  = 0  ;
	_misID 		= 0  ;
	_fidLoss 	= 0  ;
	
	_nNCpi0		= 0  ;
	_nSingleNCpi0   = 0  ;
	_nCCpi0		= 0  ;
	_event		= 0  ;
	_n_E		= 0  ;

    return true;
  }
  
  bool effpi0::analyze(storage_manager* storage) {

       //std::cout << "\n\n\n\n\nNew event: "<<std::endl; 
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

        int totalpi0 	= 0;
        int cmeson 	= 0;
	int mu 		= 0;

	bool inside        = 1; 
	bool pi0Inside     = 1; 
	bool lowEnergy     = 0;

	// If it is ccnc 
//	std::cout<<" 1 means interaction is NC: "<<mcnu.CCNC() <<std::endl ;
	if(_ccnc != mcnu.CCNC()) return false;

	//loop over all particles to look topology 1 pi0 and no pi+/-
	for(auto const& p : mcpart){
	    auto x = p.Trajectory().at(0).X() ;
	    auto y = p.Trajectory().at(0).Y() ;
	    auto z = p.Trajectory().at(0).Z() ;

	    if( p.PdgCode() == 111 && p.StatusCode() == 1 ){ 

		totalpi0++; 
		if( x < 0 || x > (256) || y < (-116) 
		            || y > (116) || z < 0 || z > (1036) ) 
		    pi0Inside = 0 ;  
		}
	    if( abs( p.PdgCode() == 13 ) && p.StatusCode() == 1 ) mu    ++ ;  
	    if( abs( p.PdgCode()) == 211 && p.StatusCode() == 1 ) cmeson++ ;

	    }

        event_mcshower selected;
        for (auto const& s : *ev_mcshower){
	    if ( s.Start().E() > _energy ) //MeV
		selected.push_back(s);

	    if ( totalpi0 == 1 ){

		if( s.MotherPdgCode() == 111 && s.PdgCode() == 22){
		    auto x = s.DetProfile().X();
	    	    auto y = s.DetProfile().Y();
	    	    auto z = s.DetProfile().Z();

		    if( x < _fv || x > (256 - _fv) || y < (-116 + _fv) || y > (116 - _fv) || z < _fv || z > (1036 - _fv) ){ 
//			std::cout<<"BREAKING OUT! Gamma vertex: "<<x<<", "<<y<<", "<<z<<std::endl ;
		    
		        inside = 0 ;  
		        break ; 
		        }
		    } 
		}
	    }   
        if (selected.size() < 2 && totalpi0 == 1){ 
	    _n_E++;
	    lowEnergy = 1 ;
	   // return false;
	    }


  
	// This will return a bool for signal events
	if( _isSignal ){
	    // Fill in the topo you want to look at -- Exclusive refers to single pi0, no pi+/-
	    if( _isExclusive == 0 ){ 
		if( totalpi0 != 1 ) 
		    return false;
		if ( !inside || !pi0Inside ){
		    _fidLoss++;
		    return false;
		    }
		 _nNCpi0++;
		}

	    if( _isExclusive == 1 ){ 
		if( totalpi0 != 1 || cmeson != 0 ) 
		    return false;
		if ( !inside || !pi0Inside ){
		    _fidLoss++;
		    return false;
		    }

		if( totalpi0 == 1){
		    _nSingleNCpi0++;
		    _nNCpi0++ ;
		    std::cout<<"\n\n\n\nEventzz: "<<_event <<std::endl ;

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
		 <<" \nLost to Shower Energy :  " << _n_E
		 <<" \nWe're left with NC Inc:  " << _nNCpi0
		 <<" \nWe're left with NC Exc:  " << _nSingleNCpi0
		 <<" \nWe're left with CC ?  :  " << _nCCpi0<<std::endl;
	std::cout<<" Percent NC Inc     :  " << _nSingleNCpi0 / _total_events 
		 <<" \nPercent NC Exc     :  " << _nNCpi0 / _total_events <<std::endl; 
    //    std::cout<<" Percent of sample " << (double) _passed_events/_total_events<<std::endl;
       std::cout<<"\n =====================\n End  of Generator Events \n ========================"<<std::endl;

  
    return true;
  }

}
#endif
