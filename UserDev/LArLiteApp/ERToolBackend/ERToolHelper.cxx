#ifndef LARLITE_ERTOOLHELPER_CXX
#define LARLITE_ERTOOLHELPER_CXX

#include "DataFormat/mctruth.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/storage_manager.h"
#include "DataFormat/shower.h"
#include "DataFormat/track.h"
#include "DataFormat/vertex.h"
#include "DataFormat/cosmictag.h"
#include "DataFormat/calorimetry.h"
#include "DataFormat/partid.h"
#include "DataFormat/event_ass.h"

#include "ParticleID.h"
#include "ERToolHelper.h"
#include "ERTool/Base/UtilFunc.h"
#include "ERTool/Base/EmptyInput.h"
#include <limits>
#include <climits>

namespace larlite {

  void ERToolHelper::FillMCInfo( const event_mctruth&   mci_v,
				 const event_mcshower&  mcs_v,
				 const event_mctrack&   mct_v,
				 ::ertool::io::EmptyInput& strm) const
  {

    std::map< ::ertool_helper::ParticleID, ::ertool::NodeID_t > part_list;

    auto& graph = strm.GetParticleGraphWriteable(true);
    
    //
    // Step 1 ... Register ALL MCShower
    //
    for(size_t i=0; i<mcs_v.size(); ++i) {

      auto const& mcs = mcs_v[i];

      ::ertool::NodeID_t nodeID;

      // If energy is above threshold, create ertool::Shower
      if(mcs.DetProfile().Momentum().E() >= _minEDep) {
	::ertool::Shower s( (mcs.DetProfile().Position() + getXShift(mcs_v[i])),
			    mcs.DetProfile().Momentum(),
			    _shrProfiler.Length( mcs.DetProfile().Momentum().E()),
			    _shrProfiler.ShowerRadius() );
	// Fill more info
	s._energy     = mcs.DetProfile().Momentum().E();
	s._dedx       = (mcs.PdgCode() == 22 ? gRandom->Gaus(4,4*0.03) : gRandom->Gaus(2,2*0.03));
	s._cosmogenic = (double)(mcs.Origin() == simb::kCosmicRay);
	
	::ertool::RecoInputID_t in_id(i,mcs_v.name());
	
	// Emplace this shower, and get corresponding particle unique ID
	//nodeID = strm.Emplace( std::move(s), std::move(in_id), true );
	nodeID = strm.Add( s, in_id, true );

      }else

	nodeID = graph.CreateParticle().ID();

      // Edit particle info
      auto& p = graph.GetParticle(nodeID);

      p.SetParticleInfo(mcs.PdgCode(),
			::ertool::ParticleMass(mcs.PdgCode()),
			mcs.Start().Position(),
			mcs.Start().Momentum());

      // Create ParticleID
      ::ertool_helper::ParticleID id(mcs);

      // Check if this ID already exists ... it should not, so throw an exception if it does
      if( part_list.find(id) != part_list.end() )

	throw ::ertool::ERException("Duplicate particle found in MCShower filling loop!");

      // Assign ParticleID <=> NodeID map value
      part_list[id] = nodeID;
    }

    //
    // Step 2 ... Register ALL MCTrack
    //
    for(size_t i=0; i<mct_v.size(); ++i) {

      auto const& mct = mct_v[i];
      // Create ertool::Track
      ::ertool::Track t;
      // Fill track info
      t.reserve(mct.size());
      TLorentzVector shift = getXShift(mct);
      for(auto const& step : mct) 
	t += (step.Position() + shift);;

      ::ertool::NodeID_t nodeID;
      // Check minimum number of physical steps
      if( t.size() >= 2 ) {

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
	
	::ertool::RecoInputID_t in_id(i,mct_v.name());
	
	// Emplace a track to EventData
	//nodeID = strm.Emplace(std::move(t),std::move(in_id),true);
	nodeID = strm.Add( t, in_id, true);

      }
      
      // if the track has only a single step point
      else
	continue;
      //nodeID = graph.CreateParticle().ID();
      
      // Edit particle info
      auto& p = graph.GetParticle(nodeID);

      p.SetParticleInfo(mct.PdgCode(),
			::ertool::ParticleMass(mct.PdgCode()),
			::geoalgo::Vector(mct.Start().Position()),
			::geoalgo::Vector(mct.Start().Momentum()));

      // Create ParticleID
      ::ertool_helper::ParticleID id(mct);

      // Check if this ID already exists ... it should not, so throw an exception if it does
      if( part_list.find(id) != part_list.end() )

	throw ::ertool::ERException("Duplicate particle found in MCTrack filling loop!");

      // Assign ParticleID <=> NodeID map value
      part_list[id] = nodeID;
      /*
      std::cout<<"New Particle..."<<std::endl
	       <<"PdgCode  : "<<id.PdgCode()<<std::endl
	       <<"Position : "<<id.Position().X() << " : " <<id.Position().Y() << " : " <<id.Position().Z() << std::endl
	       <<"Momentum : "<<id.Momentum().X() << " : " <<id.Momentum().Y() << " : " <<id.Momentum().Z() << std::endl
	       <<std::endl;
      */
    }
    
    //
    // Step 3 ... MCTruth hell
    //
    for(auto const& mci : mci_v) {

      // Create MCTruth particle ParticleID => ParticleID mapping for step 3.1
      std::map< ::ertool_helper::ParticleID,
		::ertool_helper::ParticleID > grand_parent_map;
      
      //
      // Step 3.0 ... loop over MCParticles and add to the list if not yet created
      //
      for(auto const& mcp : mci.GetParticles()) {

	// Skip if this particle has status code neither 0 nor 1
	if(mcp.StatusCode() != 0 && mcp.StatusCode() != 1) continue;

	// Construct an ID
	::ertool_helper::ParticleID id(mcp);
	
	// Check if this ID is already dispatched (=> then we don't care)
	if(part_list.find(id) == part_list.end()) {
	  // Create a new particle & fill information
	  auto& p = graph.CreateParticle();
	  p.SetParticleInfo( mcp.PdgCode(),
			     mcp.Mass()*1000.,
			     ::geoalgo::Vector(mcp.Trajectory()[0].Position()),
			     ::geoalgo::Vector(mcp.Trajectory()[0].Momentum())*1.e3 );

	  part_list[id] = p.ID();
	  /*
	  std::cout<<"New Particle..."<<std::endl
		   <<"PdgCode  : "<<id.PdgCode()<<std::endl
		   <<"Position : "<<id.Position().X() << " : " <<id.Position().Y() << " : " <<id.Position().Z() << std::endl
		   <<"Momentum : "<<id.Momentum().X() << " : " <<id.Momentum().Y() << " : " <<id.Momentum().Z() << std::endl
		   <<std::endl;
	  */
	}
	
	if(mcp.StatusCode() != 1) continue;

	//
	// Step 3.1 ... Make status 0=>1 parentage map 
	//
	bool search = true;
	int mct_parent_id = mcp.TrackId();
	while(search) {
	  search = false;
	  for(auto const& mcp_parent : mci.GetParticles()) {
	    if(mcp_parent.TrackId() == mct_parent_id) {
	      ::ertool_helper::ParticleID grand_parent_id(mcp_parent);
	      grand_parent_map[id] = grand_parent_id;
	      search = true;
	      if(mct_parent_id == mcp_parent.Mother())
		search = false;
	      mct_parent_id = mcp_parent.Mother();
	    }
	  }
	} // Finish finding a parentage within mctruth
      } // Finish looping over particles in mctruth
      
      // Is check needed for 3.1? Loop over grand_parent_map values and see if all exist in part_list
      // not done right now...
      
      //
      // Step 3.2 ... assign parentage within MCTruth
      //
      for(auto const& id_pair : grand_parent_map) {
	
	auto const& child_id  = id_pair.first;
	auto const& parent_id = id_pair.second;

	auto child_iter  = part_list.find( child_id  );

	if( part_list.find(parent_id) == part_list.end() ) {
	  auto& p = graph.CreateParticle();
	  p.SetParticleInfo( parent_id.PdgCode(),
			     ::ertool::ParticleMass(parent_id.PdgCode()),
			     ::geoalgo::Vector(parent_id.Position()),
			     ::geoalgo::Vector(parent_id.Momentum()) );
	  part_list[parent_id] = p.ID();
	}
	auto parent_iter = part_list.find( parent_id );
	
	if( parent_iter == part_list.end() || child_iter == part_list.end() )
	  
	  throw ::ertool::ERException("Missing parent/child relationship within MCTruth!");

	if((*parent_iter).second != (*child_iter).second)
	  graph.SetParentage( (*parent_iter).second, (*child_iter).second );
	
      } // finish looping over mctruth-internal particle set

    } // finish looping over MCTruth
    
    //
    // Step 4 ... MCX parentage 
    //
    // First MCShower Parentage
    for(auto const& mcs : mcs_v) {

      // This particle's ID
      ::ertool_helper::ParticleID id( mcs.PdgCode(),
				      mcs.Start().Position(),
				      mcs.Start().Momentum() );

      // Construct parent's ID
      ::ertool_helper::ParticleID parent_id( mcs.MotherPdgCode(),
					     mcs.MotherStart().Position(),
					     mcs.MotherStart().Momentum() );

      // If parent not found, register
      if(part_list.find(parent_id) == part_list.end()) {
	auto& p = graph.CreateParticle();
	p.SetParticleInfo( mcs.MotherPdgCode(),
			   ::ertool::ParticleMass(mcs.MotherPdgCode()),
			   ::geoalgo::Vector(mcs.MotherStart().Position()),
			   ::geoalgo::Vector(mcs.MotherStart().Momentum()) );
	
	part_list[parent_id] = p.ID();
      }
      // Set parentage
      auto const& parent_node = part_list[parent_id];
      auto const& node = part_list[id];
      if(parent_node != node)
	graph.SetParentage( parent_node, node );
    }

    // First MCTrack Parentage
    for(auto const& mct : mct_v) {

      if (mct.size() < 2)
	continue;

      // This particle's ID
      ::ertool_helper::ParticleID id( mct.PdgCode(),
				      mct.Start().Position(),
				      mct.Start().Momentum() );

      // Construct parent's ID
      ::ertool_helper::ParticleID parent_id( mct.MotherPdgCode(),
					     mct.MotherStart().Position(),
					     mct.MotherStart().Momentum() );

      // If parent not found, register
      if(part_list.find(parent_id) == part_list.end()) {
	auto& p = graph.CreateParticle();
	p.SetParticleInfo( mct.MotherPdgCode(),
			   ::ertool::ParticleMass(mct.MotherPdgCode()),
			   ::geoalgo::Vector(mct.MotherStart().Position()),
			   ::geoalgo::Vector(mct.MotherStart().Momentum()) );
	
	part_list[parent_id] = p.ID();
      }
      // Set parentage
      auto const& parent_node = part_list[parent_id];
      auto const& node = part_list[id];
      if(parent_node != node)
	graph.SetParentage( parent_node, node );
    }

    //
    // Step 5 ... MCX grandpa and grandma 
    //
    // MCShower
    for(auto const& mcs : mcs_v) {
      continue;

      // Construct parent's ID
      ::ertool_helper::ParticleID parent_id( mcs.MotherPdgCode(),
					     mcs.MotherStart().Position(),
					     mcs.MotherStart().Momentum() );

      // If parent has other parents, ancestor pathway is done. Continue.
      auto const& parent_node = part_list[ parent_id ];
      auto const& parent = graph.GetParticle(parent_node);
      if(parent.Parent() != parent.ID()) continue;
      
      // Construct ancestor's ID
      ::ertool_helper::ParticleID ancestor_id( mcs.AncestorPdgCode(),
					       mcs.AncestorStart().Position(),
					       mcs.AncestorStart().Momentum() );
      // If parent not found, register
      if(part_list.find(ancestor_id) == part_list.end()) {
	auto& p = graph.CreateParticle();
	p.SetParticleInfo( mcs.AncestorPdgCode(),
			   ::ertool::ParticleMass(mcs.AncestorPdgCode()),
			   ::geoalgo::Vector(mcs.AncestorStart().Position()),
			   ::geoalgo::Vector(mcs.AncestorStart().Momentum()) );
	
	part_list[ancestor_id] = p.ID();
      }
      auto const& ancestor_node = part_list[ ancestor_id ];
      
      // Create X
      auto& particle_x = graph.CreateParticle();
      // Put X in between parentage
      try{
	graph.SetParentage( ancestor_node,   particle_x.ID() );
	graph.SetParentage( particle_x.ID(), parent_node     );
      }catch( ::ertool::ERException& e ){
	std::cout << std::endl
		  << "Caught exception! Current particle info shown below..." << std::endl
		  << "MCShower: " << mcs.PdgCode()
		  << " ... " << mcs.Start().X() << " : " << mcs.Start().Y() << " : " << mcs.Start().Z() << " : "
		  << mcs.Start().E() <<std::endl
		  << "Parent: " << mcs.MotherPdgCode()
		  << " ... " << mcs.MotherStart().X() << " : " << mcs.MotherStart().Y() << " : " << mcs.MotherStart().Z() << " : "
		  << mcs.MotherStart().E() <<std::endl
		  << "Ancestor: " << mcs.AncestorPdgCode()
		  << " ... " << mcs.AncestorStart().X() << " : " << mcs.AncestorStart().Y() << " : " << mcs.AncestorStart().Z() << " : "
		  << mcs.AncestorStart().E() <<std::endl
		  << "Re-throwing..." << std::endl
		  << std::endl;
	throw e;
      }
    }
    // MCTrack
    for(auto const& mct : mct_v) {

      // Construct parent's ID
      ::ertool_helper::ParticleID parent_id( mct.MotherPdgCode(),
					     mct.MotherStart().Position(),
					     mct.MotherStart().Momentum() );
      
      // If parent has other parents, ancestor pathway is done. Continue.
      auto const& parent_node = part_list[ parent_id ];
      auto const& parent = graph.GetParticle(parent_node);
      if(parent.Parent() != parent.ID()) continue;

      // Construct ancestor's ID
      ::ertool_helper::ParticleID ancestor_id( mct.AncestorPdgCode(),
					       mct.AncestorStart().Position(),
					       mct.AncestorStart().Momentum() );

      if(parent_id == ancestor_id) continue;
      
      // If ancestor not found, register
      if(part_list.find(ancestor_id) == part_list.end()) {
	auto& p = graph.CreateParticle();
	p.SetParticleInfo( mct.AncestorPdgCode(),
			   ::ertool::ParticleMass(mct.AncestorPdgCode()),
			   ::geoalgo::Vector(mct.AncestorStart().Position()),
			   ::geoalgo::Vector(mct.AncestorStart().Momentum()) );
	
	part_list[ancestor_id] = p.ID();
      }
      auto const& ancestor_node = part_list[ ancestor_id ];
      
      // Create X
      auto& particle_x = graph.CreateParticle();
      // Put X in between parentage
      try{
	graph.SetParentage( ancestor_node,   particle_x.ID() );
	graph.SetParentage( particle_x.ID(), parent_node     );
      }catch( ::ertool::ERException& e) {
	std::cout << std::endl
		  << "Caught exception! Current particle info shown below..." << std::endl
		  << "MCTrack: " << mct.PdgCode()
		  << " ... " << mct.Start().X() << " : " << mct.Start().Y() << " : " << mct.Start().Z() << " : "
		  << mct.Start().E() <<std::endl
		  << "Parent: " << mct.MotherPdgCode()
		  << " ... " << mct.MotherStart().X() << " : " << mct.MotherStart().Y() << " : " << mct.MotherStart().Z() << " : "
		  << mct.MotherStart().E() <<std::endl
		  << "Ancestor: " << mct.AncestorPdgCode()
		  << " ... " << mct.AncestorStart().X() << " : " << mct.AncestorStart().Y() << " : " << mct.AncestorStart().Z() << " : "
		  << mct.AncestorStart().E() <<std::endl
		  << "Re-throwing..." << std::endl
		  << std::endl;
	throw e;
      }
    }
  }

  void ERToolHelper::FillTracks ( const event_mctrack&  mct_v,
				  ::ertool::io::EmptyInput& strm ) const
  {
    //SetDetWidth();
    for(size_t i=0; i<mct_v.size(); ++i){

      auto const& mct = mct_v[i];
      
      ::ertool::Track t;
      t.reserve(mct.size());
      TLorentzVector shift = getXShift(mct);
      for(auto const& step : mct)
    	  t += (step.Position() + shift);;

      if(t.size()<2) continue;
      
      //This is the TOTAL energy minus TOTAL energy, so mass is removed.
      //If you want only initial kinetic energy, remember to subtract off mass.
      t._energy     = (*mct.begin()).Momentum().E() - (*mct.rbegin()).Momentum().E();
      t._cosmogenic = (double)(mct.Origin() == simb::kCosmicRay);
      t._time = mct_v[i].End().T();
      
      if(abs(mct.PdgCode()) == 13 ) t._pid = ::ertool::Track::kMuon;
      if(mct.PdgCode() == 2212    ) t._pid = ::ertool::Track::kProton;
      if(abs(mct.PdgCode()) == 321) t._pid = ::ertool::Track::kKaon;
      if(abs(mct.PdgCode()) == 211) t._pid = ::ertool::Track::kPion;
      for(auto& v : t._pid_score) v = 100;
      if(t._pid < t._pid_score.size()) t._pid_score[t._pid] = 0.1;
      
      strm.Add(t,ertool::RecoInputID_t(i,mct_v.name()),false);
      /*
      strm.GetParticleGraphWriteable().GetParticle(nodeID).SetParticleInfo(mct.PdgCode(),
      ::ertool::ParticleMass(mct.PdgCode()),
      mct.at(0).Position(),
      ::geoalgo::Vector(mct.at(0).Momentum()));
      */
    }

    return;
  }

  void ERToolHelper::FillTracks ( const event_track&  trk_v,
				  storage_manager& storage,
				  ::ertool::io::EmptyInput& strm) const
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

    event_cosmictag* ctag_trk_v = nullptr;
    auto const& ctag_trk_ass = storage.find_one_ass(trk_v.id(), ctag_trk_v, Form("%stag",trk_v.name().c_str()));

    event_calorimetry* calo_trk_v = nullptr;
    auto const& calo_trk_ass = storage.find_one_ass(trk_v.id(), calo_trk_v, Form("%scalo",trk_v.name().c_str()));

    event_partid* pid_trk_v = nullptr;
    auto const& pid_trk_ass = storage.find_one_ass(trk_v.id(), pid_trk_v, Form("%spid",trk_v.name().c_str()));

    for(size_t t_index=0; t_index < id_v.size(); ++t_index) {

      auto  trk_index = id_v[t_index];
      auto& t = t_v[t_index];
      if(ctag_trk_v) {
	for(auto const& ctag_index : ctag_trk_ass[t_index])
	  t._cosmogenic = (*ctag_trk_v)[ctag_index].fCosmicScore;
      }

      if(calo_trk_v) {
	for(auto const& calo_index : calo_trk_ass[t_index])
	  t._energy = (*calo_trk_v)[calo_index].KineticEnergy();
      }

      if(pid_trk_v) {
	for(auto const& pid_index : pid_trk_ass[t_index]) {

	  auto const &pid = (*pid_trk_v)[pid_index];

	  std::map<double,::ertool::Track::TrackPartID_t> score_map;
	  score_map.insert(std::make_pair(pid.Chi2Proton(),::ertool::Track::kProton));
	  score_map.insert(std::make_pair(pid.Chi2Kaon(),::ertool::Track::kKaon));
	  score_map.insert(std::make_pair(pid.Chi2Pion(),::ertool::Track::kPion));
	  score_map.insert(std::make_pair(pid.Chi2Muon(),::ertool::Track::kMuon));
	  for(auto const& score_pid : score_map)
	    t._pid_score[score_pid.second] = score_pid.first;
	  t._pid = (*score_map.begin()).second;
	  t._pid_score[::ertool::Track::kPIDA] = pid.PIDA();
	}
      }
      
    }
    
    for(size_t i=0; i<t_v.size(); ++i)
      strm.Add(t_v[i],id_v[i]);
  }
  
  
  void ERToolHelper::FillShowers ( const event_mcshower& mcs_v,
				   ::ertool::io::EmptyInput& strm) const
  {

    for(size_t i=0; i<mcs_v.size(); ++i) {

      auto const& mcs = mcs_v[i];
      if(mcs.DetProfile().Momentum().E()<_minEDep) continue;
      //if(isnan(mcs.DetProfile().Momentum().E())) continue;
      //if(isnan(mcs.DetProfile().Momentum().Px())) continue;
      //if(mcs.DetProfile().Momentum().Mag2() == 0) continue;
      ::ertool::Shower s( (mcs.DetProfile().Position() + getXShift(mcs_v[i])),
			  mcs.DetProfile().Momentum(),
			  _shrProfiler.Length( mcs.DetProfile().Momentum().E()),
			  _shrProfiler.ShowerRadius() );
      s._energy     = mcs.DetProfile().Momentum().E();
      //s._energy = mcs.Start().Momentum().E();
      s._dedx       = (mcs.PdgCode() == 22 ? gRandom->Gaus(4,4*0.05) : gRandom->Gaus(2,2*0.05));
      s._cosmogenic = (double)(mcs.Origin() == simb::kCosmicRay);
      s._time = mcs_v[i].End().T();
      double mass = 0;
      if (mcs.PdgCode() == 11) { mass = 0.511; }
      strm.Add(s,ertool::RecoInputID_t(i,mcs_v.name()),false);
      /*
      mgr.ParticleGraph().GetParticle(nodeID).SetParticleInfo(mcs.PdgCode(),
							      mass,
							      mcs.Start().Position(),
							      ::geoalgo::Vector(mcs.Start().Momentum()));
      */
    }

    return;
  }

  // Single Shower Cheater:
  // Assumes a single shower is in the event
  // fills the particle information with
  // that of the mcshower
  void ERToolHelper::SingleShowerCheater ( const event_mcshower& mcs_v,
					   ::ertool::io::EmptyInput& strm ) const
  {

    // if no mcshowers, do nothing
    if (!mcs_v.size())
      return;

    // if no particles, do nothing
    auto const& nodes = strm.GetParticleGraphWriteable().GetParticleNodes(::ertool::RecoType_t::kShower);
    if (!nodes.size())
      return;

    auto &start = mcs_v[0].Start();
    ::geoalgo::Point_t  vtx(start.X(),start.Y(),start.Z());
    ::geoalgo::Vector_t mom(start.Px(),start.Py(),start.Pz());
    auto const& pdg = mcs_v[0].PdgCode();
    strm.GetParticleGraphWriteable().GetParticle(nodes[0]).SetParticleInfo(pdg,
								      0.,
								      vtx,
								      mom);
    return;
  }
  

  void ERToolHelper::FillShowers(const event_shower& shw_v,
				 storage_manager& storage,
				 ::ertool::io::EmptyInput& strm ) const
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
    
    event_cosmictag* ctag_shw_v = nullptr;
    auto const& ctag_shw_ass = storage.find_one_ass(shw_v.id(), ctag_shw_v, Form("%scalo",shw_v.name().c_str()));
    
    // Revise shower cosmogenic score
    // make sure ctag_shr_v is not empty
    if ( ctag_shw_v ){
      for(size_t s_index=0; s_index < s_v.size(); ++s_index) {

	auto const& shw_index = id_v[s_index].first;

	for(auto const& ctag_index : ctag_shw_ass[shw_index])

	  s_v[s_index]._cosmogenic = (*ctag_shw_v)[ctag_index].fCosmicScore;

      }
    }

    for(size_t i=0; i<s_v.size(); ++i)
      strm.Add(s_v[i],id_v[i]);

    return;
  }

  TLorentzVector ERToolHelper::getXShift(const mctrack& mct) const {
    
    TLorentzVector shift;
    double event_time = mct.End().T();
    double shift_x = (event_time / _DetFramePeriod) * _DetWidth;
    shift.SetXYZT(shift_x, 0., 0., 0.);
    
    return shift;
  }
  
  TLorentzVector ERToolHelper::getXShift(const mcshower& mcs) const {
    // Calculates for each mc shower, based on the time of the event, the corresponding shift in x-direction
    TLorentzVector shift;
    double event_time = mcs.End().T();
    double shift_x = (event_time / _DetFramePeriod) * _DetWidth;
    shift.SetXYZT(shift_x, 0., 0., 0.);
    
    return shift;
  }
}

#endif
