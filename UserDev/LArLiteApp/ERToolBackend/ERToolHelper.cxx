#ifndef LARLITE_ERTOOLHELPER_CXX
#define LARLITE_ERTOOLHELPER_CXX

#include "ERToolHelper.h"
#include <TDatabasePDG.h>
#include <limits>
#include <climits>
namespace larlite {

  /// Create MC EventData and ParticleSet
  void ERToolHelper::FillMCInfo( const event_mctruth&   mci_v,
				 const event_mcshower&  mcs_v,
				 const event_mctrack&   mct_v,
				 ::ertool::EventData&   event_data,
				 ::ertool::ParticleSet& particle_set) const
  {

    static TDatabasePDG pdgdb_s;
    static std::map<int,TParticlePDG*> partmap_s;

    FillTracks  (mct_v, event_data);
    FillShowers (mcs_v, event_data);
    FillVertices(mci_v, event_data);

    std::map<unsigned int, ::ertool::Particle> g4_mother_parts;
    std::map<unsigned int, PartID_t> g4_mother_id;
    size_t reco_obj_index = 0;
    for(size_t i=0; i<mcs_v.size(); ++i) {

      auto const& mcs = mcs_v[i];

      if(mcs.DetProfile().E()<1) continue;
      reco_obj_index++;

      if( mcs.MotherTrackID() != mcs.AncestorTrackID() ) continue;
      if( mcs.TrackID() == mcs.MotherTrackID() ) continue;
      
      unsigned int trkid_mom = mcs.AncestorTrackID();
      
      if(g4_mother_parts.find(trkid_mom) == g4_mother_parts.end()) {
	int pdg_mom = mcs.AncestorPdgCode();
	if(partmap_s.find(pdg_mom) == partmap_s.end())
	  partmap_s[pdg_mom] = pdgdb_s.GetParticle(pdg_mom);

	::ertool::Particle mother(pdg_mom, partmap_s[pdg_mom]->Mass());
	mother.Vertex( ::geoalgo::Vector(mcs.MotherStart().Position()) );
	mother.Momentum( ::geoalgo::Vector(mcs.MotherStart().Momentum()) );
	//g4_mother_parts[trkid_mom] = mother;
	g4_mother_parts.insert(std::make_pair(trkid_mom,mother));
	g4_mother_id[trkid_mom] = PartID_t(mother.Vertex(), mother.Momentum(), mother.PdgCode());
      }
      
      int pdg = mcs.PdgCode();
      if(partmap_s.find(pdg) == partmap_s.end())
	partmap_s[pdg] = pdgdb_s.GetParticle(pdg);

      ::ertool::Particle p(mcs.PdgCode(),partmap_s[pdg]->Mass());
      p.Vertex( ::geoalgo::Vector(mcs.DetProfile().Position()) );
      p.Momentum( ::geoalgo::Vector(mcs.DetProfile().Momentum()) );
      p.RecoObjInfo(reco_obj_index-1, ::ertool::Particle::kShower);
      g4_mother_parts[trkid_mom].AddDaughter(p);
    }

    reco_obj_index = 0;
    for(size_t i=0; i<mct_v.size(); ++i) {

      auto const& mct = mct_v[i];

      if(mct.size()<2) continue;
      reco_obj_index++;

      if( mct.MotherTrackID() != mct.AncestorTrackID() ) continue;
      if( mct.TrackID() == mct.MotherTrackID() ) continue;

      unsigned int trkid_mom = mct.AncestorTrackID();
      
      if(g4_mother_parts.find(trkid_mom) == g4_mother_parts.end()) {
	int pdg_mom = mct.AncestorPdgCode();
	if(partmap_s.find(pdg_mom) == partmap_s.end())
	  partmap_s[pdg_mom] = pdgdb_s.GetParticle(pdg_mom);

	::ertool::Particle mother(pdg_mom, partmap_s[pdg_mom]->Mass());
	mother.Vertex( ::geoalgo::Vector(mct.MotherStart().Position()) );
	mother.Momentum( ::geoalgo::Vector(mct.MotherStart().Momentum()) );
	g4_mother_parts[trkid_mom] = mother;
	g4_mother_id[trkid_mom] = PartID_t(mother.Vertex(), mother.Momentum(), mother.PdgCode());
      }
      
      int pdg = mct.PdgCode();
      if(partmap_s.find(pdg) == partmap_s.end())
	partmap_s[pdg] = pdgdb_s.GetParticle(pdg);

      ::ertool::Particle p(mct.PdgCode(),partmap_s[pdg]->Mass());
      p.Vertex( ::geoalgo::Vector(mct.front().Position()) );
      p.Momentum( ::geoalgo::Vector(mct.front().Momentum()) );
      p.RecoObjInfo(reco_obj_index-1, ::ertool::Particle::kTrack);
      g4_mother_parts[trkid_mom].AddDaughter(p);
    }

    //
    // Revise mother particle information w/ MCShower/MCTrack
    //
    reco_obj_index = 0;
    for(size_t i=0; i<mcs_v.size(); ++i) {

      auto const& mcs = mcs_v[i];
      if(mcs.DetProfile().E()<1) continue;
      reco_obj_index++;

      unsigned int trkid = mcs.TrackID();
      if(g4_mother_parts.find(trkid) != g4_mother_parts.end()) {
	g4_mother_parts[trkid].Vertex( ::geoalgo::Vector(mcs.DetProfile().Position()) );
	g4_mother_parts[trkid].Momentum( ::geoalgo::Vector(mcs.DetProfile().Momentum()) );
	g4_mother_parts[trkid].RecoObjInfo(reco_obj_index-1, ::ertool::Particle::kShower);
      }
    }
    reco_obj_index = 0;
    for(size_t i=0; i<mct_v.size(); ++i) {

      auto const& mct = mct_v[i];
      if(mct.size()<2) continue;
      reco_obj_index++;

      unsigned int trkid = mct.TrackID();
      if(g4_mother_parts.find(trkid) != g4_mother_parts.end()) {
	g4_mother_parts[trkid].Vertex( ::geoalgo::Vector(mct.front().Position()) );
	g4_mother_parts[trkid].Momentum( ::geoalgo::Vector(mct.front().Momentum()) );
	g4_mother_parts[trkid].RecoObjInfo(reco_obj_index-1, ::ertool::Particle::kTrack);
      }
    }

    //
    // Find grand mother from mctruth
    //
    particle_set.clear();
    std::set<unsigned int> g4_mother_used;

    for(auto const& mci : mci_v) {

      for(auto const& mcp : mci.GetParticles()) {
	
	if(mcp.StatusCode() != 1) continue;
	//if(mcp.Mother() == -1) continue;

	// Check if this particle exists
	PartID_t mcp_id( ::geoalgo::Vector(mcp.Trajectory()[0].Position()),
			 ::geoalgo::Vector(mcp.Trajectory()[0].Momentum())*1.e3,
			 mcp.PdgCode() );
	std::cout<< "This part info: "
		 << mcp_id._pdg << " ... "
		 << mcp_id._pos[0] << " : "
		 << mcp_id._pos[1] << " : "
		 << mcp_id._pos[2] << " ... "
		 << mcp_id._mom[0] << " : "
		 << mcp_id._mom[1] << " : "
		 << mcp_id._mom[2] << " ... " << std::endl;
	bool g4_mother_found = false;
	for(auto const& mom_pair : g4_mother_id) {

	  if( mcp_id.same(mom_pair.second) ) {

	    // Found a matched g4 mother
	    g4_mother_found = true;

	    auto grand_mother_trkid = mcp.Mother();
	    if(grand_mother_trkid<0) break;
	    if(grand_mother_trkid == mcp.TrackId()) break;

	    for(auto const& grand_mother : mci.GetParticles()) {

	      if(grand_mother.TrackId() != grand_mother_trkid) continue;

	      int pdg = grand_mother.PdgCode();
	      std::cout<<"Creating grand mother... " << pdg<<std::endl;
	      if(partmap_s.find(pdg) == partmap_s.end())
		partmap_s[pdg] = pdgdb_s.GetParticle(pdg);
	      ::ertool::Particle p(pdg,partmap_s[pdg]->Mass());
	      p.Vertex(::geoalgo::Vector(grand_mother.Trajectory()[0].Position()));
	      p.Momentum(::geoalgo::Vector(grand_mother.Trajectory()[0].Momentum()));
	      p.AddDaughter(g4_mother_parts[mom_pair.first]);
	      particle_set.push_back(p);
	      g4_mother_used.insert(mom_pair.first);
	      break;
	    }
	    break;
	  }
	  else {
	    std::cout<< "Unmatched G4 mother info: "
		     << mom_pair.second._pdg << " ... "
		     << mom_pair.second._pos[0] << " : "
		     << mom_pair.second._pos[1] << " : "
		     << mom_pair.second._pos[2] << " ... "
		     << mom_pair.second._mom[0] << " : "
		     << mom_pair.second._mom[1] << " : "
		     << mom_pair.second._mom[2] << " ... " << std::endl;
	  }
	}
	if(!g4_mother_found) {
	  int pdg = mcp.PdgCode();
	  std::cout<<"Creating mother not in G4 list... " << pdg<<std::endl;
	  if(partmap_s.find(pdg) == partmap_s.end())
	    partmap_s[pdg] = pdgdb_s.GetParticle(pdg);
	  ::ertool::Particle p(pdg,partmap_s[pdg]->Mass());
	  p.Vertex(::geoalgo::Vector(mcp.Trajectory()[0].Position()));
	  p.Momentum(::geoalgo::Vector(mcp.Trajectory()[0].Momentum()));	  
	  particle_set.push_back(p);
	}
      }
    }

    for(auto const& mom_pair : g4_mother_parts) {

      if(g4_mother_used.find(mom_pair.first) == g4_mother_used.end())

	particle_set.push_back(mom_pair.second);
      
    }

  }


  void ERToolHelper::FillTracks ( const event_mctrack&  mct_v,
				  ::ertool::EventData& res ) const
  {
    for(auto const& mct : mct_v) {
      if(mct.size()<2) continue;
      ::ertool::Track t;
      t.reserve(mct.size());
      for(auto const& step : mct)
	t += step.Position();
      t._energy     = (*mct.begin()).Momentum().E() - (*mct.rbegin()).Momentum().E();
      t._cosmogenic = (double)(mct.Origin() == simb::kCosmicRay);

      if(mct.PdgCode() == 13  || mct.PdgCode() == -13 ) t._pid = ::ertool::Track::kMuon;
      if(mct.PdgCode() == 2212                        ) t._pid = ::ertool::Track::kProton;
      if(mct.PdgCode() == 321 || mct.PdgCode() == -321) t._pid = ::ertool::Track::kKaon;
      if(mct.PdgCode() == 211 || mct.PdgCode() == -211) t._pid = ::ertool::Track::kPion;
      for(auto& v : t._pid_score) v = 100;
      if(t._pid < t._pid_score.size()) t._pid_score[t._pid] = 0.1;

      res.Add(t);
    }

    return;
  }


  void ERToolHelper::FillTracks ( const event_track&  trk_v,
				  const event_cosmictag& ctag_trk_v,
				  const event_calorimetry& calo_trk_v,
				  const event_partid& pid_trk_v,
				  ::ertool::EventData& res) const
  {
    std::vector< ::ertool::Track> t_v;
    t_v.reserve(trk_v.size());
    for(auto const& trk : trk_v) {
      
      t_v.push_back(::ertool::Track());
      auto &t = t_v.back();

      for(size_t i=0; i<trk.n_point(); ++i)
	t += trk.vertex_at(i);
      
      t._cosmogenic = -1;
      t._energy     = trk.momentum_at(0);

    }
    // Revise track cosmogenic score
    if (ctag_trk_v.size()){
      auto const& ctag_trk_ass = ctag_trk_v.association(trk_v.id());
      if(ctag_trk_ass.size()) {
	for(size_t cos_index=0; cos_index<ctag_trk_v.size(); ++cos_index) {
	  
	  auto const& ctag = ctag_trk_v[cos_index];
	  for(auto const& trk_index : ctag_trk_ass[cos_index])  
	    t_v.at(trk_index)._cosmogenic = ctag.fCosmicScore;
	}// for all associations
      }// if association is found
    }// if ctag exists
    // Revise track energy
    if (calo_trk_v.size()){
      auto const& calo_trk_ass = calo_trk_v.association(trk_v.id());
      if(calo_trk_ass.size()) {
	for(size_t calo_index=0; calo_index<calo_trk_v.size(); ++calo_index) {
	  
	  auto const& calo = calo_trk_v[calo_index];
	  for(auto const& trk_index : calo_trk_ass[calo_index])
	    t_v.at(trk_index)._energy = calo.KineticEnergy();
	}// for all associations
      }// if association is found
    }// if calo exists
    // Revise track part id
    if (pid_trk_v.size()){
      auto const& pid_trk_ass = pid_trk_v.association(trk_v.id());
      if(pid_trk_ass.size()) {
	for(size_t pid_index=0; pid_index<pid_trk_v.size(); ++pid_index) {
	  
	  auto const &pid = pid_trk_v[pid_index];
	  // pick the one w/ minimum chi2
	  for(auto const& trk_index : pid_trk_ass[pid_index]) {
	    
	    auto& spa_trk = t_v.at(trk_index);
	    std::map<double,::ertool::Track::TrackPartID_t> score_map;
	    if(spa_trk._pid==::ertool::Track::kUnknown ||
	       spa_trk._pid_score[spa_trk._pid] > pid.MinChi2()) {
	      score_map.insert(std::make_pair(pid.Chi2Proton(),::ertool::Track::kProton));
	      score_map.insert(std::make_pair(pid.Chi2Kaon(),::ertool::Track::kKaon));
	      score_map.insert(std::make_pair(pid.Chi2Pion(),::ertool::Track::kPion));
	      score_map.insert(std::make_pair(pid.Chi2Muon(),::ertool::Track::kMuon));
	      for(auto const& score_pid : score_map)
		spa_trk._pid_score[score_pid.second] = score_pid.first;
	      spa_trk._pid = (*score_map.begin()).second;
	      spa_trk._pid_score[::ertool::Track::kPIDA] = pid.PIDA();
	    }
	  }// for all associations
	}// for pid indices
      } // if association is found
    }// if pid exists

    for(auto const& t : t_v)
      res.Add(t);
  }
  
  
  void ERToolHelper::FillShowers ( const event_mcshower& mcs_v,
				   ::ertool::EventData& res ) const
  {
    for(auto const& mcs : mcs_v) {

      if(mcs.DetProfile().Momentum().E()<10) continue;
      //if(isnan(mcs.DetProfile().Momentum().E())) continue;
      //if(isnan(mcs.DetProfile().Momentum().Px())) continue;

      ::ertool::Shower s( mcs.DetProfile().Position(),
			  mcs.DetProfile().Momentum(),
			  _shrProfiler.Length( mcs.DetProfile().Momentum().E()),
			  _shrProfiler.ShowerRadius() );
      s._energy     = mcs.DetProfile().Momentum().E();
      //s._energy = mcs.Start().Momentum().E();
      s._dedx       = (mcs.PdgCode() == 22 ? gRandom->Gaus(4,4*0.3) : gRandom->Gaus(2,2*0.3));
      s._cosmogenic = (double)(mcs.Origin() == simb::kCosmicRay);
      res.Add(s);
    }

    return;
  }

  void ERToolHelper::FillShowers(const event_shower& shw_v,
				 const event_cosmictag& ctag_shw_v,
				 ::ertool::EventData& res ) const
  {
    // Fill shower
    std::vector< ::ertool::Shower> s_v;
    s_v.reserve(shw_v.size());
    for(auto const& shw : shw_v) {
      s_v.push_back( ::ertool::Shower(shw.ShowerStart(),
				      shw.Direction(),
				      _shrProfiler.Length(*(shw.Energy().rbegin())),
				      _shrProfiler.ShowerRadius()) );
      auto& s = (*s_v.rbegin());
      if(shw.best_plane()){
	s._energy = (*(shw.Energy().rbegin()));
	s._dedx   = (*(shw.dEdx().rbegin()));
      }else{
	s._energy = (*(shw.Energy().begin()));
	s._dedx   = (*(shw.dEdx().begin()));
      }
      // by default. Add cosmic score for showers to edit
      s._cosmogenic = -1;
    }

    // Revise shower cosmogenic score
    // make sure ctag_shr_v is not empty
    if ( ctag_shw_v.size() ){
      auto const& ctag_shw_ass = ctag_shw_v.association(shw_v.id());
      if(ctag_shw_ass.size()) {
	for(size_t cos_index=0; cos_index<ctag_shw_v.size(); ++cos_index) {
	  
	  auto const& ctag = ctag_shw_v[cos_index];
	  for(auto const& shw_index : ctag_shw_ass[cos_index])  
	    s_v.at(shw_index)._cosmogenic = ctag.fCosmicScore;
	}
      }
    }

    for(auto const& s : s_v) res.Add(s);
    return;
  }

  void ERToolHelper::FillVertices( const event_vertex& vtx_v, 
				   ::ertool::EventData& res ) const

  {
    // Fill vertex
    for(auto const& vtx : vtx_v) {
      ::ertool::Vertex pos(vtx.X(),vtx.Y(),vtx.Z());
      res.Add(pos);
    }
    return;
  }

  void ERToolHelper::FillVertices ( const event_mctruth& mci_v,
				    ::ertool::EventData& res) const
  {
    // Fill vertex
    std::set< ::geoalgo::Point_t> vtxs;
    for(auto const& mci : mci_v) {
      
      for(auto const& mcp : mci.GetParticles()) {
	::geoalgo::Point_t vtx(mcp.Trajectory()[0].Position());
	
	if(vtxs.find(vtx) == vtxs.end()) {
	  res.Add(::ertool::Vertex(vtx));
	  vtxs.insert(vtx);
	}
      }
    }
  }
}

#endif
