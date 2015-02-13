#ifndef ERTOOL_ERANANUMUCC_MISID_CXX
#define ERTOOL_ERANANUMUCC_MISID_CXX

#include "ERAnanumuCC_MisID.h"

namespace ertool {

  ERAnanumuCC_MisID::ERAnanumuCC_MisID() : AnaBase()
					   //					 , _ana_tree(nullptr)
  {
    _name     = "ERAnanumuCC_MisID";
    //    if(_ana_tree) {delete _ana_tree; }
    //_ana_tree->Branch("",&,"/I");

    _numEvents = 0;
    _numMisID = 0;
    
  }

  void ERAnanumuCC_MisID::Reset()
  {}

  bool ERAnanumuCC_MisID::Analyze(const EventData &data, const ParticleSet &ps)
  { 
    
    _numEvents += 1;

    //count the components of the true event
    _MCelec = 0;
    _MCphoton = 0;
    _MCmuon = 0;
    _MCpion = 0;

    //count the components of the reco-ed event
    _RECOelec = 0;
    _RECOphoton = 0;
    _RECOmuon = 0;
    _RECOpion = 0;

    _RECOunknown = 0;

    _RECOshowers = data.AllShower().size();
    _RECOtracks = data.AllTrack().size();
    
    //Parse the MC particle set 
    auto mc_ps = MCParticleSet();
    mc_ps.Collapse();
    
    //iterate through the particle set
    for(auto &mc_p : mc_ps){
      for(auto &mc_d : mc_p.AllDaughters()) {
      if(abs(mc_d->PdgCode()) == 13)
	_MCmuon += 1;
      if(abs(mc_d->PdgCode()) == 11)
	_MCelec += 1;
      if(abs(mc_d->PdgCode()) == 22)
	_MCphoton += 1;
      if(abs(mc_d->PdgCode()) == 111) 
	_MCpion += 1; 
      }

      // //iterate through the daughters of each particle
      //for(auto &mc_d: mc_p.AllDaughters()){
      //}//end iterate through daughters

    }//end iterate particle set
    
    //
    for(auto &p : ps){
      for(auto const& d: p.AllDaughters()){
      if(abs(d->PdgCode()) == 13)
	_RECOmuon += 1;
      if(abs(d->PdgCode()) == 11)
	_RECOelec += 1;
      if(abs(d->PdgCode()) == 22)
	_RECOphoton += 1;
      if(abs(d->PdgCode()) == 111) 
	_RECOpion += 1; 
      if(abs(d->PdgCode()) == 0)
	_RECOunknown += 1;
      }
    }

    return true; }


  void ERAnanumuCC_MisID::ProcessEnd(TFile* fout)
  {

    std::cout << "Results : " << std::endl;
    std::cout << "MC Muon   : \t" << _MCmuon << std::endl ;
    std::cout << "MC Elec   : \t" << _MCelec << std::endl ;
    std::cout << "MC Gamm   : \t" << _MCphoton << std::endl ;
    std::cout << "MC Pion   : \t" << _MCpion << std::endl ;

    std::cout << "\nReco Muon : \t" << _RECOmuon << std::endl ;
    std::cout << "Reco Elec : \t" << _RECOelec << std::endl ;
    std::cout << "Reco Gamm : \t" << _RECOphoton << std::endl ;
    std::cout << "Reco Pion : \t" << _RECOpion << std::endl ;

    std::cout << "\nReco Unknown : \t" << _RECOunknown << std::endl ;
    std::cout << "Reco Showers : \t" << _RECOshowers << std::endl ;
    std::cout << "Reco Tracks  : \t" << _RECOtracks << std::endl ;
  }





}

#endif
