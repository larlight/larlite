#ifndef SHOWERQUALITY_DL_CXX
#define SHOWERQUALITY_DL_CXX

#include "ShowerQuality_DL.h"

#include "DataFormat/vertex.h"
#include "DataFormat/pfpart.h"
#include "DataFormat/mctruth.h"

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

    fShowerTree = nullptr;
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
    fEventTree->Branch("mc_dcosx" , &_mc_dcosx, "mc_dcosx/D");
    fEventTree->Branch("mc_dcosy" , &_mc_dcosy, "mc_dcosy/D");
    fEventTree->Branch("mc_dcosz" , &_mc_dcosz, "mc_dcosz/D");
    fEventTree->Branch("mc_energy", &_mc_energy, "mc_energy/D");

    // 
    // Shower Tree
    //

    fShowerTree = new TTree("ShowerQuality_DL", "");
    fShowerTree->Branch("event"  , &_event  , "event/I");
    fShowerTree->Branch("subrun" , &_subrun , "subrun/I");
    fShowerTree->Branch("run"    , &_run    , "run/I");
    fShowerTree->Branch("entry"  , &_entry  , "entry/I");
    
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

    larlite::mcshower nue_shower;
    nue_shower.PdgCode(data::kINVALID_INT);
    
    _has_mc = false;
    if (ev_mcs and !ev_mcs->empty()) {
      
      assert(ev_truth);
      assert(!ev_truth->empty());

      // Read out some nu stuff
      const auto& truth = ev_truth->front();
      const auto& nu = truth.GetNeutrino();
      _mcinfoInteractionType =  (int)   nu.InteractionType();
      _mcinfoMode            =  (int)   nu.Mode();

      for(const auto& mcs : *ev_mcs) {
	if (mcs.PdgCode() == 11) {
	  if (mcs.PdgCode() == mcs.MotherPdgCode() 
	      && mcs.PdgCode() == mcs.AncestorPdgCode()) {
	    nue_shower = mcs;
	  }
	}
      }

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
    
  
    // get associated showers to the vertex
  
    // get the associated pf particles
    larlite::event_pfpart *ev_pfpart = nullptr;
    auto const& ass_pfpart_vv = storage->find_one_ass(ev_vertex->id(), ev_pfpart, ev_vertex->name());
    if (!ev_pfpart or ev_pfpart->empty()) {
      fEventTree->Fill();
      return true;
    }

    // get the associated pf clusters
    larlite::event_shower *ev_shower = nullptr;
    auto const& ass_shower_vv = storage->find_one_ass(ev_vertex->id(), ev_shower, fShowerProducer);
    if (!ev_shower or ev_shower->empty()) {
      fEventTree->Fill();
      return true;
    }
  
    for( size_t vtx_id = 0; vtx_id < ass_pfpart_vv.size(); ++vtx_id) {
      ClearVertex();
      const auto& vertex = ev_vertex->at(vtx_id);
      const auto& ass_pfpart_v = ass_pfpart_vv.at(vtx_id);
    
      std::cout << "@vtx_id=" << vtx_id << " : (" << vertex.X() << "," << vertex.Y() << "," << vertex.Z() << ")" << std::endl;
      std::cout << "..." << ass_pfpart_v.size() << " associated pfparts" << std::endl;
    
      _vtx_id = vtx_id;
      
      std::vector<size_t> shower_idx_v;
      std::vector<const larlite::shower* > shower_v;

      shower_idx_v.reserve(ev_shower->size());
      shower_v.reserve(ev_shower->size());

      for( size_t pfp_id=0; pfp_id < ass_pfpart_v.size(); ++ pfp_id) {

	const auto pfpart_id = ass_pfpart_v.at(pfp_id);
	const auto& ass_shower_v = ass_shower_vv.at(pfpart_id);

	for(size_t shr_id=0; shr_id < ass_shower_v.size(); ++ shr_id) {

	  const auto shower_id = ass_shower_v.at(shr_id);

	  shower_v.push_back(&ev_shower->at(shower_id));
	  shower_idx_v.push_back(shower_id);

	} // end ass shower
      } // end ass pfparticle

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
    return true;
  }
  
  bool ShowerQuality_DL::finalize() {
    assert(fShowerTree);
    fShowerTree->Write();

    assert(fEventTree);
    fEventTree->Write();
    
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
    
}
#endif
