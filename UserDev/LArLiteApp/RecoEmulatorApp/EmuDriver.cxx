#ifndef __LARLITE_RECOEMULATOR_EMUDRIVER_CXX__
#define __LARLITE_RECOEMULATOR_EMUDRIVER_CXX__

#include "EmuDriver.h"
#include "FhiclLite/ConfigManager.h"
#include "RecoEmulator/RecoEmulatorFactory.h"
namespace larlite {

  EmuDriver::EmuDriver(const std::string name)
    : ana_base()
    , _shower_smearing_tree(nullptr)
  {
    _name = name;
    _fout = 0;
    _mctrack_producer = _mcshower_producer = "";
    _track_emu = nullptr;
    _shower_emu = nullptr;
    _configured = false;
    _hutil = new ERToolHelperUtil();
    _hutil->SetMinEDep(10.);
    _disable_xshift = false;

  }

  bool EmuDriver::initialize() {

    // TTree setup
    if (_shower_smearing_tree) delete _shower_smearing_tree;

    _shower_smearing_tree = new TTree("shower_smearing_tree", "Shower Smearing Tree");
    _shower_smearing_tree->Branch("_dedx_in", &_dedx_in, "dedx_in/D");
    _shower_smearing_tree->Branch("_dedx_out", &_dedx_out, "dedx_out/D");
    _shower_smearing_tree->Branch("_x_in", &_x_in, "x_in/D");
    _shower_smearing_tree->Branch("_y_in", &_y_in, "y_in/D");
    _shower_smearing_tree->Branch("_z_in", &_z_in, "z_in/D");
    _shower_smearing_tree->Branch("_x_out", &_x_out, "x_out/D");
    _shower_smearing_tree->Branch("_y_out", &_y_out, "y_out/D");
    _shower_smearing_tree->Branch("_z_out", &_z_out, "z_out/D");
    _shower_smearing_tree->Branch("_px_in", &_px_in, "px_in/D");
    _shower_smearing_tree->Branch("_py_in", &_py_in, "py_in/D");
    _shower_smearing_tree->Branch("_pz_in", &_pz_in, "pz_in/D");
    _shower_smearing_tree->Branch("_px_out", &_px_out, "px_out/D");
    _shower_smearing_tree->Branch("_py_out", &_py_out, "py_out/D");
    _shower_smearing_tree->Branch("_pz_out", &_pz_out, "pz_out/D");
    _shower_smearing_tree->Branch("_e_in", &_e_in, "e_in/D");
    _shower_smearing_tree->Branch("_e_out", &_e_out, "e_out/D");




    // make sure the instance is already configured
    if (!_configured)
      print(msg::kERROR, __FUNCTION__, "Must call Configure() before starting an event loop!");

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
    if (_track_emu)  delete _track_emu;
    if (_shower_emu) delete _shower_emu;
    _track_emu  = (::recoemu::TrackEmulatorBase*)(::recoemu::RecoEmulatorFactory::get().create( main_cfg.get<std::string>("TrackEmulator" ) ));
    _shower_emu = (::recoemu::ShowerEmulatorBase*)(::recoemu::RecoEmulatorFactory::get().create( main_cfg.get<std::string>("ShowerEmulator") ));

    // Configure algorithms
    _track_emu->Configure(main_cfg.get_pset(_track_emu->Name()));
    _shower_emu->Configure(main_cfg.get_pset(_shower_emu->Name()));

    _disable_xshift = main_cfg.get<bool>("DisableXShift");
    _hutil->setDisableXShift(_disable_xshift);

    // Set config bool and exit
    _configured = true;
  }

  bool EmuDriver::analyze(storage_manager* storage) {

    // Retrieve mctrack and mcshower: make sure they exist else throw
    auto ev_mctrack = storage->get_data<event_mctrack>(_mctrack_producer);
    auto ev_mcshower = storage->get_data<event_mcshower>(_mcshower_producer);
    if (!ev_mctrack ) { throw DataFormatException("MCTrack not found..." ); }
    if (!ev_mcshower) { throw DataFormatException("MCShower not found..."); }

    // Attempt to get output track/shower data product holders (no need to be valid in case user is testing w/ kREAD mode)
    auto ev_track  = storage->get_data<event_track>("recoemu");
    auto ev_shower = storage->get_data<event_shower>("recoemu");

    if(ev_track) ev_track->clear_data();
    if(ev_shower) ev_shower->clear_data();

    // Emulate tracks
    for (auto const& mct : *ev_mctrack) {
      // Skip all mctracks that have zero or 1 size (deposit zero energy in the TPC) (nothing to emulate)
      if ( mct.size() < 2 )
        continue;
      // Convert mctrack => recoemu::Track_t
      auto emu_input = MCTrack2EmuTrack(mct);

      // Track_t.trajectory doesn't necessarily have the same # of points as mctrack
      // (GeoTrajectory won't push_back the same point twice, and many mctracks end with a duplicate point)
      if ( emu_input.trajectory.size() < 2 )
        continue;

      // Emulate and retrieve emulated output
      auto emu_output = _track_emu->Emulate(emu_input);
      // Convert recoemu::Track_t => track, if not marked for deletion
      if ( emu_output.mark_for_deletion ) {
        // HACK: If track should be "deleted", instead shift it 100 meters in the -y direction
        // This is so that RecoID's will match up between ertool reco particle graphs
        // and mc particle graphs. These tracks will be cut out by fiducial volume cuts.
        shiftTrackHack(emu_output);
        //continue;
      }
      auto reco_track = EmuTrack2RecoTrack(emu_output);
      // If output data holder exists, record the result
      if (ev_track) ev_track->emplace_back(reco_track);
    }

    for (auto const& mcs : *ev_mcshower) {
      // Skip all mcshowers that deposit zero energy (nothing to emulate) or have ill-defined startdir
      // this "10" should match _minEdep in ertoolhelper
      if (!_hutil->isViable(mcs)) continue;
      // Convert mcshower => recoemu::Shower_t
      auto emu_input = MCShower2EmuShower(mcs);
      _e_in    = emu_input.energy;
      _x_in    = emu_input.cone.Start()[0];
      _y_in    = emu_input.cone.Start()[1];
      _z_in    = emu_input.cone.Start()[2];
      _px_in   = emu_input.cone.Dir()[0];
      _py_in   = emu_input.cone.Dir()[1];
      _pz_in   = emu_input.cone.Dir()[2];
      _dedx_in = emu_input.dedx;
      // Emulate and retrieve emulated output
      auto emu_output = _shower_emu->Emulate(emu_input);
      _e_out    = emu_output.energy;
      _x_out    = emu_output.cone.Start()[0];
      _y_out    = emu_output.cone.Start()[1];
      _z_out    = emu_output.cone.Start()[2];
      _px_out   = emu_output.cone.Dir()[0];
      _py_out   = emu_output.cone.Dir()[1];
      _pz_out   = emu_output.cone.Dir()[2];
      _dedx_out = emu_output.dedx;
      _shower_smearing_tree->Fill();

      // Convert recoemu::Shower_t => shower, if not marked for deletion
      if ( emu_output.mark_for_deletion ) {
        // HACK: If track should be "deleted", instead shift it 100 meters in the -y direction
        // This is so that RecoID's will match up between ertool reco particle graphs
        // and mc particle graphs. These tracks will be cut out by fiducial volume cuts.
        shiftShowerHack(emu_output);
        //continue;
      }

      auto reco_shower = EmuShower2RecoShower(emu_output);
      // If output data holder exists, record the result
      if (ev_shower) ev_shower->emplace_back(reco_shower);
    }

    // Set event id and done
    storage->set_id(storage->run_id(), storage->subrun_id(), storage->event_id());

    return true;
  }

  bool EmuDriver::finalize() {

    if (_fout) {
      _fout->cd();
      //_shower_smearing_tree->Write();
    }

    return true;
  }

  ::recoemu::Track_t EmuDriver::MCTrack2EmuTrack(const mctrack& mct)
  {
    // Convert a provided mctrack into recoemu::Track_t
    ::recoemu::Track_t result;
    result.trajectory.reserve(mct.size());
    ::geoalgo::Point_t this_xshift = ::geoalgo::Point_t(_hutil->getXShift(mct));
    for (auto const& step : mct) {
      result.trajectory.push_back(::geoalgo::Point_t(step.X(), step.Y(), step.Z()) + this_xshift);
    }
    result.energy = mct.front().E() - mct.back().E();
    result.dedx = mct.dEdx();
    result.time = mct.front().T();
    result.pdg = mct.PdgCode();
    result.mark_for_deletion = false;

    return result;
  }

  track EmuDriver::EmuTrack2RecoTrack(const ::recoemu::Track_t& trk)
  {
    // Convert a provided recoemu::Track_t into track
    track result;

    //Add_momentum as track's deposited energy to 0th step
    result.add_momentum(trk.energy);
    for (size_t ipt = 0; ipt < trk.trajectory.size(); ++ipt) {
      auto const& pt = trk.trajectory[ipt];
      result.add_vertex(TVector3(pt[0], pt[1], pt[2]));

      //Need to add directions otherwise NumberTrajectoryPoints() returns 0 and ertoolhelper segfaults
      if (ipt < trk.trajectory.size() - 1) {
        auto const& nextpt = trk.trajectory[ipt + 1];
        result.add_direction(TVector3(nextpt[0] - pt[0], nextpt[1] - pt[1], nextpt[2] - pt[2]));
      }
      //To make # of direction points == size of track, do this for last point:
      else {
        auto const& lastpt = trk.trajectory[ipt - 1];
        result.add_direction(TVector3(pt[0] - lastpt[0], pt[1] - lastpt[1], pt[2] - lastpt[2]));
      }

    }

    result.set_track_id(trk.pdg);
    return result;
  }

  ::recoemu::Shower_t EmuDriver::MCShower2EmuShower(const mcshower& mcs)
  {
    // Convert a provided mcshower into recoemu::Shower_t
    ::recoemu::Shower_t result;
    result.cone.Start(mcs.DetProfile().X() + _hutil->getXShift(mcs).X(),
                      mcs.DetProfile().Y(),
                      mcs.DetProfile().Z());
    result.cone.Dir(mcs.StartDir()[0], mcs.StartDir()[1], mcs.StartDir()[2]);
    result.energy = mcs.DetProfile().E();
    result.dedx = _hutil->getMCShowerdEdX(mcs);
    result.time = mcs.Start().T();
    result.pdg = mcs.PdgCode();
    result.mark_for_deletion = false;

    return result;
  }

  shower EmuDriver::EmuShower2RecoShower(const ::recoemu::Shower_t& shr)
  {
    // Convert a provided recoemu::Shower_t into shower
    shower result;
    result.set_total_energy(shr.energy);
    result.set_start_point(TVector3(shr.cone.Start()[0], shr.cone.Start()[1], shr.cone.Start()[2]));
    result.set_direction(TVector3(shr.cone.Dir()[0], shr.cone.Dir()[1], shr.cone.Dir()[2]));

    // // this if-statement matches ertoolhelper
    ////// NOTE: this has to be done IN ertool helper so the same gRandom instance is used
    ////// otherwise you will get different results when comparing reco emu to MC
    // if ( (shr.dedx < 0.5) ||  (shr.dedx > 100) )
    //   result.set_dedx( shr.pdg == 22 ? gRandom->Gaus(4, 4 * 0.03) : gRandom->Gaus(2, 2 * 0.03) );
    // else
    result.set_dedx( shr.dedx );
    result.set_id( shr.pdg );

    return result;
  }

  void EmuDriver::shiftTrackHack(::recoemu::Track_t& trk){
    // Shift the entire track 100 meters in the negative y direction
    //Loop through the Track_t trajectory and smear each point
    for (auto &step : trk.trajectory)
      step[1] = step[1] - 10000.;

  }
  void EmuDriver::shiftShowerHack(::recoemu::Shower_t& shr) {
    //Shift the shower start point 100 meters in the negative y direction
    ::geoalgo::Point_t oldstart = shr.cone.Start();
    shr.cone.Start(oldstart[0], oldstart[1] - 10000., oldstart[2]);
  }

}
#endif
