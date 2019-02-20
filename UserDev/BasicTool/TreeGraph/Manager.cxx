#ifndef MANAGER_CXX
#define MANAGER_CXX

#include "Manager.h"

namespace geotree{


  // Constructor
  Manager::Manager()
    : _algoMultipleParents(nullptr)
    , _algoParentIsSiblingsSibling(nullptr)
    , _algoGenericConflict(nullptr)
  {
    
    _verbose = false;
    _loose   = false;

    std::cout << "num of nodes: " << _coll.GetNumNodes() << std::endl;

    /*
    // Initialize algorithms used
    if (_algoMultipleParents) { delete _algoMultipleParents; }
    _algoMultipleParents = new AlgoMultipleParentsHighScore(&_coll);

    if (_algoParentIsSiblingsSibling) { delete _algoParentIsSiblingsSibling; }
    _algoParentIsSiblingsSibling = new AlgoParentIsSiblingsSibling(&_coll);

    if (_algoGenericConflict) { delete _algoGenericConflict; }
    _algoGenericConflict = new AlgoGenericConflictRemoveSibling(&_coll);
    */


    

  }
  
  // Node initializer: create a node for each object
  void Manager::setObjects(size_t n){
  
    if (_verbose) { std::cout << "Setting " << n << " objects to prepare tree" << std::endl; }
    //_node_v.resize(_node_v.size()+n);
    for (size_t i=0; i < n; i++){
      // Assign an ID double the element number. Just because
      size_t nID = i*2;
      _coll.AddNode(nID);
      if (_verbose) { std::cout << "Created Node Num. " << i << "/" << n <<" with ID " << nID << std::endl; }
    }

    LinkCollectionToAlgos();

    return;
  }


  // Node initializer: create a node for each object
  void Manager::setObjects(std::vector<size_t> IDs){
  
    if (_verbose) { std::cout << "Setting " << IDs.size() << " objects to prepare tree" << std::endl; }
    for (size_t i=0; i < IDs.size(); i++){
      _coll.AddNode(IDs[i]);
      if (_verbose) { std::cout << "Created Node Num. " << i << "/" << IDs.size() <<" with ID " << IDs[i] << std::endl; }
    }

    LinkCollectionToAlgos();

    return;
  }

  void Manager::LinkCollectionToAlgos()
  {

    // link algos to the node collections in the manager
    _algoMultipleParents->SetCollection(&_coll);
    _algoParentIsSiblingsSibling->SetCollection(&_coll);
    _algoGenericConflict->SetCollection(&_coll);
    
    return;
  }
  

  // Function to be called when Trees will be made
  void Manager::MakeTree(){

    if (_verbose) { std::cout << "Making tree" << std::endl; }

    // clear the return collection storing the tree
    _coll.ClearTree();

    // loop over nodes.
    // what to do:
    // 1) if no parent or sibling -> add as head node
    // 2) if parent exists, add as child to that parent
    // 3) if sibling exists, create new parent node (example: pi0)

    for (size_t n=0; n < _coll.GetNodeIDs().size(); n++){

      NodeID_t ID = _coll.GetNodeIDs()[n];

      if (_verbose) { std::cout << "Examining node " << n << " with ID: " << ID << std::endl; }

      // check if this node has been already added to the tree
      if ( _coll.NodeAdded(ID) ){
	if (_verbose) { std::cout << "\tthis node has already been added. Skip" << std::endl; }
	continue;
      }

      // check if node is primary
      if (_coll.GetNode(ID).isPrimary()){
	if (_verbose) { std::cout << "\tnode is primary" << std::endl; }
	_coll.AddPrimaryNode(ID);
      }
      // if node has a parent add it
      if (_coll.GetNode(ID).hasParent()){
	if (_verbose) { std::cout << "\tnode has parent" << std::endl; }
	NodeID_t parent = _coll.GetNode(ID).getParent();
	_coll.GetNode(parent).addChild(ID);
	_coll.GetNode(ID).setParent(parent);
      }
      // if node has a parent && a sibling
      // find vertex consistent with all 3 objects
      if (_coll.GetNode(ID).hasConflict()){
	if (_verbose) { std::cout << "\tnode has conflict" << std::endl; }
	// The philosophy right now:
	// Add particle as child of its parent.
	// Do not worry about sibling relationship.
	// they are siblings since they come from the
	// same parent, but they may not share a vertex
	// worry about that later
	// So bottom line now:
	// if siblings & parent -> just add as child to parent
	continue;
      }
      // if node has sibling: make a common head node for the two siblings
      if (_coll.GetNode(ID).hasSiblings()){
	if (_verbose) { std::cout << "\tnode has sibling" << std::endl; }
	if (_verbose) { std::cout << "\tadding node " << ID << " now" << std::endl; }
	// get siblings
	auto siblings = _coll.GetNode(ID).getSiblings();
	if (_verbose) { std::cout << "\tnode has " << siblings.size() << " siblings" << std::endl;
	}
	// if > 1 siblings
	// Make sure all siblings share the same vertex
	if (siblings.size() > 1){
	  auto const vtx = _coll.GetNode(ID).getVtx(siblings[0]);
	  for (auto &s : siblings){
	    auto const vtx2 = _coll.GetNode(ID).getVtx(s);
	    if (vtx != vtx2)
	      throw ::larlite::geoalgo::GeoAlgoException("Multiple siblings @ different Vertices. Should have been solved by SortSiblings!");
	  }//for all siblings
	}// if multiple siblings
	// create new node to host the new siblings
	NodeID_t id = _coll.GetNumNodes();
	// Make sure this ID does not exist
	if (_coll.NodeExists(id) == true)
	  throw ::larlite::geoalgo::GeoAlgoException(Form("About to create a NodeID that already exists (%i). Not acceptable!",(int)id));
	_coll.AddNode(id);
	// add child nodes to newly created node
	_coll.GetNode(id).addChild(ID);
	_coll.GetNode(ID).setParent(id);
	// also add correlations so they show up on correlation matrix (not too important...)
	AddCorrelation(id,ID,1.,::larlite::geoalgo::Point_t(3),::geotree::RelationType_t::kParent);
	// loop over siblings and add them
	for (auto& sib : siblings){ 
	  // add node parentage
	  _coll.GetNode(id).addChild(sib);
	  _coll.GetNode(sib).setParent(id);
	AddCorrelation(id,sib,1.,::larlite::geoalgo::Point_t(3),::geotree::RelationType_t::kParent);
	  // and correlations
	}
	if (_verbose) { std::cout << "\tadding node " << id << " to tree nodes" << std::endl; }
	_coll.AddPrimaryNode(id);
	if (_verbose){ 
	  std::cout << "\tadded node " << id 
		    << " as parent of: [" << ID << ", ";
	  for (auto &sib : siblings)
	    std::cout << sib << ", ";
	  std::cout << std::endl;
	}
      }// if node has a sbinling
    }// for all nodes
    
  return;
  }


  // Correlation provided indicates relationship between node id1 and node id2
  void Manager::AddCorrelation(const NodeID_t id1, const NodeID_t id2,
			       const double score,
			       const larlite::geoalgo::Point_t& vtx,
			       const geotree::RelationType_t type){

    if (type == geotree::RelationType_t::kUnknown)
      return;

    // make sure nodes exist
    if (_coll.NodeExists(id1) == false)
      throw ::larlite::geoalgo::GeoAlgoException("Node ID not found!");
    if (_coll.NodeExists(id2) == false)
      throw ::larlite::geoalgo::GeoAlgoException("Node ID not found!");

    //type returned is the relation of 1 w.r.t. 2
    // find "inverse" relation to assign to 2 w.r.t. 1
    geotree::RelationType_t otherRel = geotree::RelationType_t::kUnknown;
    if (type == geotree::RelationType_t::kSibling)
      otherRel = geotree::RelationType_t::kSibling;
    else if (type == geotree::RelationType_t::kChild)
      otherRel = geotree::RelationType_t::kParent;
    else if (type == geotree::RelationType_t::kParent)
      otherRel = geotree::RelationType_t::kChild;

    // make sure this relation is not prohibited
    if ( _coll.GetNode(id1).isProhibited(otherRel) ||
	 _coll.GetNode(id2).isProhibited(type) ){
      if (_verbose) { std::cout << "\tCorrelation is Prohibited!" << std::endl; }
      return;
    }

    if (_verbose) { std::cout << "\tAdding Correlation..." << std::endl; }
    _coll.GetNode(id2).addCorrelation(id1,score,vtx,type);    
    _coll.GetNode(id1).addCorrelation(id2,score,vtx,otherRel);

    return;
  }


  void Manager::EditCorrelation(const NodeID_t id1, const NodeID_t id2,
				const double score,
				const larlite::geoalgo::Point_t& vtx,
				const geotree::RelationType_t type){

    // make sure nodes exist
    if (_coll.NodeExists(id1) == false)
      throw ::larlite::geoalgo::GeoAlgoException("Node ID not found!");
    if (_coll.NodeExists(id2) == false)
      throw ::larlite::geoalgo::GeoAlgoException("Node ID not found!");

    //type returned is the relation of 1 w.r.t. 2
    // find "inverse" relation to assign to 2 w.r.t. 1
    geotree::RelationType_t otherRel = geotree::RelationType_t::kUnknown;
    if (type == geotree::RelationType_t::kSibling)
      otherRel = geotree::RelationType_t::kSibling;
    else if (type == geotree::RelationType_t::kChild)
      otherRel = geotree::RelationType_t::kParent;
    else if (type == geotree::RelationType_t::kParent)
      otherRel = geotree::RelationType_t::kChild;

    // make sure this relation is not prohibited
    if ( _coll.GetNode(id1).isProhibited(otherRel) ||
	 _coll.GetNode(id2).isProhibited(type) ){
      if (_verbose) { std::cout << "\tCorrelation is Prohibited!" << std::endl; }
      return;
    }

    if (_verbose) { std::cout << "\tEditing Correlation..." << std::endl; }
    _coll.GetNode(id2).editCorrelation(id1,score,vtx,type);    
    _coll.GetNode(id1).editCorrelation(id2,score,vtx,otherRel);

    return;
  }


  void Manager::EditCorrelation(const NodeID_t id1, const NodeID_t id2,
				const double score){

    // make sure nodes exist
    if (_coll.NodeExists(id1) == false)
      throw ::larlite::geoalgo::GeoAlgoException("Node ID not found!");
    if (_coll.NodeExists(id2) == false)
      throw ::larlite::geoalgo::GeoAlgoException("Node ID not found!");

    if (_verbose) { std::cout << "\tEditing Correlation Score..." << std::endl; }
    _coll.GetNode(id2).editCorrelation(id1,score);
    _coll.GetNode(id1).editCorrelation(id2,score);

    return;
  }


  void Manager::EditCorrelation(const NodeID_t id1, const NodeID_t id2,
				const larlite::geoalgo::Point_t& vtx){

    // make sure nodes exist
    if (_coll.NodeExists(id1) == false)
      throw ::larlite::geoalgo::GeoAlgoException("Node ID not found!");
    if (_coll.NodeExists(id2) == false)
      throw ::larlite::geoalgo::GeoAlgoException("Node ID not found!");

    if (_verbose) { std::cout << "\tEditing Correlation Vtx..." << std::endl; }
    _coll.GetNode(id2).editCorrelation(id1,vtx);
    _coll.GetNode(id1).editCorrelation(id2,vtx);

    return;
  }


  void Manager::EditCorrelation(const NodeID_t id1, const NodeID_t id2,
				const geotree::RelationType_t type){

    // make sure nodes exist
    if (_coll.NodeExists(id1) == false)
      throw ::larlite::geoalgo::GeoAlgoException("Node ID not found!");
    if (_coll.NodeExists(id2) == false)
      throw ::larlite::geoalgo::GeoAlgoException("Node ID not found!");

    //type returned is the relation of 1 w.r.t. 2
    // find "inverse" relation to assign to 2 w.r.t. 1
    geotree::RelationType_t otherRel = geotree::RelationType_t::kUnknown;
    if (type == geotree::RelationType_t::kSibling)
      otherRel = geotree::RelationType_t::kSibling;
    else if (type == geotree::RelationType_t::kChild)
      otherRel = geotree::RelationType_t::kParent;
    else if (type == geotree::RelationType_t::kParent)
      otherRel = geotree::RelationType_t::kChild;

    // make sure this relation is not prohibited
    if ( _coll.GetNode(id1).isProhibited(otherRel) ||
	 _coll.GetNode(id2).isProhibited(type) ){
      if (_verbose) { std::cout << "\tCorrelation is Prohibited!" << std::endl; }
      return;
    }

    if (_verbose) { std::cout << "\tEditing Correlation Relation..." << std::endl; }
    _coll.GetNode(id2).editCorrelation(id1,type);
    _coll.GetNode(id1).editCorrelation(id2,otherRel);

    return;
  }


  /// Erase a correlation completely
  void Manager::EraseCorrelation(const NodeID_t id1, const NodeID_t id2){

    // make sure nodes exist
    if (_coll.NodeExists(id1) == false)
      throw ::larlite::geoalgo::GeoAlgoException("Node ID not found!");
    if (_coll.NodeExists(id2) == false)
      throw ::larlite::geoalgo::GeoAlgoException("Node ID not found!");

    if (_verbose) { std::cout << "\tRemoving Correlation..." << std::endl; }
    _coll.GetNode(id1).eraseCorrelation(id2);
    _coll.GetNode(id2).eraseCorrelation(id1);

    return;
  }

  void Manager::ResolveConflicts(){

    // first resolve conflict 1)
    // if multiple parents, choose the
    // one with the highest score
    if (_verbose) { std::cout << "Find Best parent of nodes if they have multiple ones..." << std::endl; }
    FindBestParent();

    // Case in which parent and sibling are siblings
    if (_verbose) { std::cout << "Make sure parent and sibling are logically consistent if they exist" << std::endl; }
    ParentIsSiblingsSibling();

    // if there is a conflict, remove sibling relation
    if (_verbose) { std::cout << "If there is an ADDITIONAL conflict, remove sibling status" << std::endl; }
    GenericConflict();

    // Conflict 3)
    // Resolve conflict of multiple siblings
    if (_verbose) { std::cout << "merge or find best sibling if there are multiple siblings..." << std::endl; }
    SortSiblings();

    return;
  }


  /// Find best parent for each Node
  void Manager::FindBestParent(){

    // Get list of nodes
    auto const IDs = _coll.GetNodeIDs();

    for (auto &ID : IDs)
      FindBestParent(ID);

    return;
  }

  /// function to find best parent of a node (remove other parents)
  void Manager::FindBestParent(NodeID_t ID){

    if (_verbose) { std::cout << "look for best parent for node: " << ID << std::endl; }

    // iterator for correlation types
    std::map<NodeID_t,geotree::Correlation>::const_iterator it;
    // get correaltions for this node
    auto const corrs  = _coll.GetNode(ID).getCorrelations();

    // vector where to hold parent IDs
    std::vector<NodeID_t> parentIDs;
    // vector where to hold parent scores
    std::vector<double> parentScores;

    for (it = corrs.begin(); it != corrs.end(); it++){
      if ((it->second).Relation() == geotree::RelationType_t::kParent){
	//if (it->second == geotree::RelationType_t::kChild){
	size_t pID = it->first;
	parentIDs.push_back(pID);
	parentScores.push_back(_coll.GetNode(ID).getScore(pID));
      }
    }

    // if < 1 parent -> continue
    if (parentIDs.size() < 2)
      return;

    // Ok, let's give the algorithm a shot! 
    // call the algorithm with node & vector of parent nodes
    if (_verbose) { std::cout << "\talgoMultipleParents called..." << std::endl; }
    _algoMultipleParents->FindBestParent(ID,parentIDs);


    // now loop through correlations found and act on them
    ApplyAlgoCorrelations(_algoMultipleParents->GetCorrelations());

    return;
  }

  // Sort all siblings at once
  void Manager::SortSiblings(){

    // Get list of nodes
    auto const IDs = _coll.GetNodeIDs();

    for (auto &ID : IDs)
      SortSiblings(ID);
   
    return;
  }
  
  // Siblings sorting:
  // Either merge siblings (loose == true)
  // or only pick the best one (loose == false)
  // After decision, modify all other correlations
  // accordingly
  void Manager::SortSiblings(NodeID_t ID){

    if (_verbose) { std::cout << "sort siblings for node: " << ID << std::endl; }

    if (_coll.GetNode(ID).hasSiblings() == false){
      if (_verbose) { std::cout << "\tno siblings. No issue..." << std::endl; }
      return;
    }

    auto siblings = _coll.GetNode(ID).getSiblings();
    if (siblings.size() == 1){
      if (_verbose) { std::cout << "\tOnly 1 sibling. No issue..." << std::endl; }
      return;
    }

    // If there are multiple siblings but with the same vertex
    // -> then we are good to go. Everything is in agreement
    bool AllSame = true;
    auto const vtx1 = _coll.GetNode(ID).getVtx(siblings[0]);
    for (size_t i=1; i < siblings.size(); i++){
      auto const vtx2 = _coll.GetNode(ID).getVtx(siblings[i]);
      if (vtx1 != vtx2){
	AllSame = false;
	break;
      }
    }// for all remaining siblings
    // if all vertices are the same -> return
    if (AllSame){
      // Just make sure siblings are correlated amongst themselves
      for (size_t s1 = 0; s1 < siblings.size(); s1++){
	for (size_t s2 = s1+1; s2 < siblings.size(); s2++){
	  // are they correlated?
	  if (_loose){
	    if ( _coll.GetNode(siblings[s1]).isCorrelated(siblings[s2]) == true){
	      // remove that correlation and replace it with a sibling correlation
	      EraseCorrelation(siblings[s1],siblings[s2]);
	    }
	    if (_verbose) { std::cout << "\tAbout to add sibling correlation..." << std::endl; }
	    AddCorrelation(siblings[s1],siblings[s2],0.,vtx1,::geotree::RelationType_t::kSibling);
	  }
	}
      }
      return;
    }
    
    // if a particle has multiple siblings we can either:
    // - merge those siblings together into a single vtx
    // - find the best sibling
    if (_loose){
      // find all vertices of siblings
      std::vector<::larlite::geoalgo::Vector_t> siblingVtxList;
      // siblings contains NodeID of all siblings. Use to get vtx
      for (auto& sID : siblings)
	siblingVtxList.push_back(_coll.GetNode(ID).getVtx(sID));
      // find "average" vertex location
      if (_verbose) { 
	std::cout << "\tFind Bounding Sphere from points: " << std::endl;
	for (size_t v=0; v < siblingVtxList.size(); v++)
	  std::cout << "\tSib: " << siblings[v] << "\tVtx: "<< siblingVtxList[v] << std::endl;
      }
      ::larlite::geoalgo::Point_t newVtx = _geoAlgo.boundingSphere(siblingVtxList).Center();
      if (_verbose) { std::cout << "\taverage vtx from " << siblings.size() << " siblings is: " << newVtx << std::endl; }
      // edit all correlations so that vertices are updated.
      for (auto& sID : siblings)
	EditCorrelation(ID,sID,newVtx);
      // also, we need to add all sibling correlations from node ID to its sisters
      for (size_t s1 = 0; s1 < siblings.size()-1; s1++){
	for (size_t s2 = s1+1; s2 < siblings.size(); s2++){
	  // if this correlation already exits -> just edit the vertex info
	  if (_coll.GetNode(siblings[s1]).isCorrelated(siblings[s2])){
	    // if correlation is not sibling then throw exception!
	    if (_coll.GetNode(siblings[s1]).getRelation(siblings[s2]) != ::geotree::RelationType_t::kSibling)
	      throw ::larlite::geoalgo::GeoAlgoException("About to edit what you think is sibling relation but is not!");
	    EditCorrelation(siblings[s1],siblings[s2],newVtx);
	  }// if the two siblings are already correlated
	  else{
	    // if not
	    AddCorrelation(siblings[s1],siblings[s2],0.,newVtx,::geotree::RelationType_t::kSibling);
	  }
	}
      }
    }// if loose (accept more siblings and smear vertex)
    // if instead one should just select the best vertex
    else{
      double maxScore = 0.;
      NodeID_t bestSibling = -1;
      for (auto& sID : siblings){
	double score = _coll.GetNode(ID).getScore(sID);
	if (score > maxScore){
	  maxScore = score;
	  bestSibling = sID;
	}
      }// for all siblings
      if (_verbose) { std::cout << "\tBest Correlation with Node " << bestSibling << " (score = " << maxScore << ")" << std::endl; }
      // now erase correlation with all other siblings
      for (auto& sID : siblings){
	if (sID != bestSibling)
	  EraseCorrelation(ID,sID);
      }
    }// if we should just keep the best correlation
    
    return;
  }


  void Manager::ParentIsSiblingsSibling(){

    // Get list of nodes
    auto const IDs = _coll.GetNodeIDs();

    for (auto &ID : IDs)
      ParentIsSiblingsSibling(ID);

    return;
  }

  void Manager::ParentIsSiblingsSibling(NodeID_t ID){

    if (_verbose) { std::cout << "Figuring out if node " << ID << " has parent-sibling conflict" << std::endl; }

    // if node has parent and sibling
    // make sure sibling is not sibling with parent
    if (_coll.GetNode(ID).hasConflict() == false)
      return;

    // get siblings
    auto const siblings = _coll.GetNode(ID).getSiblings();
    // get parent
    auto const parentID = _coll.GetNode(ID).getParent();
    
    for (auto& s : siblings){
      // check if sibling is related to parent.
      // if their relation is not that of parent-child
      // need to fix things
      if (_coll.GetNode(s).isCorrelated(parentID) == false)
	continue;
      // ok they are correlated. what is the correlation type
      auto rel = _coll.GetNode(s).getRelation(parentID);
      // if this relation is not parentID is parent of s we have a problem
      if ( rel == ::geotree::RelationType_t::kParent )
	continue;

      if (_verbose) { std::cout << "\tsibling " << s << " and parent "
			      << parentID <<  " relation is not logically consistent" << std::endl; }

      // Ok, let's give the algorithm a shot! 
      // call the algorithm with node & parent, and sibling
      if (_verbose) { std::cout << "\talgoParentIsSiblingsSibling called..." << std::endl; }
      _algoParentIsSiblingsSibling->ResolveConflict(ID,parentID,s);

      // now loop through correlations found and act on them
      ApplyAlgoCorrelations(_algoParentIsSiblingsSibling->GetCorrelations());

    }// for all siblings
    
    return;
  }

  void Manager::GenericConflict(){

    // Get list of nodes
    auto const IDs = _coll.GetNodeIDs();

    for (auto &ID : IDs)
      GenericConflict(ID);

    return;
  }

  // If there is a conflict and siblings don't have the same parent -> remove sibling relation
  void Manager::GenericConflict(NodeID_t ID){

    // if node has parent and sibling
    // do something if sibling does not have a parent
    if (_coll.GetNode(ID).hasConflict() == false)
      return;    

    if (_verbose) { std::cout << "Node has conflict...if siblings do not agree resolve" << std::endl; } 

    // get siblings
    auto const siblings = _coll.GetNode(ID).getSiblings();
    // get parent
    auto const parentID = _coll.GetNode(ID).getParent();    

    for (auto& s : siblings){
      
      if (_verbose) { std::cout << "\talgoGenericConflict called..." << std::endl; }
      _algoGenericConflict->ResolveConflict(ID,parentID,s);

      // now loop through correlations found and act on them
      ApplyAlgoCorrelations(_algoGenericConflict->GetCorrelations());

    }// for all siblings

    return;
  }


  void Manager::ApplyAlgoCorrelations(const std::map< std::pair<NodeID_t, NodeID_t>, geotree::Correlation>& algoCorrs){


      std::map< std::pair<NodeID_t,NodeID_t>, geotree::Correlation >::const_iterator corrit;
      for (corrit = algoCorrs.begin(); corrit != algoCorrs.end(); corrit++){
	// nodes involved
	NodeID_t n1 = (corrit->first).first;
	NodeID_t n2 = (corrit->first).second;
	// if correlation's score is negative -> remove
	if (corrit->second.Score() < 0)
	  EraseCorrelation(n1,n2);
	// if the correlation score is > 0
	// either we need to create a new
	// one if none exists or we need
	// to edit an already existing
	// correlation
	else if (_coll.GetNode(n1).isCorrelated(n2) == true)
	  EditCorrelation(n1,n2,corrit->second.Score(),corrit->second.Vtx(),corrit->second.Relation());
	else
	  AddCorrelation(n1,n2,corrit->second.Score(),corrit->second.Vtx(),corrit->second.Relation());
      }// for correlations returned by the algorithm

    return;
  }


  bool Manager::hasParent(const NodeID_t ID)
  {
    return _coll.GetNode(ID).hasParent();
  }

  NodeID_t Manager::getParent(const NodeID_t ID)
  {
    return _coll.GetNode(ID).getParent();
  }

  bool Manager::hasSiblings(const NodeID_t ID)
  {
    return _coll.GetNode(ID).hasSiblings();
  }

  std::vector<NodeID_t> Manager::getSiblings(const NodeID_t ID)
  {
    return _coll.GetNode(ID).getSiblings();
  }

  std::vector<NodeID_t> Manager::getChildren(const NodeID_t ID)
  {
    return _coll.GetNode(ID).getChildren();
  }
  
}

#endif
