#ifndef ERTOOL_ERANASINGLEE_CXX
#define ERTOOL_ERANASINGLEE_CXX

#include "ERAnaSingleE.h"

namespace ertool {

  ERAnaSingleE::ERAnaSingleE() : AnaBase()
			       , _ana_tree(nullptr)
			       , _result_tree(nullptr)
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

    if (_result_tree) { delete _result_tree; }
    _result_tree = new TTree("_result_tree","Result Tree");
    _result_tree->Branch("_Enu",&_Enu,"Enu/D");
    _result_tree->Branch("_Pdgnu",&_Pdgnu,"Pdgnu/I");
    _result_tree->Branch("_Elep",&_Elep,"Elep/D");
    _result_tree->Branch("_Pdglep",&_Pdglep,"Pdglep/I");
    _result_tree->Branch("_E",&_E,"E/D");
    _result_tree->Branch("_misID",&_misID,"misID/I");

    _numEvts = 0;
    _singleE = 0;

    _debug = false;
  }

  void ERAnaSingleE::Reset()
  {}

  bool ERAnaSingleE::Analyze(const EventData &data, const ParticleSet &ps)
  { 
    
    // Reset tree variables
    // Assume we will mis-ID
    _misID = 1;
    _E = 0;
    _Elep = -0;
    _Pdglep = 0;
    _Enu = 0;
    _Pdgnu = 0;

    _numEvts += 1;
    // count number of showers in event
    _MCelectrons = 0;
    _MCgammas    = 0;
    _singleMC    = 0;

    // Get MC particle set
    auto mc_ps = MCParticleSet();
    for (auto &p : mc_ps){
      // Find the Lepton and store its energy
      if (p.PdgCode() == 12 || p.PdgCode() == -12 || p.PdgCode() == 14 || p.PdgCode() == -14){
	_Enu = p.Energy();
	_Pdgnu = p.PdgCode();
	for (auto &nud : p.Daughters()){
	  if ( nud.PdgCode() == 11 || nud.PdgCode() == -11 || nud.PdgCode() == 13 || nud.PdgCode() == -13){
	    _Elep = nud.Energy();
	    _Pdglep = nud.PdgCode();
	  }
	}
      }
      // now keep track of all showers and tracks
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
    if (_singleRECO ==1 ){
      _E = ps[0].Daughters()[0].Energy();
      _misID = 0;
      _singleE += 1;
    }

    _result_tree->Fill();

    if (_debug){
      std::cout << "Ana results:" << std::endl
		<< "Showers reconstructed: " << _RECOshowers << std::endl
		<< "Tracks reconstructed : " << _RECOtracks << std::endl
		<< "MCelectrons          : " << _MCelectrons << std::endl
		<< "MCgammas             : " << _MCgammas << std::endl
		<< "SingleE Reconstructed: " << _singleRECO << std::endl << std::endl;
    }

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
      _result_tree->Write();
    }
  }

}

#endif
