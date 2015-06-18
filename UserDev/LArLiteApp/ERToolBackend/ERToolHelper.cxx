#ifndef LARLITE_ERTOOLHELPER_CXX
#define LARLITE_ERTOOLHELPER_CXX

#include "ERToolHelper.h"
#include <TDatabasePDG.h>
#include <limits>
#include <climits>
namespace larlite {

  /// Function to fill map with mass mass from PDG code
  std::map<int,double>& ERToolHelper::getMassMap(const event_mcshower& mcs_v,
						 const event_mctrack& mct_v)
  {

    static TDatabasePDG pdgdbs;

    std::map<int,double> pdgmap;

    // loop over all showers
    for (auto const& mcs : mcs_v){
      int pdg = mcs.PdgCode();
      if(pdgmap.find(pdg) == pdgmap.end()) {
	double mass = 0;
	if(pdg>1000000000)
	  mass = (double)((int(pdg/10))%1000);
	else
	  mass = pdgdbs.GetParticle(pdg)->Mass() * 1.e3; //Mass is now in MEV
	pdgmap[pdg] = mass;
      }
    }// for all showers

    // loop over all tracks
    for (auto const& mcs : mcs_v){
      int pdg = mcs.PdgCode();
      if(pdgmap.find(pdg) == pdgmap.end()) {
	double mass = 0;
	if(pdg>1000000000)
	  mass = (double)((int(pdg/10))%1000);
	else
	  mass = pdgdbs.GetParticle(pdg)->Mass() * 1.e3; //Mass is now in MEV
	pdgmap[pdg] = mass;
      }
    }// for all tracks
    
    return pdgmap;
  }
  
  /// Create MC EventData and ParticleSet
  void ERToolHelper::FillMCInfo( const event_mctruth&   mci_v,
				 const event_mcshower&  mcs_v,
				 const event_mctrack&   mct_v,
				 ::ertool::Manager&     mgr) const
  {

    static TDatabasePDG pdgdbs;

    // First, create a PDG -> MASS map
    //auto& PdgMass = getMassMap(mcs_v,mct_v);

    std::map<unsigned int, ::ertool::Particle> g4_mother_parts;
    std::map<unsigned int, PartID_t> g4_mother_id;
    size_t reco_obj_index = 0;
    for(size_t i=0; i<mcs_v.size(); ++i) {

      auto const& mcs = mcs_v[i];

      // apply cut on deposited energy
      if(mcs.DetProfile().E()<_minEDep) continue;

      reco_obj_index++;

      // create shower/particle object
      // first add Shower object to event data
      ::ertool::Shower s( mcs.DetProfile().Position(),
			  mcs.DetProfile().Momentum(),
			  _shrProfiler.Length( mcs.DetProfile().Momentum().E()),
			  _shrProfiler.ShowerRadius() );
      s._energy     = mcs.DetProfile().Momentum().E();
      s._dedx       = (mcs.PdgCode() == 22 ? gRandom->Gaus(4,4*0.03) : gRandom->Gaus(2,2*0.03));
      s._cosmogenic = (double)(mcs.Origin() == simb::kCosmicRay);
      auto nodeID = mgr.Add(s,ertool::RecoInputID_t(i,mcs_v.name()),true);
      int pdg = mcs.PdgCode();
      // then edit the particle
      mgr.MCParticleGraph().GetParticle(nodeID).SetParticleInfo(mcs.PdgCode(),
								pdgdbs.GetParticle(pdg)->Mass()*1.e-3,
								mcs.Start().Position(),
								mcs.Start().Momentum()); 
      // if not primary
      if( mcs.MotherTrackID() != mcs.AncestorTrackID() ) continue;
      
      unsigned int trkid_mom = mcs.AncestorTrackID();
      
      if(g4_mother_parts.find(trkid_mom) == g4_mother_parts.end()) {
	int pdg_mom = mcs.AncestorPdgCode();
	auto& mother = mgr.MCParticleGraph().CreateParticle();
	mother.SetParticleInfo(pdg_mom,pdgdbs.GetParticle(pdg_mom)->Mass()*1.e3,//PdgMass[pdg_mom],
			       mcs.MotherStart().Position(),
			       mcs.MotherStart().Momentum());
	
	g4_mother_parts.insert(std::make_pair(trkid_mom,mother));
	g4_mother_id[trkid_mom] = PartID_t(mother.Vertex(), mother.Momentum(), mother.PdgCode());
      }
      if(mcs.TrackID() != mcs.MotherTrackID()) {      
	
	auto const& motherNodeID = g4_mother_parts[trkid_mom].ID();
	auto const& childNodeID  = mgr.MCParticleGraph().GetParticle(nodeID).ID();
	mgr.MCParticleGraph().SetParentage(motherNodeID,
					   childNodeID);
	
       }
     }

     reco_obj_index = 0;
     for(size_t i=0; i<mct_v.size(); ++i) {

       auto const& mct = mct_v[i];

       if(mct.size()<2) continue;
       reco_obj_index++;
       
       ::ertool::Track t;
       t.reserve(mct.size());
       for(auto const& step : mct)
	 t += step.Position();
       //This is the TOTAL energy minus TOTAL energy, so mass is removed.
       //If you want only initial kinetic energy, remember to subtract off mass.
       t._energy     = (*mct.begin()).Momentum().E() - (*mct.rbegin()).Momentum().E();
       t._cosmogenic = (double)(mct.Origin() == simb::kCosmicRay);
       if(abs(mct.PdgCode()) == 13 ) t._pid = ::ertool::Track::kMuon;
       if(mct.PdgCode() == 2212    ) t._pid = ::ertool::Track::kProton;
       if(abs(mct.PdgCode()) == 321) t._pid = ::ertool::Track::kKaon;
       if(abs(mct.PdgCode()) == 211) t._pid = ::ertool::Track::kPion;
       for(auto& v : t._pid_score) v = 100;
       if(t._pid < t._pid_score.size()) t._pid_score[t._pid] = 0.1;
       
       auto nodeID = mgr.Add(t,ertool::RecoInputID_t(i,mct_v.name()),true);
       int pdg = mct.PdgCode();
       mgr.MCParticleGraph().GetParticle(nodeID).SetParticleInfo(mct.PdgCode(),
								 pdg,
								 mct.at(0).Position(),
								 ::geoalgo::Vector(mct.at(0).Momentum()));
       // if not primary
       if( mct.MotherTrackID() != mct.AncestorTrackID() ) continue;
       
       unsigned int trkid_mom = mct.AncestorTrackID();
       
       if(g4_mother_parts.find(trkid_mom) == g4_mother_parts.end()) {
	 int pdg_mom = mct.AncestorPdgCode();
	 auto& mother = mgr.MCParticleGraph().CreateParticle();
	 mother.SetParticleInfo(pdg_mom,pdgdbs.GetParticle(pdg_mom)->Mass()*1.e3,//PdgMass[pdg_mom],
				mct.MotherStart().Position(),
				mct.MotherStart().Momentum());
	 g4_mother_parts.insert(std::make_pair(trkid_mom,mother));
	 g4_mother_id[trkid_mom] = PartID_t(mother.Vertex(), mother.Momentum(), mother.PdgCode());
       }
       if(mct.TrackID() != mct.MotherTrackID()) {
	 
	 auto const& motherNodeID = g4_mother_parts[trkid_mom].ID();
	 auto const& childNodeID  = mgr.MCParticleGraph().GetParticle(nodeID).ID();
	 mgr.MCParticleGraph().SetParentage(motherNodeID,
					    childNodeID);
       }
     }
     
     //
     // Revise mother particle information w/ MCShower/MCTrack
     //
     reco_obj_index = 0;
     for(size_t i=0; i<mcs_v.size(); ++i) {
       
       auto const& mcs = mcs_v[i];
       if(mcs.DetProfile().E()<_minEDep) continue;
       reco_obj_index++;
       
       unsigned int trkid = mcs.TrackID();
       if(g4_mother_parts.find(trkid) != g4_mother_parts.end()) {
	 // revise particle information
	 auto const& ID = g4_mother_parts[trkid].ID();
	 auto const& mass = mgr.MCParticleGraph().GetParticle(ID).Mass();
	 auto const& pdg  = mgr.MCParticleGraph().GetParticle(ID).PdgCode();
	 mgr.MCParticleGraph().GetParticle(ID).SetParticleInfo(pdg,mass,
							       ::geoalgo::Vector(mcs.DetProfile().Position()),
							       ::geoalgo::Vector(mcs.DetProfile().Momentum()) );
       }
     }
     reco_obj_index = 0;
     for(size_t i=0; i<mct_v.size(); ++i) {
       
       auto const& mct = mct_v[i];
       if(mct.size()<2) continue;
       reco_obj_index++;
       
       unsigned int trkid = mct.TrackID();
       if(g4_mother_parts.find(trkid) != g4_mother_parts.end()) {
	 // revise particle information
	 auto const& ID = g4_mother_parts[trkid].ID();
	 auto const& mass = mgr.MCParticleGraph().GetParticle(ID).Mass();
	 auto const& pdg  = mgr.MCParticleGraph().GetParticle(ID).PdgCode();
	 mgr.MCParticleGraph().GetParticle(ID).SetParticleInfo(pdg,mass,
							       ::geoalgo::Vector(mct.front().Position()),
							       ::geoalgo::Vector(mct.front().Momentum()) );
       }
     }
     std::set<unsigned int> g4_mother_used;
     for(auto const& mci : mci_v) {
       
       // Make status 1 => parent mapping: key = track ID and value = index
       std::map<unsigned int, unsigned int> trkid_to_grand_mother;
       // Make grand mother => particle set mapping: key = grand_mother index, value = particle set index #
       std::map<unsigned int, unsigned int> grand_mother_to_res_index;
       for(auto const& mcp : mci.GetParticles()) {
	 if(mcp.StatusCode()!=1) continue;
	 if(mcp.PdgCode()>2000000000) continue;
	 if(mcp.Mother() == -1 || mcp.Mother() == mcp.TrackId()) continue;

	 unsigned int grand_mother_trkid = mcp.Mother();
	 bool search = true;
	 while(search) {
	   search = false;
	   for(size_t i=0; i<mci.GetParticles().size(); ++i) {
	     auto const& mom_cand = mci.GetParticles()[i];
	     if(mom_cand.TrackId() == grand_mother_trkid) {
	       // Found a grand mother particle
	       // See if further search is needed
	       if(mom_cand.Mother() != -1 && 
		  mom_cand.Mother() != mom_cand.TrackId()) { 
		 grand_mother_trkid = mom_cand.Mother();
		 search = true;
	       }else{
		 trkid_to_grand_mother[mcp.TrackId()] = i;
		 if(grand_mother_to_res_index.find(i) == grand_mother_to_res_index.end()) {

		   auto& p = mgr.MCParticleGraph().CreateParticle();
		   p.SetParticleInfo(mom_cand.PdgCode(),
				     mom_cand.Mass()*1000.,
				     ::geoalgo::Vector(mom_cand.Trajectory()[0].Position()),
				     ::geoalgo::Vector(mom_cand.Trajectory()[0].Momentum())*1.e3 );
		   grand_mother_to_res_index[i]=p.ID();
		 }		
	       }	      
	       break;
	     }
	   }
	 }
       }

       // Now try matching of MCTruth => MCShower/MCTrack
       for(auto const& mcp : mci.GetParticles()) {

	 if(mcp.PdgCode()>2000000000) continue;
	 if(mcp.StatusCode() != 1) continue;

	 // Check if this particle exists
	 PartID_t mcp_id( ::geoalgo::Vector(mcp.Trajectory()[0].Position()),
			  ::geoalgo::Vector(mcp.Trajectory()[0].Momentum())*1.e3,
			  mcp.PdgCode() );
	 bool g4_mother_found = false;

	 for(auto const& mom_pair : g4_mother_id) {

	   if( mcp_id.same(mom_pair.second) && g4_mother_used.find(mom_pair.first) == g4_mother_used.end() ) {
	     //std::cout<<"Match!"<<std::endl;
	     // Found a matched g4 mother
	     g4_mother_found = true;

	     // See if there's a grand mother
	     if(trkid_to_grand_mother.find(mcp.TrackId())==trkid_to_grand_mother.end()) break;

	     auto const& grandMotherID = mgr.MCParticleGraph().GetParticle(grand_mother_to_res_index[trkid_to_grand_mother[mcp.TrackId()]]).ID();
	     auto const& thisID = g4_mother_parts[mom_pair.first].ID();
	     mgr.MCParticleGraph().SetParentage(grandMotherID,thisID);
	     //auto& grand_mother = particle_set[grand_mother_to_res_index[trkid_to_grand_mother[mcp.TrackId()]]];
	     //grand_mother.AddDaughter(g4_mother_parts[mom_pair.first]);
	     g4_mother_used.insert(mom_pair.first);
	     break;
	   }
	 }

	 if(!g4_mother_found) {
	   int pdg = mcp.PdgCode();

	   auto& p = mgr.MCParticleGraph().CreateParticle();
	   p.SetParticleInfo(pdg,mcp.Mass()*1000.,
			     ::geoalgo::Vector(mcp.Trajectory()[0].Position()),
			     ::geoalgo::Vector(mcp.Trajectory()[0].Momentum()*1.e3));
	   if(trkid_to_grand_mother.find(mcp.TrackId())!=trkid_to_grand_mother.end()){
	     auto& parentID = grand_mother_to_res_index[trkid_to_grand_mother[mcp.TrackId()]];
	     auto& childID  = p.ID();
	     mgr.MCParticleGraph().SetParentage(parentID,childID);
	   }
	   //::ertool::Particle p(pdg,mcp.Mass()*1000.);
	   //p.Vertex(::geoalgo::Vector(mcp.Trajectory()[0].Position()));
	   //p.Momentum(::geoalgo::Vector(mcp.Trajectory()[0].Momentum())*1.e3); //MCParticle units are converted into MEV here
	   //if(trkid_to_grand_mother.find(mcp.TrackId())==trkid_to_grand_mother.end())  
	   //  particle_set.push_back(p);
	   //else
	   //  particle_set[grand_mother_to_res_index[trkid_to_grand_mother[mcp.TrackId()]]].AddDaughter(p);	    
	 }
       }
     }
     /*
     for(auto const& mom_pair : g4_mother_parts) {
       if(g4_mother_used.find(mom_pair.first) == g4_mother_used.end())
	 particle_set.push_back(mom_pair.second);
     }
     */
     return;
  }

  void ERToolHelper::FillTracks ( const event_mctrack&  mct_v,
				  ::ertool::Manager&    mgr ) const
  {
    for(size_t i=0; i<mct_v.size(); ++i){

      auto const& mct = mct_v[i];
      if(mct.size()<2) continue;
      ::ertool::Track t;
      t.reserve(mct.size());
      for(auto const& step : mct)
	t += step.Position();

      //This is the TOTAL energy minus TOTAL energy, so mass is removed.
      //If you want only initial kinetic energy, remember to subtract off mass.
      t._energy     = (*mct.begin()).Momentum().E() - (*mct.rbegin()).Momentum().E();
      t._cosmogenic = (double)(mct.Origin() == simb::kCosmicRay);

      if(abs(mct.PdgCode()) == 13 ) t._pid = ::ertool::Track::kMuon;
      if(mct.PdgCode() == 2212    ) t._pid = ::ertool::Track::kProton;
      if(abs(mct.PdgCode()) == 321) t._pid = ::ertool::Track::kKaon;
      if(abs(mct.PdgCode()) == 211) t._pid = ::ertool::Track::kPion;
      for(auto& v : t._pid_score) v = 100;
      if(t._pid < t._pid_score.size()) t._pid_score[t._pid] = 0.1;

      auto nodeID = mgr.Add(t,ertool::RecoInputID_t(i,mct_v.name()),false);
      mgr.ParticleGraph().GetParticle(nodeID).SetParticleInfo(::ertool::kINVALID_INT,
							      ::ertool::kINVALID_INT,
							      mct.at(0).Position(),
							      ::geoalgo::Vector(mct.at(0).Momentum()));

    }

    return;
  }


  void ERToolHelper::FillTracks ( const event_track&  trk_v,
				  const event_cosmictag& ctag_trk_v,
				  const event_calorimetry& calo_trk_v,
				  const event_partid& pid_trk_v,
				  const event_ass& ass_v,
				  ::ertool::Manager& mgr) const
  {
    std::vector< ::ertool::Track> t_v;
    std::vector< ::ertool::RecoInputID_t > id_v;
    t_v.reserve(trk_v.size());
    id_v.reserve(trk_v.size());
    for(size_t i=0; i<trk_v.size(); ++i) {

      auto const& trk = trk_v[i];
      
      t_v.push_back(::ertool::Track());
      id_v.emplace_back(i,trk_v.name());
      auto &t = t_v.back();

      for(size_t i=0; i<trk.NumberTrajectoryPoints(); ++i)
	t += trk.LocationAtPoint(i);
      
      t._cosmogenic = -1;
      t._energy     = trk.MomentumAtPoint(0);

    }
    
    // Revise track cosmogenic score
    if (ctag_trk_v.size()){
      //auto const& ctag_trk_ass = ctag_trk_v.association(trk_v.id());
      auto const& ctag_trk_ass = ass_v.association(trk_v.id(),ctag_trk_v.id());
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
      //auto const& calo_trk_ass = calo_trk_v.association(trk_v.id());
      auto const& calo_trk_ass = ass_v.association(trk_v.id(),calo_trk_v.id()); 
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
      //auto const& pid_trk_ass = pid_trk_v.association(trk_v.id());
      auto const& pid_trk_ass = ass_v.association(trk_v.id(),pid_trk_v.id()); 
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

    for(size_t i=0; i<t_v.size(); ++i)
      mgr.Add(t_v[i],id_v[i]);
  }
  
  
  void ERToolHelper::FillShowers ( const event_mcshower& mcs_v,
				   ::ertool::Manager&    mgr ) const
  {

    for(size_t i=0; i<mcs_v.size(); ++i) {

      auto const& mcs = mcs_v[i];
      if(mcs.DetProfile().Momentum().E()<_minEDep) continue;
      //if(isnan(mcs.DetProfile().Momentum().E())) continue;
      //if(isnan(mcs.DetProfile().Momentum().Px())) continue;
      //if(mcs.DetProfile().Momentum().Mag2() == 0) continue;
      ::ertool::Shower s( mcs.DetProfile().Position(),
			  mcs.DetProfile().Momentum(),
			  _shrProfiler.Length( mcs.DetProfile().Momentum().E()),
			  _shrProfiler.ShowerRadius() );
      s._energy     = mcs.DetProfile().Momentum().E();
      //s._energy = mcs.Start().Momentum().E();
      s._dedx       = (mcs.PdgCode() == 22 ? gRandom->Gaus(4,4*0.05) : gRandom->Gaus(2,2*0.05));
      s._cosmogenic = (double)(mcs.Origin() == simb::kCosmicRay);
      auto nodeID = mgr.Add(s,ertool::RecoInputID_t(i,mcs_v.name()),false);
      mgr.ParticleGraph().GetParticle(nodeID).SetParticleInfo(::ertool::kINVALID_INT,
							      ::ertool::kINVALID_INT,
							      mcs.Start().Position(),
							      ::geoalgo::Vector(mcs.Start().Momentum()));
    }

    return;
  }

  // Single Shower Cheater:
  // Assumes a single shower is in the event
  // fills the particle information with
  // that of the mcshower
  void ERToolHelper::SingleShowerCheater ( const event_mcshower& mcs_v,
					   ::ertool::Manager&    mgr ) const
  {

    // if no mcshowers, do nothing
    if (!mcs_v.size())
      return;

    // if no particles, do nothing
    auto const& nodes = mgr.ParticleGraph().GetParticleNodes(::ertool::RecoType_t::kShower);
    if (!nodes.size())
      return;

    auto &start = mcs_v[0].Start();
    ::geoalgo::Point_t  vtx(start.X(),start.Y(),start.Z());
    ::geoalgo::Vector_t mom(start.Px(),start.Py(),start.Pz());
    auto const& pdg = mcs_v[0].PdgCode();
    mgr.ParticleGraph().GetParticle(nodes[0]).SetParticleInfo(pdg,
							      0.,
							      vtx,
							      mom);
    return;
  }
  

  void ERToolHelper::FillShowers(const event_shower& shw_v,
				 const event_cosmictag& ctag_shw_v,
				 const event_ass& ass_v,
				 ::ertool::Manager& mgr ) const
  {
    
    // Fill shower
    std::vector< ::ertool::Shower> s_v;
    std::vector< ::ertool::RecoInputID_t> id_v;
    s_v.reserve(shw_v.size());
    id_v.reserve(shw_v.size());
    for(size_t i=0; i<shw_v.size(); ++i){
      auto const& shw = shw_v[i];
      id_v.emplace_back(i,shw_v.name());
      s_v.push_back( ::ertool::Shower(shw.ShowerStart(),
				      shw.Direction(),
				      shw.Length(),
				      _shrProfiler.ShowerRadius()) );
      auto& s = (*s_v.rbegin());
      if(shw.best_plane()){
	s._energy = shw.Energy()[shw.best_plane()];
	s._dedx   = shw.dEdx()[shw.best_plane()];
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
      //auto const& ctag_shw_ass = ctag_shw_v.association(shw_v.id());
      auto const& ctag_shw_ass = ass_v.association(shw_v.id(),ctag_shw_v.id());
      if(ctag_shw_ass.size()) {
	for(size_t cos_index=0; cos_index<ctag_shw_v.size(); ++cos_index) {
	  
	  auto const& ctag = ctag_shw_v[cos_index];
	  for(auto const& shw_index : ctag_shw_ass[cos_index])  
	    s_v.at(shw_index)._cosmogenic = ctag.fCosmicScore;
	}
      }
    }

    for(size_t i=0; i<s_v.size(); ++i)
      mgr.Add(s_v[i],id_v[i]);

    return;
  }

}

#endif
