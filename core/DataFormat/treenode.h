/**
 * \file treenode.h
 *
 * \ingroup MCPartGetter
 * 
 * \brief Class def header for a class Tree
 *
 * @author David Caratelli
 */

/** \addtogroup MCInfo

    @{*/
#ifndef TREENODE_H
#define TREENODE_H

#include <iostream>
#include <vector>

/**
   \class treenode
   User defined class Tree ... these comments are used to generate
   doxygen documentation!
 */
class treenode{

public:

  /// Default Constructor
  treenode(){ _children.clear(); };

  /// constructor to use
  treenode(int index){ _index = index; _children.clear(); };

  /// Default destructor
  virtual ~treenode(){};

  /// Set if Primary
  void setPrimary(bool on) { _isPrimary = on; }

  /// Set id
  void setId(int index) { _index = index; }

  /// is primary?
  bool isPrimary() { return _isPrimary; }

  /// Add a child to this noe
  void AddChild(treenode tn) { _children.push_back(tn); };

  /// Get children of this node
  std::vector<treenode> getChildren() { return _children; }

  /// Count number of nodes
  void countNodes(int& count);

  /// Get node index
  int getNodeIndex() const { return _index; }

  /// Get closest brother (parent's child that is next in the list)
  //treenode* getClosestBrother();

  /// Get closest uncle (paren't sibling that is next in the list)
  //treenode* getClosestUncle();

  /// Is this node a leaf (i.e. the end of the line...no sub-nodes?)
  bool isLeaf() { if (_children.size() == 0) { return true; } return false; }

  void setParentId(int id) { _parentID = id; }

  void setAncestorId(int id) { _ancestorID = id; }

  int getParentId() { return _parentID; }

  int getAncestorId() { return _ancestorID; }

  // return the tree diagram for this node
  std::string getTreeDiagram() const;
  
protected:

  void getTreeDiagram(std::string& res, std::string prefix) const;

  //when using this class for MCparticles index should be TrackId
  int _index;

  bool _isPrimary;

  int _parentID;
  
  int _ancestorID;

  std::vector<treenode> _children;

};

#endif
/** @} */ // end of doxygen group 

