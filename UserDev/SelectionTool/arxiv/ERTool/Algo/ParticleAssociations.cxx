#ifndef PARTICLEASSOCIATIONS_CXX
#define PARTICLEASSOCIATIONS_CXX

#include "ParticleAssociations.h"



ParticleAssociation::ParticleAssociation(std::vector<ertool::NodeID_t> const & cluster,
					 std::vector<geoalgo::Point_t> const & vertices,
					 geoalgo::Sphere const & sphere,
					 Double_t const goodness) :
  tcluster(cluster),
  tvertices(vertices),
  tsphere(sphere),
  tgoodness(goodness){}



void ParticleAssociation::PrintAssociation() const {
  
  for(Size_t i = 0; i < tcluster.size(); ++i)
    std::cout << "Node: " << tcluster.at(i) << " Vertices: " 
	      << tvertices.at(i) << "\n";
  
  std::cout << "\nSphere: " << tsphere.Center() << ", "
	    << tsphere.Radius() << "\n";
  
  std::cout << "\nConnections:\n";
  for(std::pair<Int_t, ertool::NodeID_t> const & pair : tcluster_connection) 
    std::cout << "Index: " << pair.first << " NodeID: " << pair.second << "\n";
  
}



void ParticleAssociations::Reset() {

  tassociations.clear();
  tnode_vec.clear();
  tindex_vec.clear();

}



void ParticleAssociations::AddAssociation(std::vector<ertool::NodeID_t> const & nodes,
					 std::vector<geoalgo::Point_t> const & vertices,
					 geoalgo::Sphere const & sphere,
					 Double_t const goodness) {
  
  tassociations.push_back
    (ParticleAssociation(nodes, vertices, sphere, goodness));
  
  Int_t const pos = tassociations.size() - 1;
  
  for(ertool::NodeID_t const n : nodes) {
    
    auto const nv_itb = tnode_vec.begin();
    auto const nv_ite = tnode_vec.end();
    
    for(auto nv_it = nv_itb; nv_it != nv_ite; ++nv_it) {
      
      nv_it = std::find(nv_it, nv_ite, n);
      
      if(nv_it != nv_ite) {
	
	Int_t const index = tindex_vec.at(nv_it - nv_itb);
	
	tassociations.at(index).AddConnection(pos, n);
	tassociations.at(pos).AddConnection(index, n);
	
      }
      
      else break;
      
    }
    
    tnode_vec.push_back(n);
    tindex_vec.push_back(pos);
    
  }      
  
}



void ParticleAssociations::AddShower(Int_t const index,
				     ertool::NodeID_t const n,
				     geoalgo::Point_t const & vert) {
  
  if(index > tassociations.size() - 1 || index < 0) {
    std::cout << "No association with index: " << index << "\n";
    return;
  }
  
  ParticleAssociation & association = tassociations.at(index);
  std::vector<ertool::NodeID_t> const & group = association.GetCluster();
  
  if(std::find(group.begin(), group.end(), n) != group.end()) {
    std::cout << "NodeID: " << n << " already added\n";
    return;
  }
  
  tnode_vec.push_back(n);
  tindex_vec.push_back(index);
  
  association.AddShower(n, vert);
  
}



std::vector<Size_t> ParticleAssociations::GetIndicesFromNode(ertool::NodeID_t const n) {

  auto const nv_itb = tnode_vec.begin();
  auto const nv_ite = tnode_vec.end();
  auto nv_it = nv_itb;
  
  std::vector<Size_t> indices;
  
  while(nv_it != nv_ite) {
    
    nv_it = std::find(nv_it, nv_ite, n);
    
    if(nv_it != nv_ite) {
      indices.push_back(nv_it - nv_itb);
      ++nv_it;
    }
    
  }
  
  return indices;
  
}



void ParticleAssociations::PrintAssociations() const {

  std::cout << "----------------------------------------\n\n";
  
  for(Size_t i = 0; i < tassociations.size(); ++i) {
    
    std::cout << "Association: " << i << "\n";
    
    tassociations.at(i).PrintAssociation();
    
    std::cout << "\n";
    
  }
  
  std::cout << "\n";
  
}



void ParticleAssociations::PrintNodes() const  {
  
  std::cout << "Nodes:\n";
  
  for(Size_t i = 0; i < tnode_vec.size(); ++i)
    std::cout << "Nodes: " << tnode_vec.at(i) << " Index: "
	      << tindex_vec.at(i) << "\n";
  
}



void ParticleAssociations::Test() const {

  for(ParticleAssociation const & pae : tassociations) {
    
    for(std::pair<Int_t, ertool::NodeID_t> const & pair : pae.GetConnections()) {
      
      Int_t const i = pair.first;
      
      std::vector<ertool::NodeID_t> const & c = tassociations.at(i).GetCluster();
      auto c_itb = c.begin();
      auto c_ite = c.end();
      
      auto c_it = c_ite;
      
      for(ertool::NodeID_t const n : pae.GetCluster()) {
	
	c_it = std::find(c_itb, c_ite, n);
	
	if(c_it != c_ite) break;
	
      }
      
      if(c_it == c_ite) std::cout << "Complain\n";
      
    }
    
  }
  
}



void ParticleAssociations::NodeCheck(ertool::ParticleGraph const & graph) {

  std::vector<ertool::NodeID_t> nodes;

  for(ertool::NodeID_t const n : tnode_vec) {
    
    if(nodes.size() < n+1) nodes.resize(n+1, 0);
    
    ++nodes.at(n);
    
  }
  
  for(Size_t i = 0; i < nodes.size(); ++i) {
    
    Size_t const s = nodes.at(i);
    
    if(s == 0) continue;
    
    ertool::Particle const & p = graph.GetParticle(i);
    
    if(p.RecoType() == ertool::kTrack && s > 2) 
      std::cout << "track > 2 entries: " << s << "\n";
    
    else if(p.RecoType() == ertool::kShower && s > 1)
      std::cout << "shower > 1 entries: " << s << "\n";
    
  }
  
}



#endif
