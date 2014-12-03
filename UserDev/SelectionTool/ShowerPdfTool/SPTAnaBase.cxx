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
    _name_mcreco    = mcst; 
    _name_track     = "";
    _name_shower    = "";
  }

  void SPTAnaBase::RecoProducer(const std::string track, const std::string shower)
  {
    if(shower.empty())
      throw ::sptool::SPAException("<<RecoProducer>> Cannot leave second argument an empty string!");
    _name_generator = "";
    _name_mcreco    = "";
    _name_track     = track; 
    _name_shower    = shower; 
  }

  bool SPTAnaBase::initialize() {
    // Nothing provided...
    if( _name_generator.empty() &&
	_name_mcreco.empty()    &&
	_name_shower.empty()    &&
	_name_track.empty() ) {
      print(msg::kERROR,__FUNCTION__,"No producer name provided!");
      return false;
    }
    return true;
  }
  
  bool SPTAnaBase::analyze(storage_manager* storage) {

    static bool one_time_warning = false;
    if(!_name_mcreco.empty()) {
      // Use MC info
      auto ev_mct = storage->get_data<event_mctrack>  (_name_mcreco);
      auto ev_mcs = storage->get_data<event_mcshower> (_name_mcreco);
      if(!ev_mct || !ev_mcs)
	throw ::sptool::SPAException("MCShower/MCTrack info not found in the event!");

      _data = _helper.Generate(*ev_mct, *ev_mcs);

      if(!_name_generator.empty()){
	
	auto ev_mci = storage->get_data<event_mctruth> (_name_generator);
	if(!ev_mci)
	  throw ::sptool::SPAException("MCTruth info not found in the event!");
	
	_helper.Append(*ev_mci,_data);
      }	

    }else{
      // Use Reco info
      auto ev_shw      = storage->get_data<event_shower>    (_name_shower);
      auto ev_ctag_shw = storage->get_data<event_cosmictag> (Form("%stag",_name_shower.c_str()));
      if(!ev_shw)
	throw ::sptool::SPAException(Form("Shower info (\"%s\") not found in the event!",_name_shower.c_str()));
      if(!ev_ctag_shw){
	event_cosmictag tmp;
	_data = _helper.Generate(*ev_shw,tmp);
	if(one_time_warning)
	  print(msg::kWARNING,__FUNCTION__,
		Form("One-Time-Warning: No cosmictag for shower available (\"%stag\")",_name_shower.c_str())
		);
      }else
	_data = _helper.Generate(*ev_shw,*ev_ctag_shw);
      
      if(!_name_track.empty()) {
	auto ev_trk      = storage->get_data<event_track>       (_name_track);
	auto ev_ctag_trk = storage->get_data<event_cosmictag>   (Form("%stag",  _name_track.c_str()));
	auto ev_calo_trk = storage->get_data<event_calorimetry> (Form("%scalo", _name_track.c_str()));
	if(!ev_trk)
	  throw ::sptool::SPAException(Form("Track info (\"%s\" not found in the event!",_name_track.c_str()));
	if(!ev_ctag_trk)
	  throw ::sptool::SPAException(Form("Track cosmictag info (\"%stag\" not found in the event!",_name_track.c_str()));
	if(!ev_calo_trk)
	  throw ::sptool::SPAException(Form("Track calorimetry info (\"%scalo\" not found in the event!",_name_track.c_str()));
	_helper.Append(*ev_trk, *ev_ctag_trk, *ev_calo_trk, _data);
      }
    }

    one_time_warning = false;
    return true;
  }

  bool SPTAnaBase::finalize() {

    return true;
  }

}
#endif
