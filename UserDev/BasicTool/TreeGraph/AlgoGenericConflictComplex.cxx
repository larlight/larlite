#ifndef ALGOGENERICCONFLICTCOMPLEX_CXX
#define ALGOGENERICCONFLICTCOMPLEX_CXX

#include "AlgoGenericConflictComplex.h"

namespace geotree{

  AlgoGenericConflictComplex::AlgoGenericConflictComplex(NodeCollection* coll)
  {
    _coll = coll;
    _name = "GenericConflictComplex";
  }

  void AlgoGenericConflictComplex::ResolveConflict(const NodeID_t& id, const NodeID_t& parent, const NodeID_t& sibling)
  {

    // since we are starting fresh, clear correlations currently stored
    ClearCorrelations();

    // create generic node/node pair to edit correlations
    std::pair<NodeID_t,NodeID_t> nodePair;

    // get all siblings
    auto siblings = _coll->GetNode(id).getSiblings();

    if (_loose){
      // Assign parent as parent to all others (with score of parent)
      auto parentScore = _coll->GetNode(id).getScore(parent);
      // vertex is average of vtx for parent and vertices for siblings
      auto parentVtx = _coll->GetNode(id).getVtx(parent);
      std::vector<larlite::geoalgo::Vector_t> vtxList;
      vtxList.push_back(parentVtx);
      for (auto& sID : siblings)
	vtxList.push_back(_coll->GetNode(id).getVtx(sID));
      // find "average" vertex location
      if (_verbose) { 
	std::cout << "\tFind Bounding Sphere from points: " << std::endl;
	for (size_t v=0; v < vtxList.size(); v++){
	  if (v==0)
	    std::cout << "\tParent ID : " << parent << "\tVtx: "<< parentVtx << std::endl;
	  else
	    std::cout << "\tSibling ID: " << parent << "\tVtx: "<< vtxList[v] << std::endl;
	}
      }
      larlite::geoalgo::Point_t newVtx = _geoAlgo.boundingSphere(vtxList).Center();
      if (_verbose) { std::cout << "\taverage vtx from " << siblings.size() << " siblings is: " << newVtx << std::endl; }
      // Edit all sibling & parent correlations to match the new vertex information
      if (_verbose) { std::cout << "\tEditing Corr Vtx between this ID " << id << " and Parent " << parent << std::endl; }
      nodePair = std::make_pair(id,parent);
      Correlation corr(_coll->GetNode(id).getScore(parent),newVtx,_coll->GetNode(id).getRelation(parent));
      _corr_v[nodePair] = corr;
      for (size_t s1 = 0; s1 < siblings.size(); s1++){
	if (_verbose) { std::cout << "\tEditing Corr Vtx between this ID " << id << "and sibling " << siblings[s1] << std::endl; }
	nodePair = std::make_pair(id,siblings[s1]);
	Correlation corr(_coll->GetNode(id).getScore(siblings[s1]),newVtx,_coll->GetNode(id).getRelation(siblings[s1]));
	_corr_v[nodePair] = corr;
	// Also, edit the correlations amongst the various siblings -> STUPID!!!
	for (size_t s2 = s1+1; s2 < siblings.size(); s2++){
	  if (s2 != id){
	    if (_verbose) { std::cout << "\tEditing Corr Vtx between siblings " << siblings[s1] << " and " << siblings[s2] << std::endl; }
	    nodePair = std::make_pair(siblings[s1],siblings[s2]);
	    Correlation corr(_coll->GetNode(siblings[s1]).getScore(siblings[s2]),newVtx,_coll->GetNode(siblings[s1]).getRelation(siblings[s2]));
	    _corr_v[nodePair] = corr;
	  }
	}
	// Finally, if a parent exists edit vtx
	// if it does not exist, add parent
	if (_coll->GetNode(siblings[s1]).hasParent() == true){
	  // ASSUME SAME PARENT!!!
	  if (_verbose) { std::cout << "\tEditing Corr Vtx between Sibling " << siblings[s1] << " & Parent " << parent << std::endl; }
	  nodePair = std::make_pair(siblings[s1],parent);
	  Correlation corr(_coll->GetNode(siblings[s1]).getScore(parent),newVtx,_coll->GetNode(siblings[s1]).getRelation(parent));
	  _corr_v[nodePair] = corr;
	}
	  else{
	    if (_verbose) { std::cout << "\tAdding Corr Vtx between Sibling " << siblings[s1] << " & Parent " << parent << std::endl; }
	    nodePair = std::make_pair(siblings[s1],parent);
	    Correlation corr(parentScore,newVtx,::geotree::RelationType_t::kChild);
	    _corr_v[nodePair] = corr;
	  }
      }// for all siblings
    }// if loose
    // if not loose -> find best relation for this node
    else{
      // get parent score
      auto parentScore  = _coll->GetNode(id).getScore(parent);
      // get score for first sibling (assuming sibling sorting already happened
      auto siblingScore = _coll->GetNode(id).getScore(siblings[0]);
      if (_verbose) { std::cout << "\tNot loose. Parent Score: " << parentScore << " and sibling score: " << siblingScore << std::endl; }
      if (parentScore > siblingScore){
	// remove sibling score
	if (_verbose) { std::cout << "\tErase sibling correlation" << std::endl; }
	// for all siblings
	for (auto& s : siblings){
	  Correlation corr(-1, larlite::geoalgo::Point_t(), ::geotree::RelationType_t::kUnknown);
	  nodePair = std::make_pair(id,s);
	  _corr_v[nodePair] = corr;
	}
      }// if parent is better
      // if sibling is better
      else{
	// remove parent score
	if (_verbose) { std::cout << "\tErase parent correlation" << std::endl; }
	Correlation corr(-1, larlite::geoalgo::Point_t(), ::geotree::RelationType_t::kUnknown);
	nodePair = std::make_pair(id,parent);
	_corr_v[nodePair] = corr;
      }
    }// if not loose
    
    return;
  }
  
}

#endif
