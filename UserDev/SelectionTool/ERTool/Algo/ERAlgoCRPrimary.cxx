#ifndef ERTOOL_ERALGOCRPRIMARY_CXX
#define ERTOOL_ERALGOCRPRIMARY_CXX

#include "ERAlgoCRPrimary.h"

namespace ertool {

  ERAlgoCRPrimary::ERAlgoCRPrimary(const std::string& name) : AlgoBase(name)
  {}

  void ERAlgoCRPrimary::Reset()
  {}

  void ERAlgoCRPrimary::AcceptPSet(const ::fcllite::PSet& cfg)
  {
    auto p = cfg.get_pset(Name());
    
    auto tpc_x = p.get<std::vector<double> >("tpc_aabox_xrange");
    auto tpc_y = p.get<std::vector<double> >("tpc_aabox_yrange");
    auto tpc_z = p.get<std::vector<double> >("tpc_aabox_zrange");
    _tpc_box.Min( tpc_x[0], tpc_y[0], tpc_z[0] );
    _tpc_box.Max( tpc_x[1], tpc_y[1], tpc_z[1] );

    auto cr_x = p.get<std::vector<double> >("cosmic_aabox_xrange");
    auto cr_y = p.get<std::vector<double> >("cosmic_aabox_yrange");
    auto cr_z = p.get<std::vector<double> >("cosmic_aabox_zrange");
    _cosmic_box.Min( cr_x[0], cr_y[0], cr_z[0] );
    _cosmic_box.Max( cr_x[1], cr_y[1], cr_z[1] );
    
    _respect_track_dir = p.get<bool>("respect_track_dir");
  }

  void ERAlgoCRPrimary::ProcessBegin()
  {}

  bool ERAlgoCRPrimary::Reconstruct(const EventData &data, ParticleGraph& graph)
  {
    // Loop over track particle
    for(auto const& id : graph.GetParticleNodes(kTrack)) {

      auto& part = graph.GetParticle(id);     // Get particle

      if(part.Descendant()) continue;
      
      auto const& trk  = data.Track(part.RecoID()); // Get track
      
      auto const& start = trk.front(); // First step
      auto const& end   = trk.back();  // Last step

      bool is_cosmic = false;
      //
      // Tag method 1: if either start/end extends outside cr box boundary, it's cosmics
      //
      if( !_cosmic_box.Contain(start) || !_cosmic_box.Contain(end))
	is_cosmic = true;
      else {
	bool   start_contained = _tpc_box.Contain(start);
	bool   end_contained   = _tpc_box.Contain(end);
      
	//
	// Tag method 1: if both ends are outside TPC, tag
	//
	if( (!start_contained && !end_contained) ||   // Both ends are outside TPC
	    (_respect_track_dir && !start_contained)  // Check only start if told to respect dir
	    )
	  is_cosmic = true;
      }

      if(is_cosmic) {
	part.SetParticleInfo(part.PdgCode(),
			     part.Mass(),
			     part.Vertex(),
			     part.Momentum(),
			     part.RecoScore(),
			     kCosmic);
	graph.SetPrimary(id);
      }
    }
    return true;
  }

  void ERAlgoCRPrimary::ProcessEnd(TFile* fout)
  {}

}

#endif
