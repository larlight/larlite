#ifndef ERTOOL_ERALGOVERTEXBUILDER_CXX
#define ERTOOL_ERALGOVERTEXBUILDER_CXX

#include "ERAlgoVertexBuilder.h"

#include "LArUtil/Geometry.h"

namespace ertool {



  ERAlgoVertexBuilder::ERAlgoVertexBuilder(Double_t const start_prox,
					   Double_t const max_rad,
					   const std::string& name) :
    AlgoBase(name),
    tstart_prox(start_prox),
    tmax_rad(max_rad),
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



  std::list<NodeID_t> ERAlgoVertexBuilder::Smear() {



  }


  
  void ERAlgoVertexBuilder::EndReconstruct(ParticleGraph const & graph) {

    if(tverbose) std::cout << graph.Diagram() << "\n";

  }



  bool ERAlgoVertexBuilder::Reconstruct(const EventData &data,
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
	
	return true;
	
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

    return true;
    
  }
  
  
  
  void ERAlgoVertexBuilder::ProcessEnd(TFile* fout) {

  }
  
  
  
}

#endif
