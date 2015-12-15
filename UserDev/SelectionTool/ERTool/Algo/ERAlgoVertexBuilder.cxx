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

    tree->Branch("pn_size", &pn_size, "pn_size/I");
    tree->Branch("association_number", &association_number, "association_number/I");
    tree->Branch("cpa", &cpa, "cpa/D");
    tree->Branch("vertx", &vertx, "vertx/D");
    tree->Branch("verty", &verty, "verty/D");
    tree->Branch("vertz", &vertz, "vertz/D");

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

    Int_t tcounter;

    std::vector<Int_t> tindex;
    std::vector< std::vector<NodeID_t> > tnodes;
    std::vector< std::vector< std::vector<NodeID_t> > > tassociation;
    std::vector< std::vector< std::vector<geoalgo::Point_t> > > tvertices;
    std::vector< std::vector< geoalgo::Sphere > > tspheres;

    std::map<NodeID_t, Int_t> tnode_association;
    std::vector< std::vector< std::vector<Int_t> > > tassociation_pairs;

  public:

    ParticleAssociation() : tcounter(0){}

    void AddAssociation(std::vector<NodeID_t> const & nodes,
			std::vector<geoalgo::Point_t> const & vertices,
			geoalgo::Sphere const & sphere) {

      tindex.push_back(tcounter);
      tnodes.push_back(nodes);
      tassociation.push_back(std::vector< std::vector<NodeID_t> >());
      tassociation.at(tcounter).push_back(nodes);
      tvertices.push_back(std::vector< std::vector<geoalgo::Point_t> >());
      tvertices.at(tcounter).push_back(vertices);
      tspheres.push_back(std::vector<geoalgo::Sphere>());
      tspheres.at(tcounter).push_back(sphere);

      for(NodeID_t const n : nodes) tnode_association.emplace(n, tcounter);

      ++tcounter;

    }
    
    void AddToAssociation(Int_t const i,
			  std::vector<NodeID_t> const & nodes,
			  std::vector<geoalgo::Point_t> const & vertices,
			  geoalgo::Sphere const & sphere) {

      std::vector<NodeID_t> & association = tnodes.at(i);

      for(NodeID_t const n : nodes) {

	if(std::find(association.begin(), association.end(), n) ==
	   association.end())
	  association.push_back(n);

      }

      tassociation.at(i).push_back(nodes);
      tvertices.at(i).push_back(vertices);
      tspheres.at(i).push_back(sphere);

      for(NodeID_t const n : nodes) tnode_association.emplace(n, i);

    }

    std::vector<Int_t> const & GetNodeAssociationIndices() {
      return tindex;
    }

    Int_t GetNodeAssociationIndex(NodeID_t const n) {

      if(tnode_association.find(n) == tnode_association.end())
	return -1;

      return tnode_association.find(n)->second;

    }

    std::vector< std::vector<NodeID_t> > const & GetNodes() {
      return tnodes;
    }

    std::vector< std::vector< std::vector<NodeID_t> > > const &
    GetNodeAssociations(){
      return tassociation;
    }
    
    std::vector< std::vector<NodeID_t> > const &
    GetNodeAssociation(Int_t const i){
      
      return tassociation.at(i);
      
    }

    std::vector< std::vector< geoalgo::Sphere > > const & GetSpheres() {
      return tspheres;
    }

    void PrintNodes() {

      for(std::vector<NodeID_t> nv : tnodes) {

	Int_t counter = 0;

	std::cout << "Association: " << counter << ":\n";

	for(NodeID_t const n : nv) std::cout << "\t\t" << n << "\n";

	++counter;

      }

    }

    void PrintNodeAssociations() {

      for(std::vector< std::vector<NodeID_t> > nvv : tassociation) {

	Int_t counter = 0;
	
	std::cout << "Association: " << counter << ":\n";

	for(std::vector<NodeID_t> nv : nvv) {

	  std::cout << "\tNode Collections:\n";

	  for(NodeID_t const n : nv) std::cout << "\t\t" << n << "\n";

	  std::cout << "\n";

	}

	std::cout << "\n";

	++counter;

      }

    }

    void PrintSpheres() {

      Int_t counter = 0;

      for(std::vector<geoalgo::Sphere> const & ss : tspheres) {

	std::cout << "Association: " << counter << ":\n";

	for(geoalgo::Sphere const & s : ss) {

	  std::cout << s.Center() << "\n";

	}

	++counter;

      }      

    }
    
  };



  void ERAlgoVertexBuilder::EndReconstructPa(const EventData &data,
					     ParticleGraph & graph,
					     ParticleAssociation & pa){

    auto const & na = pa.GetNodeAssociations();

    association_number = na.size();
    
    Double_t counter = 0;

    for(auto const & a : na) counter += a.size();

    if(association_number) cpa = counter / association_number;
   
    std::vector< std::vector<geoalgo::Sphere> > const & spheres =
      pa.GetSpheres();
    
    for(Size_t i = 0; i < spheres.size(); ++i) {
      
      std::vector<geoalgo::Sphere> const & ss = spheres.at(i);

      Double_t d = 2000;
      geoalgo::Point_t const * centre = nullptr;
      Size_t index = -1;

      for(Size_t j = 0; j < ss.size(); ++j) {

	geoalgo::Sphere const & s = ss.at(j);

	Double_t const dist = s.Center().at(2);

	if(dist < d) {
	  centre = &s.Center();
	  d = dist;
	  index = j;
	}

      }

      if(centre) {
	
	vertx = centre->at(0);
	verty = centre->at(1);
	vertz = centre->at(2);
	
	Particle & p = graph.CreateParticle();
	NodeID_t const pid = p.ID();
	graph.SetPrimary(pid);
	p.SetParticleInfo(0, RecoType_t::kInvisible, *centre);   
	
	std::vector< std::vector<NodeID_t> > const & na = pa.GetNodeAssociation(i);
	std::vector<NodeID_t> const & nac = na.at(index);

	for(NodeID_t const n : nac) graph.SetParentage(pid, n);

	for(Int_t j = 0; j < na.size(); ++j) {
	  
	  if(j == index) continue;

	  std::vector<NodeID_t> const & naa = na.at(j);

	  for(NodeID_t const n : nac) {

	    if(std::find(naa.begin(), naa.end(), n) != naa.end()) {

	      /////////////////////////////////////////////////

	    }

	  }

	}
	
      }

      else std::cout << "No centre\n";

    }

    tree->Fill();

    //if(ni.size() == 0) std::cout << data.Event_ID() << "\n";

  }



  void ERAlgoVertexBuilder::Reset() {

    pn_size = 0;
    association_number = 0;
    cpa = 0;
    vertx = 0;
    verty = 0;
    vertx = 0;

  }



  void ERAlgoVertexBuilder::WithoutTrackDir(const EventData &data,
					    ParticleGraph& graph) {

    Reset();
   
    if(tverbose)
      std::cout << "Event: " << data.Event_ID() << "\n"
		<< "=======================================================\n";
    
    std::multimap<NodeID_t, geoalgo::Point_t const *> pn;
    std::multimap<NodeID_t, geoalgo::Point_t const *> pna;

    for(NodeID_t const n : graph.GetParticleNodes()) {

      Particle const & p = graph.GetParticle(n);
    
      if(p.RecoType() == kTrack) {

	geoalgo::Point_t const * tf = &data.Track(p.RecoID()).front();
	geoalgo::Point_t const * tb = &data.Track(p.RecoID()).back();

	//if(volume.Contain(*tf) && volume.Contain(*tb)) {
	  pn.emplace(n, tf);
	  pn.emplace(n, tb);
	  pna.emplace(n, tf);
	  pna.emplace(n, tb);
	  //} 

      }

      else if(p.RecoType() == kShower) {
	geoalgo::Point_t const * sv = &data.Shower(p.RecoID()).Start();
	pn.emplace(n, sv);
	pna.emplace(n, sv);
      }

    }
    
    pn_size = pn.size();

    if(tverbose) {

      std::cout << "pn Objects:\n";

      if(pn.size()) {
      
	NodeID_t last_id = pn.end()->first;
	for(auto p : pn) {
	  if(last_id != p.first)
	    std::cout << "\n" << graph.GetParticle(p.first).PdgCode() << " "
		      << *p.second;
	  else std::cout << " " << *p.second;
	  last_id = p.first;
	}

      }

      std::cout << "\n\n";

    }

    ParticleAssociation pa;

    if(tverbose)
      std::cout << "wloop, pn.size() == " << pn.size() << "\n";

    while(pn.size() > 0) {

      if(tverbose)
	std::cout <<  "\twloop start, pn.size() == " << pn.size() << "\n";

      NodeID_t best_objectA = kINVALID_NODE_ID;
      NodeID_t best_objectB = kINVALID_NODE_ID;
      geoalgo::Point_t const * best_vertA = nullptr;
      geoalgo::Point_t const * best_vertB = nullptr;
      Double_t best_dist = tstart_prox;

      if(tverbose)
	std::cout << "\tFind the two closest vertices, pn.size() == "
		  << pn.size() << "\n"; 

      for(std::pair<NodeID_t, geoalgo::Point_t const *> const & mpair : pn) {

	 if(tverbose)
	   std::cout << "\t\tMain loop start\n";
	
	NodeID_t const mid = mpair.first;
	geoalgo::Point_t const * mvert = mpair.second;
	
	 if(tverbose)
	   std::cout << "\t\tMain NodeID_t: " << mid << "\n"
		     << "\t\tComparison loop, pna.size() == " << pna.size()
		     << "\n";

	for(std::pair<NodeID_t, geoalgo::Point_t const *> const & cpair : pna){

	  if(tverbose)
	   std::cout << "\t\t\tComparison NodeID_t: " << cpair.first << "\n";
	
	  if(cpair.first == mid) {
	    if(tverbose)
	      std::cout << "\t\t\t\tSkip\n";
	    continue;
	  }
	  
	  Double_t const dist = cpair.second->Dist(*mvert);

	   if(tverbose)
	     std::cout << "\t\t\tdist: " << dist << " best_dist: "
		       << best_dist << "\n";

	  if(dist < best_dist) {
	    if(tverbose)
	      std::cout << "\t\t\t\tAccepted\n";
	    best_objectA = mid;
	    best_objectB = cpair.first;
	    best_vertA = mvert;
	    best_vertB = cpair.second;
	    best_dist = dist;
	  }

	}

	if(tverbose)
	  std::cout << "\t\tMain loop end\n";
	
      }

      if(best_objectA == kINVALID_NODE_ID) {
	if(tverbose)
	  std::cout << "\tNo match found, ending\n";

	EndReconstructPa(data, graph, pa);

	return;
      }

      if(tverbose)
	std::cout << "\t Match found: " << best_objectA << " " << best_objectB
		  << "\n\tErasing best objects\n";

      pn.erase(best_objectA);
      pn.erase(best_objectB);

      std::vector<NodeID_t> vc;
      vc.push_back(best_objectA);
      vc.push_back(best_objectB);
      
      std::vector<geoalgo::Point_t> vcp;
      vcp.push_back(*best_vertA);
      vcp.push_back(*best_vertB);
      
      geoalgo::Sphere s(algo.boundingSphere(vcp));

      if(tverbose)
	std::cout << "\tAttempt to add objects to sphere\n";

      do {

	NodeID_t best_objectC = kINVALID_NODE_ID;
	geoalgo::Point_t const * best_vertC = nullptr;
	Double_t sbest_dist = tstart_prox;
	
	geoalgo::Point_t const & sv = s.Center();

	if(tverbose)
	  std::cout << "\t\tFind distance between vertices and sphere centre, pn.size == " << pn.size() << "\n";

	for(std::pair<NodeID_t, geoalgo::Point_t const *> const & pair : pn) {

	  if(tverbose)
	    std::cout << "\t\t\tNodeID_t: " << pair.first << "\n";
	
	  if(std::find(vc.begin(), vc.end(), pair.first) != vc.end()) {
	    if(tverbose)
	      std::cout << "\t\t\t\tSkip\n";
	    continue;
	  }
  
	  Double_t const dist = pair.second->Dist(sv);

	  if(tverbose)
	    std::cout << "\t\t\tdist: " << dist << " sbest_dist: "
		      << sbest_dist << "\n";

	  if(dist < sbest_dist) {
	    if(tverbose)
	      std::cout << "\t\t\t\tAccepted\n";
	    best_objectC = pair.first;
	    best_vertC = pair.second;
	    sbest_dist = dist;
	  }

	}
	
	if(best_objectC == kINVALID_NODE_ID) {
	  if(tverbose)
	    std::cout << "\t\tNo match found, end loop\n";
	  break;
	}

	vc.push_back(best_objectC);
	vcp.push_back(*best_vertC);

      } while(true);

      if(s.Radius() > tmax_rad) {

	std::cout << "Radius too big\n";

      }

      else {
	
	Int_t index = -1;

	for(NodeID_t const n : vc) {

	  if(pa.GetNodeAssociationIndex(n) != -1) {
	    index = pa.GetNodeAssociationIndex(n);
	    break;
	  }

	}

	if(index != -1) pa.AddToAssociation(index, vc, vcp, s);

	else pa.AddAssociation(vc, vcp, s);

	for(NodeID_t const v : vc) pn.erase(v);

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
