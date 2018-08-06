#ifndef __SEGMENT_DUMP_CXX__
#define __SEGMENT_DUMP_CXX__

#include "SegmentDump.h"

#include "DataFormat/mctruth.h"
#include "DataFormat/mcflux.h"
#include "DataFormat/gtruth.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/shower.h"

#include "LArUtil/Geometry.h"

#include "TVector3.h"

#include <cassert>
#include <exception>


namespace larlite {

  SegmentDump::SegmentDump() {
    _name = "SegmentDump";

    fEventTree = nullptr;
    fAndyTree = nullptr;
  }

  bool SegmentDump::initialize() {
    
    fEventTree = new TTree("EventMCINFO_DL", "");
    fAndyTree = new TTree("EventAndyTree","");

    AttachEvent(fEventTree);
    AttachAndy(fAndyTree);

    return true;
  }

  bool SegmentDump::analyze(storage_manager* storage) {
    std::cout << "@entry=" << storage->get_index() << std::endl;
    
    ClearEvent();
    ResetAndy();

    FillAndy(storage);
    FillSegment(storage);

    _run    = (int)storage->run_id();
    _subrun = (int)storage->subrun_id();
    _event  = (int)storage->event_id();
    _entry  = (int)storage->get_index();

    fEventTree->Fill();
    fAndyTree->Fill();
    
    return true;
  }

  bool SegmentDump::finalize() {
    
    fEventTree->Write();
    fAndyTree->Write();
    
    return true;
  }

  void SegmentDump::AttachEvent(TTree* tree) {

    assert(tree);
    tree->Branch("event"  , &_event  , "event/I");
    tree->Branch("subrun" , &_subrun , "subrun/I");
    tree->Branch("run"    , &_run    , "run/I");
    tree->Branch("entry"  , &_entry  , "entry/I");

    tree->Branch("selected1L1P", &_selected, "selected1L1P/I");
    tree->Branch("parentPDG", &_parent_pdg, "parentPDG/I");
    tree->Branch("energyInit", &_energy_init, "energyInit/D");
    tree->Branch("parentX", &_parent_x, "parentX/D");
    tree->Branch("parentY", &_parent_y, "parentY/D");
    tree->Branch("parentZ", &_parent_z, "parentZ/D");

    tree->Branch("parentSCEX", &_parent_sce_x, "parentSCEX/D");
    tree->Branch("parentSCEY", &_parent_sce_y, "parentSCEY/D");
    tree->Branch("parentSCEZ", &_parent_sce_z, "parentSCEZ/D");

    tree->Branch("parentPx", &_parent_px, "parentPx/D");
    tree->Branch("parentPy", &_parent_py, "parentPy/D");
    tree->Branch("parentPz", &_parent_pz, "parentPz/D");

    tree->Branch("ineractionMode", &_interaction_mode, "interactionMode/I");
    tree->Branch("interactionType", &_interaction_type, "interactionType/I");

    tree->Branch("dep_sum_lepton", &_dep_sum_lepton, "dep_sum_lepton/D");
    tree->Branch("dep_sum_proton", &_dep_sum_proton, "dep_sum_proton/D");

    tree->Branch("proton_Px", &_proton_Px, "proton_Px/D");
    tree->Branch("proton_Py", &_proton_Py, "proton_Py/D");
    tree->Branch("proton_Pz", &_proton_Pz, "proton_Pz/D");

    tree->Branch("lepton_Px", &_lepton_Px, "lepton_Px/D");
    tree->Branch("lepton_Py", &_lepton_Py, "lepton_Py/D");
    tree->Branch("lepton_Pz", &_lepton_Pz, "lepton_Pz/D");

    tree->Branch("daughter_energydep_v", &_daughter_energydep_v);

    tree->Branch("daughterPdg_v", &_daughter_pdg_v);

    tree->Branch("daughterX_v", &_daughterX_v);
    tree->Branch("daughterY_v", &_daughterY_v);
    tree->Branch("daughterZ_v", &_daughterZ_v);

    tree->Branch("daughterX_end_v", &_daughterX_end_v);
    tree->Branch("daughterY_end_v", &_daughterY_end_v);
    tree->Branch("daughterZ_end_v", &_daughterZ_end_v);

    tree->Branch("daughterPx_v", &_daughterPx_v);
    tree->Branch("daughterPy_v", &_daughterPy_v);
    tree->Branch("daughterPz_v", &_daughterPz_v);

  }


  void SegmentDump::AttachAndy(TTree* tree) {

    assert(tree);

    tree->Branch("event"  , &_event  , "event/I");
    tree->Branch("subrun" , &_subrun , "subrun/I");
    tree->Branch("run"    , &_run    , "run/I");
    tree->Branch("entry"  , &_entry  , "entry/I");

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

  void SegmentDump::ResetAndy() {
    
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


  void SegmentDump::FillSegment(storage_manager *sto) {
    
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

      auto ex = mct.End().X();
      auto ey = mct.End().Y();
      auto ez = mct.End().Z();

      auto px = mct.Start().Px();
      auto py = mct.Start().Py();
      auto pz = mct.Start().Pz();
      
      // don't store corsika tracks, only primaries
      if (sy < 1799 and particle.primary()) {
	_daughter_pdg_v.push_back(particle.pdg);
	_daughter_energydep_v.push_back(particle.depeng);
	_daughterX_v.push_back(sx);
	_daughterY_v.push_back(sy);
	_daughterZ_v.push_back(sz);
	_daughterX_end_v.push_back(ex);
	_daughterY_end_v.push_back(ey);
	_daughterZ_end_v.push_back(ez);
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

      auto px = mcs.Start().Px();
      auto py = mcs.Start().Py();
      auto pz = mcs.Start().Pz();

      // do not store corsika showers
      if (sy < 1799 and particle.primary()) {
	_daughter_pdg_v.push_back(particle.pdg);
	_daughter_energydep_v.push_back(particle.depeng);
	_daughterX_v.push_back(sx);
	_daughterY_v.push_back(sy);
	_daughterZ_v.push_back(sz);
	_daughterX_end_v.push_back(-1);
	_daughterY_end_v.push_back(-1);
	_daughterZ_end_v.push_back(-1);
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

  void SegmentDump::FillAndy(storage_manager *sto) {

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


  void SegmentDump::ClearEvent() {
    
    _event  = -1.0*data::kINVALID_INT;
    _subrun = -1.0*data::kINVALID_INT;
    _run    = -1.0*data::kINVALID_INT;
    _entry  = -1.0*data::kINVALID_INT;
    
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

    _daughterX_end_v.clear();
    _daughterY_end_v.clear();
    _daughterZ_end_v.clear();

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


}

#endif
