#ifndef ERTOOL_ERALGOVERTEXBUILDER_CXX
#define ERTOOL_ERALGOVERTEXBUILDER_CXX

#include "ERAlgoVertexBuilder.h"

#include "LArUtil/Geometry.h"

namespace ertool {



  ERAlgoVertexBuilder::ERAlgoVertexBuilder
  (Double_t const start_prox,
   Double_t const max_rad,
   Bool_t const withTrackDir,
   std::string const primary_vertex_selection,
   Bool_t const showerproj,
   Double_t const shower_prox,
   Double_t const cpoa_vert_prox,
   Double_t const cpoa_trackend_prox,
   const std::string& name) :
    AlgoBase(name),
    tstart_prox(start_prox),
    tmax_rad(max_rad),
    twithTrackDir(withTrackDir),
    tprimary_vertex_selection(primary_vertex_selection),
    tshowerproj(showerproj),
    tshower_prox(shower_prox),
    tcpoa_vert_prox(cpoa_vert_prox),
    tcpoa_trackend_prox(cpoa_trackend_prox),
    tverbose(false),
    tevent(-1) {

    if(tprimary_vertex_selection != mostupstream &&
       tprimary_vertex_selection != mostchildren &&
       tprimary_vertex_selection != mostenergy &&
       tprimary_vertex_selection != smallestsphere && 
       tprimary_vertex_selection != trackdirection) {

      std::cout << "Primary vertex selection option not valid\n";
      exit(0);

    }

    tree = new TTree("ERAlgoVertexBuilder", "");
    tree->Branch("event_id", &event_id, "event_id/I");
    tree->Branch("association_number", &association_number, "association_number/I");
    tree->Branch("found_vertices", &found_vertices, "found_vertices/I");
    tree->Branch("loop_counter", &loop_counter, "loop_counter/I");
    tree->Branch("lone_track_counter", &lone_track_counter, "lone_track_counter/I");
    tree->Branch("vertices_lonetracks", &vertices_lonetracks, "vertices_lonetracks/I");
    tree->Branch("radius", &radius, "radius/D");
    tree->Branch("multi_track_end_vertex",
		 &multi_track_end_vertex,
		 "multi_track_end_vertex/I");

  }



  void ERAlgoVertexBuilder::AcceptPSet(const ::fcllite::PSet& cfg){}



  void ERAlgoVertexBuilder::ProcessBegin() {

    //Set up TPC fiducial volume

    Double_t const cut = 1;

    Double_t const xfid = cut;
    Double_t const yfid = cut;
    Double_t const zfid = cut;

    larutil::Geometry const * geo = larutil::Geometry::GetME();
    
    volume.Min(xfid,
	       -geo->DetHalfHeight() + yfid,
	       zfid);

    volume.Max(2*geo->DetHalfWidth() - xfid,
	       geo->DetHalfHeight() - yfid,
	       geo->DetLength() - zfid);
    
  }



  void ERAlgoVertexBuilder::EndReconstruct(ParticleGraph const & graph) {

    if(tverbose) std::cout << graph.Diagram() << "\n";

  }



  void ERAlgoVertexBuilder::WithTrackDir(const EventData &data,
					 ParticleGraph& graph) {

    if(tverbose) std::cout << "Event: " << data.Event_ID() << "\n"
			   << "============================================\n";

    std::list<NodeID_t> pn;

    /*Loop over primary nodes and add them to pn, if the node corresponds to a
      track only add it if the track start is contained within the fiducial
      volume*/

    for(NodeID_t const n : graph.GetPrimaryNodes()) {

      Particle const & p = graph.GetParticle(n);

      if(p.RecoType() == kTrack &&
	 volume.Contain(p.Vertex()))	
	pn.push_back(n);

    }

    /*Compare all primary nodes in pn with all other primary nodes and find the
      primary node pair whose vertices are closest together*/

    while(pn.size() > 0) {

      NodeID_t best_objectA = -1;
      NodeID_t best_objectB = -1;
      Double_t best_dist = tstart_prox;
      
      //Main comparision loop

      for(NodeID_t const mp : pn) {
	
	geoalgo::Point_t const & mpv = graph.GetParticle(mp).Vertex();
	
	//Secondary comparison loop

	for(NodeID_t const cp : pn) {
	  
	  if(cp == mp) continue;
	  
	  //Find the distance between primary node vertices 

	  Double_t const dist =
	    graph.GetParticle(cp).Vertex().Dist(mpv);
	  
	  /*Vertex distance smaller than shorest distance becomes new shortest
	    distance*/

	  if(dist < best_dist) {

	    //Remember primary node pair and distance
	    
	    best_objectA = mp;
	    best_objectB = cp;
	    best_dist = dist;
	    
	  }
	  
	}
	
      }
      
      /*If a closest pair within tstart_prox of each other is not found: erase
	remaining primary nodes of pn, if a primary node is a track create a
	kInvisible "vertex" particle whose position is the front of the track*/

      if(best_objectA == -1) {
	
	for(auto pn_it = pn.begin(); pn_it != pn.end();) {
	  
	  NodeID_t const n = *pn_it;

	  pn_it = pn.erase(pn_it);

	  Particle const & p = graph.GetParticle(n);

	  if(p.RecoType() != kTrack) continue;
	  
	  Particle & vp = graph.CreateParticle();
	  graph.SetPrimary(vp.ID());
	  vp.SetParticleInfo(0, vp.Mass(), vp.Vertex(), vp.Momentum());
	  graph.SetParentage(vp.ID(), n);
	  
	}
	
	EndReconstruct(graph);
	
	return;
	
      }
      
      //Else a best match within tstart_prox distance has been found

      /*Make a vector to keep track of primary nodes associated with the vertex
	candidate*/
      std::vector<NodeID_t> vc;
      vc.push_back(best_objectA);
      vc.push_back(best_objectB);
      
      /*Make a vector to keep track of primary node positions associated with
	the vertex candidate*/
      std::vector<geoalgo::Point_t> vcp;
      vcp.push_back(graph.GetParticle(best_objectA).Vertex());
      vcp.push_back(graph.GetParticle(best_objectB).Vertex());
 
      /*Create an initial minimally bounding sphere using the closest matching
	pair positions*/
      geoalgo::Sphere s(algo.boundingSphere(vcp));

      /*Loop over remaining primary nodes (not already associated with the
	vertex candidate) closest to the sphere centre within tstart_prox. Make
	a new sphere that includes this new primary node, repeat for all 
	primary nodes witin tstart_prox of each sphere centre. When no more
	primary nodes within tstart_prox are found: end the loop.*/

      do {

	NodeID_t best_objectC = -1;
	Double_t best_dist = tstart_prox;

	//Get centre of current sphere

	geoalgo::Point_t const & sv = s.Center();

	for(NodeID_t const cp : pn) {
	
	  //Skip node in pn if it has already been added to the vertex
	  
	  if(std::find(vc.begin(), vc.end(), cp) != vc.end()) continue;

	  //Find distance between primary node position and sphere centre

	  Double_t const dist = graph.GetParticle(cp).Vertex().Dist(sv);
	
	  //If primary node is closer to sphere centre, replace

	  if(dist < best_dist) {
	    
	    best_objectC = cp;
	    best_dist = dist;
	    
	  }
	  
	}
	
	/*If no primary node within tstart_prox of the sphere centre is found,
	  end the loop*/ 

	if(best_objectC == -1) break;
	
	/*Add primary node closest to sphere centre to respective node and
	  position vectors*/
	
	vc.push_back(best_objectC);
	vcp.push_back(graph.GetParticle(best_objectC).Vertex());
	
	//Recalculate sphere to include new primary node

	s = algo.boundingSphere(vcp);

      } while(true);

      //If sphere radius is too big, do something

      if(s.Radius() > tmax_rad) {
	
	//do stuff
	std::cout << "Radius too big: " << s.Radius() << "\n";
	
      }
      
      /*Else sphere is of acceptable size, create a kInvisible vertex particle
	with pdg = 0 and add all associated primary nodes as children*/

      else {
	
	Particle & p = graph.CreateParticle();
	NodeID_t const pid = p.ID();
	graph.SetPrimary(pid);
	p.SetParticleInfo(0, p.Mass(), s.Center(), p.Momentum());

	for(NodeID_t const v : vc) {
	  
	  graph.SetParentage(pid, v);
	  pn.erase(std::find(pn.begin(), pn.end(), v));
	  
	}
	
      }
     
      /*Repeat whole process until either there are no primary nodes left in pn
	or no more pairs within start_prox of each other can be found*/
 
    }

  }



  class ERAlgoVertexBuilder::ParticleAssociation {

    std::vector<NodeID_t> tcluster;
    std::vector<geoalgo::Point_t> tvertices;
    geoalgo::Sphere tsphere;
    Double_t tgoodness;

    std::multimap<Int_t, NodeID_t> tcluster_connection;
    
  public:
    
    ParticleAssociation(std::vector<NodeID_t> const & cluster,
			std::vector<geoalgo::Point_t> const & vertices,
			geoalgo::Sphere const & sphere,
			Double_t const goodness) :
      tcluster(cluster),
      tvertices(vertices),
      tsphere(sphere),
      tgoodness(goodness){}
    
    void AddConnection(Int_t const i, NodeID_t const n) {
      tcluster_connection.emplace(i, n);
    }
    
    void AddObject(NodeID_t const n, geoalgo::Point_t const & vert) {
      tcluster.push_back(n);
      tvertices.push_back(vert);
    }

    std::vector<NodeID_t> const & GetCluster() const {
      return tcluster;
    }

    std::vector<geoalgo::Point_t> const & GetVertices() const {
      return tvertices;
    }

    geoalgo::Point_t const & GetVertexFromNode(NodeID_t const n) const {

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

    std::multimap<Int_t, NodeID_t> const & GetConnections() const {
      return tcluster_connection;
    }

    void PrintAssociation() const {
            
      for(Size_t i = 0; i < tcluster.size(); ++i)
	std::cout << "Node: " << tcluster.at(i) << " Vertices: " 
		  << tvertices.at(i) << "\n";
      
      std::cout << "\nSphere: " << tsphere.Center() << ", "
		<< tsphere.Radius() << "\n";
      
      std::cout << "\nConnections:\n";
      for(std::pair<Int_t, NodeID_t> const & pair : tcluster_connection) 
	std::cout << "Index: " << pair.first << " NodeID: " << pair.second << "\n";
      
    }
    
  };
  
  
  
  class ERAlgoVertexBuilder::ParticleAssociations {

    std::vector<ParticleAssociation> tassociations;
    std::vector<NodeID_t> tnode_vec;
    std::vector<Int_t> tindex_vec;

  public:

    void AddAssociation(std::vector<NodeID_t> const & nodes,
			std::vector<geoalgo::Point_t> const & vertices,
			geoalgo::Sphere const & sphere,
			Double_t const goodness) {
      
      tassociations.push_back
	(ParticleAssociation(nodes, vertices, sphere, goodness));
     
      Int_t const pos = tassociations.size() - 1;

      for(NodeID_t const n : nodes) {

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

    void AddObject(Int_t const index,
		   NodeID_t const n,
		   geoalgo::Point_t const & vert) {
	
      if(index > tassociations.size() - 1 || index < 0) {
	std::cout << "No association with index: " << index << "\n";
	return;
      }

      ParticleAssociation & association = tassociations.at(index);
      std::vector<NodeID_t> const & group = association.GetCluster();

      if(std::find(group.begin(), group.end(), n) != group.end()) {
	return;
      }

      tnode_vec.push_back(n);
      tindex_vec.push_back(index);

      association.AddObject(n, vert);

      auto const nv_itb = tnode_vec.begin();
      auto const nv_ite = tnode_vec.end();
      
      for(auto nv_it = nv_itb; nv_it != nv_ite; ++nv_it) {
	
	nv_it = std::find(nv_it, nv_ite, n);
	
	if(nv_it != nv_ite) {
	  
	  Int_t const index_other = tindex_vec.at(nv_it - nv_itb);
	  
	  tassociations.at(index_other).AddConnection(index, n);
	  tassociations.at(index).AddConnection(index_other, n);
	  
	}
	
	else break;
	
      } 
      
    }

    std::vector<ParticleAssociation> const & GetAssociations() const {
      return tassociations;
    }

    std::vector<NodeID_t> const & GetNodes() const {
      return tnode_vec;
    }
    
    std::vector<Int_t> const & GetIndices() const {
      return tindex_vec;
    }

    std::vector<Size_t> GetIndicesFromNode(NodeID_t const n) {

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

    void PrintAssociations() const {

      std::cout << "----------------------------------------\n\n";

      for(Size_t i = 0; i < tassociations.size(); ++i) {

	std::cout << "Association: " << i << "\n";

	tassociations.at(i).PrintAssociation();
	
	std::cout << "\n";

      }

      std::cout << "\n";

    }

    void PrintNodes() const  {

      std::cout << "Nodes:\n";

      for(Size_t i = 0; i < tnode_vec.size(); ++i)
	std::cout << "Nodes: " << tnode_vec.at(i) << " Index: "
		  << tindex_vec.at(i) << "\n";

    }

 
    void Test() const {                                                        
      
      for(ParticleAssociation const & pae : tassociations) {
	
	for(std::pair<Int_t, NodeID_t> const & pair : pae.GetConnections()) {
	  
	  Int_t const i = pair.first;

	  std::vector<NodeID_t> const & c = tassociations.at(i).GetCluster();
	  auto c_itb = c.begin();
	  auto c_ite = c.end();
	  
	  auto c_it = c_ite;
	  
	  for(NodeID_t const n : pae.GetCluster()) {
	    
	    c_it = std::find(c_itb, c_ite, n);
	    
	    if(c_it != c_ite) break;
	    
	  }
	  
	  if(c_it == c_ite) std::cout << "Complain\n";
	  
	}
	
      }

    }

    /*Check to see if a shower has been added more than once and a track has
      been added more than twice (neither of which should happen)*/
    void NodeCheck(ParticleGraph const & graph) {

      std::vector<NodeID_t> nodes;

      for(NodeID_t const n : tnode_vec) {
	
	if(nodes.size() < n+1) nodes.resize(n+1, 0);
	
	++nodes.at(n);
	
      }

      for(Size_t i = 0; i < nodes.size(); ++i) {

        Size_t const s = nodes.at(i);

	if(s == 0) continue;

	Particle const & p = graph.GetParticle(i);

	if(p.RecoType() == kTrack && s > 2) 
	  std::cout << "track > 2 entries: " << s << "\n";

	else if(p.RecoType() == kShower && s > 1)
	  std::cout << "shower > 1 entries: " << s << "\n";

      }

    }
      
  };


  
  class ERAlgoVertexBuilder::ParticleGraphSetter {

    EventData const * tdata;
    ParticleGraph * tgraph;
    ParticleAssociations const * tpa;
    std::vector<Int_t> tskip;
    std::vector<NodeID_t> tnodelist;
    Int_t tloopcounter;

  public:

    ParticleGraphSetter(EventData const & data,
			ParticleGraph & graph,
			ParticleAssociations const & pa) :
      tdata(&data),
      tgraph(&graph),
      tpa(&pa),
      tloopcounter(0){}

    void runstart(NodeID_t const parent,
		  Int_t const i) {
      
      tnodelist.clear();
      runrec(parent, i);

    }

    void runrec(NodeID_t const parent,
		Int_t const i) {
      
      std::vector<NodeID_t> bad_nodes;

      tskip.push_back(i);

      ParticleAssociation const & pa = tpa->GetAssociations().at(i);

      std::vector<NodeID_t> const & c = pa.GetCluster();
      std::vector<geoalgo::Point_t> const & v = pa.GetVertices();
      for(Size_t j = 0; j < c.size(); ++j) {
	
	NodeID_t const n = c.at(j);

	if(n == parent) continue;

	auto it = std::find(tnodelist.begin(), tnodelist.end(), n);

	if(it != tnodelist.end()) {
	  ++tloopcounter;
	  bad_nodes.push_back(*it);
	} 

	else {
	  Particle & p = tgraph->GetParticle(n);
	  //ugly way of setting energy
	  Double_t energy = 0;
	  if(p.RecoType() == kTrack)
	    energy = tdata->Track(p.RecoID())._energy; 
	  if(p.RecoType() == kShower)
	    energy = tdata->Shower(p.RecoID())._energy;
	  p.SetParticleInfo(p.PdgCode(),
			    p.Mass(),
			    v.at(j),
			    geoalgo::Point_t(energy, 0, 0));
	  tgraph->SetParentage(parent, n);
	}	  

	tnodelist.push_back(n);

      }

      for(std::pair<Int_t, NodeID_t> const & pair : pa.GetConnections()) {
	
	if(std::find(tskip.begin(), tskip.end(), pair.first) != tskip.end())
	  continue;
	if(std::find(bad_nodes.begin(), bad_nodes.end(), pair.second) !=
	   bad_nodes.end())
	  continue;
	
	runrec(tgraph->GetParticle(pair.second).ID(), pair.first);
	
      }
      
    }
    
    void runstartempty(NodeID_t const parent,
		       Int_t const i) {
      
      tnodelist.clear();
      runrecempty(parent, i);
      
    }
 
    void runrecempty(NodeID_t const parent,
		     Int_t const i) {
      
      std::vector<NodeID_t> bad_nodes;
    
      tskip.push_back(i);

      ParticleAssociation const & pa = tpa->GetAssociations().at(i);

      std::vector<NodeID_t> const & c = pa.GetCluster();
      for(Size_t j = 0; j < c.size(); ++j) {
	
	NodeID_t const n = c.at(j);

	if(n == parent) continue;
	auto it = std::find(tnodelist.begin(), tnodelist.end(), n);
	if(it != tnodelist.end()) {
	  ++tloopcounter;
	  bad_nodes.push_back(*it);
	} 

	tnodelist.push_back(n);

      }

      for(std::pair<Int_t, NodeID_t> const & pair : pa.GetConnections()) {
	
	if(std::find(tskip.begin(), tskip.end(), pair.first) != tskip.end())
	  continue;
	if(std::find(bad_nodes.begin(), bad_nodes.end(), pair.second) !=
	   bad_nodes.end())
	  continue;
	
	runrecempty(tgraph->GetParticle(pair.second).ID(), pair.first);
	
      }
      
    }
    
    std::vector<Int_t> const & GetSkip() const {
      return tskip;
    }

    Int_t GetLoopCounter() {
      return tloopcounter;
    }

  };


  
  geoalgo::Point_t const * ERAlgoVertexBuilder::GetUpstreamPrimary
  (ParticleAssociations const & pas, 
   std::vector<Int_t> const & skip,
   Int_t & index) {

    std::vector<ParticleAssociation> const & pav = pas.GetAssociations();

    index = -1;
    geoalgo::Point_t const * sc = nullptr;
    Double_t zmin = 2000;
    
    for(Size_t i = 0; i < pav.size(); ++i) {
      
      if(std::find(skip.begin(), skip.end(), i) != skip.end()) {
	continue;
      }
      
      geoalgo::Point_t const * const s = &pav.at(i).GetSphere().Center();
      Double_t const z = s->at(2);
      
      if(z < zmin) {
	index = i;
	sc = s;
	zmin = z;
      }
      
    }
    
    return sc;
    
  }



  geoalgo::Point_t const * ERAlgoVertexBuilder::SubGetUpstream
  (ParticleAssociations const & pas,
   std::vector<Int_t> const & indices,
   Int_t & index) {

    std::vector<ParticleAssociation> const & pav = pas.GetAssociations();

    index = -1;
    geoalgo::Point_t const * sc = nullptr;
    Double_t zmin = 2000;
    
    for(Int_t const i : indices) {
      
      geoalgo::Point_t const * const s = &pav.at(i).GetSphere().Center();
      Double_t const z = s->at(2);
      
      if(z < zmin) {
	index = i;
	sc = s;
	zmin = z;
      }
      
    }
    
    return sc;
    
  }


  
  geoalgo::Point_t const * ERAlgoVertexBuilder::SubSmallestSphere
  (ParticleAssociations const & pas,
   std::vector<Int_t> const & indices,
   Int_t & index) {

    std::vector<ParticleAssociation> const & pav = pas.GetAssociations();
 
    index = -1;
    geoalgo::Point_t const * sc = nullptr;
    Double_t smallest_radius = 2000;
  
    for(Int_t const i : indices) {
    
      Double_t const gradius = pav.at(i).GetGoodness();

      if(gradius < smallest_radius) {
	index = i;
	sc = &pav.at(i).GetSphere().Center();
	smallest_radius = gradius;
      }

    }
    
    return sc;
    
  }



  geoalgo::Point_t const * ERAlgoVertexBuilder::SubMostEnergy
  (EventData const & data,
   ParticleGraph const & graph,
   ParticleAssociations const & pas,
   std::vector<Int_t> const & indices,
   Int_t & index) {

    std::vector<ParticleAssociation> const & pav = pas.GetAssociations();
    
    index = -1;
    geoalgo::Point_t const * sc = nullptr;
    Double_t most_energy = 0;
  
    for(Int_t const i : indices) {
        
      Double_t energy = 0;

      for(NodeID_t const n : pav.at(i).GetCluster()) {

	Particle const & p = graph.GetParticle(n);

	if(p.RecoType() == kTrack) energy += data.Track(p.RecoID())._energy;
	else if (p.RecoType() == kShower)
	  energy += data.Shower(p.RecoID())._energy;

      }

      if(energy > most_energy) {
	index = i;
	sc = &pav.at(i).GetSphere().Center();
	most_energy = energy;	
      }

    }
    
    return sc;
    
  }
 


  geoalgo::Point_t const * ERAlgoVertexBuilder::GetMostChildrenPrimary
  (EventData const & data,
   ParticleGraph const & graph,
   ParticleAssociations const & pas, 
   std::vector<Int_t> const & skip,
   Int_t & index) {

    std::vector<ParticleAssociation> const & pav = pas.GetAssociations();
    
    index = -1;
    Size_t most_children = 0;
    std::vector<Int_t> index_vec;

    for(Size_t i = 0; i < pav.size(); ++i) {
      
      if(std::find(skip.begin(), skip.end(), i) != skip.end()) {
	continue;
      }
    
      Size_t const children = pav.at(i).GetCluster().size();

      if(children >= most_children) {

	if(most_children != children) index_vec.clear();
	 
	most_children = children;
	index_vec.push_back(i);
     
      }

    }

    if(index_vec.size() == 1) {
      index = index_vec.front();
      return &pav.at(index).GetSphere().Center();
    }

    //return SubGetUpstream(pas, index_vec, index);
    //return SubSmallestSphere(pas, index_vec, index);
    return SubMostEnergy(data, graph, pas, index_vec, index);

  }



  geoalgo::Point_t const * ERAlgoVertexBuilder::GetSmallestSpherePrimary
  (ParticleAssociations const & pas, 
   std::vector<Int_t> const & skip,
   Int_t & index) {

    std::vector<ParticleAssociation> const & pav = pas.GetAssociations();
    
    index = -1;
    geoalgo::Point_t const * sc = nullptr;
    Double_t smallest_radius = 2000;
  
    for(Size_t i = 0; i < pav.size(); ++i) {
      
      if(std::find(skip.begin(), skip.end(), i) != skip.end()) {
	continue;
      }
    
      Double_t const gradius = pav.at(i).GetGoodness();

      if(gradius < smallest_radius) {
	index = i;
	sc = &pav.at(i).GetSphere().Center();
	smallest_radius = gradius;
      }

    }
    
    return sc;

  }



  geoalgo::Point_t const * ERAlgoVertexBuilder::GetMostEnergyPrimary
  (EventData const & data,
   ParticleGraph const & graph,
   ParticleAssociations const & pas, 
   std::vector<Int_t> const & skip,
   Int_t & index) {

    std::vector<ParticleAssociation> const & pav = pas.GetAssociations();
    
    index = -1;
    geoalgo::Point_t const * sc = nullptr;
    Double_t most_energy = 0;
  
    for(Size_t i = 0; i < pav.size(); ++i) {
      
      if(std::find(skip.begin(), skip.end(), i) != skip.end()) {
	continue;
      }
    
      Double_t energy = 0;

      for(NodeID_t const n : pav.at(i).GetCluster()) {

	Particle const & p = graph.GetParticle(n);

	if(p.RecoType() == kTrack) energy += data.Track(p.RecoID())._energy;
	else if (p.RecoType() == kShower)
	  energy += data.Shower(p.RecoID())._energy;

      }

      if(energy > most_energy) {
	index = i;
	sc = &pav.at(i).GetSphere().Center();
	most_energy = energy;	
      }

    }
    
    return sc;

  }



  void ERAlgoVertexBuilder::AddAllLoneTracks
  (const EventData &data,
   ParticleGraph & graph,
   NodeID_t const n) {

    for(NodeID_t const gn : graph.GetParticleNodes(kTrack)) {
      
      Particle & p = graph.GetParticle(gn);
      
      if(p.Parent() != gn || gn == n)
	continue;
      
      Track const & t = data.Track(p.RecoID());
    
      geoalgo::Point_t const * track_end = nullptr;
      Double_t zmin = 2000;

      if(t.front().at(2) < zmin) {
	track_end = &t.front();
	zmin = t.front().at(2);
      }

      if(t.back().at(2) < zmin) {
	track_end = &t.back();
	zmin = t.back().at(2);
      }

      if(track_end) {
	
	Particle & vert = graph.CreateParticle();
	NodeID_t const pid = vert.ID();
	graph.SetPrimary(pid);
	vert.SetParticleInfo(0, vert.Mass(), *track_end, vert.Momentum());    
	p.SetParticleInfo(p.PdgCode(), p.Mass(), *track_end, p.Momentum());
	graph.SetParentage(pid, gn);

      }
      
      else std::cout << "Warning: No track end pointer\n";
      
    }
    
  }



  void ERAlgoVertexBuilder::AddUpstreamLoneTrack
  (const EventData &data,
   ParticleGraph & graph) {
    
    Double_t zmin = 2000;
    NodeID_t upstream_track_node = kINVALID_NODE_ID;
    geoalgo::Point_t const * track_end = nullptr;

    for(NodeID_t const gn : graph.GetParticleNodes(kTrack)) {
      
      Particle & p = graph.GetParticle(gn);
            
      if(p.Parent() != gn)
	continue;
      
      Track const & t = data.Track(p.RecoID());
    
      if(t.front().at(2) < zmin) {
	track_end = &t.front();
	upstream_track_node = gn;
	zmin = t.front().at(2);
      }

      if(t.back().at(2) < zmin) {
	track_end = &t.back();
	upstream_track_node = gn;
	zmin = t.back().at(2);
      }
  
    }

    if(track_end) {

      Particle & vert = graph.CreateParticle();
      NodeID_t const pid = vert.ID();
      graph.SetPrimary(pid);
      vert.SetParticleInfo(0, vert.Mass(), *track_end, vert.Momentum());    
      Particle & p = graph.GetParticle(upstream_track_node);
      p.SetParticleInfo(p.PdgCode(), p.Mass(), *track_end, p.Momentum());
      graph.SetParentage(pid, upstream_track_node);
      
    }
    
  }



  void ERAlgoVertexBuilder::AddAllLoneShowers
  (const EventData &data,
   ParticleGraph & graph,
   NodeID_t const n) {

    for(NodeID_t const gn : graph.GetParticleNodes(kShower)) {
      
      Particle & p = graph.GetParticle(gn);
      
      if(p.Parent() != gn || gn == n)
	continue;
     
      Shower const & s = data.Shower(p.RecoID());
    
      Particle & vert = graph.CreateParticle();
      NodeID_t const pid = vert.ID();
      graph.SetPrimary(pid);
      vert.SetParticleInfo(0, vert.Mass(), s.Start(), vert.Momentum());    
      p.SetParticleInfo(p.PdgCode(), p.Mass(), s.Start(), p.Momentum());
      graph.SetParentage(pid, gn);
      
    }
    
  }

  

  void ERAlgoVertexBuilder::AddTracksAndShowers
  (const EventData &data,
   ParticleGraph & graph) {
    
    Double_t zmin = 2000;
    NodeID_t upstream_node = kINVALID_NODE_ID;
    geoalgo::Point_t const * end = nullptr;

    for(NodeID_t const gn : graph.GetParticleNodes(kTrack)) {
      
      Particle & p = graph.GetParticle(gn);
            
      if(p.Parent() != gn)
	continue;
      
      Track const & t = data.Track(p.RecoID());
    
      if(t.front().at(2) < zmin) {
	end = &t.front();
	upstream_node = gn;
	zmin = t.front().at(2);
      }

      if(t.back().at(2) < zmin) {
	end = &t.back();
	upstream_node = gn;
	zmin = t.back().at(2);
      }

    }

    for(NodeID_t const gn : graph.GetParticleNodes(kShower)) {
      
      Particle & p = graph.GetParticle(gn);
            
      if(p.Parent() != gn)
	continue;
      
      Shower const & s = data.Shower(p.RecoID());
    
      if(s.Start().at(2) < zmin) {
	end = &s.Start();
	upstream_node = gn;
	zmin = s.Start().at(2);
      }
   
    }

    if(end) {

      Particle & vert = graph.CreateParticle();
      NodeID_t const pid = vert.ID();
      graph.SetPrimary(pid);
      vert.SetParticleInfo(1, vert.Mass(), *end, vert.Momentum());    
      Particle & p = graph.GetParticle(upstream_node);
      p.SetParticleInfo(p.PdgCode(), p.Mass(), *end, p.Momentum());
      graph.SetParentage(pid, upstream_node);
      
      AddAllLoneTracks(data, graph, upstream_node);
      AddAllLoneTracks(data, graph, upstream_node);
      
    }

  }



  void ERAlgoVertexBuilder::ShowerProjection
  (const EventData & data,
   ParticleGraph & graph,
   ParticleAssociations & pas) {    
    
    //tverbose = true;

    if(tverbose) std::cout << "Shower Projection\n";

    std::map<NodeID_t, Shower const *> shower_map;
  
    for(NodeID_t const n : graph.GetParticleNodes(kShower))
      shower_map.emplace(n, &data.Shower(graph.GetParticle(n).RecoID()));
  
    std::vector<ParticleAssociation> const & associations =
      pas.GetAssociations();

    while(shower_map.size()) {

      if(tverbose)
	std::cout << "\tshower_map wloop, size: " << shower_map.size() << "\n";

      NodeID_t best_shower_id = kINVALID_NODE_ID;
      NodeID_t best_other_id = kINVALID_NODE_ID;
      Int_t index = -1;
      Double_t best_dist = tshower_prox;
      geoalgo::Point_t best_vert(2000, 2000, 2000);    
      geoalgo::Point_t temp_vert(2000, 2000, 2000);
  
      for(auto const & c : shower_map) {
	
	if(tverbose)
	  std::cout << "\t\tshower_map primary floop, id: " << c.first << "\n";

	for(auto const & c2 : shower_map) {

	  if(tverbose)
	    std::cout << "\t\t\tshower_map secondary floop, id: " << c2.first
		      << "\n";

	  if(c2.first == c.first) {
	    if(tverbose) std::cout << "\t\t\t\tmatching id, continue\n";
	    continue;
	  }
	  
	  Double_t const dist =
	    findrel.FindClosestApproach(*c2.second, *c.second, temp_vert);
	  
	  if(tverbose)
	    std::cout << "\t\t\tdist: " << dist << " < best-dist: "
		      << best_dist << " ?\n";

	  if(dist < best_dist) {
	    
	    if(tverbose) std::cout << "\t\t\t\tyes\n";

	    best_shower_id = c.first;
	    best_other_id = c2.first;
	    best_vert = temp_vert;
	    best_dist = dist;
	    index = -1;
	    
	  }
	  
	}

	if(tverbose) std::cout << "\t\tshower_map secondary floop end\n";

	for(NodeID_t const n : graph.GetParticleNodes(kTrack)) {

	  if(tverbose)
	    std::cout << "\t\t\ttrack secondary floop, id: " << n << "\n";

	  Track const & t = data.Track(graph.GetParticle(n).RecoID());
		  
	  geoalgo::Point_t dont_care;

	  Double_t const dist =
	    sqrt(algo.SqDist(t, 
			     geoalgo::HalfLine(c.second->Start(),
					       c.second->Dir()*-1),
			     temp_vert,
			     dont_care));

	  if(tverbose)
	    std::cout << "\t\t\tdist: " << dist << " < best_dist: "
		      << best_dist << " ?\n";
	  
	  if(dist < best_dist) {
	    
	    if(tverbose) std::cout << "\t\t\t\tyes\n";

	    best_shower_id = c.first;
	    best_other_id = n;
	    best_vert = temp_vert;
	    best_dist = dist;
	    index  = -1;

	  }

	}

	if(tverbose) std::cout << "\t\ttrack secondary floop end\n";

	for(Size_t i = 0; i < associations.size(); ++i) {

	  if(tverbose)
	    std::cout << "\t\t\tassociation secondary floop, index: "
		      << i << "\n";

	  ParticleAssociation const & pa = associations.at(i);

	  Double_t const dist =
	    sqrt(algo.SqDist(pa.GetSphere().Center(), *c.second));

	  if(tverbose)
	    std::cout << "\t\t\tdist: " << dist << " < best-dist: "
		      << best_dist << " ?\n";

	  if(dist < best_dist) {

	    if(tverbose) std::cout << "\t\t\t\tyes\n";

	    best_shower_id = c.first;
	    index = i;
	    best_vert = algo.ClosestPt(pa.GetSphere().Center(), *c.second);
	    best_dist = dist;
	    best_other_id = kINVALID_NODE_ID;

	  }

	}

	if(tverbose) std::cout << "\t\tassociation secondary floop end\n";
	
      }
      
      if(tverbose) std::cout << "\tshower_map primary floop end\n"
			     << "\tbest_dist: " << best_dist << " >= "
			     << tshower_prox << " ?\n";

      if(best_dist >= tshower_prox) {
	if(tverbose) std::cout << "\t\tyes, return\n";	
	return;
      }

      if(tverbose) std::cout << "\tindex: " << index << " == -1 ?\n";

      if(index == -1) {

	if(tverbose) std::cout << "\t\tyes\n";

	Particle const & p1 = graph.GetParticle(best_shower_id);
	Particle const & p2 = graph.GetParticle(best_other_id);

	Int_t association_index = -1;
	Double_t best_association_dist = tcpoa_vert_prox;

	if(tverbose)
	  std::cout << "\t\tassociation floop, size: "
		    << associations.size() << "\n";

	for(Size_t i = 0; i < associations.size(); ++i) {

	  if(tverbose)
	    std::cout << "\t\t\tassociation floop, index: "
		      << i << "\n";

	  Double_t const dist =
	    best_vert.Dist(associations.at(i).GetSphere().Center());

	  if(tverbose)
	    std::cout << "\t\t\tdist: " << dist
		      << " < best_association_dist: "
		      << best_association_dist << "\n";

	  if(dist < best_association_dist) {
	    
	    if(tverbose)
	      std::cout << "\t\t\t\tyes\n";
	    
	    association_index = i;
	    best_association_dist = dist;

	  }

	}

	if(tverbose) std::cout << "\t\tOther reco type:\n";	

	if(p2.RecoType() == kShower) {

	  if(tverbose)
	    std::cout << "\t\t\tshower\n"
		      << "\t\t\tbest_association_dist: "
		      << best_association_dist
		      << " < tcpoa_vert_prox: "
		      << tcpoa_vert_prox << " ?\n";	

	  if(best_association_dist < tcpoa_vert_prox) {
	    if(tverbose) std::cout << "\t\t\t\tyes, add showers to association: "
				   << association_index << "\n";	
	    pas.AddObject(association_index, best_shower_id, best_vert);
	    pas.AddObject(association_index, best_other_id, best_vert);
	  }

	  else {

	    NodeID_t best_track = kINVALID_NODE_ID;
	    geoalgo::Point_t const * best_tp = nullptr;
	    Double_t best_showerp_dist = tcpoa_vert_prox;

	    for(NodeID_t const n : graph.GetParticleNodes(kTrack)) {

	      Track const & t = data.Track(graph.GetParticle(n).RecoID());

	      Double_t const trackend_dist = t.back().Dist(best_vert);
	      if(trackend_dist < best_showerp_dist) {
		best_track = n;
		best_tp = &t.back();
		best_showerp_dist = trackend_dist;
	      }
	   
	      Double_t const trackstart_dist = t.front().Dist(best_vert);
	      if(trackstart_dist < best_showerp_dist) {
		best_track = n;
		best_tp = &t.front();
		best_showerp_dist = trackstart_dist;
	      }
	      
	    }
	      
	    if(tverbose) std::cout << "\t\t\t\tno, create new association\n";
	    std::vector<NodeID_t> showers;
	    showers.push_back(best_shower_id);
	    showers.push_back(best_other_id);
	    if(best_showerp_dist < tcpoa_vert_prox) showers.push_back(best_track);
	    std::vector<geoalgo::Point_t> verts(2, best_vert);
	    if(best_showerp_dist < tcpoa_vert_prox) {
	      if(best_tp == nullptr) std::cout << "best_tp == nullptr\n";
	      verts.push_back(*best_tp);
	    }
	    pas.AddAssociation(showers,
			       verts,
			       geoalgo::Sphere(best_vert, best_dist),
			       best_dist);
	  
	  }

	  if(tverbose)
	    std::cout << "\t\t\terase other: " << best_other_id << "\n";
	  shower_map.erase(best_other_id);

	}

	else if(p2.RecoType() == kTrack) {

	  if(tverbose) std::cout << "\t\t\ttrack\n";
		     	  
	  Track const & t = data.Track(p2.RecoID());
	  
	  Double_t best_trackend_dist = t.front().Dist(best_vert);
	  geoalgo::Point_t const * point = &t.front();
	  geoalgo::Point_t const * otherpoint = &t.back();

	  Double_t const trackend_dist = t.back().Dist(best_vert);
	  if(trackend_dist < best_trackend_dist) {
	    best_trackend_dist = trackend_dist;
	    point = &t.back();
	    otherpoint = &t.front();
	  }
	  
	  if(tverbose)
	    std::cout << "\t\t\tbest_trackend_dist: "
		      << best_trackend_dist
		      << " < tcpoa_vert_prox: "
		      << tcpoa_vert_prox << " ?\n";	

	  if(best_trackend_dist < tcpoa_trackend_prox) {

	    std::vector<Size_t> const index_positions =
	      pas.GetIndicesFromNode(best_other_id);
	     
	    if(tverbose)
	      std::cout << "\t\t\t\tyes\n"
			<< "\t\t\t\tindex_positions.size(): "
			<< index_positions.size() << "\n";
	    
	    if(index_positions.size() == 0) {

	      if(tverbose)
		std::cout << "\t\t\t\t\tsize 0\n";	      

	      if(best_association_dist < tcpoa_vert_prox) {
		pas.AddObject(association_index, best_shower_id, best_vert);
		pas.AddObject(association_index, best_other_id, *point);
	      }
	    
	      else {

		std::vector<NodeID_t> objects;
		objects.push_back(best_shower_id);
		objects.push_back(best_other_id);
		std::vector<geoalgo::Point_t> verts;
		verts.push_back(best_vert);
		verts.push_back(*point);
		pas.AddAssociation(objects,
				   verts,
				   geoalgo::Sphere(best_vert, best_dist),
				   best_dist);	      

	      }

	    }

	    else if(index_positions.size() == 1) {

	      if(tverbose)
		std::cout << "\t\t\t\t\tsize 1\n";	      
	      
	      Size_t const index =
		pas.GetIndices().at(index_positions.front());
	      
	      geoalgo::Point_t const & added_point =
		associations.at(index).GetVertexFromNode(best_other_id);
	      
	      Double_t const point_dist = added_point.Dist(*point);
	      Double_t const otherpoint_dist = added_point.Dist(*otherpoint);

	      if(tverbose)
		std::cout << "\t\t\t\t\totherpoint_dist: "
			  << otherpoint_dist
			  << " < point_dist: "
			  << point_dist << " ?\n";	
	      
	      if(otherpoint_dist < point_dist) {

		if(associations.at(index).GetSphere().Center().
		   Dist(*otherpoint) < tstart_prox) 
		  pas.AddObject(index, best_shower_id, best_vert);

		else {

		  std::vector<NodeID_t> objects;
		  objects.push_back(best_shower_id);
		  objects.push_back(best_other_id);
		  std::vector<geoalgo::Point_t> verts;
		  verts.push_back(best_vert);	
		  verts.push_back(*point);
		  pas.AddAssociation(objects,
				     verts,
				     geoalgo::Sphere(best_vert, best_dist),
				     best_dist);	      
	      
		}

	      }
	      
	      else 
		pas.AddObject(index, best_shower_id, best_vert);	

	    }

	    else if(index_positions.size() == 2) {

	      if(tverbose)
		std::cout << "\t\t\t\t\tsize 2\n";	      
	      
	      Int_t const indexa =
		pas.GetIndices().at(index_positions.front());
	      Double_t dista = 
		associations.at(indexa).GetSphere().Center().Dist(best_vert);
	   
	      Int_t const indexb = pas.GetIndices().at(index_positions.back());   
	      Double_t distb = 
		associations.at(indexb).GetSphere().Center().Dist(best_vert);
	      
	      if(dista < distb)
		pas.AddObject(indexa,
			      best_shower_id,
			      best_vert);	
	      
	      else
		pas.AddObject(indexb,
			      best_shower_id,
			      best_vert);	

	    }
	    
	    else if(index_positions.size() > 2)
	      std::cout << "Warning: more than two indices found\n";
	    
	  }
	
	  else {
	    Particle & p = graph.GetParticle(best_shower_id);
	    //Ugly way of setting the energy
	    Double_t energy = 0;
	    if(p.RecoType() == kTrack)
	      energy = data.Track(p.RecoID())._energy; 
	    if(p.RecoType() == kShower)
	      energy = data.Shower(p.RecoID())._energy;
	    p.SetParticleInfo(p.PdgCode(),
			      p.Mass(),
			      best_vert,
			      geoalgo::Point_t(energy, 0, 0));
	    graph.SetParentage(best_other_id, best_shower_id);
	  }
	
	}

      }

      else
	pas.AddObject(index, best_shower_id, best_vert);
     
      shower_map.erase(best_shower_id);

    }
 
    //pas.PrintAssociations();

    tverbose = false;

  }



  void ERAlgoVertexBuilder::EndReconstructPa(const EventData &data,
					     ParticleGraph & graph,
					     ParticleAssociations & pas){

    //tverbose = true;

    if(tshowerproj) ShowerProjection(data, graph, pas);

    pas.NodeCheck(graph);

    std::vector<ParticleAssociation> const & pav = pas.GetAssociations();
    association_number = pav.size();
    Int_t vertex_counter = 0;
   
    if(pav.size()) {
      
      ParticleGraphSetter setter(data, graph, pas);
      std::vector<Int_t> const & skip = setter.GetSkip();
   
      if(tverbose) pas.PrintAssociations();
      
      Bool_t first = true;

      while(skip.size() != pav.size()) {

	Int_t index = -1;
	geoalgo::Point_t const * sc = nullptr;

	if(tprimary_vertex_selection == mostupstream)
	  sc = GetUpstreamPrimary(pas, skip, index);
	else if(tprimary_vertex_selection == mostchildren)
	  sc = GetMostChildrenPrimary(data, graph, pas, skip, index);
	else if(tprimary_vertex_selection == mostenergy)
	  sc = GetMostEnergyPrimary(data, graph, pas, skip, index);
	else if(tprimary_vertex_selection == smallestsphere)
	  sc = GetSmallestSpherePrimary(pas, skip, index);
	else if(tprimary_vertex_selection == trackdirection)
	  //sc = GetTrackDirectionPrimary(data, graph, pas, skip, index);
	
	if(sc == nullptr) {
	  std::cout << "No sc\n";
	  return;
	}
	
	Particle & p = graph.CreateParticle();
	NodeID_t const pid = p.ID();
	graph.SetPrimary(pid);

	Int_t pdg = 0;
	if(first) {
	  pdg = 1;
	  first = false;
	}

	p.SetParticleInfo(pdg, p.Mass(), *sc, p.Momentum());
    	
	setter.runstart(pid, index);

	++vertex_counter;
	loop_counter += setter.GetLoopCounter();
	
      }
      
      AddAllLoneTracks(data, graph);
      AddAllLoneShowers(data, graph);

    }

    else AddTracksAndShowers(data, graph);
   
    std::vector<NodeID_t> const & pa_nodes = pas.GetNodes();
    Int_t track_counter = 0;
    for(NodeID_t const gn : graph.GetParticleNodes()) {
      
      Particle & p = graph.GetParticle(gn);
      
      if(p.RecoType() != kTrack) continue;
      
      if(std::find(pa_nodes.begin(), pa_nodes.end(), gn) != pa_nodes.end())
	continue;
     
      ++track_counter;

    }

    found_vertices = vertex_counter;  
    lone_track_counter = track_counter;
    vertices_lonetracks = vertex_counter + track_counter;
    tree->Fill();   

    tverbose = false;

  }
 


  void ERAlgoVertexBuilder::Reset() {

    event_id = -1;
    found_vertices = 0;
    loop_counter = 0;
    lone_track_counter = 0;
    vertices_lonetracks = 0;
    radius = 2000;
    multi_track_end_vertex = 0;

  }



  void ERAlgoVertexBuilder::Erase
  (EventData const & data,
   ParticleGraph const & graph,
   std::multimap<NodeID_t, geoalgo::Point_t const *> & pn,
   std::multimap<NodeID_t, geoalgo::Point_t const *>::iterator const best_it,
   geoalgo::Point_t const & sv) {

    NodeID_t const node_o = best_it->first;
        
    pn.erase(best_it);
    
    auto const pn_it = pn.find(node_o);
    Particle const & p = graph.GetParticle(node_o);

    if((pn_it != pn.end() &&
	pn_it->second->Dist(sv) < tstart_prox) ||
       (p.RecoType() == kTrack &&
	data.Track(p.RecoID()).Length() < tstart_prox)) {
      
      if(tverbose) std::cout << "\t\terase: " << node_o << "\n";
      pn.erase(pn_it);
      
    }
    
  }
  


  void ERAlgoVertexBuilder::WithoutTrackDir(const EventData &data,
					    ParticleGraph& graph) {

    if(data.Event_ID() != tevent && tevent != -1) return;

    Reset();

    event_id = data.Event_ID();
   
    if(tverbose)
      std::cout << "Event: " << data.Event_ID() << "\n"
		<< "=======================================================\n";
    
    std::multimap<NodeID_t, geoalgo::Point_t const *> pn;

    for(NodeID_t const n : graph.GetParticleNodes()) {

      Particle const & p = graph.GetParticle(n);
    
      if(p.RecoType() == kTrack) {

	geoalgo::Point_t const * tf = &data.Track(p.RecoID()).front();
	geoalgo::Point_t const * tb = &data.Track(p.RecoID()).back();

	//if(volume.Contain(*tf) && volume.Contain(*tb)) {
	  pn.emplace(n, tf);
	  pn.emplace(n, tb);
	  //} 

      }

    }
    
    if(tverbose) {

      std::cout << "pn Objects:\n";

      if(pn.size()) {
      
	NodeID_t last_id = pn.end()->first;
	for(auto p : pn) {
	  Particle const & part = graph.GetParticle(p.first);
	  if(last_id != p.first)
	    std::cout << "\n" << p.first << " "
		      << part.PdgCode() << " "
		      << *p.second;
	  else std::cout << " " << *p.second;
	  if(part.RecoType() == kTrack)
	    std::cout << data.Track(part.RecoID()).Length();
	  last_id = p.first;
	}

      }

      std::cout << "\n\n";

    }

    ParticleAssociations pa;

    if(tverbose)
      std::cout << "wloop, pn.size() == " << pn.size() << "\n";

    while(pn.size() > 1) {

      if(tverbose)
	std::cout <<  "\twloop start, pn.size() == " << pn.size() << "\n";

      auto best_m = pn.end();
      auto best_c = best_m;
     
      Double_t best_dist = tstart_prox;

      if(tverbose)
	std::cout << "\tFind the two closest vertices, pn.size() == "
		  << pn.size() << "\n"; 

      for(auto m_it = pn.begin(); m_it != pn.end(); ++m_it) {

	 if(tverbose)
	   std::cout << "\t\tMain loop start\n";
	
	NodeID_t const mid = m_it->first;
	geoalgo::Point_t const * mvert = m_it->second;
	
	 if(tverbose)
	   std::cout << "\t\tMain NodeID_t: " << mid << "\n";

	for(auto c_it = pn.begin(); c_it != pn.end(); ++c_it) {

	  if(tverbose)
	   std::cout << "\t\t\tComparison NodeID_t: " << c_it->first;
	
	  if(c_it->first == mid) {
	    if(tverbose)
	      std::cout << " Skip\n";
	    continue;
	  }
	  
	  Double_t const dist = c_it->second->Dist(*mvert);

	   if(tverbose)
	     std::cout << " dist: " << dist << " best_dist: "
		       << best_dist << "\n";

	  if(dist < best_dist) {
	    if(tverbose)
	      std::cout << "\t\t\t\tAccepted\n";
	    best_m = m_it;
	    best_c = c_it;
	    best_dist = dist;
	  }

	}

	if(tverbose)
	  std::cout << "\t\tMain loop end\n";
	
      }

      if(best_m == pn.end()) {

	if(tverbose)
	  std::cout << "\tNo match found, ending\n";

	EndReconstructPa(data, graph, pa);

	return;
      }

      std::vector<NodeID_t> vc;
      vc.push_back(best_m->first);
      vc.push_back(best_c->first);

      std::vector<geoalgo::Point_t> vcp;
      vcp.push_back(*best_m->second);
      vcp.push_back(*best_c->second);
      
      geoalgo::Sphere s(algo.boundingSphere(vcp));
      
      Erase(data, graph, pn, best_m, s.Center());
      Erase(data, graph, pn, best_c, s.Center());
      
      if(tverbose)
	std::cout << "\tAttempt to add objects to sphere\n";

      do {

	auto best_o = pn.end();
	//NodeID_t best_objectC = kINVALID_NODE_ID;
	//geoalgo::Point_t const * best_vertC = nullptr;
	Double_t sbest_dist = tstart_prox;
	
	geoalgo::Point_t const & sv = s.Center();

	if(tverbose)
	  std::cout << "\t\tFind distance between vertices and sphere centre, pn.size == " << pn.size() << "\n";

	for(auto o_it = pn.begin(); o_it != pn.end(); ++o_it) {

	  if(tverbose)
	    std::cout << "\t\t\tNodeID_t: " << o_it->first;
	
	  if(std::find(vc.begin(), vc.end(), o_it->first) != vc.end()) {
	    if(tverbose)
	      std::cout << " Skip\n";
	    continue;
	  }
  
	  Double_t const dist = o_it->second->Dist(sv);

	  if(tverbose)
	    std::cout << " dist: " << dist << " sbest_dist: "
		      << sbest_dist << "\n";

	  if(dist < sbest_dist) {
	    if(tverbose)
	      std::cout << "\t\t\t\tAccepted\n";
	    best_o = o_it;
	    sbest_dist = dist;
	  }

	}
	
	if(best_o == pn.end()) {
	  if(tverbose)
	    std::cout << "\t\tNo match found, end loop\n";
	  break;
	}

	vc.push_back(best_o->first);
	vcp.push_back(*best_o->second);

	Erase(data, graph, pn, best_o, *best_o->second);

	//s = algo.boundingSphere(vcp);

      } while(true);
      
      /*
      if(s.Radius() > tmax_rad) {
	
	std::cout << "\tRadius too big\n";

      }
      
      else {

	pa.AddAssociation(vc, vcp, s, algo.boundingSphere(vcp).Radius());

      }
      */      

      pa.AddAssociation(vc, vcp, s, algo.boundingSphere(vcp).Radius());

    }

    EndReconstructPa(data, graph, pa);    

    return;

  }



  bool ERAlgoVertexBuilder::Reconstruct(const EventData &data,
					ParticleGraph& graph) {

    if(twithTrackDir) WithTrackDir(data, graph);

    else WithoutTrackDir(data, graph);

    return true;
    
  }
  
  
  
  void ERAlgoVertexBuilder::ProcessEnd(TFile* fout) {

    tree->Write();

  }
  
  
  
}

#endif
