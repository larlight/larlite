#ifndef ERTOOL_ERANASINGLEE_CXX
#define ERTOOL_ERANASINGLEE_CXX

#include "ERAnaSingleE.h"

namespace ertool {

  ERAnaSingleE::ERAnaSingleE() : AnaBase()
			       , _ana_tree(nullptr)
  {
    _name     = "ERAnaSingleE";
    if (_ana_tree) { delete _ana_tree; }
    _ana_tree = new TTree("_ana_tree","Single EAnalysis Tree");
    _ana_tree->Branch("_singleRECO",&_singleRECO,"singleRECO/I");
    _ana_tree->Branch("_singleMC",&_singleMC,"singleMC/I");
    _ana_tree->Branch("_RECOshowers",&_RECOshowers,"RECOshowers/I");
    _ana_tree->Branch("_RECOtracks",&_RECOtracks,"RECOtracks/I");
    _ana_tree->Branch("_MCelectrons",&_MCelectrons,"_MCelectrons/I");
    _ana_tree->Branch("_MCgammas",&_MCgammas,"_MCgammas/I");

    _numEvts = 0;
    _singleE = 0;
  }

  void ERAnaSingleE::Reset()
  {}

  bool ERAnaSingleE::Analyze(const EventData &data, const ParticleSet &ps)
  { 
    _numEvts += 1;
    // count number of showers in event
    _MCelectrons = 0;
    _MCgammas    = 0;
    _singleMC    = 0;
    // Get MC particle set
    auto mc_ps = MCParticleSet();
    for (auto &p : mc_ps){
      if (p.PdgCode() == 22)
	_MCgammas += 1;
      if ((p.PdgCode() == 11) || (p.PdgCode() == -11)){
	_singleMC += 1;
	_MCelectrons += 1;
      }
      for (auto &d : p.AllDaughters()){
	if (d->PdgCode() == 22)
	  _MCgammas += 1;
	if ((d->PdgCode() == 11) || (d->PdgCode() == -11)){
	  _singleMC += 1;
	  _MCelectrons += 1;
	}
      }
    }


    // get data to fill number of RECO tracks & RECO showers
    _RECOshowers = data.AllShower().size();
    _RECOtracks  = data.AllTrack().size();
    
    // ps is the reconstructed ParticleSet
    
    // size of ParticleSet should be the number of
    // single E showers found
    _singleRECO = ps.size();
    if (_singleRECO ==1)
      _singleE += 1;

    _ana_tree->Fill();
  
    return true;
  }
  
  void ERAnaSingleE::ProcessEnd(TFile* fout)
  {

    std::cout << "RESULTS: " << std::endl
	      << "Tot Events    : " << _numEvts << std::endl
	      << "SingleE Events: " << _singleE << std::endl;
    
    if (fout){
      fout->cd();
      _ana_tree->Write();
    }
  }

}

#endif
