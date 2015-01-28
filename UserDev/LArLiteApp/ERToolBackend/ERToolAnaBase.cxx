#ifndef ERToolANABASE_CXX
#define ERToolANABASE_CXX

#include "ERToolAnaBase.h"

namespace larlite {

  ERToolAnaBase::ERToolAnaBase() : ana_base()
  { 
    _name="ERToolAnaBase";
    //MCProducer("generator","mcreco");
    _name_generator = "generator";
    _name_mcshr     = "mcreco";
    _name_mctrk     = "mcreco";
    _name_mcvtx     = "generator";
    _name_track     = "";
    _name_shower    = "";
    _name_vertex    = "";
  }

  void ERToolAnaBase::SetShowerProducer(const bool mc, const std::string prod)
  {
    if (mc){
      _mcshowers   = true;
      _name_mcshr = prod;
    }
    else{
      _mcshowers   = false;
      _name_shower = prod;
    }
  
  return;
  }

  void ERToolAnaBase::SetTrackProducer(const bool mc, const std::string prod)
  {
    if (mc){
      _mctracks   = true;
      _name_mctrk = prod;
    }
    else{
      _mctracks   = false;
      _name_track = prod;
    }
  
  return;
  }

  void ERToolAnaBase::SetVtxProducer(const bool mc, const std::string prod)
  {
    if (mc){
      _mcvtx = true;
      _name_mcvtx = prod;
    }
    else{
      _mcvtx = false;
      _name_vertex = prod;
    }

    return;
  }
      

  bool ERToolAnaBase::initialize() {
    // Nothing provided...
    if( _name_generator.empty() &&
	_name_mctrk.empty()    &&
	_name_mcshr.empty()    &&
	_name_shower.empty()    &&
	_name_track.empty() ) {
      print(msg::kERROR,__FUNCTION__,"No producer name provided!");
      return false;
    }

    // Set minimum Energy Deposited for Helper
    _helper.SetMinEDep(10);

    return true;
  }
  
  bool ERToolAnaBase::analyze(storage_manager* storage) {
    _data.Reset();
    _mc_data.Reset();
    _RecoParticles.clear();
    _McParticles.clear();
    TStopwatch fWatch;
    fWatch.Start();

    static bool one_time_warning = true;

    //------------------------------
    // Start filling SPAData object!
    
    // Fill showers
    if (_mcshowers && !_name_mcshr.empty()){
      auto ev_mcs = storage->get_data<event_mcshower> (_name_mcshr);
      if (!ev_mcs){
	print(msg::kERROR,__FUNCTION__,
	      "MCShower info not found in the event!");
	throw std::exception();
      }
	_helper.FillShowers(*ev_mcs, _data);
    }
    else if(!_mcshowers && !_name_shower.empty()){
      auto ev_shw      = storage->get_data<event_shower>    (_name_shower);
      auto ev_ctag_shw = storage->get_data<event_cosmictag> (Form("%stag",_name_shower.c_str()));
      if (!ev_shw) {
	print(msg::kERROR,__FUNCTION__,
	      "RecoShower info not found in the event!");
	throw std::exception();
      }
      if(!ev_ctag_shw){
	if(one_time_warning)
	  print(msg::kWARNING,__FUNCTION__,
		Form("One-Time-Warning: No cosmictag for shower available (\"%stag\")",_name_shower.c_str()) );
	event_cosmictag tmp;
	_helper.FillShowers(*ev_shw, tmp, _data);	  
      }
      else
	_helper.FillShowers(*ev_shw, *ev_ctag_shw, _data);
    }

    // Fill tracks
    if (_mctracks && !_name_mctrk.empty()){
      auto ev_mct = storage->get_data<event_mctrack>  (_name_mctrk);
      if (!ev_mct) {
	print(msg::kERROR,__FUNCTION__,
	      "MCTrack info not found in the event!");
      }
      _helper.FillTracks(*ev_mct, _data);
    }
    else if(!_mctracks && !_name_track.empty()) {

      auto ev_trk      = storage->get_data<event_track>       (_name_track);
      auto ev_ctag_trk = storage->get_data<event_cosmictag>   (Form("%stag",  _name_track.c_str()));
      auto ev_calo_trk = storage->get_data<event_calorimetry> (Form("%scalo", _name_track.c_str()));
      auto ev_pid_trk  = storage->get_data<event_partid>      (Form("%spid",  _name_track.c_str()));
      if (!ev_trk) {
	print(msg::kERROR,__FUNCTION__,
	      Form(" Track \"%s\" not found in the event!",_name_track.c_str()));
	throw std::exception();
      }
      if(!ev_ctag_trk){
	if(one_time_warning)
	  print(msg::kWARNING,__FUNCTION__,Form("One-Time-Warning: No cosmictag for track available (\"%stag\")",_name_track.c_str()) );
	delete ev_ctag_trk;
	ev_ctag_trk = new event_cosmictag();
      }
      if(!ev_calo_trk){
	if(one_time_warning)
	  print(msg::kWARNING,__FUNCTION__,Form("One-Time-Warning: No calorimetry for track available (\"%stag\")",_name_track.c_str()) );
	delete ev_calo_trk;
	ev_calo_trk = new event_calorimetry();
      }
      if(!ev_pid_trk){
	if(one_time_warning)
	  print(msg::kWARNING,__FUNCTION__,Form("One-Time-Warning: No PID for track available (\"%stag\")",_name_track.c_str()) );
	delete ev_pid_trk;
	ev_pid_trk = new event_partid();
      }
      _helper.FillTracks( *ev_trk, *ev_ctag_trk, *ev_calo_trk, *ev_pid_trk, _data );
    }

    // Fill Vertices
    if (_mcvtx && !_name_mcvtx.empty()){
      auto ev_mci = storage->get_data<event_mctruth> (_name_mcvtx);
      if (!ev_mci) {
	print(msg::kERROR,__FUNCTION__,
	      "MCTruth info not found in the event!");
	throw std::exception();
      }
      _helper.FillVertices(*ev_mci, _data);
    }

    // Done filling SPAData Object!
    // ----------------------------

    // If MC info is found, fill MCParticles into a ParticleSet
    if( !_name_generator.empty() &&
	!_name_mcshr.empty()     &&
	!_name_mctrk.empty() ) {
      auto ev_mci = storage->get_data<event_mctruth>  (_name_generator);
      auto ev_mcs = storage->get_data<event_mcshower> (_name_mcshr);
      auto ev_mct = storage->get_data<event_mctrack>  (_name_mctrk);
      // Make sure the data is there
      if (ev_mci && ev_mcs && ev_mct)
	_helper.FillMCInfo(*ev_mci,
			   *ev_mcs,
			   *ev_mct,
			   _mc_data,
			   _McParticles);
    }
      
    one_time_warning = false;

    //print(msg::kWARNING,__FUNCTION__,
    //Form("Time filling SPAData: %g",fWatch.RealTime()));

    return true;
  }

  bool ERToolAnaBase::finalize() {

    return true;
  }

}
#endif

