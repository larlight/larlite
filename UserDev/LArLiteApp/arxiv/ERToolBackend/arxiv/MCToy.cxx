#ifndef MCTOY_CXX
#define MCTOY_CXX

#include "MCToy.h"
#include "ParticleID.h"

namespace ertool {

  void MCToy::FillMCInfo( const event_mctruth&   mci_v,
			  const event_mcshower&  mcs_v,
			  const event_mctrack&   mct_v,
			  ::ertool::io::EmptyInput& strm) const
  {

    std::map< ::ertool_helper::ParticleID_t, NodeID_t > part_list;

    auto& graph = strm.GetParticleGraphWriteable(true);
    
    //
    // Step 1 ... Register ALL MCShower
    //
    for(auto const& mcs : mcs_v) {

      // Create ertool::Shower
      ::ertool::Shower s( mcs.DetProfile().Position(),
			  mcs.DetProfile().Momentum(),
			  _shrProfiler.Length( mcs.DetProfile().Momentum().E()),
			  _shrProfiler.ShowerRadius() );
      // Fill more info
      s._energy     = mcs.DetProfile().Momentum().E();
      s._dedx       = (mcs.PdgCode() == 22 ? gRandom->Gaus(4,4*0.03) : gRandom->Gaus(2,2*0.03));
      s._cosmogenic = (double)(mcs.Origin() == simb::kCosmicRay);

      // Emplace this shower, and get corresponding particle unique ID
      auto nodeID = strm.Emplace( s, ertool::RecoInputID_t(i,mcs_v.name()), true );

      // Edit particle info
      auto& p = graph.GetParticle(nodeID);

      p.SetParticleInfo(mcs.PdgCode(),
			::ertool::ParticleMass(pdg),
			mcs.Start().Position(),
			mcs.Start().Momentum());

      // Create ParticleID
      ::ertool_helper::ParticleID id(mcs);

      // Check if this ID already exists ... it should not, so throw an exception if it does
      if( part_list.find(id) != part_list.end() )

	throw ERException("Duplicate particle found in MCShower filling loop!");

      // Assign ParticleID <=> NodeID map value
      part_list[id] = nodeID;
    }

    //
    // Step 2 ... Register ALL MCTrack
    //
    for(auto const& mct : mct_v) {

      // Create ertool::Track
      ::ertool::Track t;

      // Fill track info
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

      // Emplace a track to EventData
      auto nodeID = strm.Emplace(t,ertool::RecoInputID_t(i,mct_v.name()),true);

      // Edit particle info
      auto& p = graph.GetParticle(nodeID);

      p.SetParticleInfo(mct.PdgCode(),
			::ertool::ParticleMass(pdg),
			mct.at(0).Position(),
			::geoalgo::Vector(mct.at(0).Momentum()));
      // Create ParticleID
      ::ertool_helper::ParticleID id(mct);

      // Check if this ID already exists ... it should not, so throw an exception if it does
      if( part_list.find(id) != part_list.end() )

	throw ERException("Duplicate particle found in MCTrack filling loop!");

      // Assign ParticleID <=> NodeID map value
      part_list[id] = nodeID;      
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
	}
	
	if(mcp.StatusCode() != 1) continue;

	//
	// Step 3.1 ... Make status 0=>1 parentage map 
	//
	bool search = true;
	unsigned int grand_parent_track_id = mcp.TrackId();
	while(search) {
	  search = false;
	  auto const& mcp_list = mci.GetParticles();
	  for(size_t i=0; i<mcp_list.size(); ++i) {
	    auto const& mom_cand = mcp_list[i];
	    if(mom_cand.TrackId() == grand_parent_track_id) {
	      // Found a grand parent particle
	      ::ertool_helper::ParticleID grand_parent_id(mom_cand);
	      grand_parent_map[id] = grand_parent_id;

	      // See if further search is needed
	      if(mom_cand.Mother() != -1 && 
		 mom_cand.Mother() != mom_cand.TrackId()) { 
		grand_parent_track_id = mom_cand.Mother();
		search = true;
	       }
	      break;
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
	
	auto parent_iter = part_list.find(parent_id);
	auto child_iter  = part_list.find(child_id);
	
	if( parent_iter == part_list.end() || child_iter == part_list.end() )
	  
	  throw ERException("Missing parent/child relationship within MCTruth!");
	
	graph.SetParentage( (*parent_iter).second, (*child_iter).second );
	
      } // finish looping over mctruth-internal particle set
    } // finish looping over MCTruth
    
    //
    // Step 4 ... MCX parentage 
    //
    // First MCShower Parentage
    for(auto const& mcs : mcs_v) {

      // This particle's ID
      ParticleID id( mcs.PdgCode(),
		     mcs.Start().Position(),
		     mcs.Start().Momentum() );

      // Construct parent's ID
      ParticleID parent_id( mcs.MotherPdgCode(),
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
      graph.SetParentage( part_list[parent_id], part_list[id] );

    }

    // First MCTrack Parentage
    for(auto const& mct : mct_v) {

      // This particle's ID
      ParticleID id( mct.PdgCode(),
		     mct.Start().Position(),
		     mct.Start().Momentum() );

      // Construct parent's ID
      ParticleID parent_id( mct.MotherPdgCode(),
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
      graph.SetParentage( part_list[parent_id], part_list[id] );

    }

    //
    // Step 5 ... MCX grandpa and grandma 
    //
    // MCShower
    for(auto const& mcs : mcs_v) {

      // Construct parent's ID
      ParticleID parent_id( mcs.MotherPdgCode(),
			    mcs.MotherStart().Position(),
			    mcs.MotherStart().Momentum() );
      
      // Construct ancestor's ID
      ParticleID ancestor_id( mcs.AncestorPdgCode(),
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

      //
      // 5.1 ... Check if a parent exists as an ancestor's child
      //
      auto const& parent_node   = _part_list[ parent_id   ];
      auto const& ancestor_node = _part_list[ ancestor_id ];
      // If parent's parent is not ancestor, insert particle X in between... (for details, ask Andrzej)
      if( graph.GetParticle(parent_node).Parent() != ancestor_node ) {
	// Create X
	auto& particle_x = graph.CreateParticle();
	// Put X in between parentage
	graph.SetParentage( ancestor_node,   particle_x.ID() );
	graph.SetParentage( particle_x.ID(), parent_node     );
      }
    }
    // MCTrack
    for(auto const& mct : mct_v) {

      // Construct parent's ID
      ParticleID parent_id( mct.MotherPdgCode(),
			    mct.MotherStart().Position(),
			    mct.MotherStart().Momentum() );
      
      // Construct ancestor's ID
      ParticleID ancestor_id( mct.AncestorPdgCode(),
			      mct.AncestorStart().Position(),
			      mct.AncestorStart().Momentum() );

      // If parent not found, register
      if(part_list.find(ancestor_id) == part_list.end()) {
	auto& p = graph.CreateParticle();
	p.SetParticleInfo( mct.AncestorPdgCode(),
			   ::ertool::ParticleMass(mct.AncestorPdgCode()),
			   ::geoalgo::Vector(mct.AncestorStart().Position()),
			   ::geoalgo::Vector(mct.AncestorStart().Momentum()) );
	
	part_list[ancestor_id] = p.ID();
      }

      //
      // 5.1 ... Check if a parent exists as an ancestor's child
      //
      auto const& parent_node   = _part_list[ parent_id   ];
      auto const& ancestor_node = _part_list[ ancestor_id ];
      // If parent's parent is not ancestor, insert particle X in between... (for details, ask Andrzej)
      if( graph.GetParticle(parent_node).Parent() != ancestor_node ) {
	// Create X
	auto& particle_x = graph.CreateParticle();
	// Put X in between parentage
	graph.SetParentage( ancestor_node,   particle_x.ID() );
	graph.SetParentage( particle_x.ID(), parent_node     );
      }
    }
  }
  
}
#endif
