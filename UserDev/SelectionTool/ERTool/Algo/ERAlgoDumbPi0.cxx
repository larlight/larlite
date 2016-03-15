#ifndef ERTOOL_ERALGODUMBPI0_CXX
#define ERTOOL_ERALGODUMBPI0_CXX

#include "ERAlgoDumbPi0.h"

namespace ertool {

  ERAlgoDumbPi0::ERAlgoDumbPi0(const std::string& name) : AlgoBase(name)
  {}

  void ERAlgoDumbPi0::Reset()
  {}

  void ERAlgoDumbPi0::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAlgoDumbPi0::ProcessBegin()
  {}

  bool ERAlgoDumbPi0::Reconstruct(const EventData &data, ParticleGraph& graph)
  {

	//
	if(graph.GetParticleNodes(RecoType_t::kShower).size() != 2){std::cout<<"Bad pi0"<<std::endl; return true;}

	// Loop over all the showers... if size ==2 then make it a pi0
	if(graph.GetParticleNodes(RecoType_t::kShower).size() == 2){
		auto shrIDs = graph.GetParticleNodes(RecoType_t::kShower);
		// Make the pi0 from each shower
                Particle& pi0 = graph.CreateParticle();
                geoalgo::Point_t vertex(3);// need to fill out 
                geoalgo::Vector_t momentum(3);// need to fill out
                pi0.SetParticleInfo(111,135,vertex,momentum);


                graph.SetParentage(pi0.ID(),graph.GetParticle(shrIDs[0]).ID());
                graph.SetParentage(pi0.ID(),graph.GetParticle(shrIDs[1]).ID());
                graph.SetSiblings(graph.GetParticle(shrIDs[0]).ID(),graph.GetParticle(shrIDs[1]).ID());

	}// if shower ==2

return true;}

  void ERAlgoDumbPi0::ProcessEnd(TFile* fout)
  {}

}

#endif
