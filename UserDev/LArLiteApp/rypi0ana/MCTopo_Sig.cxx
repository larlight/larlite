#ifndef LARLITE_MCTOPO_SIG_CXX
#define LARLITE_MCTOPO_SIG_CXX

#include "MCTopo_Sig.h"

namespace larlite {

  bool MCTopo_Sig::initialize() {

    return true;
  }
  
  bool MCTopo_Sig::analyze(storage_manager* storage) {
	

        // Bring in the info for the event
        auto mctruth = storage->get_data<event_mctruth>("generator");
            if(!mctruth) {
                print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mctruth!"));
                return false;}// if no mctruth
	// Get all the particles
	// Get the Neutrino
        auto mcpart = mctruth->at(0).GetParticles();
        auto mcnu = mctruth->at(0).GetNeutrino();
        auto ev_mcshower = storage->get_data<event_mcshower>("mcreco");// Get the mcshower info
	
	auto nupart = mcnu.Nu();
	//=&==&==&==&==&==&
	// if ccnc or both 
	//=&==&==&==&==&==&
	_nEvt++;
	if( mcnu.CCNC()==0) return false;

	bool nio = false;
            auto nux = nupart.Trajectory().at(0).X() ;
            auto nuy = nupart.Trajectory().at(0).Y() ;
            auto nuz = nupart.Trajectory().at(0).Z() ;
auto DetW = larutil::Geometry::GetME()->DetHalfWidth();
auto DetH = larutil::Geometry::GetME()->DetHalfHeight();
auto DetL = larutil::Geometry::GetME()->DetLength();
//	std::cout<<"Width "<<DetW<<"Height "<<DetH<<"Length "<<DetL<<std::endl;
		if(nux<2.*DetW-_fid && nux>0.+_fid && nuy<DetH-_fid && nuy>-DetH+_fid && nuz<DetL-_fid && nuz> 0.0 +_fid){ 
			_nEvtinvol++;
			nio = true;
		}
	if(!nio) return false;


	//=&==&==&==&==&==&
	// What Type of Event 
	//=&==&==&==&==&==&
        int pi0    = 0;
        int notpripi0    = 0;
        int cmeson      = 0;
        int ckaon      = 0;
	double pi0momentum = 123456789;
	
        for(auto const& p : mcpart){
            auto x = p.Trajectory().at(0).X() ;
            auto y = p.Trajectory().at(0).Y() ;
            auto z = p.Trajectory().at(0).Z() ;
		// If the particle is contained then log 
	//	if((x>256.35-_fid || x<0+_fid || y>116.5-_fid || y< -116.5+_fid || z>1036.8-_fid || z< 0+_fid)&&p.StatusCode()==1) std::cout<<"Particle Outside? "<<std::endl;
		//if(x<256.35-_fid && x>0.+_fid && y<116.5-_fid && y> -116.5+_fid && z<1036.8-_fid && z> 0.+_fid){ 
		if(x<2.*DetW-_fid && x>0.+_fid && y<DetH-_fid && y>-DetH+_fid && z<DetL-_fid && z> 0.0 +_fid){ 
			if(p.PdgCode()==111 && p.StatusCode()==1){ pi0++; pi0momentum =p.Trajectory().at(0).E();}
			//if(p.PdgCode()==111 && p.StatusCode()==1) pi0++;
			//if(p.PdgCode()==111 && p.StatusCode()!=1){std::cout<<"STATUS CODE"<<p.StatusCode()<<std::endl; notpripi0++;}
			if(abs(p.PdgCode())==211 && p.StatusCode()==1) cmeson++;
			//if(abs(p.PdgCode()==321) && p.StatusCode()==1) ckaon++;
			if(abs(p.PdgCode())==321 && p.StatusCode()==1){std::cout<<"OOOOOOOOOOH CRAP a primary char Kaon :) "<<std::endl; ckaon++;}
			if(abs(p.PdgCode())==311 && p.StatusCode()==1){std::cout<<"OOOOOOOOOOH CRAP a primary 0Kaon :) "<<std::endl; ckaon++;}
		}// If inside of the fiducial
	}// for loop over mcpart

	// if it is not signal....	
	 if(pi0!=1 || cmeson!=0 || ckaon!=0){ return false;} 
//	if(pi0==1 && pi0momentum<250) return false;
	 //if(pi0!=1 || cmeson!=0){ return false;} 
	//=&==&==&==&==&==&==&==&==&==&==&
	// If we make it this far
	// Look at the daughter products 
	//=&==&==&==&==&==&==&==&==&==&==&
	// First we need to check if it is a dalitz decay
	event_mcshower showers;
        for (auto const& s : *ev_mcshower) 
		{if(s.MotherPdgCode() == 111){
			if(abs(s.PdgCode()) == 11){ _Lost_Dalitz++; return false;}
			if(s.Start().E() >= _showerenergy && s.DetProfile().E()>=_showerdetenergy && s.Start().E()/s.DetProfile().E()>=_showercontain && s.MotherProcess()=="primary") showers.push_back(s);}
			//if(s.Start().E() > _showerenergy && s.DetProfile().E()>_showerdetenergy && s.Start().E()/s.DetProfile().E()>_showercontain && s.MotherProcess()=="primary") showers.push_back(s);}
		}
			
	if(showers.size()>2 && pi0==1 && cmeson==0 && ckaon==0 ){ std::cout<<"Larger than 2 shower?!?!?! This will come up when looking at background only"<<std::endl;}
	if(showers.size()!=2 && pi0==1 && cmeson==0 && ckaon==0 ){ _Lost_Shower++; return false;}

 _PassedEvt++;
//if(notpripi0!=0) std::cout<<" NOT PRI PI0 :"<<notpripi0<<std::endl;
    return true;
	// This is bad c++ but works
  }

  bool MCTopo_Sig::finalize() {

	std::cout<<"Summary of MCTOPO:\n \t Filter mode CCNC "<< _ccnc <<" Was it Both? " <<_bothccnc<<std::endl;
	std::cout<<"\n \t Signal Or Bk "<< _sig <<std::endl;
	std::cout<<"  \t PassedEvt  "<< _PassedEvt<<" Number of evt Lost to Showers"<< _Lost_Shower<<" Number for Dalitz " << _Lost_Dalitz <<"  Total NC evts in volume " <<_nEvtinvol<< " out of "<<_nEvt<<std::endl;

    return true;
  }

}
#endif
