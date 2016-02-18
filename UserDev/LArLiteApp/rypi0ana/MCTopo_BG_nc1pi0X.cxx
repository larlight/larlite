#ifndef LARLITE_MCTOPO_BG_NC1PI0X_CXX
#define LARLITE_MCTOPO_BG_NC1PI0X_CXX

#include "MCTopo_BG_nc1pi0X.h"

namespace larlite {

  bool MCTopo_BG_nc1pi0X::initialize() {

    return true;
  }
  
  bool MCTopo_BG_nc1pi0X::analyze(storage_manager* storage) {
	_nEvt++;
	
        // Bring in the info for the event
        auto mctruth = storage->get_data<event_mctruth>("generator");
            if(!mctruth) {
                print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mctruth!"));
                return false;}// if no mctruth
	// Get all the particles
	// Get the Neutrino
        auto mcpart = mctruth->at(0).GetParticles();
        auto mcnu = mctruth->at(0).GetNeutrino();
	auto nupart = mcnu.Nu();

	bool nio = false;
            auto nux = nupart.Trajectory().at(0).X() ;
            auto nuy = nupart.Trajectory().at(0).Y() ;
            auto nuz = nupart.Trajectory().at(0).Z() ;
auto DetW = larutil::Geometry::GetME()->DetHalfWidth();
auto DetH = larutil::Geometry::GetME()->DetHalfHeight();
auto DetL = larutil::Geometry::GetME()->DetLength();
                if(nux<2.*DetW-_fid && nux>0.+_fid && nuy<DetH-_fid && nuy>-DetH+_fid && nuz<DetL-_fid && nuz> 0.0 +_fid){
			_nEvtinvol++;
			nio = true;
		}
	if(!nio) return false;
	if( mcnu.CCNC()==0){ return false;}
	//=&==&==&==&==&==&
	// What Type of Event 
	//=&==&==&==&==&==&
        int pi0    = 0;
        int cmeson      = 0;
        int ckaon      = 0;
	
        for(auto const& p : mcpart){
            auto x = p.Trajectory().at(0).X() ;
            auto y = p.Trajectory().at(0).Y() ;
            auto z = p.Trajectory().at(0).Z() ;
auto DetW = larutil::Geometry::GetME()->DetHalfWidth();
auto DetH = larutil::Geometry::GetME()->DetHalfHeight();
auto DetL = larutil::Geometry::GetME()->DetLength();
                if(nux<2.*DetW-_fid && nux>0.+_fid && nuy<DetH-_fid && nuy>-DetH+_fid && nuz<DetL-_fid && nuz> 0.0 +_fid){
			if(p.PdgCode()==111 && p.StatusCode()==1) pi0++;
			if(abs(p.PdgCode())==211 && p.StatusCode()==1) cmeson++;
			if(abs(p.PdgCode())==321 && p.StatusCode()==1) ckaon++;
			if(abs(p.PdgCode())==311 && p.StatusCode()==1) ckaon++;
		}// If inside of the fiducial
	}// for loop over mcpart
	// if it is not signal....	
	 if(pi0==1 && (cmeson!=0 || ckaon!=0)){ _PassedEvt++; return true ;} 


		    return false;
  }

  bool MCTopo_BG_nc1pi0X::finalize() {
	std::cout<<"  \t PassedEvt  "<< _PassedEvt<<" evts in volume " <<_nEvtinvol<< " out of "<<_nEvt<<std::endl;

    return true;
  }

}
#endif
