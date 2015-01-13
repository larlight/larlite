#ifndef SPTANABASE_CXX
#define SPTANABASE_CXX

#include "SPTAnaBase.h"

namespace larlite {

  SPTAnaBase::SPTAnaBase() : ana_base()
  { 
    _name="SPTAnaBase";
    MCProducer("generator","mcreco");
  }

  void SPTAnaBase::MCProducer(const std::string gen, const std::string mcst) 
  { 
    if(mcst.empty())
      throw ::sptool::SPAException("<<RecoProducer>> Cannot leave second argument an empty string!");
    _name_generator = gen; 
    _name_mcshr    = mcst; 
    _name_mctrk    = mcst; 
    _name_track     = "";
    _name_shower    = "";
  }

  void SPTAnaBase::SetShowerProducer(const bool mc, const std::string prod)
  {
    if (mc){
      _mcshowers   = true;
      _name_mcshr = prod;
      _name_shower = "";
    }
    else{
      _mcshowers   = false;
      _name_mcshr = "";
      _name_shower = prod;
    }
  
  return;
  }

  void SPTAnaBase::SetTrackProducer(const bool mc, const std::string prod)
  {
    if (mc){
      _mctracks   = true;
      _name_mctrk = prod;
      _name_track = "";
    }
    else{
      _mctracks   = false;
      _name_mctrk = "";
      _name_track = prod;
    }
  
  return;
  }

  void SPTAnaBase::SetVtxProducer(const bool mc, const std::string prod)
  {
    if (mc){
      _mcvtx = true;
      _name_mcvtx = prod;
      _name_vertex = "";
    }
    else{
      _mcvtx = false;
      _name_mcvtx = "";
      _name_vertex = prod;
    }

    return;
  }
      

  void SPTAnaBase::RecoProducer(const std::string track, const std::string shower)
  {
    if(shower.empty())
      throw ::sptool::SPAException("<<RecoProducer>> Cannot leave second argument an empty string!");
    _name_generator = "";
    _name_mctrk    = "";
    _name_mcshr    = "";
    _name_track     = track; 
    _name_shower    = shower; 
  }

  bool SPTAnaBase::initialize() {
    // Nothing provided...
    if( _name_generator.empty() &&
	_name_mctrk.empty()    &&
	_name_mcshr.empty()    &&
	_name_shower.empty()    &&
	_name_track.empty() ) {
      print(msg::kERROR,__FUNCTION__,"No producer name provided!");
      return false;
    }
    return true;
  }
  
  bool SPTAnaBase::analyze(storage_manager* storage) {

    static bool one_time_warning = false;

    //------------------------------
    // Start filling SPAData object!
    
    // Fill showers
    if (_mcshowers){
      if (!_name_mcshr.empty()){
	auto ev_mcs = storage->get_data<event_mcshower> (_name_mcshr);
	std::cout << "***************************\nmcshowers: " << ev_mcs->size() << std::endl;
	if (!ev_mcs)
	  throw ::sptool::SPAException("MCShower info not found in the event!");
	_helper.FillShowers(*ev_mcs, _data);
      }
    }
    else{
      if (!_name_shower.empty()){
	auto ev_shw      = storage->get_data<event_shower>    (_name_shower);
	auto ev_ctag_shw = storage->get_data<event_cosmictag> (Form("%stag",_name_shower.c_str()));
	if (!ev_shw)
	  throw ::sptool::SPAException("RecoShower info not found in the event!");
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
    }

    // Fill tracks
    if (_mctracks){
      if (!_name_mctrk.empty()){
	auto ev_mct = storage->get_data<event_mctrack>  (_name_mctrk);
	if (!ev_mct)
	  throw ::sptool::SPAException("MCTrack info not found in the event!");
	_helper.FillTracks(*ev_mct, _data);
      }
    }
    else{
      if (!_name_track.empty()) {
	auto ev_trk      = storage->get_data<event_track>       (_name_track);
	auto ev_ctag_trk = storage->get_data<event_cosmictag>   (Form("%stag",  _name_track.c_str()));
	auto ev_calo_trk = storage->get_data<event_calorimetry> (Form("%scalo", _name_track.c_str()));
	auto ev_pid_trk  = storage->get_data<event_partid>      (Form("%spid",  _name_track.c_str()));
	if (!ev_trk)
	  throw ::sptool::SPAException("RECO Track info not found in the event!");
	if(!ev_ctag_trk)
	  throw ::sptool::SPAException(Form("Track cosmictag info (\"%stag\" not found in the event!",_name_track.c_str()));
	if(!ev_calo_trk)
	  throw ::sptool::SPAException(Form("Track calorimetry info (\"%scalo\" not found in the event!",_name_track.c_str()));
	if(!ev_pid_trk)
	  throw ::sptool::SPAException(Form("Track partid info (\"%spid\" not found in the event!",_name_track.c_str()));
	_helper.FillTracks( *ev_trk, *ev_ctag_trk, *ev_calo_trk, *ev_pid_trk, _data );
      }
    }

    // Fill Vertices
    if (_mcvtx){
      if (!_name_mcvtx.empty()){
	auto ev_mci = storage->get_data<event_mctruth> (_name_generator);
	if (!ev_mci)
	  throw ::sptool::SPAException("MCTruth info not found in the event!");
	_helper.FillVertices(*ev_mci, _data);
      }
    }

    // Done filling SPAData Object!
    // ----------------------------

      
    /*
    if(!_name_mcreco.empty()) {
      // Use MC info
      auto ev_mct = storage->get_data<event_mctrack>  (_name_mcreco);
      auto ev_mcs = storage->get_data<event_mcshower> (_name_mcreco);
      if(!ev_mct || !ev_mcs)
	throw ::sptool::SPAException("MCShower/MCTrack info not found in the event!");

      // Generate SPAData w/ MCTrack and MCShower
      _data = _helper.Generate(*ev_mct, *ev_mcs);

      if(!_name_generator.empty()){
	
	auto ev_mci = storage->get_data<event_mctruth> (_name_generator);
	if(!ev_mci)
	  throw ::sptool::SPAException("MCTruth info not found in the event!");

	// Append MCTruth for vtx
	_helper.Append(*ev_mci,_data);
      }	

    }
    else{
      // Use Reco info
      auto ev_shw      = storage->get_data<event_shower>    (_name_shower);
      auto ev_ctag_shw = storage->get_data<event_cosmictag> (Form("%stag",_name_shower.c_str()));
      if(!ev_shw)
	throw ::sptool::SPAException(Form("Shower info (\"%s\") not found in the event!",_name_shower.c_str()));
      // if no cosmic tag info available
      if(!ev_ctag_shw && ev_shw->size()){
	event_cosmictag tmp;
	_data = _helper.Generate(*ev_shw,tmp);
	if(one_time_warning)
	  print(msg::kWARNING,__FUNCTION__,
		Form("One-Time-Warning: No cosmictag for shower available (\"%stag\")",_name_shower.c_str())
		);
      }
      // if cosmic tag info is available
      else if(ev_shw->size()) 
	_data = _helper.Generate(*ev_shw,*ev_ctag_shw);
      // if track information exists
      if(!_name_track.empty()) {
	auto ev_trk      = storage->get_data<event_track>       (_name_track);
	auto ev_ctag_trk = storage->get_data<event_cosmictag>   (Form("%stag",  _name_track.c_str()));
	auto ev_calo_trk = storage->get_data<event_calorimetry> (Form("%scalo", _name_track.c_str()));
	auto ev_pid_trk  = storage->get_data<event_partid>      (Form("%spid",  _name_track.c_str()));
	if(!ev_trk)
	  throw ::sptool::SPAException(Form("Track info (\"%s\" not found in the event!",_name_track.c_str()));
	if(ev_trk->size()) {
	  if(!ev_ctag_trk)
	    throw ::sptool::SPAException(Form("Track cosmictag info (\"%stag\" not found in the event!",_name_track.c_str()));
	  if(!ev_calo_trk)
	    throw ::sptool::SPAException(Form("Track calorimetry info (\"%scalo\" not found in the event!",_name_track.c_str()));
	  if(!ev_pid_trk)
	    throw ::sptool::SPAException(Form("Track partid info (\"%spid\" not found in the event!",_name_track.c_str()));
	  if(ev_trk->size())
	    _helper.Append(*ev_trk, *ev_ctag_trk, *ev_calo_trk, *ev_pid_trk, _data);
	}
      }
    }
    */

    one_time_warning = false;
    return true;
  }

  bool SPTAnaBase::finalize() {

    return true;
  }

}
#endif
