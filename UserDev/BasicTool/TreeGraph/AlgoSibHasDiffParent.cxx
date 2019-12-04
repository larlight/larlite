#ifndef ALGOSIBHASDIFFPARENT_CXX
#define ALGOSIBHASDIFFPARENT_CXX

#include "AlgoSibHasDiffParent.h"

namespace geotree{

  AlgoSibHasDiffParent::AlgoSibHasDiffParent(NodeCollection* coll)
  {
    _coll = coll;
    _name = "SibHasDiffParent";
  }

  void AlgoSibHasDiffParent::ResolveConflict(const NodeID_t& id, const NodeID_t& parent, const NodeID_t& sibling)
  {

    // make pair holder
    std::pair<NodeID_t,NodeID_t> nodePair;

    // since we are starting fresh, clear correlations currently stored
    ClearCorrelations();

    // get siblings of this node
    auto const siblings = _coll->GetNode(id).getSiblings();

    // if multiple siblings -> remove sibling relation
    if (siblings.size() > 1){
      if (_verbose) { std::cout << "\tMany siblings: removing sibling relation because easiest now!" << std::endl; }
      nodePair = std::make_pair(id,parent);
      Correlation corr(-1, larlite::geoalgo::Point_t(), ::geotree::RelationType_t::kUnknown);
      _corr_v[nodePair] = corr;
      return;
    }

    // if we made it to here then we have a problem!
    // the sibling has a different parent. Fix this!
    // situation is like this:
    // (2) is the node we are examining, with ID = ID 
    // (1) parent  of (2)
    // (3) parent  of (4)
    // (2) sibling of (4)
    // compare scores:
    // (1,2) + (2,4) -> break parentage between (3) and (4)      -> case A)
    // (3,4) + (2,4) -> break parentage between (1) and (2)      -> case B)
    // (1,2) + (3,4) -> break sibling state between (2) and (4)  -> case C)
    // sibling score
    
    double sibScore       = _coll->GetNode(id).getScore(sibling);
    double parentScore    = _coll->GetNode(id).getScore(parent);
    // get the sibling's parent
    NodeID_t sibParent    = _coll->GetNode(sibling).getParent();
    double sibParentScore = _coll->GetNode(sibling).getScore(sibParent);
    
    //A)
    double A = parentScore + sibScore;
    double B = sibParentScore + sibScore;
    double C = parentScore + sibParentScore;
    
    if (_verbose){
      std::cout << "\t\tA) : (ID,Sibling) + (ID,parent)         = " << A << std::endl;
      std::cout << "\t\tB) : (ID,Sibling) + (sibling,sibParent) = " << B << std::endl;
      std::cout << "\t\tC) : (ID,parent)  + (sibling,sibParent) = " << C << std::endl;
    }
    
    if ( (A > B) and (A > C) ){
      // remove sibling's parentage correlation
      if (_verbose) { std::cout << "\tChoosing A" << std::endl; }
      nodePair = std::make_pair(sibling,parent);
      Correlation corr(-1, larlite::geoalgo::Point_t(), ::geotree::RelationType_t::kUnknown);
      _corr_v[nodePair] = corr;
    }
    else if (B > C){
      // remove this node's parent correlation
      if (_verbose) { std::cout << "\tChoosing B" << std::endl; }
      nodePair = std::make_pair(id,parent);
      Correlation corr(-1, larlite::geoalgo::Point_t(), ::geotree::RelationType_t::kUnknown);
      _corr_v[nodePair] = corr;
    }
    else{
      // remove sibling correlation
      if (_verbose) { std::cout << "\tChoosing C" << std::endl; }
      nodePair = std::make_pair(id,sibling);
      Correlation corr(-1, larlite::geoalgo::Point_t(), ::geotree::RelationType_t::kUnknown);
      _corr_v[nodePair] = corr;
    }

    return;
  }
  
}

#endif
