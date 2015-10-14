#ifndef ERToolANABASE_CXX
#define ERToolANABASE_CXX

#include "ERToolAnaBase.h"
#include "DataFormat/calorimetry.h"
#include "DataFormat/cosmictag.h"
#include "DataFormat/partid.h"
namespace larlite {

  ERToolAnaBase::ERToolAnaBase( const ::ertool::io::StreamType_t in_strm,
				const ::ertool::io::StreamType_t out_strm)
    : ana_base()
    , _mgr(in_strm,out_strm)
  { 
    _name="ERToolAnaBase";
    //MCProducer("generator","mcreco");
    _name_generator = "generator";
    _name_mcshr     = "mcreco";
    _name_mctrk     = "mcreco";
    _name_track     = "";
    _name_shower    = "";
    _name_flash     = "";
    // set cheater for single showers
    _cheater        = false;

    // turn profile mode off
    _mgr._profile_mode = false;
  }

  const ::ertool::EventData& ERToolAnaBase::GetData(bool mc) const
  {
    return (mc ? _mgr.MCEventData() : _mgr.EventData());
  }

  const ::ertool::ParticleGraph& ERToolAnaBase::GetParticles(bool mc)
  {
    return (mc ? _mgr.MCParticleGraph() : _mgr.ParticleGraph());
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

  void ERToolAnaBase::SetFlashProducer(const std::string prod)
  {
    _name_flash = prod;
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

    _mgr.Initialize();
    _mgr.GetIOHandle().Open();
    return true;
  }
  
  bool ERToolAnaBase::analyze(storage_manager* storage) {

    _mgr.ClearData();

    TStopwatch fWatch;
    fWatch.Start();

    auto in_strm = (ertool::io::EmptyInput*)(_mgr.GetIOHandle().InputStream());

    static bool one_time_warning = true;
    in_strm->SetID(storage->event_id(),
		   storage->run_id(),
		   storage->subrun_id());
    // Fill Flash
    if(!_name_flash.empty()) {

      auto ev_flash = storage->get_data<event_opflash>(_name_flash);
      if(!ev_flash) {
	print(msg::kERROR,__FUNCTION__,"OpFlash not found in the input data file!");
	throw std::exception();
      }

      _helper.FillFlashes(*ev_flash,*in_strm);

    }

    //------------------------------
    // Start filling SPAData object!
    // Fill showers
    if (_mcshowers && _name_shower.empty()){
      auto ev_mcs = storage->get_data<event_mcshower> (_name_mcshr);
      if (!ev_mcs){
	print(msg::kERROR,__FUNCTION__,
	      Form("MCShower info not found in the event for producer %s !\nLikely the producer type/name is set incorrectly\nCheck your python script!\nA producer name for MCShower needs to be set to (False,\"\") if none being used.",_name_mcshr.c_str()));
	throw std::exception();
      }

      _helper.FillShowers(*ev_mcs, *in_strm);
      // if cheater for single showers is to be used:
      if (_cheater)
	_helper.SingleShowerCheater(*ev_mcs,*in_strm);
    }
    else if(!_mcshowers && !_name_shower.empty()){

      auto ev_shw      = storage->get_data<event_shower>    (_name_shower);
      auto ev_ctag_shw = storage->get_data<event_cosmictag> (Form("%stag",_name_shower.c_str()));
      if (!ev_shw) {
	print(msg::kERROR,__FUNCTION__,
	      "RecoShower info not found in the event!\nLikely the producer type/name is set incorrectly\nCheck your python script!\nA producer name for MCShower needs to be set to (False,\"\") if none being used.");
	throw std::exception();
      }
      if(!ev_ctag_shw){
	if(one_time_warning)
	  print(msg::kWARNING,__FUNCTION__,
		Form("One-Time-Warning: No cosmictag for shower available (\"%stag\")",_name_shower.c_str()) );
      }
      _helper.FillShowers(*ev_shw, *storage, *in_strm);

      // if cheater for single showers is to be used:
      if (_cheater){
	// get mcshowers
	auto ev_mcs = storage->get_data<event_mcshower> ("mcreco");
	if (!ev_mcs){
	  print(msg::kERROR,__FUNCTION__,
		"MCShower info (for cheater) not found in the event for producer mcreco !\nLikely the producer type/name is set incorrectly\nCheck your python script!\nA producer name for MCShower needs to be set to (False,\"\") if none being used.");
	  throw std::exception();
	}
	_helper.SingleShowerCheater(*ev_mcs,*in_strm);
      }// cheater
    }

    // Fill tracks
    if (_mctracks && _name_track.empty()){
      auto ev_mct = storage->get_data<event_mctrack>  (_name_mctrk);
      if (!ev_mct) {
	print(msg::kERROR,__FUNCTION__,
	      "MCTrack info not found in the event!\nLikely the producer type/name is set incorrectly\nCheck your python script!\nA producer name for MCTrack needs to be set to (False,\"\") if none being used.");
      }
      _helper.FillTracks(*ev_mct, *in_strm);
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
      }
      if(!ev_calo_trk){
	if(one_time_warning)
	  print(msg::kWARNING,__FUNCTION__,Form("One-Time-Warning: No calorimetry for track available (\"%stag\")",_name_track.c_str()) );
      }
      if(!ev_pid_trk){
	if(one_time_warning)
	  print(msg::kWARNING,__FUNCTION__,Form("One-Time-Warning: No PID for track available (\"%stag\")",_name_track.c_str()) );
      }
      _helper.FillTracks( *ev_trk, *storage, *in_strm);
    }

    // Done filling SPAData Object!
    // ----------------------------
    // If MC info is found, fill MCParticles into a ParticleSet
    if( !_name_generator.empty() &&
	!_name_mcshr.empty()     &&
	!_name_mctrk.empty() ) {
      auto ev_mcf = storage->get_data<event_mcflux>   (_name_generator);
      auto ev_mci = storage->get_data<event_mctruth>  (_name_generator);
      auto ev_mcs = storage->get_data<event_mcshower> (_name_mcshr);
      auto ev_mct = storage->get_data<event_mctrack>  (_name_mctrk);
      // Make sure the data is there
      if (ev_mci && ev_mcs && ev_mct && ev_mcf)
	_helper.FillMCInfo(*ev_mcf,
			   *ev_mci,
			   *ev_mcs,
			   *ev_mct,
			   *in_strm);
    }

    one_time_warning = false;

    return true;
  }

  bool ERToolAnaBase::finalize() {
    _mgr.GetIOHandle().Close();
    _mgr.Finalize(_fout);
    return true;
  }

}
#endif

