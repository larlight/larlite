#ifndef ERTOOL_PARTICLEGRAPH_CXX
#define ERTOOL_PARTICLEGRAPH_CXX

#include "ParticleGraph.h"
#include <iostream>
#include "ERException.h"
#include "UtilFunc.h"
namespace ertool {

  void ParticleGraph::ValidNode(const NodeID_t& id) const
  {
    if(id >= _particle_v.size()) throw ERException("Invalid particle ID!");
  }
  
  Particle& ParticleGraph::CreateParticle()
  {
    _particle_v.emplace_back(_particle_v.size());
    return _particle_v.back();
  }

  Particle& ParticleGraph::CreateParticle(const RecoObjBase& reco_obj)
  {
    _particle_v.emplace_back( (NodeID_t)(_particle_v.size()),
			      reco_obj.RecoType(),
			      reco_obj.RecoID() );
    return _particle_v.back();
  }
  
  Particle& ParticleGraph::GetParticle(const NodeID_t id)
  {
    ValidNode(id);
    return _particle_v[id];
  }

  const Particle& ParticleGraph::GetParticle(const NodeID_t id) const
  {
    ValidNode(id);
    return _particle_v[id];
  }
  
  const std::deque< ::ertool::Particle>& ParticleGraph::GetParticleArray() const
  {
    return _particle_v;
  }

  size_t ParticleGraph::GetNumParticles() const
  {
    return _particle_v.size();
  }

  size_t ParticleGraph::GetNumPrimaries() const
  {
    size_t ctr=0;
    for(auto const& p : _particle_v) if(p.Primary()) ctr+=1;
    return ctr;
  }
  
  const std::vector<NodeID_t> ParticleGraph::GetAllDescendants(const NodeID_t target) const
  {
    auto const& p = GetParticle(target);
    std::vector<NodeID_t> res;
    for(auto const& c : p.Children()) {
      res.push_back(c);
      auto tmp_res = GetAllDescendants(c);
      res.reserve(res.size() + tmp_res.size());
      for(auto const& id : tmp_res) res.push_back(id);
    }
    return res;
  }

  const std::vector<NodeID_t> ParticleGraph::GetSiblings(const NodeID_t target) const
  {
    auto const& p = GetParticle(target);
    std::vector<NodeID_t> res;
    if(p.Parent() == p.ID()) return res;
    auto const& parent = GetParticle(p.Parent());
    for(auto const& id : parent.Children()) {
      if(id == target) continue;
      res.push_back(id);
    }
    return res;
  }
  
  const std::vector<NodeID_t> ParticleGraph::GetParticles ( const RecoType_t type,
							    const bool unassessed_only,
							    const int pdg_code) const
  {
    std::vector<NodeID_t> res;
    res.reserve(_particle_v.size());
    for(auto const& p : _particle_v) {

      if( (type == kINVALID_RECO_TYPE || p.RecoType() == type) &&
	  (!unassessed_only || p.RelationAssessed()) &&
	  (pdg_code == 0 || pdg_code == p.PdgCode()) )

	res.push_back(p.ID());
    }
    return res;
  }

  const std::vector<NodeID_t> ParticleGraph::GetPrimaries ( const RecoType_t type,
							    const bool unassessed_only,
							    const int pdg_code) const
  {
    std::vector<NodeID_t> res;
    res.reserve(_particle_v.size());
    for(auto const& p : _particle_v) {

      if( (type == kINVALID_RECO_TYPE || p.RecoType() == type) &&
	  (!unassessed_only || p.RelationAssessed()) &&
	  (pdg_code == 0 || pdg_code == p.PdgCode()) &&
	  p.Primary())

	res.push_back(p.ID());
    }
    return res;
  }

  const CombinationSet_t ParticleGraph::GetCombinations ( const size_t combination_size,
							  const RecoType_t type,
							  const bool unassessed_only,
							  const int pdg_code) const
  {
    CombinationSet_t res;
    if(combination_size < _particle_v.size()) return res;
    
    auto const node_v = GetParticles(type,unassessed_only,pdg_code);
    if(combination_size < node_v.size()) return res;
    
    auto const comb_v = Combination(node_v.size(),combination_size);
    for(auto const& comb : comb_v) {
      Combination_t node_comb;
      node_comb.reserve(combination_size);
      for(auto const& id : comb)
	node_comb.push_back(node_v[id]);
      res.emplace_back(node_comb);
    }
    return res;
  }
  
  void ParticleGraph::SameParticle(NodeID_t a_id, NodeID_t b_id)
  {
    if(a_id==b_id) return;
    ValidNode(a_id);
    ValidNode(b_id);

    // Make sure a_id is smaller: we merge b's info into a.
    if(a_id > b_id) std::swap(a_id,b_id);

    auto& part_a = _particle_v[a_id];
    auto& part_b = _particle_v[b_id];

    //
    // Check cases where this operation is prohibited
    //

    // 0) Check parentage ... OK only if
    //                        0-a) neither has a parent
    //                        0-b) only one of them have a parent
    //                        0-c) shares the same parent
    if( part_a.Parent() != part_a.ID() && part_b.Parent() != part_b.ID() && part_a.Parent() != part_b.Parent() )
      throw ERException("Two particles with different parent particle cannot be unified!");

    // 1) Check ancestor ... OK only if
    //                       1-a) neither has a ancestor
    //                       1-b) only one of them have a ancestor
    //                       1-c) shares the same ancestor
    if( part_a.Ancestor() != part_a.ID() && part_b.Ancestor() != part_b.ID() && part_a.Ancestor() != part_b.Ancestor() )
      throw ERException("Two particles with different ancestors cannot be unified!");
    
    // 2) Check layer ... OK only if
    //                    2-a) neither has a valid layer
    //                    2-b) only one of them has a valid layer
    //                    2-c) shares the same layer in the 
    if( part_a.Layer() != kDefaultLayer && part_b.Layer() != kDefaultLayer ) {
      if( part_a.Ancestor() != part_b.Ancestor() && !part_a.Primary() && !part_b.Primary() )
	throw ERException("Two non-primary particles from different ancestor cannot be unified!");
      if( part_a.Ancestor() == part_b.Ancestor() && part_a.Parent() != part_b.Parent() )
	throw ERException("Two particles from the same ancestor but from different parents cannot be unified!");
    }
    // 3) Check particle type ... OK only if
    //                            3-a) both are kInvisible
    //                            3-b) only one of them is kInvisible
    if( part_a.RecoType() != kInvisible && part_b.RecoType() != kInvisible )
      throw ERException("Two particles both with Shower/Track type cannot be unified!");

    // 4) Check particle info ... OK only if
    //                            4-a) both are invalid
    //                            4-b) only one of them is filled
    bool reco_validity_a = ( part_a.PdgCode()  != kINVALID_INT      ||
			     part_a.Mass()     != kINVALID_DOUBLE   ||
			     part_a.Vertex()   != kINVALID_VERTEX   ||
			     part_a.Momentum() != kINVALID_MOMENTUM );
    bool reco_validity_b = ( part_b.PdgCode()  != kINVALID_INT      ||
			     part_b.Mass()     != kINVALID_DOUBLE   ||
			     part_b.Vertex()   != kINVALID_VERTEX   ||
			     part_b.Momentum() != kINVALID_MOMENTUM );

    if( reco_validity_a && reco_validity_b )
      throw ERException("Two particles both with valid particle info cannot be unified!");

    //
    // If reaching this point, these particles can be combined. Merge b into a.
    //

    // First merge children
    for(auto const& child_id : part_b.Children())
      AddChild ( part_a.ID(), child_id );
    // Second propagate new parent/ancestor/layer to children
    auto parent_id   = ( part_b.Parent()   == part_b.ID()   ? part_a.Parent()   : part_b.Parent()   );
    auto ancestor_id = ( part_b.Ancestor() == part_b.ID()   ? part_a.Ancestor() : part_b.Ancestor() );
    auto layer       = ( part_b.Layer()    == kDefaultLayer ? part_a.Layer()    : part_b.Layer()    );
    UpdateParentID   ( part_a.ID(), parent_id   );
    UpdateAncestorID ( part_a.ID(), ancestor_id ); 
    UpdateLayerID    ( part_a.ID(), layer       );
    // Update reco info
    if( part_b.RecoType() != kInvisible )
      part_a.SetRecoInfo ( part_b.RecoType(), part_b.RecoID() );
    // Update particle info if necessary
    if( reco_validity_b ) {
      part_a.SetParticleInfo( part_b.PdgCode(),
			      part_b.Mass(),
			      part_b.Vertex(),
			      part_b.Momentum() );
    }

    // Unify score: take the better score for each particle relationship
    if(part_a._score_v.size() < part_b._score_v.size())
      part_a._score_v.resize(part_b._score_v.size(),-1);
    
    for(size_t id = 0; id < part_b._score_v.size(); ++id) {
      if(part_a._score_v[id] < part_b._score_v[id])
	part_a._score_v[id] = part_b._score_v[id];
    }

    //
    // Removal of b
    //
    // Remove b from parent (if child) and add a (if a is already a child, this does nothing)
    _particle_v[parent_id].RemoveChild ( part_b.ID() );
    _particle_v[parent_id].AddChild    ( part_a.ID() );
    // Remove b
    auto rm_id = part_b.ID();
    auto rm_iter = _particle_v.begin();
    rm_iter += rm_id;
    _particle_v.erase(rm_iter);
    // Update all particles' ID
    for(auto& p : _particle_v)
      p.UpdateAfterRemoval(rm_id);

  }

  void ParticleGraph::UpdateParentID(const NodeID_t target, const NodeID_t parent)
  {
    _particle_v[target]._parent_id = parent;
  }
  
  void ParticleGraph::UpdateLayerID(const NodeID_t target, const LayerID_t layer)
  {
    if(_particle_v[target]._layer_id != layer ) {
      _particle_v[target]._layer_id = layer;
      for(auto const& child_id : _particle_v[target].Children())
	UpdateLayerID(child_id, layer+1);
    }
  }

  void ParticleGraph::UpdateAncestorID(const NodeID_t target, const NodeID_t ancestor)
  {
    if(_particle_v[target].Ancestor() != ancestor) {
      _particle_v[target]._ancestor_id = ancestor;
      for(auto const& child_id : _particle_v[target].Children())
	UpdateAncestorID(child_id, ancestor);
    }
  }

  void ParticleGraph::UpdateRecoInfo(const NodeID_t target,
				     const RecoType_t reco_type,
				     const RecoID_t reco_id )
  {
    auto& p = GetParticle(target);
    if( p.RecoType() != kInvisible || p.RecoID() != kINVALID_RECO_ID )
      throw ERException("Reco info is already set for this particle (cannot over-write)!");
    p.SetRecoInfo(reco_type,reco_id);
  }

  void ParticleGraph::AddChild(const NodeID_t parent_id, const NodeID_t child_id, const float score)
  {
    // Add child
    auto& parent = GetParticle(parent_id);
    auto& child  = GetParticle(child_id);
    ValidNode(child_id);
    parent.AddChild(child_id);
    parent.SetScore(child_id,score);
    child.SetScore(parent_id,score);
    // Update layer if not yet set
    if(parent.Layer() == kDefaultLayer)
      UpdateLayerID(parent_id, 0);
    // Update child
    UpdateParentID   ( child_id, parent_id         );
    UpdateLayerID    ( child_id, parent.Layer()+1  );
    UpdateAncestorID ( child_id, parent.Ancestor() );
  }

  void ParticleGraph::Diagram(const NodeID_t target, std::string& res, std::string prefix) const
  {
    auto const& p = GetParticle(target);
    res += prefix + std::to_string(p.PdgCode()) + " ... ";
    res += std::to_string(p.Vertex()[0]) + " [cm] : " + std::to_string(p.Vertex()[1]) + " [cm] : " + std::to_string(p.Vertex()[2]);
    res += " [cm] --> " + std::to_string( p.KineticEnergy() );
    res += " MeV \n";
    prefix += "  ";
    for(auto const& c : p.Children()) Diagram(c,res,prefix);
  }

  std::string ParticleGraph::Diagram(const NodeID_t target) const
  {
    std::string res;
    Diagram(target,res," ");
    return res;    
  }

  std::string ParticleGraph::Diagram() const
  {
    std::string res;
    for(auto const& part_id : GetParticles()) {
      auto const& p = GetParticle(part_id);
      if( !p.Primary() && p.RelationAssessed() ) continue;
      std::string part_diagram;
      if( !p.RelationAssessed() )
	part_diagram += "UN-ASSESSED " + std::to_string(p.PdgCode());
      else {
	part_diagram  = " ";
	Diagram(part_id,part_diagram," ");
	part_diagram += "\n";
	res += part_diagram;
      }
    }
    return res;    
  }
  
  void ParticleGraph::SetParentage(const NodeID_t parent_id, const NodeID_t child_id, const float score)
  {
    ValidNode(parent_id);
    ValidNode(child_id);
    auto& child  = _particle_v[child_id];

    // Not OK if a child is not associated with anything
    if( child.RelationAssessed() && !child.Primary() )
      throw ERException("Cannot make a parentage for a chile who is neither lonely nor primary!");

    AddChild(parent_id, child_id, score);
  }

  void ParticleGraph::SetSiblings(const NodeID_t his_id, const NodeID_t her_id, const float score)
  {
    ValidNode(his_id);
    ValidNode(her_id);

    auto& he  = _particle_v[his_id];
    auto& she = _particle_v[her_id];

    // Question is whether they are allowed to have a common parent

    // Case 0: they both have a parent
    if( he.Parent() != he.ID() && she.Parent() != she.ID() ) {
      // 0-a) their parents are the same ... nothing to be done
      if( he.Parent() == she.Parent() ) {
	he.SetScore(her_id,score);
	she.SetScore(his_id,score);	
	return;
      }
      else throw ERException("Two particles already have different parents!");
    }
    // Case 1: Neither has a parent ... create a new particle and assign parentage
    if( he.Parent() == he.ID() && she.Parent() == she.ID() ) {
      auto& parent = CreateParticle();
      AddChild ( parent.ID(), his_id, score );
      AddChild ( parent.ID(), her_id, score );
      he.SetScore(her_id,score);
      she.SetScore(his_id,score);
      return;
    }
    // Case 2: he has a parent but she doesn't ... add her to his parent
    if( he.Parent() != he.ID() && she.Parent() == she.ID() ) {
      AddChild ( he.Parent(), her_id, score );
      he.SetScore(her_id,score);
      she.SetScore(his_id,score);
      return;
    }
    // Case 3: she has a parent but he doesn't ... add him to her parent
    if( he.Parent() == he.ID() && she.Parent() != she.ID() ) {
      AddChild ( she.Parent(), his_id, score );
      he.SetScore(her_id,score);
      she.SetScore(his_id,score);
      return;
    }
    throw ERException("Logic Error: report to the developers!");
  }

  void ParticleGraph::Reset()
  {
    _particle_v.clear();
  }

}


#endif
