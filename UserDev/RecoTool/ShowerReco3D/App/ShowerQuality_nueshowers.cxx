#ifndef SHOWERQUALITY_NUESHOWERS_CXX
#define SHOWERQUALITY_NUESHOWERS_CXX

#include "ShowerQuality_nueshowers.h"
#include "LArUtil/Geometry.h"
#include "EMShowerTools/EMShowerProfile.h"
#include "TVector3.h"

namespace larlite {

  ShowerQuality_nueshowers::ShowerQuality_nueshowers() {
    _name = "ShowerQuality_nueshowers";
    _fout = 0;

    fShowerProducer  = "";
    _single_particle_quality = true;

    fShowerTree = nullptr;
    fEventTree = nullptr;
  }

  bool ShowerQuality_nueshowers::initialize() {
    if (fShowerProducer.empty()) {
      print(msg::kERROR, __FUNCTION__, "Shower producer's name is not set!");
      return false;
    }
    InitializeAnaTrees();
    return true;
  }

  bool ShowerQuality_nueshowers::analyze(storage_manager* storage) {

    ResetEventTreeParams();

    if (!_single_particle_quality) {
      print(msg::kERROR, __FUNCTION__, Form("Running on non-single-particle file not implemented yet!\n"));
      return false;
    }

    _alg.InitializeForEvent(storage, fShowerProducer);

    // Retrieve mcshower data product
    auto ev_mcs = storage->get_data<event_mcshower>("mcreco");

    _event  = storage->event_id();
    _run    = storage->run_id();
    _subrun = storage->subrun_id();

    larlite::mcshower nue_shower;

    if (!ev_mcs->empty()) {

      // Before getting the reconstructed showers, we store some true (mcshower) information
      // to be used as the denominator in efficiency calculations (n reco showers / n true showers, etc)
      fEventTreeParams.n_mcshowers = 1;
    
      for(const auto& mcs : *ev_mcs) {
	if (mcs.PdgCode() == 11) {
	  if (mcs.PdgCode() == mcs.MotherPdgCode() && mcs.PdgCode() == mcs.AncestorPdgCode()) {
	    nue_shower = mcs;
	  }
	}
      }

      fEventTreeParams.mcs_E = nue_shower.DetProfile().E();
      fEventTreeParams.mc_containment = nue_shower.DetProfile().E() / nue_shower.Start().E();
    }
    

    // Retrieve shower data product
    auto ev_shower = storage->get_data<event_shower>(fShowerProducer);
    if (!ev_shower) {
      print(msg::kERROR, __FUNCTION__, Form("Did not find shower produced by \"%s\"", fShowerProducer.c_str()));
      return false;
    }

    //Fill event TTree number of reconstructed showers
    fEventTreeParams.n_recoshowers = ev_shower->size();

    if (!ev_shower->size()) {
      //Fill the once-per-event TTree before you quit out early
      fEventTree->Fill();
      return false;
    }

    for (size_t shower_index = 0; shower_index < ev_shower->size(); ++shower_index) {
      auto const& myrecoshower = ev_shower->at(shower_index);
      auto const& mymcshower   = nue_shower;
      FillQualityInfo(myrecoshower, mymcshower, shower_index);
    }


    fEventTree->Fill();

    return true;

  }

  bool ShowerQuality_nueshowers::finalize() {

    if (_fout) {
      if (fShowerTree) fShowerTree->Write();
      if (fEventTree) fEventTree->Write();
    }
    return true;
  }

  void ShowerQuality_nueshowers::FillQualityInfo(const shower& reco_shower, 
						 const mcshower& mc_shower, 
						 size_t shower_index)
  {

    ResetShowerTreeParams();

    fShowerTreeParams.event = _event;
    fShowerTreeParams.run   = _run;
    fShowerTreeParams.event = _event;

    // MC Info
    if (mc_shower.PdgCode() != data::kINVALID_INT) {
      fShowerTreeParams.mc_x = mc_shower.DetProfile().X();
      fShowerTreeParams.mc_y = mc_shower.DetProfile().Y();
      fShowerTreeParams.mc_z = mc_shower.DetProfile().Z();

      fShowerTreeParams.mc_energy = mc_shower.DetProfile().E();
      fShowerTreeParams.mc_containment = mc_shower.DetProfile().E() / mc_shower.Start().E();

      fShowerTreeParams.mc_dcosx = mc_shower.Start().Px() / mc_shower.Start().E();
      fShowerTreeParams.mc_dcosy = mc_shower.Start().Py() / mc_shower.Start().E();
      fShowerTreeParams.mc_dcosz = mc_shower.Start().Pz() / mc_shower.Start().E();

      EMShowerProfile mcshower_helper;
      fShowerTreeParams.mc_length = mcshower_helper.Length( mc_shower.DetProfile().E() );
      TVector3 WildShowerDir( mc_shower.End().X() - mc_shower.Start().X(), mc_shower.End().Y() - mc_shower.Start().Y(), mc_shower.End().Z() - mc_shower.Start().Z() );
      fShowerTreeParams.mc_wildlength = WildShowerDir.Mag();

      // Reco - MC angle diff
      fShowerTreeParams.mc_reco_anglediff = acos( fShowerTreeParams.reco_dcosx * fShowerTreeParams.mc_dcosx +
						  fShowerTreeParams.reco_dcosy * fShowerTreeParams.mc_dcosy +
						  fShowerTreeParams.reco_dcosz * fShowerTreeParams.mc_dcosz ) / 3.14159265359 * 180.;

      // Reco - MC vtx distance
      fShowerTreeParams.mc_reco_dist = sqrt( pow(fShowerTreeParams.reco_x - fShowerTreeParams.mc_x, 2) +
					     pow(fShowerTreeParams.reco_y - fShowerTreeParams.mc_y, 2) +
					     pow(fShowerTreeParams.reco_z - fShowerTreeParams.mc_z, 2) );
      
      
      fShowerTreeParams.mc_dedx     = mc_shower.dEdx();
      fShowerTreeParams.mc_dqdx_U   = mc_shower.dQdx(0);
      fShowerTreeParams.mc_dqdx_V   = mc_shower.dQdx(1);
      fShowerTreeParams.mc_dqdx_Y   = mc_shower.dQdx(2);
    
      fShowerTreeParams.mc_charge_U = mc_shower.Charge(0);
      fShowerTreeParams.mc_charge_V = mc_shower.Charge(1);
      fShowerTreeParams.mc_charge_Y = mc_shower.Charge(2);
      
      fShowerTreeParams.mc_pdg_code = mc_shower.PdgCode();
    }

    // Reco vtx
    fShowerTreeParams.reco_x = reco_shower.ShowerStart()[0];
    fShowerTreeParams.reco_y = reco_shower.ShowerStart()[1];
    fShowerTreeParams.reco_z = reco_shower.ShowerStart()[2];

    // Reco angle
    fShowerTreeParams.reco_dcosx = reco_shower.Direction()[0];
    fShowerTreeParams.reco_dcosy = reco_shower.Direction()[1];
    fShowerTreeParams.reco_dcosz = reco_shower.Direction()[2];


    // Reco cluster efficiency & purity
    auto eff_purs = _alg.ClusterEP(shower_index);
    fShowerTreeParams.cluster_eff_U = eff_purs.at(0).first;
    fShowerTreeParams.cluster_eff_V = eff_purs.at(1).first;
    fShowerTreeParams.cluster_eff_Y = eff_purs.at(2).first;
    fShowerTreeParams.cluster_pur_U = eff_purs.at(0).second;
    fShowerTreeParams.cluster_pur_V = eff_purs.at(1).second;
    fShowerTreeParams.cluster_pur_Y = eff_purs.at(2).second;

    fShowerTreeParams.reco_energy_U = reco_shower.Energy_v().at(0);
    fShowerTreeParams.reco_energy_V = reco_shower.Energy_v().at(1);
    fShowerTreeParams.reco_energy_Y = reco_shower.Energy_v().at(2);

    fShowerTreeParams.reco_dedx     = reco_shower.dEdx();
    fShowerTreeParams.reco_dedx_U   = reco_shower.dEdx_v().at(0);
    fShowerTreeParams.reco_dedx_V   = reco_shower.dEdx_v().at(1);
    fShowerTreeParams.reco_dedx_Y   = reco_shower.dEdx_v().at(2);

    fShowerTreeParams.reco_dqdx   = reco_shower.dQdx();
    fShowerTreeParams.reco_dqdx_U = reco_shower.dQdx_v().at(0);
    fShowerTreeParams.reco_dqdx_V = reco_shower.dQdx_v().at(1);                                                           
    fShowerTreeParams.reco_dqdx_Y = reco_shower.dQdx_v().at(2);

    fShowerTreeParams.reco_length = reco_shower.Length();
    fShowerTreeParams.reco_width1 = reco_shower.Width()[0];
    fShowerTreeParams.reco_width2 = reco_shower.Width()[1];

    fShowerTreeParams.match = 1;

    // Fill Tree
    fShowerTree->Fill();

  }

  void ShowerQuality_nueshowers::InitializeAnaTrees()
  {

    //////////////////////////////////////////////////////
    // This tree is filled once per reconstructed shower
    //////////////////////////////////////////////////////
    if (fShowerTree) delete fShowerTree;
    fShowerTree = new TTree("fShowerTree_nueshowers", "");

    fShowerTree->Branch("event", &_event, "event/I");
    fShowerTree->Branch("run", &_run, "run/I");
    fShowerTree->Branch("subrun", &_subrun, "subrun/I");
    fShowerTree->Branch("reco_x", &fShowerTreeParams.reco_x, "reco_x/D");
    fShowerTree->Branch("reco_y", &fShowerTreeParams.reco_y, "reco_y/D");
    fShowerTree->Branch("reco_z", &fShowerTreeParams.reco_z, "reco_z/D");
    fShowerTree->Branch("reco_dcosx", &fShowerTreeParams.reco_dcosx, "reco_dcosx/D");
    fShowerTree->Branch("reco_dcosy", &fShowerTreeParams.reco_dcosy, "reco_dcosy/D");
    fShowerTree->Branch("reco_dcosz", &fShowerTreeParams.reco_dcosz, "reco_dcosz/D");
    fShowerTree->Branch("reco_energy_U", &fShowerTreeParams.reco_energy_U, "reco_energy_U/D");
    fShowerTree->Branch("reco_energy_V", &fShowerTreeParams.reco_energy_V, "reco_energy_V/D");
    fShowerTree->Branch("reco_energy_Y", &fShowerTreeParams.reco_energy_Y, "reco_energy_Y/D");
    fShowerTree->Branch("mc_x", &fShowerTreeParams.mc_x, "mc_x/D");
    fShowerTree->Branch("mc_y", &fShowerTreeParams.mc_y, "mc_y/D");
    fShowerTree->Branch("mc_z", &fShowerTreeParams.mc_z, "mc_z/D");
    fShowerTree->Branch("mc_dcosx", &fShowerTreeParams.mc_dcosx, "mc_dcosx/D");
    fShowerTree->Branch("mc_dcosy", &fShowerTreeParams.mc_dcosy, "mc_dcosy/D");
    fShowerTree->Branch("mc_dcosz", &fShowerTreeParams.mc_dcosz, "mc_dcosz/D");
    fShowerTree->Branch("reco_dqdx", &fShowerTreeParams.reco_dqdx, "reco_dqdx/D");
    fShowerTree->Branch("reco_dqdx_U", &fShowerTreeParams.reco_dqdx_U, "reco_dqdx_U/D");
    fShowerTree->Branch("reco_dqdx_V", &fShowerTreeParams.reco_dqdx_V, "reco_dqdx_V/D");
    fShowerTree->Branch("reco_dqdx_Y", &fShowerTreeParams.reco_dqdx_Y, "reco_dqdx_Y/D");
    fShowerTree->Branch("mc_energy", &fShowerTreeParams.mc_energy, "mc_energy/D");
    fShowerTree->Branch("reco_dedx", &fShowerTreeParams.reco_dedx, "reco_dedx/D");
    fShowerTree->Branch("reco_dedx_U", &fShowerTreeParams.reco_dedx_U, "reco_dedx_U/D");
    fShowerTree->Branch("reco_dedx_V", &fShowerTreeParams.reco_dedx_V, "reco_dedx_V/D");
    fShowerTree->Branch("reco_dedx_Y", &fShowerTreeParams.reco_dedx_Y, "reco_dedx_Y/D");
    fShowerTree->Branch("mc_reco_anglediff", &fShowerTreeParams.mc_reco_anglediff, "mc_reco_anglediff/D");
    fShowerTree->Branch("mc_reco_dist", &fShowerTreeParams.mc_reco_dist, "mc_reco_dist/D");
    fShowerTree->Branch("cluster_eff_U", &fShowerTreeParams.cluster_eff_U, "cluster_eff_U/D");
    fShowerTree->Branch("cluster_eff_V", &fShowerTreeParams.cluster_eff_V, "cluster_eff_V/D");
    fShowerTree->Branch("cluster_eff_Y", &fShowerTreeParams.cluster_eff_Y, "cluster_eff_Y/D");
    fShowerTree->Branch("cluster_pur_U", &fShowerTreeParams.cluster_pur_U, "cluster_pur_U/D");
    fShowerTree->Branch("cluster_pur_V", &fShowerTreeParams.cluster_pur_V, "cluster_pur_V/D");
    fShowerTree->Branch("cluster_pur_Y", &fShowerTreeParams.cluster_pur_Y, "cluster_pur_Y/D");
    fShowerTree->Branch("mc_containment", &fShowerTreeParams.mc_containment, "mc_containment/D");
    fShowerTree->Branch("reco_length",&fShowerTreeParams.reco_length,"reco_length/D");
    fShowerTree->Branch("reco_width1",&fShowerTreeParams.reco_width1,"reco_width1/D");
    fShowerTree->Branch("reco_width2",&fShowerTreeParams.reco_width2,"reco_width2/D");
    fShowerTree->Branch("mc_length",  &fShowerTreeParams.mc_length,  "mc_length/D");
    fShowerTree->Branch("mc_wildlength", &fShowerTreeParams.mc_wildlength, "mc_wildlength/D");
    fShowerTree->Branch("match", &fShowerTreeParams.match, "match/I");

    fShowerTree->Branch("mc_dedx"     , &fShowerTreeParams.mc_dedx     , "mc_dedx/D");
    fShowerTree->Branch("mc_dqdx_U"   , &fShowerTreeParams.mc_dqdx_U   , "mc_dqdx_U/D");
    fShowerTree->Branch("mc_dqdx_V"   , &fShowerTreeParams.mc_dqdx_V   , "mc_dqdx_V/D");
    fShowerTree->Branch("mc_dqdx_Y"   , &fShowerTreeParams.mc_dqdx_Y   , "mc_dqdx_Y/D");
    fShowerTree->Branch("mc_charge_U" , &fShowerTreeParams.mc_charge_U , "mc_charge_U/D");
    fShowerTree->Branch("mc_charge_V" , &fShowerTreeParams.mc_charge_V , "mc_charge_V/D");
    fShowerTree->Branch("mc_charge_Y" , &fShowerTreeParams.mc_charge_Y , "mc_charge_Y/D");
    fShowerTree->Branch("mc_pdg_code" , &fShowerTreeParams.mc_pdg_code , "mc_pdg_code/I");


    //////////////////////////////////////////////////////
    // This tree is filled once per event
    //////////////////////////////////////////////////////
    if (fEventTree) delete fEventTree;
    fEventTree = new TTree("fEventTree_nueshowers", "");
    fEventTree->Branch("event", &_event, "event/I");
    fEventTree->Branch("run", &_run, "run/I");
    fEventTree->Branch("subrun", &_subrun, "subrun/I");
    fEventTree->Branch("n_recoshowers", &fEventTreeParams.n_recoshowers, "n_recoshowers/I");
    fEventTree->Branch("n_mcshowers", &fEventTreeParams.n_mcshowers, "n_mcshowers/I");
    fEventTree->Branch("mcs_E", &fEventTreeParams.mcs_E, "mcs_E/D");
    fEventTree->Branch("mc_containment", &fEventTreeParams.mc_containment, "mc_containment/D");
  }

  void ShowerQuality_nueshowers::ResetShowerTreeParams() {

    fShowerTreeParams.reco_x = -1.0*data::kINVALID_DOUBLE; 
    fShowerTreeParams.reco_y = -1.0*data::kINVALID_DOUBLE; 
    fShowerTreeParams.reco_z = -1.0*data::kINVALID_DOUBLE;

    fShowerTreeParams.reco_dcosx = -1.0*data::kINVALID_DOUBLE; 
    fShowerTreeParams.reco_dcosy = -1.0*data::kINVALID_DOUBLE; 
    fShowerTreeParams.reco_dcosz = -1.0*data::kINVALID_DOUBLE;

    fShowerTreeParams.reco_energy_U = -1.0*data::kINVALID_DOUBLE;
    fShowerTreeParams.reco_energy_V = -1.0*data::kINVALID_DOUBLE;
    fShowerTreeParams.reco_energy_Y = -1.0*data::kINVALID_DOUBLE;

    fShowerTreeParams.reco_dedx_U = -1.0*data::kINVALID_DOUBLE;
    fShowerTreeParams.reco_dedx_V = -1.0*data::kINVALID_DOUBLE;
    fShowerTreeParams.reco_dedx_Y = -1.0*data::kINVALID_DOUBLE;

    fShowerTreeParams.reco_dqdx_U =-1.0*data::kINVALID_DOUBLE;
    fShowerTreeParams.reco_dqdx_V =-1.0*data::kINVALID_DOUBLE;
    fShowerTreeParams.reco_dqdx_Y =-1.0*data::kINVALID_DOUBLE;

    fShowerTreeParams.mc_x = -1.0*data::kINVALID_DOUBLE; 
    fShowerTreeParams.mc_y = -1.0*data::kINVALID_DOUBLE; 
    fShowerTreeParams.mc_z = -1.0*data::kINVALID_DOUBLE;

    fShowerTreeParams.mc_dcosx = -1.0*data::kINVALID_DOUBLE; 
    fShowerTreeParams.mc_dcosy = -1.0*data::kINVALID_DOUBLE; 
    fShowerTreeParams.mc_dcosz = -1.0*data::kINVALID_DOUBLE;

    fShowerTreeParams.mc_energy = -1.0*data::kINVALID_DOUBLE;

    fShowerTreeParams.mc_reco_anglediff = -1.0*data::kINVALID_DOUBLE;

    fShowerTreeParams.mc_reco_dist = -1.0*data::kINVALID_DOUBLE;

    fShowerTreeParams.cluster_eff_U = -1.0*data::kINVALID_DOUBLE;
    fShowerTreeParams.cluster_eff_V = -1.0*data::kINVALID_DOUBLE;
    fShowerTreeParams.cluster_eff_Y = -1.0*data::kINVALID_DOUBLE;

    fShowerTreeParams.cluster_pur_U = -1.0*data::kINVALID_DOUBLE;
    fShowerTreeParams.cluster_pur_V = -1.0*data::kINVALID_DOUBLE;
    fShowerTreeParams.cluster_pur_Y = -1.0*data::kINVALID_DOUBLE;

    fShowerTreeParams.mc_containment = -1.0*data::kINVALID_DOUBLE;

    fShowerTreeParams.reco_length = -1.0*data::kINVALID_DOUBLE;
    fShowerTreeParams.reco_width1 = -1.0*data::kINVALID_DOUBLE;
    fShowerTreeParams.reco_width2 = -1.0*data::kINVALID_DOUBLE;

    fShowerTreeParams.mc_length   = -1.0*data::kINVALID_DOUBLE;
    fShowerTreeParams.mc_wildlength  = -1.0*data::kINVALID_DOUBLE;
    
    fShowerTreeParams.mc_dedx     = -1.0*data::kINVALID_DOUBLE;
    fShowerTreeParams.mc_dqdx_U   = -1.0*data::kINVALID_DOUBLE;
    fShowerTreeParams.mc_dqdx_V   = -1.0*data::kINVALID_DOUBLE;
    fShowerTreeParams.mc_dqdx_Y   = -1.0*data::kINVALID_DOUBLE;

    fShowerTreeParams.mc_charge_U = -1.0*data::kINVALID_DOUBLE;
    fShowerTreeParams.mc_charge_V = -1.0*data::kINVALID_DOUBLE;
    fShowerTreeParams.mc_charge_Y = -1.0*data::kINVALID_DOUBLE;

    fShowerTreeParams.mc_pdg_code = -1.0*data::kINVALID_INT;

  }

  void ShowerQuality_nueshowers::ResetEventTreeParams() {
    fEventTreeParams.n_mcshowers = 0;
    fEventTreeParams.n_recoshowers = 0;
    fEventTreeParams.mcs_E = -1.0*data::kINVALID_DOUBLE;
    fEventTreeParams.mc_containment = -1.0*data::kINVALID_DOUBLE;
  }


}
#endif
