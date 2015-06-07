/**
 * \file NodeCollection.h
 *
 * \ingroup GeoTree
 * 
 * \brief Class def header for a class geotree::NodeCollection
 *
 * @author david caratelli
 */

/** \addtogroup GeoTree
    
    @{*/
#ifndef NODECOLLECTION_H
#define NODECOLLECTION_H

#include <deque>
#include "Node.h"
#include "GeoAlgo/GeoVector.h"
#include <iomanip> // to pad with zeros

namespace geotree{


  /**
     \class geotree::Coollection
     User defined class geograph::NodeCollection
     Responsible for holding a collection
     of nodes and ensuring their uniqueness
  */
  
  class NodeCollection{

  public:

    /// Default constructor
    NodeCollection(){ _verbose = false; }

    // Default destructor
    virtual ~NodeCollection(){}

    /// Add a node to the internal vector of nodes
    void AddNode(const size_t ID);

    /// Add a primary node
    void AddPrimaryNode(const NodeID_t ID);

    /// Get a node
    Node& GetNode(const NodeID_t ID);

    /// Get a list of node IDs
    std::vector<NodeID_t> GetNodeIDs() { return _IDs; }

    /// Clear collection
    void Reset() { _nodes.clear(); _n_map.clear(); _idx_map.clear(); _head_node_v.clear(); _IDs.clear(); }

    /// Clear the tree
    void ClearTree() { _head_node_v.clear(); }

    /// Check if node exists in collection. Returns boolean
    bool NodeExists(const size_t ID);

    /// check if the node has been added to the tree
    bool NodeAdded(const NodeID_t ID);

    /// Print correlation matrix for nodes in event
    void CorrelationMatrix();

    /// Print entire diagram
    void Diagram();

    /// Print diagram for one node
    void Diagram(NodeID_t id, int gen);

    /// Find the NodeID from the position in the node vector
    NodeID_t FindID(size_t idx);

    /// verbosity setter
    void SetVerbose(bool on) { _verbose = on; }


  private:

    /// verbosity flag
    bool _verbose;

    /// Check if a node is a subnode of another node
    bool IsSubNode(NodeID_t search, NodeID_t top);
    
    /// NodeCollection of all nodes created
    std::vector<::geotree::Node> _nodes;

    /// keep track of the indices of primary nodes
    std::deque<NodeID_t> _head_node_v;

    /// Keep track of NodeIDs
    std::vector<NodeID_t> _IDs;

    /// Map that goes from NodeID_t to position in node vector
    std::map<NodeID_t, size_t> _n_map;

    /// map to link position in node vector with ID
    std::map<size_t, NodeID_t> _idx_map;

  };
}
#endif

