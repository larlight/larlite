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
    tverbose(false){}



  void ERAlgoVertexBuilder::Reset(){}



  void ERAlgoVertexBuilder::AcceptPSet(const ::fcllite::PSet& cfg){}



  void ERAlgoVertexBuilder::ProcessBegin() {

    //Set up TPC fiducial volume

    Double_t const xfid = 1;
    Double_t const yfid = 1;
    Double_t const zfid = 1;

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

    EndReconstruct(graph);

  }



  void ERAlgoVertexBuilder::WithoutTrackDir(const EventData &data,
					    ParticleGraph& graph) {
    
    if(tverbose) std::cout << "Event: " << data.Event_ID() << "\n"
			   << "============================================\n";
    
    std::map<Int_t, std::vector<NodeID_t> > particle_family;
    std::map<NodeID_t, Int_t> family_associate;

    std::list<NodeID_t> pn;
    std::vector<NodeID_t> pnc;

    /*Loop over primary nodes and add them to pn, if the node corresponds to a
      track only add it if the track start and end are contained within the
      fiducial volume*/

    for(NodeID_t const n : graph.GetPrimaryNodes()) {

      Particle const & p = graph.GetParticle(n);

      if(p.RecoType() == kTrack) {

	Track const & t = data.Track(p.RecoID());

	if(volume.Contain(t.front()) && volume.Contain(t.back())){	
	  pn.push_back(n);
	  pnc.push_back(n);
	}

      }

    }
    
    std::map<NodeID_t const, geoalgo::Point_t const *> vertex_map_all;
    
    for(NodeID_t const mp : pn) {
      
      Particle const & mpp = graph.GetParticle(mp);
      
      if(mpp.RecoType() == kTrack) {
	
	Track const & mpt = data.Track(mpp.RecoID());
	
	vertex_map_all.emplace(mp, &mpt.front());
	vertex_map_all.emplace(mp, &mpt.back());
	
      }
      
      else {
	
	vertex_map_all.emplace(mp, &mpp.Vertex());
	
      }
      
    }
    
    while(pn.size() > 0) {
      
      NodeID_t best_objectA = -1;
      NodeID_t best_objectB = -1;
      geoalgo::Point_t const * best_vertA = nullptr;
      geoalgo::Point_t const * best_vertB = nullptr;
      Double_t best_dist = tstart_prox;
      
      std::map<NodeID_t const, geoalgo::Point_t const *> vertex_map;

      for(NodeID_t const mp : pn) {

	Particle const & mpp = graph.GetParticle(mp);

	if(mpp.RecoType() == kTrack) {

	  Track const & mpt = data.Track(mpp.RecoID());

	  vertex_map.emplace(mp, &mpt.front());
	  vertex_map.emplace(mp, &mpt.back());

	}

	else {

	  vertex_map.emplace(mp, &mpp.Vertex());

	}

      }

      for(auto vm_it = vertex_map.begin();
	  vm_it != vertex_map.end();
	  vm_it = vertex_map.erase(vm_it)) {

	geoalgo::Point_t const * mvert = vm_it->second;

	for(std::pair<NodeID_t const, geoalgo::Point_t const *> const & pair :
	    vertex_map_all) {

	  if(pair.first == vm_it->first) continue;

	  Double_t const dist = mvert->Dist(*pair.second);

	  if(dist < best_dist) {
	    best_objectA = vm_it->first;
	    best_objectB = pair.first;
	    best_vertA = mvert;
	    best_vertB = pair.second;
	    best_dist = dist;
	  }

	}

      }

      if(best_objectA == -1) {
	/*
	for(auto pn_it = pn.begin();
	    pn_it != pn.end();
	    pn_it = pn.erase(pn_it)) {
	  
	  NodeID_t const n = *pn_it;
	  Particle const & p = graph.GetParticle(n);

	  if(p.RecoType() != kTrack) continue;
	  
	  Particle & vp = graph.CreateParticle();
	  graph.SetPrimary(vp.ID());
	  vp.SetParticleInfo(0, RecoType_t::kInvisible, p.Vertex());
	  graph.SetParentage(vp.ID(), n);
	
	}
	*/
	EndReconstruct(graph);
	
	return;
	
      }

      /*Make a vector to keep track of primary nodes associated with the vertex
	candidate*/
      std::vector<NodeID_t> vc;
      vc.push_back(best_objectA);
      vc.push_back(best_objectB);
      
      /*Make a vector to keep track of primary node positions associated with
	the vertex candidate*/
      std::vector<geoalgo::Point_t> vcp;
      vcp.push_back(*best_vertA);
      vcp.push_back(*best_vertB);
 
      /*Create an initial minimally bounding sphere using the closest matching
	pair positions*/
      geoalgo::Sphere s(algo.boundingSphere(vcp));

      do {
	
	NodeID_t best_objectC = -1;
	geoalgo::Point_t const * best_vertC = nullptr;
	Double_t best_dist = tstart_prox;

	//Get centre of current sphere

	geoalgo::Point_t const & sv = s.Center();

	for(NodeID_t const cp : pn) {
	
	  //Skip node in pn if it has already been added to the vertex
	  
	  if(std::find(vc.begin(), vc.end(), cp) != vc.end()) continue;

	  Particle const & cpp = graph.GetParticle(cp);

	  //Find distance between primary node position and sphere centre

	  geoalgo::Point_t const * tempvertex = nullptr;
	  Double_t dist = 0;

	  if(cpp.RecoType() == kTrack) {
	    
	    geoalgo::Point_t const & tf = data.Track(cpp.RecoID()).front();
	    geoalgo::Point_t const & tb = data.Track(cpp.RecoID()).back();

	    tempvertex = &tf;
	    dist = tf.Dist(sv);

	    Double_t const otherdist = tb.Dist(sv);
	    if(otherdist < dist) {
	      tempvertex = &tb;
	      dist = otherdist;
	    }

	  }

	  else {
	    tempvertex = &cpp.Vertex();
	    dist = cpp.Vertex().Dist(sv);
	  }
	
	  //If primary node is closer to sphere centre, replace

	  if(dist < best_dist) {
	    
	    best_objectC = cp;
	    best_vertC = tempvertex;
	    best_dist = dist;

	  }
	  
	}
	
	/*If no primary node within tstart_prox of the sphere centre is found,
	  end the loop*/ 

	if(best_objectC == -1) break;
	
	/*Add primary node closest to sphere centre to respective node and
	  position vectors*/
	
	vc.push_back(best_objectC);
	vcp.push_back(*best_vertC);
	
	//Recalculate sphere to include new primary node

	s = algo.boundingSphere(vcp);

      } while(true);

      //If sphere radius is too big, do something

      if(s.Radius() > tmax_rad) {
	
	//do stuff
	std::cout << "Radius too big: " << s.Radius() << "\n";
	
      }

      else {

	auto pa_it = family_associate.end();

	for(NodeID_t const nid : vc) {

	  auto const temp_it = family_associate.find(nid);

	  if(temp_it != pa_it) {
	    pa_it = temp_it;
	    break;
	  }

	}

	if(pa_it == family_associate.end()) {

	  if(particle_family.size() == 0) {

	    particle_family.emplace(1, vc);

	  }

	  else {

	    particle_family.emplace((++particle_family.rend())->first + 1, vc);

	  }

	}

	else {

	  std::vector<NodeID_t> & family = 
	    particle_family.find(pa_it->second)->second;

	  family.insert(family.end(), vc.begin(), vc.end());

	}

      }

      for(NodeID_t const v : vc) {
	
	pn.erase(std::find(pn.begin(), pn.end(), v));
	
      }	
      
    }
    
    EndReconstruct(graph);

    return;
  }



  bool ERAlgoVertexBuilder::Reconstruct(const EventData &data,
					ParticleGraph& graph) {

    if(twithTrackDir) WithTrackDir(data, graph);

    else WithoutTrackDir(data, graph);

    return true;
    
  }
  
  
  
  void ERAlgoVertexBuilder::ProcessEnd(TFile* fout) {

  }
  
  
  
}

#endif
