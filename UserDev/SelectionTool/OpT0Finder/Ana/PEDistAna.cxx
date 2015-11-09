#ifndef LARLITE_PEDISTANA_CXX
#define LARLITE_PEDISTANA_CXX

#include "PEDistAna.h"
#include "LArUtil/Geometry.h"
#include "DataFormat/opdetwaveform.h"
#include "DataFormat/ophit.h"
#include "DataFormat/opflash.h"
#include "DataFormat/trigger.h"
#include "DataFormat/simphotons.h"
#include "LArUtil/TimeService.h"
namespace larlite {

  PEDistAna::PEDistAna()
  {
    _name="PEDistAna";
    _fout=0;
    _spe_area = 118;
    _min_time = -2;
    _max_time = 10;
    _event_tree = nullptr;
    _run_tree = nullptr;
    _ch_tree = nullptr;
  }

  bool PEDistAna::initialize() {

    // Initialize PE distribution if producer is specified
    if( !_g4_producer.empty() ) {
      _g4_event_pe.resize(32,0);
      _g4_run_pe.resize(32);
    }
    
    if( ! _opdigit_producer.empty() ) {
      if( _trigger_producer.empty() ) {
	print(msg::kERROR,__FUNCTION__,"Trigger producer must be specified to use OpDetWaveform!");
	return false;
      }
      _wf_event_pe.resize(32,0);
      _wf_run_pe.resize(32);
    }

    if( ! _ophit_producer.empty()   ) {
      _hit_event_pe.resize(32,0);
      _hit_run_pe.resize(32);
    }
    
    if( ! _opflash_producer.empty() ) {
      _flash_event_pe.resize(32,0);
      _flash_run_pe.resize(32);
    }

    if( _wf_event_pe.empty() && _hit_event_pe.empty() && _flash_event_pe.empty() ) {

      print(msg::kERROR,__FUNCTION__,"No optical PE distribution to be created...");

      return false;
    }

    print(msg::kWARNING,__FUNCTION__,"This code currently only consider 32 channels! Any channel >= 32 are ignored.");

    if(_event_tree) delete _event_tree;
    _event_tree = new TTree("ev_tree","");
    _event_tree->Branch( "run",    &_run,    "run/i"    );
    _event_tree->Branch( "subrun", &_subrun, "subrun/i" );
    _event_tree->Branch( "event",  &_event,  "event/i"  );
    _event_tree->Branch("g4_pe_v",     "std::vector<double>", &_g4_event_pe    );
    _event_tree->Branch( "wf_pe_v",    "std::vector<double>", &_wf_event_pe    );
    _event_tree->Branch( "hit_pe_v",   "std::vector<double>", &_hit_event_pe   );
    _event_tree->Branch( "flash_pe_v", "std::vector<double>", &_flash_event_pe );

    if(_ch_tree) delete _ch_tree;
    _ch_tree = new TTree("ch_tree","");
    _ch_tree->Branch( "run",    &_run,    "run/i"    );
    _ch_tree->Branch( "subrun", &_subrun, "subrun/i" );
    _ch_tree->Branch( "event",  &_event,  "event/i"  );
    _ch_tree->Branch( "ch",     &_ch,     "ch/i"     );
    _ch_tree->Branch( "g4_sum_pe",    &_g4_sum_pe,    "g4_sum_pe/D"    );
    _ch_tree->Branch( "wf_sum_pe",    &_wf_sum_pe,    "wf_sum_pe/D"    );
    _ch_tree->Branch( "hit_sum_pe",   &_hit_sum_pe,   "hit_sum_pe/D"   );
    _ch_tree->Branch( "flash_sum_pe", &_flash_sum_pe, "flash_sum_pe/D" );

    return true;
  }
  
  bool PEDistAna::analyze(storage_manager* storage) {

    // Initialize PE distribution
    for(auto& v : _g4_event_pe)    v = 0; 
    for(auto& v : _wf_event_pe)    v = 0;
    for(auto& v : _hit_event_pe)   v = 0;
    for(auto& v : _flash_event_pe) v = 0;

    _g4_sum_pe = _wf_sum_pe = _hit_sum_pe = _flash_sum_pe = -1;

    // Some size reservation to reduce process time
    if(storage->get_index() % 1000 == 0) {
      for(size_t i=0; i<32; ++i) {
	_g4_run_pe[i].reserve    ( _g4_run_pe[i].size()    + 1000 );
	_wf_run_pe[i].reserve    ( _wf_run_pe[i].size()    + 1000 );
	_hit_run_pe[i].reserve   ( _hit_run_pe[i].size()   + 1000 );
	_flash_run_pe[i].reserve ( _flash_run_pe[i].size() + 1000 );
      }
    }

    // Event id retrieval
    _run    = storage->run_id();
    _subrun = storage->subrun_id();
    _event  = storage->event_id();

    //
    // Analyze G4 photons
    //
    if(!_g4_producer.empty()) {
      auto ev_simph = storage->get_data<event_simphotons>(_g4_producer);
      
      if(!ev_simph || ev_simph->empty() )

	print(msg::kERROR,__FUNCTION__,Form("G4 SimPhotons by %s not found!", _g4_producer.c_str()));

      else {

	for(auto const& simph : *ev_simph) {

	  auto const ch = simph.OpChannel();

	  if(ch>31) continue;

	  // Check time
	  for(auto const& ph : simph) {

	    if(ph.Time < _min_time * 1000 || ph.Time > _max_time * 1000) continue;
	    
	    _g4_event_pe[ch] += 1;
	  }
	  
	}

	for(size_t i=0; i<_g4_event_pe.size(); ++i)

	  _g4_run_pe[i].push_back( _g4_event_pe[i] );
      }
    }
    
    //
    // Analyze waveform
    //
    if(!_opdigit_producer.empty()) {

      // Get OpDetWaveform & trigger
      auto ev_opdigit = storage->get_data<event_opdetwaveform>(_opdigit_producer);
      auto trig_ptr = storage->get_data<trigger>(_trigger_producer);
      
      // Skip if not found
      if( !ev_opdigit || ev_opdigit->empty() )

	print(msg::kERROR,__FUNCTION__,Form("OpDetWaveform by %s not found!",_opdigit_producer.c_str()));

      else if( !trig_ptr )

	print(msg::kERROR,__FUNCTION__,Form("Trigger by %s not found!",_trigger_producer.c_str()));

      else {

	auto const tick_period = larutil::TimeService::GetME()->OpticalClock().TickPeriod();

	auto const trig_time = trig_ptr->TriggerTime();

	// Loop over waveforms
	for(auto const& wf : *ev_opdigit) {

	  // Skip channel > 32
	  auto const ch = wf.ChannelNumber();
	  if(ch > 32) continue;
	  
	  // Skip pmt_id > 32
	  auto const pmt_id = ::larutil::Geometry::GetME()->OpDetFromOpChannel(ch);
	  if(pmt_id > 32) continue;

	  double wf_start = wf.TimeStamp() - trig_time;
	  double wf_end   = wf.TimeStamp() + (double)(wf.size() * tick_period) - trig_time;

	  // Skip out-of-time waveforms
	  if(wf_end < _min_time || wf_start > _max_time) continue;

	  double q = 0;
	  double ped_mean = wf[0];

	  for(size_t i=0; i<wf.size(); ++i) {

	    double tick_time = wf_start + (double)(i * tick_period);

	    if(tick_time < _min_time || tick_time > _max_time) continue;

	    q += wf[i] - ped_mean;
	  }

	  _wf_event_pe[pmt_id] += ( q / _spe_area );
	}

	for(size_t i=0; i<_wf_event_pe.size(); ++i)

	  _wf_run_pe[i].push_back( _wf_event_pe[i] );

      }
    }

    //
    // Analyze OpHit
    // 
    if(!_ophit_producer.empty()) {

      // Get OpHit
      auto ev_ophit = storage->get_data<event_ophit>(_ophit_producer);

      // Skip if not found
      if(!ev_ophit || ev_ophit->empty())

	print(msg::kERROR,__FUNCTION__,Form("OpHit by %s not found!",_ophit_producer.c_str()));

      else {

	// Loop over waveforms
	for(auto const& h : *ev_ophit) {

	  // Skip channel > 32
	  auto const ch = h.OpChannel();
	  if(ch > 32) continue;
	  
	  // Skip pmt_id > 32
	  auto const pmt_id = ::larutil::Geometry::GetME()->OpDetFromOpChannel(ch);
	  if(pmt_id > 32) continue;

	  // Skip out-of-time waveforms
	  if(h.PeakTime() < _min_time || h.PeakTime() > _max_time) continue;

	  _hit_event_pe[pmt_id] += h.PE();

	}

	for(size_t i=0; i<_hit_event_pe.size(); ++i)

	  _hit_run_pe[i].push_back( _hit_event_pe[i] );
	
      }
    }

    //
    // Analyze OpFlash
    //
    if(!_opflash_producer.empty()) {
	  
      // Get OpFlash
      auto ev_opflash = storage->get_data<event_opflash>(_opflash_producer);

      // Skip if not found
      if(!ev_opflash || ev_opflash->empty())

	print(msg::kERROR,__FUNCTION__,Form("OpFlash by %s not found!",_opflash_producer.c_str()));

      else {

	// Loop over waveforms
	for(auto const& f : *ev_opflash) {

	  // Skip out-of-time waveforms
	  if(f.Time() < _min_time || f.Time() > _max_time) continue;

	  for(size_t ch=0; ch<32; ++ch) {

	    // Skip pmt_id > 32
	    auto const pmt_id = ::larutil::Geometry::GetME()->OpDetFromOpChannel(ch);
	    if(pmt_id > 32) continue;
	  
	    _flash_event_pe[pmt_id] += f.PE(ch);
	  }
	}

	for(size_t i=0; i<_flash_event_pe.size(); ++i) {

	  // Skip channel > 32
	  _flash_run_pe[i].push_back( _flash_event_pe[i] );

	}
	
      }
    }

    _event_tree->Fill();

    // Fill ch-wise TTree
    for(size_t i=0; i<32; ++i) {

      _ch = i;
      if(_g4_event_pe.size())    _g4_sum_pe    = _g4_event_pe[i];
      if(_wf_event_pe.size())    _wf_sum_pe    = _wf_event_pe[i];
      if(_hit_event_pe.size())   _hit_sum_pe   = _hit_event_pe[i];
      if(_flash_event_pe.size()) _flash_sum_pe = _flash_event_pe[i];

      _ch_tree->Fill();
    }
    
    return true;
  }

  bool PEDistAna::finalize() {

    if(_fout) {

      if(_run_tree) delete _run_tree;
      _run_tree = new TTree("run_tree","");
      _run_tree->Branch( "wf_name",    "std::string", &_opdigit_producer );
      _run_tree->Branch( "hit_name",   "std::string", &_ophit_producer   );
      _run_tree->Branch( "flash_name", "std::string", &_opflash_producer );
      for(size_t i=0; i<32; ++i) {
	_run_tree->Branch( Form( "g4_pe_pmt%02zu_v",    i), "std::vector<double>", &_g4_run_pe[i]    );
	_run_tree->Branch( Form( "wf_pe_pmt%02zu_v",    i), "std::vector<double>", &_wf_run_pe[i]    );
	_run_tree->Branch( Form( "hit_pe_pmt%02zu_v",   i), "std::vector<double>", &_hit_run_pe[i]   );
	_run_tree->Branch( Form( "flash_pe_pmt%02zu_v", i), "std::vector<double>", &_flash_run_pe[i] );
      }
      _run_tree->Fill();

      _event_tree->Write();
      _ch_tree->Write();
      _run_tree->Write();
    }
    
    return true;
  }

}
#endif
