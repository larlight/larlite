#ifndef ERTOOL_MAKEGRAPH_CXX
#define ERTOOL_MAKEGRAPH_CXX

#include "AlgoMakeGraph.h"
#include <sstream>
namespace ertool {

  AlgoMakeGraph::AlgoMakeGraph(const std::string& name) : AlgoBase(name)
  {
    _minScore   = 1.;
  }

  /*
  void AlgoMakeGraph::setVerbose(const bool on)
  { _verbose = on;
    _findRel.setDebug(on); 
    if (_nodeMgr)
      _nodeMgr->setVerbose(on);
  }
  */  
  void AlgoMakeGraph::AcceptPSet(const ::fcllite::PSet& cfg)
  {}
  
  void AlgoMakeGraph::ProcessEnd(TFile* fout){
 
    return;
  }

  void AlgoMakeGraph::Finalize()
  {
    return;
  }

  void AlgoMakeGraph::ProcessBegin()
  {
    return;
  }

  bool AlgoMakeGraph::Reconstruct(const EventData &data, ParticleGraph& graph)
  { 

    auto datacpy = data;
    
    // reset the node manager
    _nodeMgr->Reset();    

    if (Debug()) Debug("***********BEGIN RECONSTRUCTION************");
      

    // set node IDs in manager
    _nodeMgr->setObjects(graph.GetParticleNodes());

    // make all 2-pair combinations of objects possible
    auto combos = graph.GetNodeCombinations(2);

    if (Debug()) {
      std::stringstream ss;
      ss << "Number of combinations: " << combos.size();
      Debug(__FUNCTION__,ss.str());
    }

    for (auto const& comb : combos){
      
      // get particles associated with these node IDs
      auto const& p1 = graph.GetParticle(comb[0]);
      auto const& p2 = graph.GetParticle(comb[1]);

      // get RecoTypes associated with these particles
      auto const& type1 = p1.RecoType();
      auto const& type2 = p2.RecoType();

      // get reco ID for both
      auto const& recoID1 = p1.RecoID();
      auto const& recoID2 = p2.RecoID();

      // do not consider tracks of 0-length
      if (type1 == kTrack){
	if (datacpy.Track(recoID1).size() <= 1)
	  continue;
      }
      if (type2 == kTrack){
	if (datacpy.Track(recoID2).size() <= 1)
	  continue;
      }

      if ( ( (type1 != kShower) and (type1 != kTrack) ) or
	   ( (type1 != kShower) and (type1 != kTrack) ) )
	continue;
      
      if (Debug()){
	std::stringstream ss;
	ss << "comparing particles [" << comb[0] << ", " << comb[1] << "]" << std::endl;
	Debug(__FUNCTION__,ss.str());
      } 
      
      ::geoalgo::Point_t vtx(3);
      double score;
      RelationType_t rel = kINVALID_RELATION_TYPE;
      
      if ( (type1 == kShower) and (type2 == kShower) )
	rel = _findRel.FindRelation(datacpy.Shower(recoID1),
				    datacpy.Shower(recoID2),
				    vtx, score);
      
      if ( (type1 == kShower) and (type2 == kTrack) )
	rel = _findRel.FindRelation(datacpy.Shower(recoID1),
				    datacpy.Track(recoID2),
				    vtx, score);
      
      if ( (type1 == kTrack) and (type2 == kShower) )
	rel = _findRel.FindRelation(datacpy.Track(recoID1),
				    datacpy.Shower(recoID2),
				    vtx, score);
      
      if ( (type1 == kTrack) and (type2 == kTrack) )
	rel = _findRel.FindRelation(datacpy.Track(recoID1),
				    datacpy.Track(recoID2),
				    vtx, score);
      
      if (Debug()) { Debug("done assessing relation type"); }
      
      // Add the correlation to the tree
      if ( (rel != kINVALID_RELATION_TYPE) and (score > _minScore) ){
	if (Debug()) {
	  std::stringstream ss;
	  ss << "Assigning relation " << rel << " with score: " << score;
	  Debug(__FUNCTION__,ss.str());
	}
	_nodeMgr->AddCorrelation(comb[0],comb[1],score,vtx,
				GetGeoTreeRelation(rel));
      }
      

    }// for all combos  
    
    if (Debug()) { _nodeMgr->CorrelationMatrix(); }
    // All correlations have been added. Now solve any conflicts
    _nodeMgr->ResolveConflicts();
    // Make particle tree
    _nodeMgr->MakeTree();
    // print correlation matrix
    if (Debug()) { _nodeMgr->CorrelationMatrix(); }
    // print out diagram
    if (Debug()) { _nodeMgr->Diagram(); }

    // Now take tree and use it to sort the ParticleGraph

    // first create extra nodes that were created
    auto const& Nnodes = _nodeMgr->getNumNodes();
    // node ID is equal to position in node vector
    // just create extra nodes for all the ones missing
    // in the graph
    for (size_t i = graph.GetNumParticles(); i < Nnodes; i++){
      auto& p = graph.CreateParticle();
      if (Debug()) {
	std::stringstream ss;
	ss << "Created Particle with node ID: " << p.ID();
	Debug(__FUNCTION__,ss.str());
      }
    }
	
    // make sure node vectors now have the same size
    if (graph.GetNumParticles() != _nodeMgr->getNumNodes())
      throw ERException("Number of Nodes and Particles does not match!");

    if (Debug()) {
      std::stringstream ss;
      ss << "Nodes: " << _nodeMgr->getNumNodes() << "\tParticles: " << graph.GetNumParticles();
      Debug(__FUNCTION__,ss.str());
      Debug("now assigning relationships found by TreeGraph");
    }
    // loop through all nodes...
    // and assign parent
    for (auto const& node : graph.GetParticleNodes()){
      // does the node have a parent?
      if (_nodeMgr->hasParent(node))
	{
	  auto const& parent = _nodeMgr->getParent(node);
	  // establish relationship
	  if (Debug()) {
	    std::stringstream ss;
	    ss << "Assigning " << parent << " as parent of node " << node;
	    Debug(__FUNCTION__,ss.str());
	  }
	  graph.SetParentage(parent,node);
	}// if there is a parent
    }
      
    if (Debug()){ Debug(__FUNCTION__,graph.Diagram()); }
    
    return true;
  }

  ::geotree::RelationType_t AlgoMakeGraph::GetGeoTreeRelation(RelationType_t rel){
    
    if (rel == kSibling)
      return ::geotree::RelationType_t::kSibling;

    if (rel == kParent)
      return ::geotree::RelationType_t::kParent;
    
    if (rel == kChild)
      return ::geotree::RelationType_t::kChild;

    return ::geotree::RelationType_t::kUnknown;

  }
  
}

#endif
