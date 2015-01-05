#ifndef SELECTIONTOOL_SPAHELPER_CXX
#define SELECTIONTOOL_SPAHELPER_CXX

#include "SPAHelper.h"
#include <limits>
#include <climits>
namespace sptool {

  SPAData SPAHelper::Generate ( const ::larlite::event_mctrack&  mct_v,
				const ::larlite::event_mcshower& mcs_v ) const
  {
    SPAData res;
    // Fill tracks
    res._tracks.clear();
    res._tracks.reserve(mct_v.size());
    for(auto const& mct : mct_v) {
      if(mct.size()<2) continue;
      res._tracks.push_back(SPATrack());
      auto& t = (*res._tracks.rbegin());

      t.reserve(mct.size());
      for(auto const& step : mct)
	t += step.Position();
      t._energy     = (*mct.rbegin()).Momentum().E() - (*mct.begin()).Momentum().E();
      t._cosmogenic = (double)(mct.Origin() == ::larlite::simb::kCosmicRay);

      if(mct.PdgCode() == 13  || mct.PdgCode() == -13 ) t._pid = SPATrack::kMuon;
      if(mct.PdgCode() == 2212                        ) t._pid = SPATrack::kProton;
      if(mct.PdgCode() == 321 || mct.PdgCode() == -321) t._pid = SPATrack::kKaon;
      if(mct.PdgCode() == 211 || mct.PdgCode() == -211) t._pid = SPATrack::kPion;
      for(auto& v : t._pid_score) v = 100;
      t._pid_score[t._pid] = 0.1;
    }
    // Fill showers
    res._showers.clear();
    res._showers.reserve(mcs_v.size());
    for(auto const& mcs : mcs_v) {

      if(mcs.DetProfile().Momentum().E()<1) continue;
      if(isnan(mcs.DetProfile().Momentum().E())) continue;
      if(isnan(mcs.DetProfile().Momentum().Px())) continue;
      res._showers.push_back(SPAShower(mcs.DetProfile().Position(),
				       mcs.DetProfile().Momentum())
			     );
      /*
      res._showers.push_back(SPAShower(mcs.Start().Position(),
				       mcs.Start().Momentum()));
      */
      auto& s = (*res._showers.rbegin());
      s._energy     = mcs.DetProfile().Momentum().E();
      //s._energy = mcs.Start().Momentum().E();
      s._dedx       = (mcs.PdgCode() == 22 ? gRandom->Gaus(4,4*0.3) : gRandom->Gaus(2,2*0.3));
      s._cosmogenic = (double)(mcs.Origin() == ::larlite::simb::kCosmicRay);


      //double _radius;
      //double _cone_angle;
    }

    return res;
  }

  void SPAHelper::Append ( const::larlite::event_mctruth& mci_v,
			   SPAData& res) const
  {
    // Fill vertex
    std::set< ::geoalgo::Point_t> vtxs;
    for(auto const& mci : mci_v) {
      
      res._vtxs.reserve(res._vtxs.size() + mci.GetParticles().size());
      for(auto const& mcp : mci.GetParticles()) {

	::geoalgo::Point_t vtx(mcp.Trajectory()[0].Position());

	if(vtxs.find(vtx) == vtxs.end()) {
	  res._vtxs.push_back(vtx);
	  vtxs.insert(vtx);
	}
      }
    }
  }

  SPAData SPAHelper::Generate ( const ::larlite::event_mctruth&  mci_v,
				const ::larlite::event_mctrack&  mct_v,
				const ::larlite::event_mcshower& mcs_v ) const
  {
    auto res = Generate(mct_v,mcs_v);
    Append(mci_v,res);
    return res;
  }

  void SPAHelper::Append( const ::larlite::event_vertex& vtx_v, 
			  SPAData& res ) const
  {
    // Fill vertex
    for(auto const& vtx : vtx_v)
      
      res._vtxs.push_back( ::geoalgo::Point_t(vtx.X(),vtx.Y(),vtx.Z()));
  }

  void SPAHelper::Append ( const ::larlite::event_track&  trk_v,
			   const ::larlite::event_cosmictag& ctag_trk_v,
			   const ::larlite::event_calorimetry& calo_trk_v,
			   const ::larlite::event_partid& pid_trk_v,
			   SPAData& res) const
  {
    // Fill tracks
    res._tracks.reserve(res._tracks.size() + trk_v.size());
    for(auto const& trk : trk_v) {

      res._tracks.push_back(SPATrack());
      auto& t = (*res._tracks.rbegin());

      for(size_t i=0; i<trk.n_point(); ++i)
	t += trk.vertex_at(i);
      
      t._cosmogenic = -1;
      t._energy     = -1;
    }
    // Revise track cosmogenic score
    auto const& ctag_trk_ass = ctag_trk_v.association(trk_v.id());
    if(ctag_trk_ass.size()) {
      for(size_t cos_index=0; cos_index<ctag_trk_v.size(); ++cos_index) {
	
	auto const& ctag = ctag_trk_v[cos_index];
	for(auto const& trk_index : ctag_trk_ass[cos_index])  
	  res._tracks.at(trk_index)._cosmogenic = ctag.fCosmicScore;
      }
    }
    // Revise track energy
    auto const& calo_trk_ass = calo_trk_v.association(trk_v.id());
    if(calo_trk_ass.size()) {
      for(size_t calo_index=0; calo_index<calo_trk_v.size(); ++calo_index) {

	auto const& calo = calo_trk_v[calo_index];
	for(auto const& trk_index : calo_trk_ass[calo_index])
	  res._tracks.at(trk_index)._energy = calo.KineticEnergy();
	
      }
    }
    // Revise track part id
    auto const& pid_trk_ass = pid_trk_v.association(trk_v.id());
    if(pid_trk_ass.size()) {
      for(size_t pid_index=0; pid_index<pid_trk_v.size(); ++pid_index) {

	auto const &pid = pid_trk_v[pid_index];
	// pick the one w/ minimum chi2
	for(auto const& trk_index : pid_trk_ass[pid_index]) {

	  auto& spa_trk = res._tracks.at(trk_index);
	  std::map<double,SPATrack::TrackPartID_t> score_map;
	  if(spa_trk._pid==SPATrack::kUnknown ||
	     spa_trk._pid_score[spa_trk._pid] > pid.MinChi2()) {
	    score_map.insert(std::make_pair(pid.Chi2Proton(),SPATrack::kProton));
	    score_map.insert(std::make_pair(pid.Chi2Kaon(),SPATrack::kKaon));
	    score_map.insert(std::make_pair(pid.Chi2Pion(),SPATrack::kPion));
	    score_map.insert(std::make_pair(pid.Chi2Muon(),SPATrack::kMuon));
	    for(auto const& score_pid : score_map)
	      spa_trk._pid_score[score_pid.second] = score_pid.first;
	    spa_trk._pid = (*score_map.begin()).second;
	    spa_trk._pid_score[SPATrack::kPIDA] = pid.PIDA();
	  }
	}
      }
    }
  }


  void SPAHelper::Append ( const ::larlite::event_shower& shw_v,
			   const ::larlite::event_cosmictag& ctag_shw_v,
			   SPAData& res ) const
  {
    // Fill shower
    res._showers.reserve(res._showers.size() + shw_v.size());
    for(auto const& shw : shw_v) {

      res._showers.push_back(SPAShower(shw.ShowerStart(),
				       shw.Direction())
			     );
      auto& s = (*res._showers.rbegin());
      if(shw.best_plane()){
	s._energy = (*(shw.Energy().rbegin()));
	s._dedx   = (*(shw.dEdx().rbegin()));
      }else{
	s._energy = (*(shw.Energy().begin()));
	s._dedx   = (*(shw.dEdx().begin()));
      }
      s._cosmogenic = -1;
    }
    // Revise shower cosmogenic score
    auto const& ctag_shw_ass = ctag_shw_v.association(shw_v.id());
    if(ctag_shw_ass.size()) {
      for(size_t cos_index=0; cos_index<ctag_shw_v.size(); ++cos_index) {
	
	auto const& ctag = ctag_shw_v[cos_index];
	for(auto const& shw_index : ctag_shw_ass[cos_index])  
	  res._showers.at(shw_index)._cosmogenic = ctag.fCosmicScore;
      }
    }

  }

  SPAData SPAHelper::Generate ( const ::larlite::event_shower& shw_v,
				const ::larlite::event_cosmictag& ctag_shw_v) const
  {
    SPAData res;
    Append(shw_v,ctag_shw_v,res);
    return res;
  }

  SPAData SPAHelper::Generate ( const ::larlite::event_vertex& vtx_v,
				const ::larlite::event_track&  trk_v,
				const ::larlite::event_shower& shw_v,
				const ::larlite::event_cosmictag& ctag_trk_v,
				const ::larlite::event_cosmictag& ctag_shw_v,
				const ::larlite::event_calorimetry& calo_trk_v,
				const ::larlite::event_partid& pid_trk_v) const
  {
    auto res = Generate(shw_v,ctag_shw_v);
    Append(vtx_v,res);
    Append(trk_v,ctag_trk_v,calo_trk_v,pid_trk_v,res);
    return res;
  }



}

#endif
