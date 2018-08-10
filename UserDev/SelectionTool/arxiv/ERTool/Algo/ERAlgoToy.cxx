#ifndef SELECTIONTOOL_ERTOOL_ERALGOTOY_CXX
#define SELECTIONTOOL_ERTOOL_ERALGOTOY_CXX

#include "ERAlgoToy.h"
#include "ERTool/Base/ERException.h"
#include "GeoAlgo/GeoSphere.h"
#include <sstream>
namespace ertool {

  ERAlgoToy::ERAlgoToy(const std::string& name)
    : AlgoBase ( name    )
    , _hRadius ( nullptr )
  {}

  void ERAlgoToy::Reset()
  {}

  void ERAlgoToy::AcceptPSet(const ::fcllite::PSet& cfg)
  {
    std::cout << this->Name().c_str() << " called... " << std::endl;

    // Look for a configuration
    if(cfg.contains_pset(this->Name())) {

      std::cout << "oops I found my config... ("
		<< this->Name()
		<< ") ... dumping it!"
		<< std::endl;

      auto const& pset = cfg.get_pset(this->Name());
      
      std::cout << pset.dump() << std::endl;

      _vtx_min_radius = pset.get<double>("vtx_min_radius");

    }else{
      std::cout << "oops I didn't find my config... ("
		<< this->Name()
		<< ")"
		<< std::endl;
      throw ERException();
    }
  }

  void ERAlgoToy::ProcessBegin()
  {
    delete _hRadius;
    _hRadius = new TH1D("hRadius","Radius of Vtx; Radius [cm]; # Verteces",
			100, 0, _vtx_min_radius);
  }

  bool ERAlgoToy::Reconstruct(const EventData &data, ParticleGraph& graph)
  {
    if(Debug())

      Debug(__FUNCTION__,"New event!");
    
    // Loop over all possible combinations of 2 particles
    for(auto const& combination_set : graph.GetNodeCombinations(2)) {

      // Get 2 particle instances
      auto const& part_a = graph.GetParticle( combination_set[0] );
      auto const& part_b = graph.GetParticle( combination_set[1] );

      if(Debug()) {
	std::stringstream ss;
	ss << "Processing particles: " << part_a.ID() << " & " << part_b.ID();
	Debug(__FUNCTION__,ss.str());
      }

      // Start 3D point variables
      ::geoalgo::Vector vtx_a(0,0,0);
      ::geoalgo::Vector vtx_b(0,0,0);

      // Fill vtx_a with particle A's start (track or shower)
      if(part_a.RecoType() == kShower) vtx_a = data.Shower(part_a).Start();
      else vtx_a = data.Track(part_a).front();

      // Fill vtx_b with particle B's start (track or shower)
      if(part_b.RecoType() == kShower) vtx_b = data.Shower(part_b).Start();
      else vtx_b = data.Track(part_b).front();

      // Construct a sphere w/ minimum radius to contain 2 points
      ::geoalgo::Sphere s(vtx_a,vtx_b);

      // If radius of containment is smaller than minimum set in config, make them into siblings
      if(s.Radius() < _vtx_min_radius) {

	if(Debug()) {
	  std::stringstream ss;
	  ss << "  ... they are sibings! "
	     << " radius = " << s.Radius() << " [cm] "
	     << std::endl;
	  Debug(__FUNCTION__,ss.str());
	}

	// Fill radius histogram
	_hRadius->Fill(s.Radius());

	// If they both have parents, we claim they are the same parent particles
	if( part_a.ID() != part_a.Parent() && part_b.ID() != part_b.Parent() )

	  graph.SameParticle(part_a.Parent(), part_b.Parent());

	// else make them as a new sibling
	else

	  graph.SetSiblings(part_a.ID(), part_b.ID());

      }else

	if(Debug()) Debug(__FUNCTION__,"  ... they are NOT sibings...");
    }
    
    return true;
  }

  void ERAlgoToy::ProcessEnd(TFile* fout)
  {
    // If output file handler given, write out
    if(fout) {
      fout->cd();
      _hRadius->Write();
    }
  }

}

#endif
