#ifndef ERTOOL_ERALGOVERTEXBUILDER_CXX
#define ERTOOL_ERALGOVERTEXBUILDER_CXX

#include "ERAlgoVertexBuilder.h"

#include "LArUtil/Geometry.h"

namespace ertool {



  ERAlgoVertexBuilder::ERAlgoVertexBuilder(Double_t const start_prox,
					   Double_t const max_rad,
					   Bool_t const withTrackDir,
					   const std::string& name) :
    AlgoBase(name),
    tstart_prox(start_prox),
    tmax_rad(max_rad),
    twithTrackDir(withTrackDir),
    tverbose(false) {

    tree = new TTree("ERAlgoVertexBuilder", "");
    tree->Branch("event_id", &event_id, "event_id/I");
    tree->Branch("association_number", &association_number, "association_number/I");
    tree->Branch("loop_counter", &loop_counter, "loop_counter/I");

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
	  vp.SetParticleInfo(0, RecoType_t::kInvisible, p.Vertex());
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
	p.SetParticleInfo(0, RecoType_t::kInvisible, s.Center());

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
    
    std::multimap<Int_t, NodeID_t> tcluster_connection;
    
  public:
    
    ParticleAssociation(std::vector<NodeID_t> const & cluster,
			std::vector<geoalgo::Point_t> const & vertices,
			geoalgo::Sphere const & sphere) :
      tcluster(cluster),
      tvertices(vertices),
      tsphere(sphere){}
    
    void AddConnection(Int_t const i, NodeID_t const n) {
      tcluster_connection.emplace(i, n);
    }
    
    std::vector<NodeID_t> const & GetCluster() const {
      return tcluster;
    }

    std::vector<geoalgo::Point_t> const & GetVertices() const {
      return tvertices;
    }

    geoalgo::Sphere const & GetSphere() const {
      return tsphere;
    }

    std::multimap<Int_t, NodeID_t> const & GetConnections() const {
      return tcluster_connection;
    }

    void PrintAssociation() const {
      
      for(Size_t i = 0; i < tcluster.size(); ++i)
	std::cout << "Nodes: " << tcluster.at(i) << " Vertices: " 
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

    void AddCluster(std::vector<NodeID_t> const & nodes,
		    std::vector<geoalgo::Point_t> const & vertices,
		    geoalgo::Sphere const & sphere) {
      
      tassociations.push_back(ParticleAssociation(nodes, vertices, sphere));
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

    std::vector<ParticleAssociation> const & GetAssociations() const {
      return tassociations;
    }

    void PrintAssociations() const {

      std::cout << "----------------------------------------\n\n";

      for(Size_t i = 0; i < tassociations.size(); ++i) {

	std::cout << "Association: " << i << "\n";

	tassociations.at(i).PrintAssociation();
	
	std::cout << "\n";

      }

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

      //std::cout << "\trun: parent: " << parent << " index: " << i << "\n";

      tskip.push_back(i);

      ParticleAssociation const & pae = tpa->GetAssociations().at(i);

      std::vector<NodeID_t> const & c = pae.GetCluster();
      std::vector<geoalgo::Point_t> const & v = pae.GetVertices();
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
	  p.SetParticleInfo(p.PdgCode(), p.Mass(), v.at(j));
	  tgraph->SetParentage(parent, n);
	}	  

	tnodelist.push_back(n);

      }

      for(std::pair<Int_t, NodeID_t> const & pair : pae.GetConnections()) {
	
	if(std::find(tskip.begin(), tskip.end(), pair.first) != tskip.end())
	  continue;
	if(std::find(bad_nodes.begin(), bad_nodes.end(), pair.second) !=
	   bad_nodes.end())
	  continue;
	
	runrec(tgraph->GetParticle(pair.second).ID(), pair.first);
	
      }
      
    }
    
    std::vector<Int_t> const & GetSkip() const {
      return tskip;
    }

    Int_t GetLoopCounter() {
      return tloopcounter;
    }

  };



  void ERAlgoVertexBuilder::EndReconstructPa(const EventData &data,
					     ParticleGraph & graph,
					     ParticleAssociations const & pa){

    std::vector<ParticleAssociation> const & pav = pa.GetAssociations();
    association_number = pav.size();
    Int_t loops = 0;

    if(pav.size()) {
      
      ParticleGraphSetter setter(data, graph, pa);
      std::vector<Int_t> const & skip = setter.GetSkip();
   
      if(tverbose) pa.PrintAssociations();

      while(skip.size() != pav.size()) {

	Int_t index = -1;
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
	
	if(sc == nullptr) {
	  std::cout << "No sc\n";
	  return;
	}
	
	Particle & p = graph.CreateParticle();
	NodeID_t const pid = p.ID();
	graph.SetPrimary(pid);
	p.SetParticleInfo(0, RecoType_t::kInvisible, *sc);    
	
	setter.runstart(pid, index);
	loops += setter.GetLoopCounter();
	
      }

    }
      
    loop_counter = loops;
    tree->Fill();
      
  }
 


  void ERAlgoVertexBuilder::Reset() {

    loop_counter = 0;

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

    //if(data.Event_ID() != 53181) return;

    event_id = data.Event_ID();

    Reset();
   
    if(tverbose)
      std::cout << "Event: " << data.Event_ID() << "\n"
		<< "=======================================================\n";
    
    std::multimap<NodeID_t, geoalgo::Point_t const *> pn;
    //std::multimap<NodeID_t, geoalgo::Point_t const *> pna;

    for(NodeID_t const n : graph.GetParticleNodes()) {

      Particle const & p = graph.GetParticle(n);
    
      if(p.RecoType() == kTrack) {

	geoalgo::Point_t const * tf = &data.Track(p.RecoID()).front();
	geoalgo::Point_t const * tb = &data.Track(p.RecoID()).back();

	//if(volume.Contain(*tf) && volume.Contain(*tb)) {
	  pn.emplace(n, tf);
	  pn.emplace(n, tb);
	  //pna.emplace(n, tf);
	  //pna.emplace(n, tb);
	  //} 

      }

      else if(p.RecoType() == kShower) {
	geoalgo::Point_t const * sv = &data.Shower(p.RecoID()).Start();
	pn.emplace(n, sv);
	//pna.emplace(n, sv);
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
	  if(part.RecoType() == kTrack) std::cout << data.Track(part.RecoID()).Length();
	  last_id = p.first;
	}

      }

      std::cout << "\n\n";

    }

    ParticleAssociations pa;

    if(tverbose)
      std::cout << "wloop, pn.size() == " << pn.size() << "\n";

    while(pn.size() > 0) {

      if(tverbose)
	std::cout <<  "\twloop start, pn.size() == " << pn.size() << "\n";

      auto best_m = pn.end();
      auto best_c = best_m;

      /*
	NodeID_t best_objectA = kINVALID_NODE_ID;
	NodeID_t best_objectB = kINVALID_NODE_ID;
	geoalgo::Point_t const * best_vertA = nullptr;
	geoalgo::Point_t const * best_vertB = nullptr;
      */
     
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
	     //<< "\t\tComparison loop, pna.size() == " << pna.size()

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

      } while(true);

      if(s.Radius() > tmax_rad) {

	std::cout << "\tRadius too big\n";

      }
      
      else {

	pa.AddCluster(vc, vcp, s);

      }
      
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
