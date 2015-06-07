#ifndef ERTOOL_MAKETREE_CXX
#define ERTOOL_MAKETREE_CXX

#include "AlgoMakeTree.h"

namespace ertool {

  AlgoMakeTree::AlgoMakeTree() : AlgoBase()
  {
    _name       = "AlgoMakeTree";
    _minScore   = 1.;
    _verbose    = false;
  }

  void AlgoMakeTree::ProcessEnd(TFile* fout){
 
    return;
  }

  void AlgoMakeTree::Finalize()
  {
    return;
  }

  void AlgoMakeTree::ProcessBegin()
  {
    return;
  }

  void AlgoMakeTree::LoadParams(std::string fname, size_t version){
    
    return;
  }

  bool AlgoMakeTree::Reconstruct(const EventData &data, ParticleGraph& graph)
  { 

    auto datacpy = data;
    
    // reset the node manager
    _nodeMgr.Reset();    

    if (_verbose) { std::cout << "***********BEGIN RECONSTRUCTION************" << std::endl; }

    // set node IDs in manager
    _nodeMgr.setObjects(graph.GetParticleNodes());

    // make all 2-pair combinations of objects possible
    auto combos = graph.GetNodeCombinations(2);

    if (_verbose) { std::cout << "Number of combinations: " << combos.size() << std::endl; }

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
      
      if (_verbose) { std::cout << "comparing particles [" << comb[0] << ", " << comb[1] << "]" << std::endl; } 
      
      ::geoalgo::Point_t vtx(3);
      double score;
      RelationType_t rel = kINVALID_RELATION_TYPE;

      if ( (type1 == kShower) and (type2 == kShower) )
	rel = _findRel.FindRelation(datacpy.Shower(recoID1),
				    datacpy.Shower(recoID2),
				    vtx, score);

      else if ( (type1 == kShower) and (type2 == kTrack) )
	rel = _findRel.FindRelation(datacpy.Shower(recoID1),
				    datacpy.Track(recoID2),
				    vtx, score);

      else if ( (type1 == kTrack) and (type2 == kShower) )
	rel = _findRel.FindRelation(datacpy.Track(recoID1),
				    datacpy.Shower(recoID2),
				    vtx, score);

      else if ( (type1 == kTrack) and (type2 == kTrack) )
	rel = _findRel.FindRelation(datacpy.Track(recoID1),
				    datacpy.Track(recoID2),
				    vtx, score);

      // Add the correlation to the tree
      if (rel != kINVALID_RELATION_TYPE)
	_nodeMgr.AddCorrelation(comb[0],comb[1],score,vtx,
				GetGeoTreeRelation(rel));

    }// for all combos  
    
    if (_verbose) { _nodeMgr.CorrelationMatrix(); }
    // All correlations have been added. Now solve any conflicts
    _nodeMgr.ResolveConflicts();
    // Make particle tree
    _nodeMgr.MakeTree();
    // print correlation matrix
    if (_verbose) { _nodeMgr.CorrelationMatrix(); }
    // print out diagram
    if (_verbose) { _nodeMgr.Diagram(); }
    /*    
	  if (_verbose){
	  std::cout << std::endl
	  << "Node ID and associated energy: " << std::endl;
	  for (size_t i=0; i < _posMap.size(); i++){
	  double E = 0;
	  ::geoalgo::Point_t vtx(0,0,0);
	  if (_typeMap[i] == Particle::RecoObjType_t::kShower){
	  E = data.Shower(_posMap[i])._energy;
	  vtx = data.Shower(_posMap[i]).Start();
	  }
	  if (_typeMap[i] == Particle::RecoObjType_t::kTrack){
	  E = data.Track(_posMap[i])._energy;
	  vtx = data.Track(_posMap[i]).front();
	  }
	  std::cout << "Node " << _nodeMgr.FindID(i) 
	  << "\t Energy: " << E 
	  << "\t Vtx: " << vtx << std::endl;
	  }
	  std::cout << std::endl;
	  }// if verbose
    */    
    
    return true;
  }

  ::geotree::RelationType_t AlgoMakeTree::GetGeoTreeRelation(RelationType_t rel){
    
    if (rel == kSibling)
      return ::geotree::RelationType_t::kSibling;

    if (rel == kParent)
      return ::geotree::RelationType_t::kParent;
    
    if (rel == kChild)
      return ::geotree::RelationType_t::kParent;

    return ::geotree::RelationType_t::kUnknown;

  }
  
}

#endif
