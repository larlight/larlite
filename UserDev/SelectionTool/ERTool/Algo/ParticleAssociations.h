/**
 * \file ParticleAssociations.h
 *
 * \ingroup Algo
 * 
 * \brief Class def header for a class ParticleAssociations
 *
 * @author rmurrell
 */

/** \addtogroup Algo

    @{*/
#ifndef PARTICLEASSOCIATIONS_H
#define PARTICLEASSOCIATIONS_H

#include <iostream>
#include "ERTool/Base/ERToolTypes.h"
#include "GeoAlgo/GeoVector.h"
#include "GeoAlgo/GeoSphere.h"
#include "ERTool/Base/ParticleGraph.h"

/**
   \class ParticleAssociations
   User defined class ParticleAssociations ... these comments are used to generate
   doxygen documentation!
 */



class ParticleAssociation {
  
  std::vector<ertool::NodeID_t> tcluster;
  std::vector<geoalgo::Point_t> tvertices;
  geoalgo::Sphere tsphere;
  Double_t tgoodness;
  
  std::multimap<Int_t, ertool::NodeID_t> tcluster_connection;
  
public:
  
  ParticleAssociation(std::vector<ertool::NodeID_t> const & cluster,
		      std::vector<geoalgo::Point_t> const & vertices,
		      geoalgo::Sphere const & sphere,
		      Double_t const goodness);

  void AddConnection(Int_t const i, ertool::NodeID_t const n) {
    tcluster_connection.emplace(i, n);
  }
  
  void AddShower(ertool::NodeID_t const n, geoalgo::Point_t const & vert) {
    tcluster.push_back(n);
    tvertices.push_back(vert);
  }
  
  std::vector<ertool::NodeID_t> const & GetCluster() const {
    return tcluster;
  }
  
  std::vector<geoalgo::Point_t> const & GetVertices() const {
    return tvertices;
  }
  
  geoalgo::Point_t const & GetVertexFromNode(ertool::NodeID_t const n) const {
    
    auto const nv_itb = tcluster.begin();
    auto const nv_ite = tcluster.end();
    
    return tvertices.at(std::find(nv_itb, nv_ite, n) - nv_itb);
    
  }
  
  geoalgo::Sphere const & GetSphere() const {
    return tsphere;
  }
  
  Double_t GetGoodness() const {
    return tgoodness;
  }
  
  std::multimap<Int_t, ertool::NodeID_t> const & GetConnections() const {
    return tcluster_connection;
  }
  
  void PrintAssociation() const;
  
};





class ParticleAssociations {



  std::vector<ParticleAssociation> tassociations;
  std::vector<ertool::NodeID_t> tnode_vec;
  std::vector<Int_t> tindex_vec;



 public:



  void Reset();



  void AddAssociation(std::vector<ertool::NodeID_t> const & nodes,
		      std::vector<geoalgo::Point_t> const & vertices,
		      geoalgo::Sphere const & sphere,
		      Double_t const goodness);



  void AddShower(Int_t const index,
		 ertool::NodeID_t const n,
		 geoalgo::Point_t const & vert);



  std::vector<ParticleAssociation> const & GetAssociations() const {
    return tassociations;
  }



  std::vector<ertool::NodeID_t> const & GetNodes() const {
    return tnode_vec;
  }


  
  std::vector<Int_t> const & GetIndices() const {
    return tindex_vec;
  }


  
  std::vector<Size_t> GetIndicesFromNode(ertool::NodeID_t const n);



  void PrintAssociations() const;



  void PrintNodes() const;


 
  void Test() const;



  /*Check to see if a shower has been added more than once and a track has
    been added more than twice (neither of which should happen)*/
  void NodeCheck(ertool::ParticleGraph const & graph);



};



#endif
/** @} */ // end of doxygen group 

