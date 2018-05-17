#ifndef SHOWERQUALITY_DL_CXX
#define SHOWERQUALITY_DL_CXX

#include "ShowerQuality_DL.h"

#include "DataFormat/vertex.h"
#include "DataFormat/pfpart.h"
#include "DataFormat/mctruth.h"
#include "DataFormat/mcflux.h"
#include "DataFormat/gtruth.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/shower.h"

#include "LArUtil/Geometry.h"
#include "EMShowerTools/EMShowerProfile.h"

#include "TVector3.h"

#include <cassert>
#include <exception>

namespace larlite {

  ShowerQuality_DL::ShowerQuality_DL() {
    _name = "ShowerQuality_DL";

    fVertexProducer = "";
    fShowerProducer = "";

    fEventTree = nullptr;
    fShowerTree = nullptr;
    fAndyTree = nullptr;
  }

  bool ShowerQuality_DL::initialize() {
    if (fVertexProducer.empty()) {
      print(msg::kERROR, __FUNCTION__, "Vertex producer's name is not set!");
      throw std::exception();
    }

    if (fShowerProducer.empty()) {
      print(msg::kERROR, __FUNCTION__, "Shower producer's name is not set!");
      throw std::exception();
    }

    //
    // Event Tree
    //
    fEventTree = new TTree("EventMCINFO_DL", "");
    fEventTree->Branch("event"  , &_event  , "event/I");
    fEventTree->Branch("subrun" , &_subrun , "subrun/I");
    fEventTree->Branch("run"    , &_run    , "run/I");
    fEventTree->Branch("entry"  , &_entry  , "entry/I");

    fEventTree->Branch("mc_energy", &_mc_energy, "mc_energy/D");
    fEventTree->Branch("mcinfoInteractionType", &_mcinfoInteractionType, "mcinfoInteractionType/I");
    fEventTree->Branch("mcinfoMode", &_mcinfoMode, "mcinfoMode/I");
    fEventTree->Branch("mc_pdg_code", &_mc_pdg_code, "mc_pdg_code/I");
    fEventTree->Branch("mc_x", &_mc_x, "mc_x/D");
    fEventTree->Branch("mc_y", &_mc_y, "mc_y/D");
    fEventTree->Branch("mc_z", &_mc_z, "mc_z/D");
    fEventTree->Branch("mc_dcosx" , &_mc_dcosx,  "mc_dcosx/D");
    fEventTree->Branch("mc_dcosy" , &_mc_dcosy,  "mc_dcosy/D");
    fEventTree->Branch("mc_dcosz" , &_mc_dcosz,  "mc_dcosz/D");
    fEventTree->Branch("mc_energy", &_mc_energy, "mc_energy/D");

    // 
    // when segment shits are not available
    //
    fEventTree->Branch("selected1L1P", &_selected, "selected1L1P/I");
    fEventTree->Branch("parentPDG", &_parent_pdg, "parentPDG/I");
    fEventTree->Branch("energyInit", &_energy_init, "energyInit/D");
    fEventTree->Branch("parentX", &_parent_x, "parentX/D");
    fEventTree->Branch("parentY", &_parent_y, "parentY/D");
    fEventTree->Branch("parentZ", &_parent_z, "parentZ/D");

    fEventTree->Branch("parentSCEX", &_parent_sce_x, "parentSCEX/D");
    fEventTree->Branch("parentSCEY", &_parent_sce_y, "parentSCEY/D");
    fEventTree->Branch("parentSCEZ", &_parent_sce_z, "parentSCEZ/D");

    fEventTree->Branch("parentPx", &_parent_px, "parentPx/D");
    fEventTree->Branch("parentPy", &_parent_py, "parentPy/D");
    fEventTree->Branch("parentPz", &_parent_pz, "parentPz/D");

    fEventTree->Branch("ineractionMode", &_interaction_mode, "interactionMode/I");
    fEventTree->Branch("interactionType", &_interaction_type, "interactionType/I");

    fEventTree->Branch("dep_sum_lepton", &_dep_sum_lepton, "dep_sum_lepton/D");
    fEventTree->Branch("dep_sum_proton", &_dep_sum_proton, "dep_sum_proton/D");

    fEventTree->Branch("proton_Px", &_proton_Px, "proton_Px/D");
    fEventTree->Branch("proton_Py", &_proton_Py, "proton_Py/D");
    fEventTree->Branch("proton_Pz", &_proton_Pz, "proton_Pz/D");

    fEventTree->Branch("lepton_Px", &_lepton_Px, "lepton_Px/D");
    fEventTree->Branch("lepton_Py", &_lepton_Py, "lepton_Py/D");
    fEventTree->Branch("lepton_Pz", &_lepton_Pz, "lepton_Pz/D");

    fEventTree->Branch("daughter_energydep_v", &_daughter_energydep_v);

    fEventTree->Branch("daughterPdg_v", &_daughter_pdg_v);

    fEventTree->Branch("daughterX_v", &_daughterX_v);
    fEventTree->Branch("daughterY_v", &_daughterY_v);
    fEventTree->Branch("daughterZ_v", &_daughterZ_v);
    
    fEventTree->Branch("daughterPx_v", &_daughterPx_v);
    fEventTree->Branch("daughterPy_v", &_daughterPy_v);
    fEventTree->Branch("daughterPz_v", &_daughterPz_v);


    // 
    // Shower Tree
    //

    fShowerTree = new TTree("ShowerQuality_DL", "");
    fShowerTree->Branch("event"  , &_event  , "event/I");
    fShowerTree->Branch("subrun" , &_subrun , "subrun/I");
    fShowerTree->Branch("run"    , &_run    , "run/I");
    fShowerTree->Branch("entry"  , &_entry  , "entry/I");

    //
    // Andy Tree
    //
    fAndyTree = new TTree("EventAndyTree","");
    fAndyTree->Branch("event"  , &_event  , "event/I");
    fAndyTree->Branch("subrun" , &_subrun , "subrun/I");
    fAndyTree->Branch("run"    , &_run    , "run/I");
    fAndyTree->Branch("entry"  , &_entry  , "entry/I");
    AttachAndy(fAndyTree);
    
    //
    // reco
    //
    fShowerTree->Branch("vtx_id", &_vtx_id, "vtx_id/I");
    fShowerTree->Branch("nshowers", &_nshowers, "nshowers/I");
    
    fShowerTree->Branch("reco_x_v", &_reco_x_v);
    fShowerTree->Branch("reco_y_v", &_reco_y_v);
    fShowerTree->Branch("reco_z_v", &_reco_z_v);
    fShowerTree->Branch("reco_dcosx_v", &_reco_dcosx_v);
    fShowerTree->Branch("reco_dcosy_v", &_reco_dcosy_v);
    fShowerTree->Branch("reco_dcosz_v", &_reco_dcosz_v);
    fShowerTree->Branch("reco_phi_v", &_reco_phi_v);
    fShowerTree->Branch("reco_theta_v", &_reco_theta_v);
    fShowerTree->Branch("reco_energy_U_v", &_reco_energy_U_v);
    fShowerTree->Branch("reco_energy_V_v", &_reco_energy_V_v);
    fShowerTree->Branch("reco_energy_Y_v", &_reco_energy_Y_v);
    fShowerTree->Branch("reco_dedx_v", &_reco_dedx_v);
    fShowerTree->Branch("reco_dedx_U_v", &_reco_dedx_U_v);
    fShowerTree->Branch("reco_dedx_V_v", &_reco_dedx_V_v);
    fShowerTree->Branch("reco_dedx_Y_v", &_reco_dedx_Y_v);
    fShowerTree->Branch("reco_dqdx_v", &_reco_dqdx_v);
    fShowerTree->Branch("reco_dqdx_U_v", &_reco_dqdx_U_v);
    fShowerTree->Branch("reco_dqdx_V_v", &_reco_dqdx_V_v);
    fShowerTree->Branch("reco_dqdx_Y_v", &_reco_dqdx_Y_v);
    fShowerTree->Branch("reco_length_v", &_reco_length_v);
    fShowerTree->Branch("reco_width1_v", &_reco_width1_v);
    fShowerTree->Branch("reco_width2_v", &_reco_width2_v);

    fShowerTree->Branch("mc_reco_anglediff_v", &_mc_reco_anglediff_v);
    fShowerTree->Branch("mc_reco_dist_v", &_mc_reco_dist_v);
    fShowerTree->Branch("cluster_eff_U_v", &_cluster_eff_U_v);
    fShowerTree->Branch("cluster_eff_V_v", &_cluster_eff_V_v);
    fShowerTree->Branch("cluster_eff_Y_v", &_cluster_eff_Y_v);
    fShowerTree->Branch("cluster_pur_U_v", &_cluster_pur_U_v);
    fShowerTree->Branch("cluster_pur_V_v", &_cluster_pur_V_v);
    fShowerTree->Branch("cluster_pur_Y_v", &_cluster_pur_Y_v);
    
    //
    // mc
    //
    fShowerTree->Branch("mcinfoInteractionType", &_mcinfoInteractionType, "mcinfoInteractionType/I");
    fShowerTree->Branch("mcinfoMode", &_mcinfoMode, "mcinfoMode/I");
    fShowerTree->Branch("mc_pdg_code", &_mc_pdg_code, "mc_pdg_code/I");
    fShowerTree->Branch("mc_x", &_mc_x, "mc_x/D");
    fShowerTree->Branch("mc_y", &_mc_y, "mc_y/D");
    fShowerTree->Branch("mc_z", &_mc_z, "mc_z/D");
    fShowerTree->Branch("mc_dcosx", &_mc_dcosx, "mc_dcosx/D");
    fShowerTree->Branch("mc_dcosy", &_mc_dcosy, "mc_dcosy/D");
    fShowerTree->Branch("mc_dcosz", &_mc_dcosz, "mc_dcosz/D");
    fShowerTree->Branch("mc_energy", &_mc_energy, "mc_energy/D");

    fShowerTree->Branch("mc_dedx", &_mc_dedx, "mc_dedx/D");
    fShowerTree->Branch("mc_dqdx_U", &_mc_dqdx_U, "mc_dqdx_U/D");
    fShowerTree->Branch("mc_dqdx_V", &_mc_dqdx_V, "mc_dqdx_V/D");
    fShowerTree->Branch("mc_dqdx_Y", &_mc_dqdx_Y, "mc_dqdx_Y/D");
    fShowerTree->Branch("mc_charge_U", &_mc_charge_U, "mc_charge_U/D");
    fShowerTree->Branch("mc_charge_V", &_mc_charge_V, "mc_charge_V/D");
    fShowerTree->Branch("mc_charge_Y", &_mc_charge_Y, "mc_charge_Y/D");

    fShowerTree->Branch("mcs_E", &_mcs_E, "mcs_E/D");
    fShowerTree->Branch("mc_containment", &_mc_containment, "mc_containment/D");
    fShowerTree->Branch("mc_length", &_mc_length, "mc_length/D");
    fShowerTree->Branch("mc_wildlength", &_mc_wildlength, "mc_wildlength/D");

    return true;
  }

  bool ShowerQuality_DL::analyze(storage_manager* storage) {
    std::cout << std::endl;
    std::cout << "@entry=" << storage->get_index() << std::endl;

    ClearEvent();
    ClearVertex();

    // Retrieve reco vertex data product
    auto ev_vertex = (event_vertex*) storage->get_data<event_vertex>(fVertexProducer);
    
    _alg.InitializeForEvent(storage, fShowerProducer);
    
    // Retrieve mcshower data product
    auto ev_mcs   = (event_mcshower*)storage->get_data<event_mcshower>("mcreco");
    auto ev_truth = (event_mctruth*) storage->get_data<event_mctruth>("generator");

    _run    = (int)storage->run_id();
    _subrun = (int)storage->subrun_id();
    _event  = (int)storage->event_id();
    _entry  = (int)storage->get_index();
    
    std::cout << "(r,s,e)=(" << _run << "," << _subrun << "," << _event << ")" << std::endl;
    larlite::mcshower nue_shower;
    nue_shower.PdgCode(data::kINVALID_INT);
    
    _has_mc = false;
    if (ev_truth and !ev_truth->empty()) {
      
      // Read out some nu stuff
      const auto& truth = ev_truth->front();
      const auto& nu = truth.GetNeutrino();
      _mcinfoInteractionType =  (int) nu.InteractionType();
      _mcinfoMode            =  (int) nu.Mode();

      for(const auto& mcs : *ev_mcs) {
	if (mcs.PdgCode() == 11) {
	  if (mcs.PdgCode() == mcs.MotherPdgCode() 
	      && mcs.PdgCode() == mcs.AncestorPdgCode()) {
	    nue_shower = mcs;
	  }
	}
      }

      FillSegment(storage);
      FillAndy(storage);
	
      if (nue_shower.PdgCode() != data::kINVALID_INT)
	_has_mc = true;

      if (_has_mc) {
	_mcs_E = nue_shower.DetProfile().E();
	_mc_containment = nue_shower.DetProfile().E() / nue_shower.Start().E();

	_mc_x = nue_shower.DetProfile().X();
	_mc_y = nue_shower.DetProfile().Y();
	_mc_z = nue_shower.DetProfile().Z();

	_mc_energy = nue_shower.DetProfile().E();
	_mc_containment = nue_shower.DetProfile().E() / nue_shower.Start().E();

	_mc_dcosx = nue_shower.Start().Px() / nue_shower.Start().E();
	_mc_dcosy = nue_shower.Start().Py() / nue_shower.Start().E();
	_mc_dcosz = nue_shower.Start().Pz() / nue_shower.Start().E();

	EMShowerProfile mcshower_helper;
	_mc_length = mcshower_helper.Length( nue_shower.DetProfile().E() );

	TVector3 WildShowerDir( nue_shower.End().X() - nue_shower.Start().X(), 
				nue_shower.End().Y() - nue_shower.Start().Y(),
				nue_shower.End().Z() - nue_shower.Start().Z() );

	_mc_wildlength = WildShowerDir.Mag();

	_mc_dedx     = nue_shower.dEdx();
	_mc_dqdx_U   = nue_shower.dQdx(0);
	_mc_dqdx_V   = nue_shower.dQdx(1);
	_mc_dqdx_Y   = nue_shower.dQdx(2);
    
	_mc_charge_U = nue_shower.Charge(0);
	_mc_charge_V = nue_shower.Charge(1);
	_mc_charge_Y = nue_shower.Charge(2);
      
	_mc_pdg_code = nue_shower.PdgCode();
      }
    } // end truth
    

    std::cout << "ev_vertex sz=" << ev_vertex->size() << std::endl;
    
    std::cout << "associate vertex  with ass name=" << ev_vertex->name() << std::endl;
    // get the associated pf particles
    larlite::event_pfpart *ev_pfpart = nullptr;
    auto const& ass_pfpart_vv = storage->find_one_ass(ev_vertex->id(), ev_pfpart, ev_vertex->name());
    if (!ev_pfpart or ev_pfpart->empty()) {
      fEventTree->Fill();
      fAndyTree->Fill();
      std::cout << "no pf particles... next!" << std::endl;
      return true;
    }
    std::cout << "ev_pfpart sz=" << ev_pfpart->size() << std::endl;
    std::cout << "ass_pfpart_vv sz=" << ass_pfpart_vv.size() << std::endl;

    std::cout << "associate pfpart with ass name=" << fShowerProducer << std::endl;

    // get the associated pf clusters
    larlite::event_shower *ev_shower = nullptr;
    auto const& ass_shower_vv = storage->find_one_ass(ev_pfpart->id(), ev_shower, fShowerProducer);
    
    if(ev_shower)    
      std::cout << "ev_shower sz=" << ev_shower->size() << std::endl;
    
    std::cout << "ass_shower_vv sz=" << ass_shower_vv.size() << std::endl;

    for( size_t vtx_id = 0; vtx_id < ass_pfpart_vv.size(); ++vtx_id) {
      ClearVertex();
      const auto& vertex = ev_vertex->at(vtx_id);
      const auto& ass_pfpart_v = ass_pfpart_vv.at(vtx_id);
    
      std::cout << "@vtx_id=" << vtx_id << " : (" << vertex.X() << "," << vertex.Y() << "," << vertex.Z() << ")" << std::endl;
      std::cout << "..." << ass_pfpart_v.size() << " associated pfparts" << std::endl;
    
      _vtx_id = vtx_id;
      
      std::vector<size_t> shower_idx_v;
      std::vector<const larlite::shower* > shower_v;

      if (!ev_shower) {
        _nshowers = 0;
        fShowerTree->Fill();
        continue;
      }
      size_t ev_shr_size = ev_shower->size();
      
      shower_idx_v.clear();
      shower_idx_v.reserve(ev_shr_size);

      shower_v.clear();
      shower_v.reserve(ev_shr_size);

      for( size_t pfp_id=0; pfp_id < ass_pfpart_v.size(); ++ pfp_id) {
	const auto pfpart_id = ass_pfpart_v.at(pfp_id);
	std::cout << "@pfp_id=" << pfp_id << " pfpart_id=" << pfpart_id << std::endl;

	if (pfpart_id >= ass_shower_vv.size()) {
	  std::cout << std::endl;
	  std::cout << "---------------------------" << std::endl;
	  std::cout << "Edge case detected!" << std::endl;
	  std::cout << "Vertex found, pfparticle exists, but no shower found?" << std::endl;
	  std::cout << "Skip for now..." << std::endl;
	  std::cout << "---------------------------" << std::endl << std::endl;
	  continue;
	}

	const auto& ass_shower_v = ass_shower_vv.at(pfpart_id);
	std::cout << "ass_shower_v sz=" << ass_shower_v.size() << std::endl;

	for(size_t shr_id=0; shr_id < ass_shower_v.size(); ++ shr_id) {
	  const auto shower_id = ass_shower_v.at(shr_id);
	  std::cout << "@shr_id=" << shr_id << " shower_id=" << shower_id << std::endl;

	  shower_idx_v.push_back(shower_id);
	  shower_v.push_back(&(ev_shower->at(shower_id)));
	} // end ass shower
	std::cout << std::endl;
      } // end ass pfparticle
      std::cout << std::endl;
      std::cout << "... _nshowers=" << shower_v.size() << std::endl;

      _nshowers = (int)shower_v.size();
      ResizeVectors(_nshowers);
    
      for(size_t shr_id=0; shr_id < shower_v.size(); ++shr_id) {
	const auto& reco_shower = *(shower_v[shr_id]);
	const auto shower_id = shower_idx_v[shr_id];

	auto& reco_x = _reco_x_v[shr_id];
	auto& reco_y = _reco_y_v[shr_id];
	auto& reco_z = _reco_z_v[shr_id];
	auto& reco_dcosx = _reco_dcosx_v[shr_id];
	auto& reco_dcosy = _reco_dcosy_v[shr_id];
	auto& reco_dcosz = _reco_dcosz_v[shr_id];
	auto& reco_phi = _reco_phi_v[shr_id];
	auto& reco_theta = _reco_theta_v[shr_id];
	auto& reco_energy_U = _reco_energy_U_v[shr_id];
	auto& reco_energy_V = _reco_energy_V_v[shr_id];
	auto& reco_energy_Y = _reco_energy_Y_v[shr_id];
	auto& reco_dedx = _reco_dedx_v[shr_id];
	auto& reco_dedx_U = _reco_dedx_U_v[shr_id];
	auto& reco_dedx_V = _reco_dedx_V_v[shr_id];
	auto& reco_dedx_Y = _reco_dedx_Y_v[shr_id];
	auto& reco_dqdx = _reco_dqdx_v[shr_id];
	auto& reco_dqdx_U = _reco_dqdx_U_v[shr_id];
	auto& reco_dqdx_V = _reco_dqdx_V_v[shr_id];
	auto& reco_dqdx_Y = _reco_dqdx_Y_v[shr_id];
	auto& reco_length = _reco_length_v[shr_id];
	auto& reco_width1 = _reco_width1_v[shr_id];
	auto& reco_width2 = _reco_width2_v[shr_id];
	
	reco_x = reco_shower.ShowerStart()[0];
	reco_y = reco_shower.ShowerStart()[1];
	reco_z = reco_shower.ShowerStart()[2];
	
	reco_dcosx = reco_shower.Direction()[0];
	reco_dcosy = reco_shower.Direction()[1];
	reco_dcosz = reco_shower.Direction()[2];

	reco_energy_U = reco_shower.Energy_v().at(0);
	reco_energy_V = reco_shower.Energy_v().at(1);
	reco_energy_Y = reco_shower.Energy_v().at(2);

	reco_dedx     = reco_shower.dEdx();
	reco_dedx_U   = reco_shower.dEdx_v().at(0);
	reco_dedx_V   = reco_shower.dEdx_v().at(1);
	reco_dedx_Y   = reco_shower.dEdx_v().at(2);

	reco_dqdx   = reco_shower.dQdx();
	reco_dqdx_U = reco_shower.dQdx_v().at(0);
	reco_dqdx_V = reco_shower.dQdx_v().at(1);                                                           
	reco_dqdx_Y = reco_shower.dQdx_v().at(2);

	reco_length = reco_shower.Length();
	reco_width1 = reco_shower.Width()[0];
	reco_width2 = reco_shower.Width()[1];

	
	if(_has_mc) {
	  auto& mc_reco_anglediff = _mc_reco_anglediff_v[shr_id];
	  auto& mc_reco_dist = _mc_reco_dist_v[shr_id];
	  auto& cluster_eff_U = _cluster_eff_U_v[shr_id];
	  auto& cluster_eff_V = _cluster_eff_V_v[shr_id];
	  auto& cluster_eff_Y = _cluster_eff_Y_v[shr_id];
	  auto& cluster_pur_U = _cluster_pur_U_v[shr_id];
	  auto& cluster_pur_V = _cluster_pur_V_v[shr_id];
	  auto& cluster_pur_Y = _cluster_pur_Y_v[shr_id];
	
	  // Reco - MC angle diff
	  mc_reco_anglediff = acos( reco_dcosx * _mc_dcosx +
				    reco_dcosy * _mc_dcosy +
				    reco_dcosz * _mc_dcosz ) / 3.14159265359 * 180.;
	
	  // Reco - MC vtx distance
	  mc_reco_dist = sqrt( pow(reco_x - _mc_x, 2) +
			       pow(reco_y - _mc_y, 2) +
			       pow(reco_z - _mc_z, 2) );
	
	  auto eff_purs = _alg.ClusterEP(shower_id);
	  cluster_eff_U = eff_purs.at(0).first;
	  cluster_eff_V = eff_purs.at(1).first;
	  cluster_eff_Y = eff_purs.at(2).first;
	  cluster_pur_U = eff_purs.at(0).second;
	  cluster_pur_V = eff_purs.at(1).second;
	  cluster_pur_Y = eff_purs.at(2).second;
	}
      } // end this shower


      fShowerTree->Fill();
    } // end this vertex
    
    fEventTree->Fill();
    fAndyTree->Fill();
    std::cout << std::endl;
    return true;
  }
  
  bool ShowerQuality_DL::finalize() {

    std::cout << "...Finalize..." << std::endl;

    assert(fShowerTree);
    fShowerTree->Write();

    assert(fEventTree);
    fEventTree->Write();
    
    assert(fAndyTree);
    fAndyTree->Write();

    return true;
  }

  void ShowerQuality_DL::ClearEvent() {
    
    _event  = -1.0*data::kINVALID_INT;
    _subrun = -1.0*data::kINVALID_INT;
    _run    = -1.0*data::kINVALID_INT;
    _entry  = -1.0*data::kINVALID_INT;
    
    _mcinfoInteractionType = -1.0*data::kINVALID_INT;
    _mcinfoMode = -1.0*data::kINVALID_INT;
    _mc_pdg_code = -1.0*data::kINVALID_INT;
    
    _mc_x = -1.0*data::kINVALID_DOUBLE;
    _mc_y = -1.0*data::kINVALID_DOUBLE;
    _mc_z = -1.0*data::kINVALID_DOUBLE;
    _mc_dcosx = -1.0*data::kINVALID_DOUBLE;
    _mc_dcosy = -1.0*data::kINVALID_DOUBLE;
    _mc_dcosz = -1.0*data::kINVALID_DOUBLE;
    _mc_energy = -1.0*data::kINVALID_DOUBLE;

    _mc_dedx = -1.0*data::kINVALID_DOUBLE;
    _mc_dqdx_U = -1.0*data::kINVALID_DOUBLE;
    _mc_dqdx_V = -1.0*data::kINVALID_DOUBLE;
    _mc_dqdx_Y = -1.0*data::kINVALID_DOUBLE;
    _mc_charge_U = -1.0*data::kINVALID_DOUBLE;
    _mc_charge_V = -1.0*data::kINVALID_DOUBLE;
    _mc_charge_Y = -1.0*data::kINVALID_DOUBLE;

    _mcs_E = -1.0 * data::kINVALID_DOUBLE;
    _mc_containment = -1.0*data::kINVALID_DOUBLE;
    _mc_length = -1.0*data::kINVALID_DOUBLE;
    _mc_wildlength = -1.0*data::kINVALID_DOUBLE;

    
    //
    // segment
    //
    
    _selected = -1.0*data::kINVALID_INT;
    _parent_pdg = -1.0*data::kINVALID_INT;

    _energy_init = -1.0*data::kINVALID_DOUBLE;

    _parent_x = -1.0*data::kINVALID_DOUBLE;
    _parent_y = -1.0*data::kINVALID_DOUBLE;
    _parent_z = -1.0*data::kINVALID_DOUBLE;

    _parent_sce_x = -1.0*data::kINVALID_DOUBLE;
    _parent_sce_y = -1.0*data::kINVALID_DOUBLE;
    _parent_sce_z = -1.0*data::kINVALID_DOUBLE;

    _parent_px = -1.0*data::kINVALID_DOUBLE;
    _parent_py = -1.0*data::kINVALID_DOUBLE;
    _parent_pz = -1.0*data::kINVALID_DOUBLE;

    _interaction_mode = -1.0*data::kINVALID_INT;
    _interaction_type = -1.0*data::kINVALID_INT;

    _dep_sum_lepton = -1.0*data::kINVALID_DOUBLE;
    _dep_sum_proton = -1.0*data::kINVALID_DOUBLE;
      
    _daughterPx_v.clear();
    _daughterPy_v.clear();
    _daughterPz_v.clear();

    _daughterX_v.clear();
    _daughterY_v.clear();
    _daughterZ_v.clear();

    _proton_Px = -1.0*data::kINVALID_DOUBLE;
    _proton_Py = -1.0*data::kINVALID_DOUBLE;
    _proton_Pz = -1.0*data::kINVALID_DOUBLE;
    
    _lepton_Px = -1.0*data::kINVALID_DOUBLE;
    _lepton_Py = -1.0*data::kINVALID_DOUBLE;
    _lepton_Pz = -1.0*data::kINVALID_DOUBLE;

    _daughter_energydep_v.clear();

    _daughter_pdg_v.clear();

    return;
  }

  void ShowerQuality_DL::ClearVertex() {
    _nshowers = -1.0*data::kINVALID_INT;
    _vtx_id = -1.0*data::kINVALID_INT;

    _reco_x_v.clear();
    _reco_y_v.clear();
    _reco_z_v.clear();
    _reco_dcosx_v.clear();
    _reco_dcosy_v.clear();
    _reco_dcosz_v.clear();
    _reco_phi_v.clear();
    _reco_theta_v.clear();
    _reco_energy_U_v.clear();
    _reco_energy_V_v.clear();
    _reco_energy_Y_v.clear();
    _reco_dedx_v.clear();
    _reco_dedx_U_v.clear();
    _reco_dedx_V_v.clear();
    _reco_dedx_Y_v.clear();
    _reco_dqdx_v.clear();
    _reco_dqdx_U_v.clear();
    _reco_dqdx_V_v.clear();
    _reco_dqdx_Y_v.clear();
    _reco_length_v.clear();
    _reco_width1_v.clear();
    _reco_width2_v.clear();

    _mc_reco_anglediff_v.clear();
    _mc_reco_dist_v.clear();
    _cluster_eff_U_v.clear();
    _cluster_eff_V_v.clear();
    _cluster_eff_Y_v.clear();
    _cluster_pur_U_v.clear();
    _cluster_pur_V_v.clear();
    _cluster_pur_Y_v.clear();

    return;
  }															     

  void ShowerQuality_DL::ResizeVectors(int nshowers){ 

    _reco_x_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);
    _reco_y_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);
    _reco_z_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);
    _reco_dcosx_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);
    _reco_dcosy_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);
    _reco_dcosz_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);
    _reco_phi_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);
    _reco_theta_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);
    _reco_energy_U_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);
    _reco_energy_V_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);
    _reco_energy_Y_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);
    _reco_dedx_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);
    _reco_dedx_U_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);
    _reco_dedx_V_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);
    _reco_dedx_Y_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);
    _reco_dqdx_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);
    _reco_dqdx_U_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);
    _reco_dqdx_V_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);
    _reco_dqdx_Y_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);
    _reco_length_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);
    _reco_width1_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);
    _reco_width2_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);

    _mc_reco_anglediff_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);
    _mc_reco_dist_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);
    _cluster_eff_U_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);
    _cluster_eff_V_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);
    _cluster_eff_Y_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);
    _cluster_pur_U_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);
    _cluster_pur_V_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);
    _cluster_pur_Y_v.resize(nshowers,-1.0*data::kINVALID_DOUBLE);

    return;
  }

  void ShowerQuality_DL::AttachAndy(TTree* tree) {

    assert(tree);
    tree->Branch("MCFlux_NuPosX",   &MCFlux_NuPosX    , "MCFlux_NuPosX/D");
    tree->Branch("MCFlux_NuPosY",   &MCFlux_NuPosY    , "MCFlux_NuPosY/D");
    tree->Branch("MCFlux_NuPosZ",   &MCFlux_NuPosZ    , "MCFlux_NuPosZ/D");
    tree->Branch("MCFlux_NuMomX",   &MCFlux_NuMomX    , "MCFlux_NuMomX/D");
    tree->Branch("MCFlux_NuMomY",   &MCFlux_NuMomY    , "MCFlux_NuMomY/D");
    tree->Branch("MCFlux_NuMomZ",   &MCFlux_NuMomZ    , "MCFlux_NuMomZ/D");
    tree->Branch("MCFlux_NuMomE",   &MCFlux_NuMomE    , "MCFlux_NuMomE/D");
    tree->Branch("MCFlux_ntype",    &MCFlux_ntype     , "MCFlux_ntype/I");
    tree->Branch("MCFlux_ptype",    &MCFlux_ptype     , "MCFlux_ptype/I");
    tree->Branch("MCFlux_nimpwt",   &MCFlux_nimpwt    , "MCFlux_nimpwt/D");
    tree->Branch("MCFlux_dk2gen",   &MCFlux_dk2gen    , "MCFlux_dk2gen/D");
    tree->Branch("MCFlux_nenergyn", &MCFlux_nenergyn  , "MCFlux_nenergyn/D");
    tree->Branch("MCFlux_tpx",      &MCFlux_tpx       , "MCFlux_tpx/D");
    tree->Branch("MCFlux_tpy",      &MCFlux_tpy       , "MCFlux_tpy/D");
    tree->Branch("MCFlux_tpz",      &MCFlux_tpz       , "MCFlux_tpz/D");
    tree->Branch("MCFlux_tptype",   &MCFlux_tptype    , "MCFlux_tptype/I");
    tree->Branch("MCFlux_vx",       &MCFlux_vx        , "MCFlux_vx/D");
    tree->Branch("MCFlux_vy",       &MCFlux_vy        , "MCFlux_vy/D");
    tree->Branch("MCFlux_vz",       &MCFlux_vz        , "MCFlux_vz/D");
    
    tree->Branch("MCTruth_NParticles",           &MCTruth_NParticles,"MCTruth_NParticles/I");
    tree->Branch("MCTruth_particles_TrackId",    &MCTruth_particles_TrackId);
    tree->Branch("MCTruth_particles_PdgCode",    &MCTruth_particles_PdgCode);
    tree->Branch("MCTruth_particles_Mother",     &MCTruth_particles_Mother);
    tree->Branch("MCTruth_particles_StatusCode", &MCTruth_particles_StatusCode);
    tree->Branch("MCTruth_particles_NDaughters", &MCTruth_particles_NDaughters);
    tree->Branch("MCTruth_particles_Daughters",  &MCTruth_particles_Daughters);
    tree->Branch("MCTruth_particles_Gvx",        &MCTruth_particles_Gvx);
    tree->Branch("MCTruth_particles_Gvy",        &MCTruth_particles_Gvy);
    tree->Branch("MCTruth_particles_Gvz",        &MCTruth_particles_Gvz);
    tree->Branch("MCTruth_particles_Gvt",        &MCTruth_particles_Gvt);
    tree->Branch("MCTruth_particles_px0",        &MCTruth_particles_px0);
    tree->Branch("MCTruth_particles_py0",        &MCTruth_particles_py0);
    tree->Branch("MCTruth_particles_pz0",        &MCTruth_particles_pz0);
    tree->Branch("MCTruth_particles_e0",         &MCTruth_particles_e0);
    tree->Branch("MCTruth_particles_Rescatter",  &MCTruth_particles_Rescatter);
    tree->Branch("MCTruth_particles_polx",       &MCTruth_particles_polx);
    tree->Branch("MCTruth_particles_poly",       &MCTruth_particles_poly);
    tree->Branch("MCTruth_particles_polz",       &MCTruth_particles_polz);

    tree->Branch("MCTruth_neutrino_CCNC",            &MCTruth_neutrino_CCNC, "MCTruth_neutrino_CCNC/I");
    tree->Branch("MCTruth_neutrino_mode",            &MCTruth_neutrino_mode, "MCTruth_neutrino_mode/I");
    tree->Branch("MCTruth_neutrino_interactionType", &MCTruth_neutrino_interactionType, "MCTruth_neutrino_interactionType/I");
    tree->Branch("MCTruth_neutrino_target",          &MCTruth_neutrino_target, "MCTruth_neutrino_target/I");         
    tree->Branch("MCTruth_neutrino_nucleon",         &MCTruth_neutrino_nucleon, "MCTruth_neutrino_nucleon/I");        
    tree->Branch("MCTruth_neutrino_quark",           &MCTruth_neutrino_quark, "MCTruth_neutrino_quark/I");          
    tree->Branch("MCTruth_neutrino_W",               &MCTruth_neutrino_W, "MCTruth_neutrino_W/D");              
    tree->Branch("MCTruth_neutrino_X",               &MCTruth_neutrino_X, "MCTruth_neutrino_X/D");              
    tree->Branch("MCTruth_neutrino_Y",               &MCTruth_neutrino_Y, "MCTruth_neutrino_Y/D");              
    tree->Branch("MCTruth_neutrino_Q2",              &MCTruth_neutrino_Q2, "MCTruth_neutrino_Q2/D");             

    tree->Branch("GTruth_ProbePDG"    , &GTruth_ProbePDG, "GTruth_ProbePDG/I");                  
    tree->Branch("GTruth_IsSeaQuark"  , &GTruth_IsSeaQuark, "GTruth_IsSeaQuark/I");                
    tree->Branch("GTruth_tgtPDG"      , &GTruth_tgtPDG, "GTruth_tgtPDG/I");                    
    tree->Branch("GTruth_weight"      , &GTruth_weight, "GTruth_weight/D");                    
    tree->Branch("GTruth_probability" , &GTruth_probability, "GTruth_probability/D");               
    tree->Branch("GTruth_Xsec"        , &GTruth_Xsec, "GTruth_Xsec/D");                      
    tree->Branch("GTruth_fDiffXsec"   , &GTruth_fDiffXsec, "GTruth_fDiffXsec/D");                 
    tree->Branch("GTruth_vertexX"     , &GTruth_vertexX, "GTruth_vertexX/D");                   
    tree->Branch("GTruth_vertexY"     , &GTruth_vertexY, "GTruth_vertexY/D");                   
    tree->Branch("GTruth_vertexZ"     , &GTruth_vertexZ, "GTruth_vertexZ/D");                   
    tree->Branch("GTruth_vertexT"     , &GTruth_vertexT, "GTruth_vertexT/D");                   
    tree->Branch("GTruth_Gscatter"    , &GTruth_Gscatter, "GTruth_Gscatter/I");                  
    tree->Branch("GTruth_Gint"        , &GTruth_Gint, "GTruth_Gint/I");                      
    tree->Branch("GTruth_ResNum"      , &GTruth_ResNum, "GTruth_ResNum/I");                    
    tree->Branch("GTruth_NumPiPlus"   , &GTruth_NumPiPlus, "GTruth_NumPiPlus/I");                 
    tree->Branch("GTruth_NumPi0"      , &GTruth_NumPi0, "GTruth_NumPi0/I");                    
    tree->Branch("GTruth_NumPiMinus"  , &GTruth_NumPiMinus, "GTruth_NumPiMinus/I");                
    tree->Branch("GTruth_NumProton"   , &GTruth_NumProton, "GTruth_NumProton/I");                 
    tree->Branch("GTruth_NumNeutron"  , &GTruth_NumNeutron, "GTruth_NumNeutron/I");                
    tree->Branch("GTruth_IsCharm"     , &GTruth_IsCharm, "GTruth_IsCharm/I");                   
    tree->Branch("GTruth_gX" , &GTruth_gX  , "GTruth_gX/D");                        
    tree->Branch("GTruth_gY" , &GTruth_gY  , "GTruth_gY/D");                        
    tree->Branch("GTruth_gZ" , &GTruth_gZ  , "GTruth_gZ/D");                        
    tree->Branch("GTruth_gT" , &GTruth_gT  , "GTruth_gT/D");                        
    tree->Branch("GTruth_gW" , &GTruth_gW  , "GTruth_gW/D");                        
    tree->Branch("GTruth_gQ2", &GTruth_gQ2 , "GTruth_gQ2/D");                       
    tree->Branch("GTruth_gq2", &GTruth_gq2 , "GTruth_gq2/D");                       
    tree->Branch("GTruth_ProbePDG" , &GTruth_ProbePDG  , "GTruth_ProbePDG/I");                  
    tree->Branch("GTruth_ProbeP4x" , &GTruth_ProbeP4x  , "GTruth_ProbeP4x/D");                  
    tree->Branch("GTruth_ProbeP4y" , &GTruth_ProbeP4y  , "GTruth_ProbeP4y/D");                  
    tree->Branch("GTruth_ProbeP4z" , &GTruth_ProbeP4z  , "GTruth_ProbeP4z/D");                  
    tree->Branch("GTruth_ProbeP4E" , &GTruth_ProbeP4E  , "GTruth_ProbeP4E/D");                  
    tree->Branch("GTruth_HitNucP4x", &GTruth_HitNucP4x , "GTruth_HitNucP4x/D");                 
    tree->Branch("GTruth_HitNucP4y", &GTruth_HitNucP4y , "GTruth_HitNucP4y/D");                 
    tree->Branch("GTruth_HitNucP4z", &GTruth_HitNucP4z , "GTruth_HitNucP4z/D");                 
    tree->Branch("GTruth_HitNucP4E", &GTruth_HitNucP4E , "GTruth_HitNucP4E/D");                 
    tree->Branch("GTruth_FShadSystP4x", &GTruth_FShadSystP4x, "GTruth_FShadSystP4x/D");              
    tree->Branch("GTruth_FShadSystP4y", &GTruth_FShadSystP4y, "GTruth_FShadSystP4y/D");              
    tree->Branch("GTruth_FShadSystP4z", &GTruth_FShadSystP4z, "GTruth_FShadSystP4z/D");              
    tree->Branch("GTruth_FShadSystP4E", &GTruth_FShadSystP4E, "GTruth_FShadSystP4E/D");              
    
  }

  void ShowerQuality_DL::ResetAndy() {
    
    MCFlux_NuPosX = data::kINVALID_DOUBLE;
    MCFlux_NuPosY = data::kINVALID_DOUBLE;
    MCFlux_NuPosZ = data::kINVALID_DOUBLE;
    MCFlux_NuMomX = data::kINVALID_DOUBLE;
    MCFlux_NuMomY = data::kINVALID_DOUBLE;
    MCFlux_NuMomZ = data::kINVALID_DOUBLE;
    MCFlux_NuMomE = data::kINVALID_DOUBLE;
    MCFlux_ntype = data::kINVALID_INT;
    MCFlux_ptype = data::kINVALID_INT;
    MCFlux_nimpwt = data::kINVALID_DOUBLE;
    MCFlux_dk2gen = data::kINVALID_DOUBLE;
    MCFlux_nenergyn = data::kINVALID_DOUBLE;
    MCFlux_tpx = data::kINVALID_DOUBLE;
    MCFlux_tpy = data::kINVALID_DOUBLE;
    MCFlux_tpz = data::kINVALID_DOUBLE;
    MCFlux_tptype = data::kINVALID_INT;
    MCFlux_vx = data::kINVALID_DOUBLE;
    MCFlux_vy = data::kINVALID_DOUBLE;
    MCFlux_vz = data::kINVALID_DOUBLE;
    
    MCTruth_NParticles = data::kINVALID_INT;
    MCTruth_particles_TrackId.clear();
    MCTruth_particles_PdgCode.clear();
    MCTruth_particles_Mother.clear();
    MCTruth_particles_StatusCode.clear();
    MCTruth_particles_NDaughters.clear();
    MCTruth_particles_Daughters.clear();
    MCTruth_particles_Gvx.clear();
    MCTruth_particles_Gvy.clear();
    MCTruth_particles_Gvz.clear();
    MCTruth_particles_Gvt.clear();
    MCTruth_particles_px0.clear();
    MCTruth_particles_py0.clear();
    MCTruth_particles_pz0.clear();
    MCTruth_particles_e0.clear();
    MCTruth_particles_Rescatter.clear();
    MCTruth_particles_polx.clear();
    MCTruth_particles_poly.clear();
    MCTruth_particles_polz.clear();

    MCTruth_neutrino_CCNC = data::kINVALID_INT;
    MCTruth_neutrino_mode = data::kINVALID_INT;
    MCTruth_neutrino_interactionType = data::kINVALID_INT;
    MCTruth_neutrino_target = data::kINVALID_INT;
    MCTruth_neutrino_nucleon = data::kINVALID_INT;
    MCTruth_neutrino_quark = data::kINVALID_INT;
    MCTruth_neutrino_W = data::kINVALID_DOUBLE;
    MCTruth_neutrino_X = data::kINVALID_DOUBLE;
    MCTruth_neutrino_Y = data::kINVALID_DOUBLE;
    MCTruth_neutrino_Q2 = data::kINVALID_DOUBLE;

    GTruth_ProbePDG = data::kINVALID_INT;
    GTruth_IsSeaQuark = data::kINVALID_INT;
    GTruth_tgtPDG = data::kINVALID_INT;
    GTruth_weight = data::kINVALID_DOUBLE;
    GTruth_probability = data::kINVALID_DOUBLE;
    GTruth_Xsec = data::kINVALID_DOUBLE;
    GTruth_fDiffXsec = data::kINVALID_DOUBLE;
    GTruth_vertexX = data::kINVALID_DOUBLE;
    GTruth_vertexY = data::kINVALID_DOUBLE;
    GTruth_vertexZ = data::kINVALID_DOUBLE;
    GTruth_vertexT = data::kINVALID_DOUBLE;
    GTruth_Gscatter = data::kINVALID_INT;
    GTruth_Gint = data::kINVALID_INT;
    GTruth_ResNum = data::kINVALID_INT;
    GTruth_NumPiPlus = data::kINVALID_INT;
    GTruth_NumPi0 = data::kINVALID_INT;
    GTruth_NumPiMinus = data::kINVALID_INT;
    GTruth_NumProton = data::kINVALID_INT;
    GTruth_NumNeutron = data::kINVALID_INT;
    GTruth_IsCharm = data::kINVALID_INT;
    GTruth_gX = data::kINVALID_DOUBLE;
    GTruth_gY = data::kINVALID_DOUBLE;
    GTruth_gZ = data::kINVALID_DOUBLE;
    GTruth_gT = data::kINVALID_DOUBLE;
    GTruth_gW = data::kINVALID_DOUBLE;
    GTruth_gQ2 = data::kINVALID_DOUBLE;
    GTruth_gq2 = data::kINVALID_DOUBLE;
    GTruth_ProbePDG = data::kINVALID_INT;
    GTruth_ProbeP4x = data::kINVALID_DOUBLE;
    GTruth_ProbeP4y = data::kINVALID_DOUBLE;
    GTruth_ProbeP4z = data::kINVALID_DOUBLE;
    GTruth_ProbeP4E = data::kINVALID_DOUBLE;
    GTruth_HitNucP4x = data::kINVALID_DOUBLE;
    GTruth_HitNucP4y = data::kINVALID_DOUBLE;
    GTruth_HitNucP4z = data::kINVALID_DOUBLE;
    GTruth_HitNucP4E = data::kINVALID_DOUBLE;
    GTruth_FShadSystP4x = data::kINVALID_DOUBLE;
    GTruth_FShadSystP4y = data::kINVALID_DOUBLE;
    GTruth_FShadSystP4z = data::kINVALID_DOUBLE;
    GTruth_FShadSystP4E = data::kINVALID_DOUBLE;

    return;
  }


  void ShowerQuality_DL::FillSegment(storage_manager *sto) {
    
    // get mctruth info
    auto ev_mctruth = (event_mctruth*)sto->get_data<event_mctruth>("generator");

    assert(ev_mctruth);

    auto const& mcnu = ev_mctruth->front().GetNeutrino();
    
    _parent_pdg = (int) mcnu.Nu().PdgCode();

    _interaction_mode = (int) mcnu.Mode();
    _interaction_type = (int) mcnu.InteractionType();

    auto const& nu_start = mcnu.Nu().Trajectory().front();

    _energy_init = nu_start.E();

    _parent_x = nu_start.X();
    _parent_y = nu_start.Y();
    _parent_z = nu_start.Z();

    _parent_px = nu_start.Px();
    _parent_py = nu_start.Py();
    _parent_pz = nu_start.Pz();

    const auto offset = _sce.GetPosOffsets(_parent_x,_parent_y,_parent_z);    

    _parent_sce_x = _parent_x - offset[0] + 0.7;
    _parent_sce_y = _parent_y + offset[1];
    _parent_sce_z = _parent_z + offset[2];

    // get mctracks and mcshowers
    auto ev_mctrack = (event_mctrack*)sto->get_data<event_mctrack>("mcreco");
    auto ev_mcshower = (event_mcshower*)sto->get_data<event_mcshower>("mcreco");

    assert(ev_mctrack);
    assert(ev_mcshower);

    std::vector<aparticle> proton_v;
    std::vector<aparticle> muon_v;
    std::vector<aparticle> electron_v;
    std::vector<aparticle> other_v;

    proton_v.clear();
    muon_v.clear();
    electron_v.clear();
    other_v.clear();

    for(auto const& mct : *ev_mctrack) {

      aparticle particle;

      particle.pdg = mct.PdgCode();
      particle.trackid = mct.TrackID();
      particle.parenttrackid = mct.MotherTrackID();
      particle.ancestortrackid = mct.AncestorTrackID();
      particle.depeng = mct.Start().E() - mct.End().E();

      auto sx = mct.Start().X();
      auto sy = mct.Start().Y();
      auto sz = mct.Start().Z();

      auto px = mct.Start().X();
      auto py = mct.Start().Y();
      auto pz = mct.Start().Z();
      
      // don't store corsika tracks, only primaries
      if (sy < 1799 and particle.primary()) {
	_daughter_pdg_v.push_back(particle.pdg);
	_daughter_energydep_v.push_back(particle.depeng);
	_daughterX_v.push_back(sx);
	_daughterY_v.push_back(sy);
	_daughterZ_v.push_back(sz);
	_daughterPx_v.push_back(px);
	_daughterPy_v.push_back(py);
	_daughterPz_v.push_back(pz);
      }

      // must be ANY proton, but not from corsika
      if(mct.PdgCode() == 2212 and sy < 1799) {
	proton_v.emplace_back(std::move(particle));
	_proton_Px = px;
	_proton_Py = py;
	_proton_Pz = pz;
      }
      
      // must be a muon, primary, not from corsika
      else if(std::abs(mct.PdgCode()) == 13 and particle.primary() and sy < 1799) {
	muon_v.emplace_back(std::move(particle));
	_lepton_Px = px;
	_lepton_Py = py;
	_lepton_Pz = pz;
      } 

      // it's something else, maybe corsika
      else {
	other_v.emplace_back(std::move(particle));
      }

    }

    for(auto const& mcs : *ev_mcshower) {
      
      aparticle particle;

      particle.pdg = mcs.PdgCode();
      particle.trackid = mcs.TrackID();
      particle.parenttrackid = mcs.MotherTrackID();
      particle.ancestortrackid = mcs.AncestorTrackID();
      particle.depeng = mcs.DetProfile().E();

      auto sx = mcs.Start().X();
      auto sy = mcs.Start().Y();
      auto sz = mcs.Start().Z();

      auto px = mcs.Start().X();
      auto py = mcs.Start().Y();
      auto pz = mcs.Start().Z();

      // do not store corsika showers
      if (sy < 1799 and particle.primary()) {
	_daughter_pdg_v.push_back(particle.pdg);
	_daughter_energydep_v.push_back(particle.depeng);
	_daughterX_v.push_back(sx);
	_daughterY_v.push_back(sy);
	_daughterZ_v.push_back(sz);
	_daughterPx_v.push_back(px);
	_daughterPy_v.push_back(py);
	_daughterPz_v.push_back(pz);
      }      
      
      // store only electron, primary, not corsika
      if(std::abs(mcs.PdgCode()) == 11 and particle.primary() and sy < 1799) {
	electron_v.emplace_back(std::move(particle));
	_lepton_Px = px;
	_lepton_Py = py;
	_lepton_Pz = pz;
      } 

      // it's something else
      else {
	other_v.emplace_back(std::move(particle));
      }
      
    }
    
    int nprotons = 0;
    int nmuons = 0;
    int nelectrons = 0;

    std::vector<size_t> proton_id_v;
    std::vector<float> proton_e_v;

    for(size_t pid=0; pid<proton_v.size(); ++pid) {
      if (proton_v[pid].primary()) {
	proton_id_v.push_back(pid);
	proton_e_v.push_back(proton_v.at(pid).depeng);
      }
    }


    for(size_t ppid=0; ppid<proton_id_v.size(); ++ppid) {
      auto pid1 = proton_id_v[ppid];
      const auto& proton1 = proton_v[pid1];
      for(size_t pid2=0; pid2<proton_v.size(); ++pid2) {
	if (pid1 == pid2) continue;
	const auto& proton2 = proton_v[pid2];
	if (proton2.ancestorof(proton1)) {
	  proton_e_v[ppid] += proton2.depeng;
	}
      }

    }

    for(auto proton_e : proton_e_v) {
      if (proton_e > 60) {
	nprotons += 1;
      }
    }

    if (!proton_e_v.empty())
      _dep_sum_proton = *(std::max_element(std::begin(proton_e_v),
					   std::end(proton_e_v)));

    float muon_e = 0;
    float electron_e = 0;

    if (!muon_v.empty()) {
      const auto& muon1 = muon_v.front();
      muon_e = muon1.depeng;
      for(const auto& other : other_v) {
	if (other.ancestorof(muon1)) {
	  muon_e += other.depeng;
	}
      }
    }

    if (!electron_v.empty()) {
      const auto& electron1 = electron_v.front();
      electron_e = electron1.depeng;
      for(const auto& other : other_v) {
	if (other.ancestorof(electron1)) {
	  electron_e += other.depeng;
	}
      }
    }

    if (muon_e > 35) nmuons = 1;
    if (electron_e > 35) nelectrons = 1;

    _selected = 0;

    bool is_1mu1p = false;
    bool is_1e1p = false;

    if (nprotons == 1 and nmuons == 1) {
      is_1mu1p = true;
      _dep_sum_lepton = muon_e;
    }

    if (nprotons == 1 and nelectrons == 1) {
      is_1e1p = true;
      _dep_sum_lepton = electron_e;
    }

    if ((is_1mu1p || is_1e1p) and (is_1mu1p != is_1e1p)) {
      _selected = 1;
    }

    return;
  }

  void ShowerQuality_DL::FillAndy(storage_manager *sto) {

    auto ev_flux = (event_mcflux*)sto->get_data<event_mcflux>("generator");
    assert(ev_flux && !ev_flux->empty());

    auto ev_truth = (event_mctruth*)sto->get_data<event_mctruth>("generator");
    assert(ev_truth && !ev_truth->empty());

    auto ev_gtruth = (event_gtruth*)sto->get_data<event_gtruth>("generator");
    assert(ev_gtruth && !ev_gtruth->empty());

    const auto& flux   = ev_flux->front();
    const auto& truth  = ev_truth->front();
    const auto& gtruth = ev_gtruth->front();

    MCFlux_NuPosX = truth.GetNeutrino().Nu().Trajectory().front().X();
    MCFlux_NuPosY = truth.GetNeutrino().Nu().Trajectory().front().Y();
    MCFlux_NuPosZ = truth.GetNeutrino().Nu().Trajectory().front().Z();
    MCFlux_NuMomX = truth.GetNeutrino().Nu().Trajectory().front().Px();
    MCFlux_NuMomY = truth.GetNeutrino().Nu().Trajectory().front().Py();
    MCFlux_NuMomZ = truth.GetNeutrino().Nu().Trajectory().front().Pz();
    MCFlux_NuMomE = truth.GetNeutrino().Nu().Trajectory().front().E();
    MCFlux_ntype    = flux.fntype;
    MCFlux_ptype    = flux.fptype;
    MCFlux_nimpwt   = flux.fnimpwt;
    MCFlux_dk2gen   = flux.fdk2gen;
    MCFlux_nenergyn = flux.fnenergyn;
    MCFlux_tpx      = flux.ftpx;
    MCFlux_tpy      = flux.ftpy;
    MCFlux_tpz      = flux.ftpz;
    MCFlux_tptype   = flux.ftptype;
    MCFlux_vx       = flux.fvx;
    MCFlux_vy       = flux.fvy;
    MCFlux_vz       = flux.fvz;
    
    MCTruth_NParticles = (int) truth.GetParticles().size();

    MCTruth_particles_TrackId.resize(MCTruth_NParticles);
    MCTruth_particles_PdgCode.resize(MCTruth_NParticles);
    MCTruth_particles_Mother.resize(MCTruth_NParticles);
    MCTruth_particles_StatusCode.resize(MCTruth_NParticles);
    MCTruth_particles_NDaughters.resize(MCTruth_NParticles);
    MCTruth_particles_Daughters.resize(MCTruth_NParticles);
    MCTruth_particles_Gvx.resize(MCTruth_NParticles);
    MCTruth_particles_Gvy.resize(MCTruth_NParticles);
    MCTruth_particles_Gvz.resize(MCTruth_NParticles);
    MCTruth_particles_Gvt.resize(MCTruth_NParticles);
    MCTruth_particles_px0.resize(MCTruth_NParticles);
    MCTruth_particles_py0.resize(MCTruth_NParticles);
    MCTruth_particles_pz0.resize(MCTruth_NParticles);
    MCTruth_particles_e0.resize(MCTruth_NParticles);
    MCTruth_particles_Rescatter.resize(MCTruth_NParticles);
    MCTruth_particles_polx.resize(MCTruth_NParticles);
    MCTruth_particles_poly.resize(MCTruth_NParticles);
    MCTruth_particles_polz.resize(MCTruth_NParticles);

    for(int pid=0; pid < MCTruth_NParticles; ++pid) {
      
      MCTruth_particles_TrackId[pid] = truth.GetParticles()[pid].TrackId();
      MCTruth_particles_PdgCode[pid] = truth.GetParticles()[pid].PdgCode();
      MCTruth_particles_Mother[pid] = truth.GetParticles()[pid].Mother();
      MCTruth_particles_StatusCode[pid] = truth.GetParticles()[pid].StatusCode();
      MCTruth_particles_NDaughters[pid] = (int)truth.GetParticles()[pid].Daughters().size();

      MCTruth_particles_Daughters[pid].reserve(MCTruth_particles_NDaughters[pid]);

      for(auto did : truth.GetParticles()[pid].Daughters()) {
	MCTruth_particles_Daughters[pid].push_back(did);
      }
      
      MCTruth_particles_Gvx[pid] = truth.GetParticles()[pid].GetGvtx().X();
      MCTruth_particles_Gvy[pid] = truth.GetParticles()[pid].GetGvtx().Y();
      MCTruth_particles_Gvz[pid] = truth.GetParticles()[pid].GetGvtx().Z();
      MCTruth_particles_Gvt[pid] = truth.GetParticles()[pid].GetGvtx().T();
      MCTruth_particles_px0[pid] = truth.GetParticles()[pid].Trajectory().front().Px();
      MCTruth_particles_py0[pid] = truth.GetParticles()[pid].Trajectory().front().Py();
      MCTruth_particles_pz0[pid] = truth.GetParticles()[pid].Trajectory().front().Pz();
      MCTruth_particles_e0[pid]  = truth.GetParticles()[pid].Trajectory().front().E();
      MCTruth_particles_Rescatter[pid] = truth.GetParticles()[pid].Rescatter();
      MCTruth_particles_polx[pid] = truth.GetParticles()[pid].Polarization().X();
      MCTruth_particles_poly[pid] = truth.GetParticles()[pid].Polarization().Y();
      MCTruth_particles_polz[pid] = truth.GetParticles()[pid].Polarization().Z();
      
    }

    MCTruth_neutrino_CCNC = truth.GetNeutrino().CCNC();
    MCTruth_neutrino_mode = truth.GetNeutrino().Mode();
    MCTruth_neutrino_interactionType = truth.GetNeutrino().InteractionType();
    MCTruth_neutrino_target  = truth.GetNeutrino().Target();
    MCTruth_neutrino_nucleon = truth.GetNeutrino().HitNuc();
    MCTruth_neutrino_quark   = truth.GetNeutrino().HitQuark();
    MCTruth_neutrino_W  = truth.GetNeutrino().W();
    MCTruth_neutrino_X  = truth.GetNeutrino().X();
    MCTruth_neutrino_Y  = truth.GetNeutrino().Y();
    MCTruth_neutrino_Q2 = truth.GetNeutrino().QSqr();

    GTruth_ProbePDG = gtruth.fProbePDG;
    GTruth_IsSeaQuark = (int)gtruth.fIsSeaQuark;
    GTruth_tgtPDG = gtruth.ftgtPDG;
    GTruth_weight = gtruth.fweight;
    GTruth_probability = gtruth.fprobability;
    GTruth_Xsec = gtruth.fXsec;
    
    GTruth_fDiffXsec = gtruth.fDiffXsec;

    GTruth_vertexX = gtruth.fVertex.X();
    GTruth_vertexY = gtruth.fVertex.Y();
    GTruth_vertexZ = gtruth.fVertex.Z();
    GTruth_vertexT = gtruth.fVertex.T();
    GTruth_Gscatter = gtruth.fGscatter;
    GTruth_Gint = gtruth.fGint;
    GTruth_ResNum = gtruth.fResNum;
    GTruth_NumPiPlus = gtruth.fNumPiPlus;
    GTruth_NumPi0 = gtruth.fNumPi0;
    GTruth_NumPiMinus = gtruth.fNumPiMinus;
    GTruth_NumProton = gtruth.fNumProton;
    GTruth_NumNeutron = gtruth.fNumNeutron;
    GTruth_IsCharm = (int)gtruth.fIsCharm;
    GTruth_gX = gtruth.fgX;
    GTruth_gY = gtruth.fgY;
    //GTruth_gZ = gtruth.fgZ;  -- it's missing
    GTruth_gT = gtruth.fgT;
    GTruth_gW = gtruth.fgW;
    GTruth_gQ2 = gtruth.fgQ2;
    GTruth_gq2 = gtruth.fgq2;
    GTruth_ProbeP4x = gtruth.fProbeP4.X();
    GTruth_ProbeP4y = gtruth.fProbeP4.Y();
    GTruth_ProbeP4z = gtruth.fProbeP4.Z();
    GTruth_ProbeP4E = gtruth.fProbeP4.E();
    GTruth_HitNucP4x = gtruth.fHitNucP4.X();
    GTruth_HitNucP4y = gtruth.fHitNucP4.Y();
    GTruth_HitNucP4z = gtruth.fHitNucP4.Z();
    GTruth_HitNucP4E = gtruth.fHitNucP4.E();
    GTruth_FShadSystP4x = gtruth.fFShadSystP4.X();
    GTruth_FShadSystP4y = gtruth.fFShadSystP4.Y();
    GTruth_FShadSystP4z = gtruth.fFShadSystP4.Z();
    GTruth_FShadSystP4E = gtruth.fFShadSystP4.E();

    return;
  }

    
}
#endif
