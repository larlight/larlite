#ifndef __LARLITE_RECOEMULATOR_EMUDRIVER_CXX__
#define __LARLITE_RECOEMULATOR_EMUDRIVER_CXX__

#include "EmuDriver.h"
#include "FhiclLite/ConfigManager.h"
#include "RecoEmulator/RecoEmulatorFactory.h"
namespace larlite {

  EmuDriver::EmuDriver(const std::string name) : ana_base()
  {
    _name=name;
    _fout=0;
    _mctrack_producer = _mcshower_producer = "";
    _track_emu = nullptr;
    _shower_emu = nullptr;
    _configured = false;
  }

  bool EmuDriver::initialize() {

    // make sure the instance is already configured
    if(!_configured)
      print(msg::kERROR,__FUNCTION__,"Must call Configure() before starting an event loop!");

    return _configured;
  }

  void EmuDriver::Configure(const std::string& cfg_file)
  {
    //
    // This method needs to 0) configure self, and 1) configure track/shower emulator algorithms
    //

    // Retrieve configuration
    ::fcllite::ConfigManager cfg_mgr(_name);
    cfg_mgr.AddCfgFile(cfg_file);
    auto const& main_cfg = cfg_mgr.Config().get_pset(_name);

    // Retrieve self configurations
    _mctrack_producer  = main_cfg.get<std::string>("MCTrackProducer");
    _mcshower_producer = main_cfg.get<std::string>("MCShowerProducer");

    // Retrieve track/shower emulator algorithm type and instantiate using a factory
    if(_track_emu)  delete _track_emu;
    if(_shower_emu) delete _shower_emu;
    _track_emu  = (::recoemu::TrackEmulatorBase*)(::recoemu::RecoEmulatorFactory::get().create( main_cfg.get<std::string>("TrackEmulator" ) ));
    _shower_emu = (::recoemu::ShowerEmulatorBase*)(::recoemu::RecoEmulatorFactory::get().create( main_cfg.get<std::string>("ShowerEmulator") ));

    // Configure algorithms
    _track_emu->Configure(main_cfg.get_pset(_track_emu->Name()));
    _shower_emu->Configure(main_cfg.get_pset(_shower_emu->Name()));

    // Set config bool and exit
    _configured = true;
  }
  
  bool EmuDriver::analyze(storage_manager* storage) {

    // Retrieve mctrack and mcshower: make sure they exist else throw
    auto ev_mctrack = storage->get_data<event_mctrack>(_mctrack_producer);
    auto ev_mcshower = storage->get_data<event_mcshower>(_mcshower_producer);
    if(!ev_mctrack ) { throw DataFormatException("MCTrack not found..." ); }
    if(!ev_mcshower) { throw DataFormatException("MCShower not found..."); }

    // Attempt to get output track/shower data product holders (no need to be valid in case user is testing w/ kREAD mode)
    auto ev_track  = storage->get_data<event_track>("recoemu");
    auto ev_shower = storage->get_data<event_shower>("recoemu");

    // Emulate tracks
    for(auto const& mct : *ev_mctrack) {
      // Convert mctrack => recoemu::Track_t
      auto emu_input = MCTrack2EmuTrack(mct);
      // Emulate and retrieve emulated output
      auto emu_output = _track_emu->Emulate(emu_input);
      // Convert recoemu::Track_t => track
      auto reco_track = EmuTrack2RecoTrack(emu_output);
      // If output data holder exists, record the result
      if(ev_track) ev_track->emplace_back(reco_track);
    }

    for(auto const& mcs : *ev_mcshower) {
      // Convert mcshower => recoemu::Shower_t
      auto emu_input = MCShower2EmuShower(mcs);
      // Emulate and retrieve emulated output
      auto emu_output = _shower_emu->Emulate(emu_input);
      // Convert recoemu::Shower_t => shower
      auto reco_shower = EmuShower2RecoShower(emu_output);
      // If output data holder exists, record the result
      if(ev_shower) ev_shower->emplace_back(reco_shower);
    }

    // Set event id and done
    storage->set_id(storage->run_id(),storage->subrun_id(),storage->event_id());

    return true;
  }

  bool EmuDriver::finalize() {
    return true;
  }

  ::recoemu::Track_t EmuDriver::MCTrack2EmuTrack(const mctrack& mct)
  {
    // Convert a provided mctrack into recoemu::Track_t
    ::recoemu::Track_t result;
    result.trajectory.reserve(mct.size());
    for(auto const& step : mct)
      result.trajectory.push_back(::geoalgo::Point_t(step.X(),step.Y(),step.Z()));
    result.energy = mct.front().E() - mct.back().E();
    result.dedx = mct.dEdx();
    return result;
  }

  track EmuDriver::EmuTrack2RecoTrack(const ::recoemu::Track_t& trk)
  {
    // Convert a provided recoemu::Track_t into track
    track result;
    for(auto const& pt : trk.trajectory)

      result.add_vertex(TVector3(pt[0],pt[1],pt[2]));

    return result;
  }

  ::recoemu::Shower_t EmuDriver::MCShower2EmuShower(const mcshower& mcs)
  {
    // Convert a provided mcshower into recoemu::Shower_t
    ::recoemu::Shower_t result;
    result.cone.Start(mcs.DetProfile().X(),
		      mcs.DetProfile().Y(),
		      mcs.DetProfile().Z());
    result.cone.Dir(mcs.StartDir()[0],mcs.StartDir()[1],mcs.StartDir()[2]);
    result.energy = mcs.DetProfile().E();
    return result;
  }

  shower EmuDriver::EmuShower2RecoShower(const ::recoemu::Shower_t& shr)
  {
    // Convert a provided recoemu::Shower_t into shower
    shower result;
    result.set_total_energy(shr.energy);
    result.set_start_point(TVector3(shr.cone.Start()[0],shr.cone.Start()[1],shr.cone.Start()[2]));
    result.set_direction(TVector3(shr.cone.Dir()[0],shr.cone.Dir()[1],shr.cone.Dir()[2]));
    return result;
  }
  
}
#endif
