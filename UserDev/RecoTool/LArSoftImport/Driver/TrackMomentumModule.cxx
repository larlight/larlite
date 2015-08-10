#ifndef LARLITE_TRACKMOMENTUMMODULE_CXX
#define LARLITE_TRACKMOMENTUMMODULE_CXX

#include "TrackMomentumModule.h"
#include "DataFormat/trackmom.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/track.h"
#include "DataFormat/event_ass.h"
namespace larlite {

  bool TrackMomentumModule::initialize() {

    return true;
  }
  
  bool TrackMomentumModule::analyze(storage_manager* storage) {

    std::vector<Float_t> x_v, y_v, z_v;

    for(auto const& module : _mctrack_module_v) {
      
      auto const ev_track = storage->get_data<event_mctrack>(module);

      if(!ev_track ) continue;

      std::string mom_module_name(Form("%smom",module.c_str()));

      auto ev_trkmom = storage->get_data<event_trackmom>(mom_module_name);
      auto ev_ass    = storage->get_data<event_ass>(mom_module_name);

      AssSet_t ass;
      if(ev_track->empty()) continue;

      ass.reserve(ev_track->size());
      for(auto const& trk : *ev_track) {

	if( x_v.size() != trk.size() ) {
	  x_v.resize(trk.size());
	  y_v.resize(trk.size());
	  z_v.resize(trk.size());
	}

	for(size_t step_index=0; step_index < trk.size(); ++step_index) {
	  x_v[step_index] = trk[step_index].X();
	  y_v[step_index] = trk[step_index].Y();
	  z_v[step_index] = trk[step_index].Z();
	}

	trackmom mom;
	mom.Momentum(data::kMomLLHD,_alg.GetMomentumMultiScatterLLHD(x_v,y_v,z_v));
	mom.Momentum(data::kMomChi2,_alg.GetMomentumMultiScatterChi2(x_v,y_v,z_v));
	mom.Momentum(data::kMomMuLLHD,_alg.GetMuMultiScatterLLHD(x_v,y_v,z_v));

	ev_trkmom->emplace_back(mom);
	ass.emplace_back(AssUnit_t(1,ass.size()));
      }
      ev_ass->set_association( ev_trkmom->id(),
			       ev_track->id(),
			       ass );
    }

    for(auto const& module : _track_module_v) {
      
      auto const ev_track = storage->get_data<event_track>(module);

      if(!ev_track ) continue;

      std::string mom_module_name(Form("%smom",module.c_str()));

      auto ev_trkmom = storage->get_data<event_trackmom>(mom_module_name);
      auto ev_ass    = storage->get_data<event_ass>(mom_module_name);

      AssSet_t ass;
      if(ev_track->empty()) continue;

      ass.reserve(ev_track->size());
      for(auto const& trk : *ev_track) {

	if( x_v.size() != trk.NumberTrajectoryPoints() ) {
	  x_v.resize(trk.NumberTrajectoryPoints());
	  y_v.resize(trk.NumberTrajectoryPoints());
	  z_v.resize(trk.NumberTrajectoryPoints());
	}

	for(size_t step_index=0; step_index < trk.NumberTrajectoryPoints(); ++step_index) {
	  x_v[step_index] = trk.LocationAtPoint(step_index)[0];
	  y_v[step_index] = trk.LocationAtPoint(step_index)[1];
	  z_v[step_index] = trk.LocationAtPoint(step_index)[2];
	}

	trackmom mom;
	mom.Momentum(data::kMomLLHD,_alg.GetMomentumMultiScatterLLHD(x_v,y_v,z_v));
	mom.Momentum(data::kMomChi2,_alg.GetMomentumMultiScatterChi2(x_v,y_v,z_v));
	mom.Momentum(data::kMomMuLLHD,_alg.GetMuMultiScatterLLHD(x_v,y_v,z_v));

	ev_trkmom->emplace_back(mom);
	ass.emplace_back(AssUnit_t(1,ass.size()));
      }
      ev_ass->set_association( ev_trkmom->id(),
			       ev_track->id(),
			       ass );
    }
    
    return true;
  }

  bool TrackMomentumModule::finalize() {

    return true;
  }

}
#endif
