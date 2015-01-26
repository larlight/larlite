#ifndef LARLITE_ERTOOLHELPER_CXX
#define LARLITE_ERTOOLHELPER_CXX

#include "ERToolHelper.h"
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
    FillTracks  (mct_v, event_data);
    FillShowers (mcs_v, event_data);
    FillVertices(mci_v, event_data);

    // To be updated: need a complete particle tree structure here
    for(auto const& mci : mci_v) {

      int src_id = -1;
      for(auto const& mcp : mci.GetParticles()) {
	if(mcp.StatusCode() == 0) {
	  ::ertool::Particle src(mcp.PdgCode(),mcp.Mass());
	  src.Momentum( ::geoalgo::Vector(mcp.Trajectory()[0].Momentum()) );
	  src.Vertex  ( ::geoalgo::Vector(mcp.Trajectory()[0].Position()) );
	  particle_set.push_back(src);
	  src_id = mcp.TrackId();
	  break;
	}
      }

      if(src_id < 0){
	for(auto const& mcp : mci.GetParticles()) {
	  if(mcp.StatusCode() == 1) {
	    ::ertool::Particle src(mcp.PdgCode(),mcp.Mass());
	    src.Momentum( ::geoalgo::Vector(mcp.Trajectory()[0].Momentum()) );
	    src.Vertex  ( ::geoalgo::Vector(mcp.Trajectory()[0].Position()) );
	    particle_set.push_back(src);
	    src_id = mcp.TrackId();
	    break;
	  }
	}
      }

      if(src_id<0) continue;
      
      for(auto const& mcp : mci.GetParticles()) {
	if(mcp.Mother() == src_id) {
	  ::ertool::Particle p(mcp.PdgCode(),mcp.Mass());
	  p.Momentum( ::geoalgo::Vector(mcp.Trajectory()[0].Momentum()) );
	  p.Vertex  ( ::geoalgo::Vector(mcp.Trajectory()[0].Position()) );
	  particle_set.back().AddDaughter(p);
	}
      }
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

      if(mcs.DetProfile().Momentum().E()<1) continue;
      if(isnan(mcs.DetProfile().Momentum().E())) continue;
      if(isnan(mcs.DetProfile().Momentum().Px())) continue;

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
