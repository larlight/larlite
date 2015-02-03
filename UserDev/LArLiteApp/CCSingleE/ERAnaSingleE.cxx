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
  }

  void ERAnaSingleE::Reset()
  {}

  bool ERAnaSingleE::Analyze(const EventData &data, const ParticleSet &ps)
  { 
    
    // ps is the reconstructed ParticleSet
    
    // size of ParticleSet should be the number of
    // single E showers found
    _singleRECO = ps.size();

    // now search for the same process in MC
    // get MC
    _singleMC = 0;
    auto mc = MCParticleSet();
    for (auto &p : mc){
      if ( (p.PdgCode() == 11) || (p.PdgCode() == -11) )
	_singleMC +=1;
      for (auto &d : p.AllDaughters()){
	if ( (d->PdgCode() == 11) || (d->PdgCode() == -11) )
	  _singleMC +=1;
      }
    }
    
    _ana_tree->Fill();
  
    return true;
  }
  
  void ERAnaSingleE::ProcessEnd(TFile* fout)
  {
    if (fout){
      fout->cd();
      _ana_tree->Write();
    }
  }

}

#endif
