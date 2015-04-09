#ifndef ERTOOL_ERANASINGLEE_NCBKGD_CXX
#define ERTOOL_ERANASINGLEE_NCBKGD_CXX

#include "ERAnaSingleE_NCBkgd.h"

namespace ertool {

  ERAnaSingleE_NCBkgd::ERAnaSingleE_NCBkgd() : AnaBase()
			       , _result_tree(nullptr)
  {
    _name     = "ERAnaSingleE_NCBkgd";

    if (_result_tree) { delete _result_tree; }
    _result_tree = new TTree("_result_tree","Result Tree");

    //true event
    _result_tree->Branch("_n_showers",&_n_showers,"n_showers/I");
    _result_tree->Branch("_n_gamma",&_n_gamma,"n_gamma/I");
    _result_tree->Branch("_n_elec",&_n_elec,"n_elec/I");
    _result_tree->Branch("_n_track",&_n_track,"n_track/I");
    _result_tree->Branch("_n_protons",&_n_protons,"n_protons/I");
    _result_tree->Branch("_n_neutrons",&_n_neutrons,"n_neutrons/I");
    _result_tree->Branch("_n_piplus",&_n_piplus,"n_piplus/I");
    _result_tree->Branch("_n_pi0",&_n_pi0,"n_pi0/I");
    _result_tree->Branch("_e_nu",&_e_nu,"e_nu/D");
    _result_tree->Branch("_e_lep_candidate",&_e_lep_candidate,"e_lep_candidate/D");
    _result_tree->Branch("_e_mom_lep_candidate",&_e_mom_lep_candidate,"e_mom_lep_candidate/D");
    _result_tree->Branch("_pdg_mom_lep_candidate",&_pdg_mom_lep_candidate,"pdg_mom_lep_candidate/I");
    _result_tree->Branch("_pdg_lep_candidate",&_pdg_lep_candidate,"pdg_lep_candidate/I");
    _result_tree->Branch("_vtxdis_lep_candidate",&_vtxdis_lep_candidate,"vtxdis_lep_candidate/D");
    _result_tree->Branch("_showerL_lep_candidate",&_showerL_lep_candidate,"showerL_lep_candidate/D");

    //reco event
    _result_tree->Branch("_n_showers_RECO",&_n_showers_RECO,"n_showers_RECO/I");
    _result_tree->Branch("_n_gamma_RECO",&_n_gamma_RECO,"n_gamma_RECO/I");
    _result_tree->Branch("_n_elec_RECO",&_n_elec_RECO,"n_elec_RECO/I");
    _result_tree->Branch("_n_track_RECO",&_n_track_RECO,"n_track_RECO/I");
    _result_tree->Branch("_n_protons_RECO",&_n_protons_RECO,"n_protons_RECO/I");
    _result_tree->Branch("_n_neutrons_RECO",&_n_neutrons_RECO,"n_neutrons_RECO/I");
    _result_tree->Branch("_n_piplus_RECO",&_n_piplus_RECO,"n_piplus_RECO/I");
    _result_tree->Branch("_n_pi0_RECO",&_n_pi0_RECO,"n_pi0_RECO/I");
    _result_tree->Branch("_e_nu_RECO",&_e_nu_RECO,"e_nu_RECO/D");
    _result_tree->Branch("_e_lep_candidate_RECO",&_e_lep_candidate_RECO,"e_lep_candidate_RECO/D");
    _result_tree->Branch("_e_mom_lep_candidate_RECO",&_e_mom_lep_candidate_RECO,"e_mom_lep_candidate_RECO/D");
    _result_tree->Branch("_pdg_mom_lep_candidate_RECO",&_pdg_mom_lep_candidate_RECO,"pdg_mom_lep_candidate_RECO/I");
    _result_tree->Branch("_pdg_lep_candidate_RECO",&_pdg_lep_candidate_RECO,"pdg_lep_candidate_RECO/I");
    _result_tree->Branch("_vtxdis_lep_candidate_RECO",&_vtxdis_lep_candidate_RECO,"vtxdis_lep_candidate_RECO/D");
    _result_tree->Branch("_showerL_lep_candidate_RECO",&_showerL_lep_candidate_RECO,"showerL_lep_candidate_RECO/D");
    

  }

  void ERAnaSingleE_NCBkgd::Reset()
  {}

  bool ERAnaSingleE_NCBkgd::Analyze(const EventData &data, const ParticleSet &ps)
  { 



    //Reset all tree variables
    ResetTreeVariables();

    //Get MC EventData (shower/tracks/etc.)
    auto mc_data = MCEventData();
  
    for(auto &s : mc_data.AllShower())
      if(s._energy > 20/*MeV*/){_n_showers += 1;}
    for(auto &t : mc_data.AllTrack())
      if(t._energy > 20/*MeV*/){ _n_track += 1;} 


    //Get EventData
    for(auto &s : data.AllShower())
      if(s._energy > 20/*MeV*/){_n_showers_RECO += 1;}
    for(auto &t : data.AllTrack())
      if(t._energy > 20/*MeV*/){ _n_track_RECO += 1;} 


    //Get MC Particle Set (truth-y truth) 
    auto mc_ps = MCParticleSet();

    for(auto &mc_p : mc_ps){

      int pdg = abs(mc_p.PdgCode());

      if(pdg == 22) _n_gamma += 1;
      else if(pdg == 11) _n_elec += 1;
      else if(pdg == 111) _n_pi0 += 1;
      else if(pdg == 211) _n_piplus += 1;
      else if(pdg == 2212) _n_protons += 1;
      else if(pdg == 2112) _n_neutrons += 1;
      
      //neutrino candidate

      //lepton candidate
      

    }
  


    //Reconstructed Particle Set
    // already loaded as "ps"! 
    
    
    return true; }

  void ERAnaSingleE_NCBkgd::ResetTreeVariables(){

    

  }


}

#endif
