#ifndef LARLITE_TRACKRECOANA_CXX
#define LARLITE_TRACKRECOANA_CXX

#include "TrackRecoAna.h"

#include "DataFormat/vertex.h"
#include "DataFormat/track.h"
#include "DataFormat/mctrack.h"

#include <cassert>

namespace larlite {

  TrackRecoAna::TrackRecoAna() {
    _name = "TrackRecoAna";
    _fout = 0;
    _tree = nullptr;
    _TrunkLength = -1.0*data::kINVALID_FLOAT;
  }

  bool TrackRecoAna::initialize() {

    _tree = new TTree("TrackRecoAna","");
    _tree->Branch("run"    , &_run    , "run/I");
    _tree->Branch("subrun" , &_subrun , "subrun/I");
    _tree->Branch("event"  , &_event  , "event/I");
    _tree->Branch("entry"  , &_entry  , "entry/I");
    
    //
    // mc tracks
    //
    _tree->Branch("MuonStartX"  , &_MuonStartX  , "MuonStartX/F");
    _tree->Branch("MuonStartY"  , &_MuonStartY  , "MuonStartY/F");
    _tree->Branch("MuonStartZ"  , &_MuonStartZ  , "MuonStartZ/F");

    _tree->Branch("MuonEndX"  , &_MuonEndX  , "MuonEndX/F");
    _tree->Branch("MuonEndY"  , &_MuonEndY  , "MuonEndY/F");
    _tree->Branch("MuonEndZ"  , &_MuonEndZ  , "MuonEndZ/F");

    _tree->Branch("MuonStartE", &_MuonStartE , "MuonStartE/F");
    _tree->Branch("MuonEndE"  , &_MuonEndE   , "MuonEndE/F");

    _tree->Branch("Muon_dE" , &_Muon_dE , "Muon_dE/F");
    _tree->Branch("Muon_dL" , &_Muon_dL , "Muon_dL/F");

    _tree->Branch("ProtonStartX"  , &_ProtonStartX  , "ProtonStartX/F");
    _tree->Branch("ProtonStartY"  , &_ProtonStartY  , "ProtonStartY/F");
    _tree->Branch("ProtonStartZ"  , &_ProtonStartZ  , "ProtonStartZ/F");

    _tree->Branch("ProtonEndX"  , &_ProtonEndX  , "ProtonEndX/F");
    _tree->Branch("ProtonEndY"  , &_ProtonEndY  , "ProtonEndY/F");
    _tree->Branch("ProtonEndZ"  , &_ProtonEndZ  , "ProtonEndZ/F");

    _tree->Branch("ProtonStartE", &_ProtonStartE , "ProtonStartE/F");
    _tree->Branch("ProtonEndE"  , &_ProtonEndE   , "ProtonEndE/F");

    _tree->Branch("Proton_dE" , &_Proton_dE , "Proton_dE/F");
    _tree->Branch("Proton_dL" , &_Proton_dL , "Proton_dL/F");

    
    //
    // reco tracks
    //
    _tree->Branch("vtxid"  , &_vtxid  , "vtxid/I");
    _tree->Branch("vtx_x"  , &_vtx_x  , "vtx_x/F");
    _tree->Branch("vtx_y"  , &_vtx_y  , "vtx_y/F");
    _tree->Branch("vtx_z"  , &_vtx_z  , "vtx_z/F");

    _tree->Branch("ntracks", &_ntracks, "ntracks/I");
    _tree->Branch("npts_v" , &_npts_v);

    _tree->Branch("TrunkLength", &_TrunkLength, "TrunkLength/F");
    
    _tree->Branch("dL_v"  , &_dL_v);
    _tree->Branch("sL_v"  , &_sL_v);
    _tree->Branch("EndX_v", &_EndX_v);
    _tree->Branch("EndY_v", &_EndY_v);
    _tree->Branch("EndZ_v", &_EndZ_v);
    
    _tree->Branch("Theta_v", &_Theta_v);
    _tree->Branch("Phi_v"  , &_Phi_v);
    
    _tree->Branch("dQdx_U_vv", &_dQdx_U_vv);
    _tree->Branch("dQdx_V_vv", &_dQdx_V_vv);
    _tree->Branch("dQdx_Y_vv", &_dQdx_Y_vv);

    _tree->Branch("Length_vv", &_Length_vv);

    _tree->Branch("TrunkLength_v", &_TrunkLength_v);
    _tree->Branch("TrunkdQdx_U_v", &_TrunkdQdx_U_v);
    _tree->Branch("TrunkdQdx_V_v", &_TrunkdQdx_V_v);
    _tree->Branch("TrunkdQdx_Y_v", &_TrunkdQdx_Y_v);
    _tree->Branch("TrunkTheta_v" , &_TrunkTheta_v);
    _tree->Branch("TrunkPhi_v"   , &_TrunkPhi_v);

    return true;
  }
  
  bool TrackRecoAna::analyze(storage_manager* storage) {
    
    assert(_TrunkLength>0);

    ClearEvent();

    _run    = (int) storage->run_id();
    _subrun = (int) storage->subrun_id();
    _event  = (int) storage->event_id();
    _entry  = (int) storage->get_index();
    

    //
    // MC track
    //
    const larlite::event_mctrack* ev_mctrack= nullptr;
    if(!_mc_producer.empty()) {
      ev_mctrack  = (larlite::event_mctrack*)storage->get_data(data::kMCTrack,_mc_producer);
      
      const larlite::mctrack* muon_track = nullptr;
      const larlite::mctrack* proton_track = nullptr;
      
      
      for(const auto& mct : *ev_mctrack) {
	if (mct.PdgCode() == 13) {
	  if (mct.PdgCode() == mct.MotherPdgCode() and mct.PdgCode() == mct.AncestorPdgCode()) {
	    muon_track = &mct;
	  }
	}
	if (mct.PdgCode() == 2212) {
	  if (mct.PdgCode() == mct.MotherPdgCode() and mct.PdgCode() == mct.AncestorPdgCode()) {
	    if (!proton_track){
	      proton_track = &mct;
	    }
	    else if(mct.Start().E() > proton_track->Start().E()) {
	      proton_track = &mct;
	    }
	  }
	}
      }
      
      if (muon_track) {
	
	_MuonStartX = muon_track->Start().X();
	_MuonStartY = muon_track->Start().Y();
	_MuonStartZ = muon_track->Start().Z();
	
	_MuonEndX = muon_track->End().X();
	_MuonEndY = muon_track->End().Y();
	_MuonEndZ = muon_track->End().Z();
	
	_MuonStartE = muon_track->Start().E();
	_MuonEndE   = muon_track->End().E();
	
	_Muon_dE = _MuonStartE - _MuonEndE;
	_Muon_dL = std::sqrt(std::pow(_MuonStartX - _MuonEndX,2) +
			     std::pow(_MuonStartY - _MuonEndY,2) +
			     std::pow(_MuonStartZ - _MuonEndZ,2));

      }      

      if (proton_track) {

	_ProtonStartX = proton_track->Start().X();
	_ProtonStartY = proton_track->Start().Y();
	_ProtonStartZ = proton_track->Start().Z();
	
	_ProtonEndX = proton_track->End().X();
	_ProtonEndY = proton_track->End().Y();
	_ProtonEndZ = proton_track->End().Z();
	
	_ProtonStartE = proton_track->Start().E();
	_ProtonEndE   = proton_track->End().E();
	
	_Proton_dE = _ProtonStartE - _ProtonEndE;
	_Proton_dL = std::sqrt(std::pow(_ProtonStartX - _ProtonEndX,2) +
			       std::pow(_ProtonStartY - _ProtonEndY,2) +
			       std::pow(_ProtonStartZ - _ProtonEndZ,2));

      }
      

    } // end MC
    

    //
    // reconstructed tracks
    //

    const auto ev_vertex  = (larlite::event_vertex*)storage->get_data(data::kVertex,_trk_vtx_prod);
    larlite::event_track *ev_track = nullptr;
    auto const& ass_track_vv = storage->find_one_ass(ev_vertex->id(), ev_track, ev_vertex->name());
    if (!ev_track) return true;

    for( size_t vtx_id = 0; vtx_id < ass_track_vv.size(); ++vtx_id) {
      ClearVertex();
      const auto& vertex      = ev_vertex->at(vtx_id);
      const auto& ass_track_v = ass_track_vv[vtx_id];
      
      _vtx_x = vertex.X();
      _vtx_y = vertex.Y();
      _vtx_z = vertex.Z();
      
      _vtxid   = (int)vtx_id;
      _ntracks = (int)ass_track_v.size();
      
      ResizeVectors(_ntracks);

      for(size_t aid=0; aid<ass_track_v.size(); ++aid) {
	auto assid = ass_track_v[aid];
	const auto& track = ev_track->at(assid);
	
	auto ntps = track.NumberTrajectoryPoints();

	auto& npts = _npts_v[aid];
	auto& dL = _dL_v[aid];
	auto& sL = _sL_v[aid];
	auto& EndX = _EndX_v[aid];
	auto& EndY = _EndY_v[aid];
	auto& EndZ = _EndZ_v[aid];
	
	auto& Theta = _Theta_v[aid];
	auto& Phi = _Phi_v[aid];
	
	auto& dQdx_U_v = _dQdx_U_vv[aid];
	auto& dQdx_V_v = _dQdx_V_vv[aid];
	auto& dQdx_Y_v = _dQdx_Y_vv[aid];
	
	auto& Length_v = _Length_vv[aid];
	
	auto& TrunkLength = _TrunkLength_v[aid];

	auto& TrunkdQdx_U = _TrunkdQdx_U_v[aid];
	auto& TrunkdQdx_V = _TrunkdQdx_V_v[aid];
	auto& TrunkdQdx_Y = _TrunkdQdx_Y_v[aid];
	
	auto& TrunkTheta = _TrunkTheta_v[aid];
	auto& TrunkPhi   = _TrunkPhi_v[aid];

	npts = ntps;

	auto dir = track.End() - track.Vertex();
	auto dir_u = dir.Unit();

	dL =  dir.Mag();
	EndX = track.End().X();
	EndY = track.End().Y();
	EndZ = track.End().Z();
	
	Theta = std::atan2(dir_u.Y(),dir_u.Z());
	Phi   = std::atan2(dir_u.X(),dir_u.Z());

	dQdx_U_v.resize(npts,-1.0*data::kINVALID_FLOAT);
	dQdx_V_v.resize(npts,-1.0*data::kINVALID_FLOAT);
	dQdx_Y_v.resize(npts,-1.0*data::kINVALID_FLOAT);
	Length_v.resize(npts,-1.0*data::kINVALID_FLOAT);

	const TVector3* trunk_pt = nullptr;

	TrunkdQdx_U = 0;
	TrunkdQdx_V = 0;
	TrunkdQdx_Y = 0;

	for(size_t pid=0; pid< track.NumberTrajectoryPoints(); ++pid) {
	  const auto& pt = track.LocationAtPoint(pid);      
	  auto tdist = (pt - track.Vertex()).Mag();

	  auto& dQdx_U = dQdx_U_v[pid];
	  auto& dQdx_V = dQdx_V_v[pid];
	  auto& dQdx_Y = dQdx_Y_v[pid];
	  auto& Length = Length_v[pid];
	  
	  dQdx_U = track.DQdxAtPoint(pid,(geo::View_t)0);
	  dQdx_V = track.DQdxAtPoint(pid,(geo::View_t)1);
	  dQdx_Y = track.DQdxAtPoint(pid,(geo::View_t)2);
	  Length = track.Length(pid);
	    
	  if (tdist > _TrunkLength) continue;
	  trunk_pt = &pt;

	  TrunkdQdx_U += dQdx_U;
	  TrunkdQdx_V += dQdx_V;
	  TrunkdQdx_Y += dQdx_Y;
	}

	if (!trunk_pt) trunk_pt = &(track.Vertex()); 

	auto trunk_dir = (*trunk_pt - track.Vertex());
	auto trunk_dir_u = trunk_dir.Unit();

	TrunkLength = trunk_dir.Mag();

	TrunkdQdx_U /= TrunkLength;
	TrunkdQdx_V /= TrunkLength;
	TrunkdQdx_Y /= TrunkLength;
	
	TrunkTheta = std::atan2(trunk_dir_u.Y(),trunk_dir_u.Z());
	TrunkPhi   = std::atan2(trunk_dir_u.X(),trunk_dir_u.Z());
	
	sL = Length_v.front();

      }
      _tree->Fill();
    } // end this vertex

    
    return true;
  }

  bool TrackRecoAna::finalize() {
    assert(_tree);
    _tree->Write();
    return true;
  }

  void TrackRecoAna::ClearEvent() {

    _run    = -1.0*data::kINVALID_INT;
    _subrun = -1.0*data::kINVALID_INT;
    _event  = -1.0*data::kINVALID_INT;
    _entry  = -1.0*data::kINVALID_INT;

    _MuonStartX = -1.0*data::kINVALID_FLOAT;
    _MuonStartY = -1.0*data::kINVALID_FLOAT;
    _MuonStartZ = -1.0*data::kINVALID_FLOAT;

    _MuonEndX = -1.0*data::kINVALID_FLOAT;
    _MuonEndY = -1.0*data::kINVALID_FLOAT;
    _MuonEndZ = -1.0*data::kINVALID_FLOAT;

    _MuonStartE = -1.0*data::kINVALID_FLOAT;
    _MuonEndE = -1.0*data::kINVALID_FLOAT;

    _Muon_dE = -1.0*data::kINVALID_FLOAT;
    _Muon_dL = -1.0*data::kINVALID_FLOAT;

    _ProtonStartX = -1.0*data::kINVALID_FLOAT;
    _ProtonStartY = -1.0*data::kINVALID_FLOAT;
    _ProtonStartZ = -1.0*data::kINVALID_FLOAT;

    _ProtonEndX = -1.0*data::kINVALID_FLOAT;
    _ProtonEndY = -1.0*data::kINVALID_FLOAT;
    _ProtonEndZ = -1.0*data::kINVALID_FLOAT;

    _ProtonStartE = -1.0*data::kINVALID_FLOAT;
    _ProtonEndE = -1.0*data::kINVALID_FLOAT;

    _Proton_dE = -1.0*data::kINVALID_FLOAT;
    _Proton_dL = -1.0*data::kINVALID_FLOAT;
      
    return;
  }

  void TrackRecoAna::ClearVertex() {
    _vtxid = -1.0*data::kINVALID_INT;
    
    _vtx_x = -1.0*data::kINVALID_FLOAT;
    _vtx_y = -1.0*data::kINVALID_FLOAT;
    _vtx_z = -1.0*data::kINVALID_FLOAT;
    
    _ntracks = -1.0*data::kINVALID_INT;

    _npts_v.clear();
    _dL_v.clear();
    _sL_v.clear();
    _EndX_v.clear();
    _EndY_v.clear();
    _EndZ_v.clear();

    _Theta_v.clear();
    _Phi_v.clear();

    _dQdx_U_vv.clear();
    _dQdx_V_vv.clear();
    _dQdx_Y_vv.clear();
    
    _Length_vv.clear();
    
    _TrunkLength_v.clear();

    _TrunkdQdx_U_v.clear();
    _TrunkdQdx_V_v.clear();
    _TrunkdQdx_Y_v.clear();
      
    _TrunkTheta_v.clear();
    _TrunkPhi_v.clear();
    
    return;
  }

  void TrackRecoAna::ResizeVectors(int ntracks) {

    _npts_v.resize(ntracks,-1.0*data::kINVALID_INT);

    _dL_v.resize(ntracks,-1.0*data::kINVALID_FLOAT);
    _sL_v.resize(ntracks,-1.0*data::kINVALID_FLOAT);
    _EndX_v.resize(ntracks,-1.0*data::kINVALID_FLOAT);
    _EndY_v.resize(ntracks,-1.0*data::kINVALID_FLOAT);
    _EndZ_v.resize(ntracks,-1.0*data::kINVALID_FLOAT);

    _Theta_v.resize(ntracks,-1.0*data::kINVALID_FLOAT);
    _Phi_v.resize(ntracks,-1.0*data::kINVALID_FLOAT);

    _dQdx_U_vv.resize(ntracks);
    _dQdx_V_vv.resize(ntracks);
    _dQdx_Y_vv.resize(ntracks);
    
    _Length_vv.resize(ntracks);

    _TrunkLength_v.resize(ntracks,-1.0*data::kINVALID_FLOAT);

    _TrunkdQdx_U_v.resize(ntracks,-1.0*data::kINVALID_FLOAT);
    _TrunkdQdx_V_v.resize(ntracks,-1.0*data::kINVALID_FLOAT);
    _TrunkdQdx_Y_v.resize(ntracks,-1.0*data::kINVALID_FLOAT);
      
    _TrunkTheta_v.resize(ntracks,-1.0*data::kINVALID_FLOAT);
    _TrunkPhi_v.resize(ntracks,-1.0*data::kINVALID_FLOAT);

    return;
  }

}
#endif
