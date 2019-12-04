#ifndef NODE_CXX
#define NODE_CXX

#include "Node.h"

namespace geotree{

  void Node::addCorrelation(const NodeID_t id, const double score,
			    const ::larlite::geoalgo::Point_t& vtx,
			    const geotree::RelationType_t type){

    // if correlation exists then return exception!
    if (this->isCorrelated(id) == true)
      throw ::larlite::geoalgo::GeoAlgoException("Error: Adding correlation that already exists!");
    
    if (_verbose){
      std::cout << "\tThis node: " << this->ID()
		<< "\tCorrelation: " << id << "\tVtx: " << vtx << "\tScore: " << score << "\tType: " << type << std::endl;
    }
    ::geotree::Correlation thisCorr(score,vtx,type);
    _corr[id] = thisCorr;

    return;
  }


  void Node::editCorrelation(const NodeID_t id, const double score,
			     const ::larlite::geoalgo::Point_t& vtx,
			     const geotree::RelationType_t type){

    // if correlation between these nodes not found, raise exception
    // this function should be called only if correlation exists
    // and needs to be edited
    if ( _corr.find(id) == _corr.end() )
      throw ::larlite::geoalgo::GeoAlgoException("Error: editing correlation that does not exist!");      

    if (_verbose){
      std::cout << "This node: " << this->ID()
		<< "\tCorrelation: " << id << "\tVtx: " << vtx << "\tScore: " << score << "\tType: " << type << std::endl;
    }
    _corr[id].EditCorrelation(score,vtx,type);

    return;
  }

  
  void Node::editCorrelation(const NodeID_t id, const double score)
  {
    
    // if correlation between these nodes not found, raise exception
    // this function should be called only if correlation exists
    // and needs to be edited
    if ( _corr.find(id) == _corr.end() )
      throw ::larlite::geoalgo::GeoAlgoException("Error: editing correlation that does not exist!");      

    if (_verbose){
      std::cout << "\tThis node: " << this->ID()
		<< "\tCorrelation: " << id << "\t new Score: " << score << std::endl;
    }
    _corr[id].EditCorrelation(score);

    return;
  }


  void Node::editCorrelation(const NodeID_t id, const ::larlite::geoalgo::Point_t& vtx)
  {
    
    // if correlation between these nodes not found, raise exception
    // this function should be called only if correlation exists
    // and needs to be edited
    if ( _corr.find(id) == _corr.end() )
      throw ::larlite::geoalgo::GeoAlgoException("Error: editing correlation that does not exist!");      

    if (_verbose){
      std::cout << "\tThis node: " << this->ID()
		<< "\tCorrelation: " << id << "\t new vertex: " << vtx << std::endl;
    }
    _corr[id].EditCorrelation(vtx);

    return;
  }


  void Node::editCorrelation(const NodeID_t id,
			     const geotree::RelationType_t type){

    // if correlation between these nodes not found, raise exception
    // this function should be called only if correlation exists
    // and needs to be edited
    if ( _corr.find(id) == _corr.end() )
      throw ::larlite::geoalgo::GeoAlgoException("Error: editing correlation that does not exist!");      

    if (_verbose){
      std::cout << "\tThis node: " << this->ID()
		<< "\tCorrelation: " << id << "\tType: " << type << std::endl;
    }
    _corr[id].EditCorrelation(type);

    return;
  }

  /// Erase a correlated element
  void Node::eraseCorrelation(const NodeID_t node){
    
    if (_verbose){
      std::cout << "\tThis node: " << this->ID()
		<< "\tRemoving Correlation with: " << node << std::endl;
    }
    
    _corr.erase(node);

    return;
  }

  double Node::getScore(NodeID_t node)
  {

    if (_corr.find(node) == _corr.end()){
      throw ::larlite::geoalgo::GeoAlgoException("Trying to get correlation score for a correlation that does not exist");
      return -1;
    }
    //return _corrScores[node];
    return _corr[node].Score();
  }

  ::larlite::geoalgo::Point_t Node::getVtx(NodeID_t node)
  {

    if (_corr.find(node) == _corr.end()){
      throw ::larlite::geoalgo::GeoAlgoException("Trying to get correlation vertex for a correlation that does not exist");
      return ::larlite::geoalgo::Point_t();
    }
    //return _corrScores[node];
    return _corr[node].Vtx();
  }

  ::geotree::RelationType_t Node::getRelation(NodeID_t node)
  {

    if (_corr.find(node) == _corr.end()){
      throw ::larlite::geoalgo::GeoAlgoException("Trying to get correlation type for a correlation that does not exist");
      return ::geotree::RelationType_t::kUnknown;
    }
    //return _corrScores[node];
    return _corr[node].Relation();
  }

  /// check if a node is primary (has no parent or sibling)
  bool Node::isPrimary() const
  {

    if (_verbose) { std::cout << "\tchecking if node is primary..."; }
    std::map<NodeID_t, geotree::Correlation>::const_iterator it;
    for (it = _corr.begin(); it != _corr.end(); it++){
      if ( ((it->second).Relation() == geotree::RelationType_t::kParent) or
	   ((it->second).Relation() == geotree::RelationType_t::kSibling) ){
	if (_verbose) { std::cout << "\tNot primary" << std::endl; }
	return false;
      }
    }
    
    if (_verbose) { std::cout << "\tPrimary!" << std::endl; }
    return true;
  }

  
  /// check if a node has a potential conflict (has parent & sibling)
  bool Node::hasConflict() const
  {

    int parents  = 0;
    int siblings = 0;

    std::map<NodeID_t, geotree::Correlation>::const_iterator it;
    for (it = _corr.begin(); it != _corr.end(); it++){
      if ((it->second).Relation() == geotree::RelationType_t::kParent){
	if (_verbose) { std::cout << "\tparent! ID: " << it->first << std::endl; }
	parents += 1;
      }
      if ((it->second).Relation() == geotree::RelationType_t::kSibling)
	siblings += 1;
    }
    
    // if more than 1 parent something went wrong!
    if (parents > 1)
      throw ::larlite::geoalgo::GeoAlgoException("hasConflict: Node has more than 1 parent! something went wrong!");

    if ( (parents > 0) and (siblings > 0) )
      return true;

    return false;
  }


  /// check if node has a parent
  bool Node::hasParent() const
  {
    
    int parents = 0;

    std::map<NodeID_t, geotree::Correlation>::const_iterator it;
    for (it = _corr.begin(); it != _corr.end(); it++){
      if ((it->second).Relation() == geotree::RelationType_t::kParent)
	parents += 1;
    }

    if (parents > 1)
      throw ::larlite::geoalgo::GeoAlgoException("hasParent: Node has more than 1 parent! something went wrong!");

    if (parents == 1)
      return true;

    return false;
  }


  /// get parent ID
  NodeID_t Node::getParent() const
  {
    
    NodeID_t parent = -1;
    int parents = 0;

    std::map<NodeID_t, geotree::Correlation>::const_iterator it;
    for (it = _corr.begin(); it != _corr.end(); it++){
      if ((it->second).Relation() == geotree::RelationType_t::kParent){
	parent = it->first;
	parents += 1;
      }
    }

    if (parents > 1)
      throw ::larlite::geoalgo::GeoAlgoException("getParent: Node has more than 1 parent! something went wrong!");

    if (parents == 0)
      throw ::larlite::geoalgo::GeoAlgoException("No parent when one expected! something went wrong!");

    return parent;
  }


  /// check if node has a parent
  bool Node::hasSiblings() const
  {
    
    int siblings = 0;

    std::map<NodeID_t, geotree::Correlation>::const_iterator it;
    for (it = _corr.begin(); it != _corr.end(); it++){
      if ((it->second).Relation() == geotree::RelationType_t::kSibling)
	siblings += 1;
    }

    if (siblings >= 1)
      return true;

    return false;
  }


  /// get sibling IDs
  std::vector<NodeID_t> Node::getSiblings() const
  {
    
    std::vector<NodeID_t> siblings;

    std::map<NodeID_t, geotree::Correlation>::const_iterator it;
    for (it = _corr.begin(); it != _corr.end(); it++){
      if ((it->second).Relation() == geotree::RelationType_t::kSibling)
	siblings.push_back(it->first);
    }

    if (siblings.size() == 0)
      throw ::larlite::geoalgo::GeoAlgoException("Trying to get siblings expecting >=1 but got 0! something went wrong!");
    
    return siblings;
  }


  /// get sibling IDs
  std::vector<NodeID_t> Node::getChildren() const
  {
    return _child_id_v;
  }

  // Check if node is correlated with another
  bool Node::isCorrelated(NodeID_t id){
    
    if (_corr.find(id) != _corr.end())
      return true;

    return false;
  }

  /// check if a specific relation type is prohibited
  bool Node::isProhibited(::geotree::RelationType_t rel){

    for (size_t i=0; i < _prohibits.size(); i++){
      if (_prohibits[i] == rel)
	return true;
    }

    return false;
  }

}

#endif
