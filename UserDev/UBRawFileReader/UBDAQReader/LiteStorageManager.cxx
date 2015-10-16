#ifndef LITESTORAGEMANAGER_CXX
#define LITESTORAGEMANAGER_CXX

#include "LiteStorageManager.h"
#include <iostream>
#include <iosfwd>
#include <memory>
#include <algorithm>
#include <iomanip>
#include <stdio.h>
#include <unistd.h>

#include "datatypes/raw_data_access.h"
#include "datatypes/uboone_data_utils.h"
//#include "datatypes/ub_EventRecord.h"

#include "DataFormat/storage_manager.h"
#include "DataFormat/fifo.h"
#include "DataFormat/trigger.h"
#include "DataFormat/rawdigit.h"
#include "DataFormat/opdetwaveform.h"
#include <TString.h>

namespace ubdaq {

  LiteStorageManager::LiteStorageManager()
    : _read_trigger       (true)
    , _read_fifo          (true)
    , _read_opdetwaveform (true)
    , _read_rawdigit      (true)

    , _ev_rawdigit        (nullptr)
    , _ev_fifo            (nullptr)
    , _ev_opdetwaveform   (nullptr)
    , _trigger            (nullptr)

    , _current_input_index (-1)
    , _eventRecord(nullptr)
    , _outfile("")

    , _is(nullptr)
  {}

  void LiteStorageManager::Reset()
  {
    if(_mgr.is_open()) {
      _mgr.close();
      _mgr.reset();
      _ev_rawdigit = nullptr;
      _ev_fifo     = nullptr;
      _ev_opdetwaveform = nullptr;
      _trigger     = nullptr;
    }else{
      if(_ev_rawdigit) delete _ev_rawdigit;
      if(_ev_fifo)     delete _ev_fifo;
      if(_ev_opdetwaveform) delete _ev_opdetwaveform;
      if(_trigger)     delete _trigger;
      _ev_rawdigit = nullptr;
      _ev_fifo     = nullptr;
      _ev_opdetwaveform = nullptr;
      _trigger     = nullptr;      
    }
    if(_is) delete _is;
    _is = nullptr;
  }

  void LiteStorageManager::Initialize()
  {
    if(_current_input_index>=0)
      throw std::exception();
    if(_input_v.empty()) 
      throw std::exception();

    if(!_outfile.empty()) {
      _mgr.set_io_mode(::larlite::storage_manager::kWRITE);
      _mgr.set_out_filename(_outfile);
      _mgr.open();
      if(!_mgr.is_open()) throw std::exception();
    }
    LoadUBChannelMap(fChannelMap,fChannelReverseMap);
    
    _current_input_index = 0;
    _eof = true;
    _event_ctr = 0;

    ::peek_at_next_event<ub_TPC_CardData_v6>(false);
    ::peek_at_next_event<ub_PMT_CardData_v6>(false);
    ::handle_missing_words<ub_TPC_CardData_v6>(true);
    ::handle_missing_words<ub_PMT_CardData_v6>(true);
  }

  const ::larlite::event_rawdigit& LiteStorageManager::RawDigit () const
  { 
    if(!_ev_rawdigit) {
      std::cerr<<"RawDigit not yet read-in!"<<std::endl;
      throw std::exception();
    }
    return *_ev_rawdigit;
  }
  const ::larlite::event_fifo& LiteStorageManager::FIFO () const
  { 
    if(!_ev_fifo) {
      std::cerr<<"FIFO not yet read-in!"<<std::endl;
      throw std::exception();
    }
    return *_ev_fifo;
  }
  const ::larlite::event_opdetwaveform& LiteStorageManager::OpDetWaveform () const
  { 
    if(!_ev_opdetwaveform) {
      std::cerr<<"OpDetWaveform not yet read-in!"<<std::endl;
      throw std::exception();
    }
    return *_ev_opdetwaveform;
  }
  const ::larlite::trigger& LiteStorageManager::Trigger () const
  {
    if(!_trigger) {
      std::cerr<<"Trigger not yet read-in!"<<std::endl;
      throw std::exception();
    }
    return *_trigger;
  }

  bool LiteStorageManager::ProcessEvent()
  {
    if(_current_input_index < 0){
      std::cerr << "Initialize first!" << std::endl;
      throw std::exception();
    }

    if(_eof){
      if(_current_input_index >= _input_v.size()) {
	std::cout << "Finished processing all files..." << std::endl;
	if(!_outfile.empty() && _mgr.is_open()) _mgr.close();
	return false;
      }
      else {
	std::cout << "Opening a file: " << _input_v[_current_input_index].c_str() << std::endl;
	if(_is) { _is->close(); delete _is;}
	_is = new std::ifstream( _input_v[_current_input_index], std::ios::binary | std::ios::in);
	_eventRecord = new ub_EventRecord;
	_eof = false;
	return ProcessEvent();
      }
    }

    //try{
    ::boost::archive::binary_iarchive ia(*_is);
    ub_EventRecord  eventRecord;
    ia >> eventRecord;
    ProcessRecord(eventRecord);
    //}catch(...) {
    //std::cerr << "Could not interpret data... skipping (possibly eof)" <<std::endl;
    //_eof = true;
    //_current_input_index +=1;
    //return ProcessEvent();
    //}
    ++_event_ctr;
    std::cout << _event_ctr<< std::endl;
    std::cout << "Run:" << _mgr.run_id() << std::endl;
    std::cout << "Event: " << _mgr.event_id() << std::endl;
    std::cout << "SubRun: " << _mgr.subrun_id() << std::endl;
    return true;
  }

  void LiteStorageManager::ProcessRecord(ub_EventRecord& eventRecord)
  {
    global_header_t const& globalHeader = eventRecord.getGlobalHeader();
    //size_t n_total_fragments = eventRecord.getFragmentCount();

    if(_read_trigger) {
      
      if(_mgr.is_open()) _trigger = _mgr.get_data<larlite::trigger>("daq");
      else if(!_trigger) _trigger = new ::larlite::trigger;

      for(auto const& trig_map : eventRecord.getTRIGSEBMap()){
	  
	auto const& trig_data = trig_map.second.getTriggerCardData();
	  
	larlite::trigger trig((unsigned int)(trig_data.getTrigNumber()),
			      (double)((trig_data.getFrame()*1600. + trig_data.getSample()*(1./16.) )),
			      (double)0,
			      (unsigned int)(trig_data.getTriggerData().trig_data_1));
	(*_trigger) = trig;
      }
    }
    
    if(_read_fifo) {

      if(_mgr.is_open()) _ev_fifo = _mgr.get_data<larlite::event_fifo>("daq");
      else if(!_ev_fifo) _ev_fifo = new ::larlite::event_fifo;

      for(auto const& seb : eventRecord.getPMTSEBMap()){
	  
	//unsigned int crate = seb.first;
	auto const& crate_data = seb.second;
	  
	if ( !crate_data.wasDissected() ) {
	  std::cerr << "Warning crate data corrupted! Skipping." << std::endl;
	  crate_data.dissectionException().what();
	  continue;
	}
	  
	bool first_card=true;
	for(auto const& card_data : crate_data.getCards()) {
	      
	  if ( !card_data.wasDissected() ) {
	    std::cerr << "Warning card data corrupted! Skipping." << std::endl;
	    card_data.dissectionException().what();
	    continue;
	  }
	      
	  unsigned int trig_frame = RollOver(card_data.getFrame(),
					      card_data.getTrigFrameMod16(),
					      4);
	      
	  if(first_card){
	    // Do first-card action
	    _ev_fifo->set_event_number(card_data.getEvent());
	    _ev_fifo->set_checksum(card_data.getChecksum());
	    _ev_fifo->set_nwords(card_data.getWordCount());
	    _ev_fifo->set_event_frame_number(card_data.getFrame());
	    _ev_fifo->set_fem_trig_frame_number(trig_frame);
	    _ev_fifo->set_fem_trig_sample_number(card_data.getTrigSample());
	          
	  }
	  else{
	    // Do checks for 2nd and more cards
	    if( _ev_fifo->fem_trig_frame_number()      != trig_frame ||
		_ev_fifo->event_frame_number()         != card_data.getFrame() ||
		_ev_fifo->fem_trig_sample_number_RAW() != card_data.getTrigSample() )
	      std::cerr<<"\033[93m<<ERROR>>\033[00m disagreement on inter-card timings..."<<std::endl
		       <<"  First card ... event frame: "<<_ev_fifo->event_frame_number()
		       <<" trig frame: " <<_ev_fifo->fem_trig_frame_number()
		       <<" trig sample: "<<_ev_fifo->fem_trig_sample_number_RAW()
		       <<std::endl
		       <<"  This card ... event frame: "<< card_data.getFrame()
		       <<" trig frame: " <<trig_frame
		       <<" trig sample: "<<card_data.getTrigSample()
		       << std::endl;
	  }
	      
	  for(auto const& ch_data : card_data.getChannels()) {
	    /*
	      if ( !ch_data.wasDissected() ) {
	      std::cerr << "Warning channel data corrupted! Skipping." << std::endl;
	      //channel.dissectionException().what();                                                                               
	      continue;
	      }
	    */    
	    auto const& window_v = ch_data.getWindows();
	    for(auto const& window : window_v) {
	      auto const& window_header = window.header();
	      const ub_RawData& window_data = window.data();
	      
	      unsigned int ch_frame = RollOver(card_data.getFrame(),window_header.getFrame(),3);
	      ::larlite::fifo pmtwf;
	      pmtwf.reserve(window_data.size());
	      pmtwf.set_channel_number( ch_data.getChannelNumber() );
	      pmtwf.set_readout_frame_number( ch_frame );
	      pmtwf.set_readout_sample_number( window_header.getSample() );
	      pmtwf.set_module_address( card_data.getModule() );
	      pmtwf.set_disc_id((larlite::fem::Discriminator_t)(window_header.getDiscriminantor()));
	      
	      for(auto const& adc : window_data) pmtwf.push_back(adc & 0xfff);
	      
	      _ev_fifo->emplace_back(pmtwf);
	      
	    }
	  }
	}
      }
    }

    if(_read_opdetwaveform) {

      if(_mgr.is_open()) _ev_opdetwaveform = _mgr.get_data<larlite::event_opdetwaveform>("daq");
      else if(!_ev_opdetwaveform) _ev_opdetwaveform = new ::larlite::event_opdetwaveform;

    }

    if(_read_rawdigit) {

      if(_mgr.is_open()) _ev_rawdigit = _mgr.get_data<larlite::event_rawdigit>("daq");
      else if(!_ev_rawdigit) _ev_rawdigit = new ::larlite::event_rawdigit;

      for( auto const& seb_it : eventRecord.getTPCSEBMap()) {    // I think auto should be tpc_map_t::const_iterator  -NJT
	  
	tpc_crate_data_t const& tpc_crate = seb_it.second; // (ub_TPC_CrateData_v6)
	int crate_number = seb_it.first; // confirmed this is correct. for now, crate's are not given their ID number to store and then retrieve.
	
	if ( !tpc_crate.wasDissected() ) {
	  std::cerr << "Warning crate data corrupted! Skipping." << std::endl;
	  tpc_crate.dissectionException().what();
	  continue;
	}
	
	auto const& tpc_crate_DAQ_header = tpc_crate.crateHeader(); // I think auto should be tpc_crate_data_t::ub_CrateHeader_t --NJT
	
	for(auto const& card : tpc_crate.getCards()){  // This auto is tpc_crate_data_t::card_t
	      
	  if ( !card.wasDissected() ) {
	    std::cerr << "Warning card data corrupted! Skipping." << std::endl;
	    card.dissectionException().what();
	    continue;
	  }
	      
	  for(auto const& channel : card.getChannels()){ // auto here tpc_crate_data_t::card_t::card_channel_type
	          
	          
	    if ( !channel.wasDissected() ) {
	      std::cerr << "Warning channel data corrupted! Skipping." << std::endl;
	      //channel.dissectionException().what();
	      continue;
	    }
	          
	    auto const tpc_channel_number = channel.getChannelNumber(); // auto is int here
	          
	          
	    // output:
	    std::vector<short> adclist;
	    adclist.reserve(9600);
	    size_t chdsize(0); 
	          
	    channel.decompress(adclist); // All-in-one call.
	    uint16_t frailer = channel.getChannelTrailerWord();
	    /*
	    short lachadawin = adclist.at( adclist.size()-1 );
	    if ( (frailer>>12 != 0x5) || ( (frailer&0xfff) != tpc_channel_number ) ) {
	      std::vector<short> kaxufix = decodeChannelTrailer( (unsigned short)lachadawin, (unsigned short)frailer );
	      for ( auto& it : kaxufix )
		adclist.emplace_back( it );
	    }
	    */	          
	    chdsize = adclist.size();
	    const static size_t          fAdcList_size = chdsize;
	    if (fAdcList_size!=chdsize) {
	      std::cerr << "Unexpected data: Number of words for channel: " 
			<< tpc_channel_number << " is " << chdsize << " and different than first waveform in the readout"
			<< " (" << fAdcList_size << "). That's really bad!!!" << std::endl;
	      throw std::exception();
	    }
	          
	    UBDaqID daqId( crate_number, card.getModule(), tpc_channel_number);
	          
	    int ch=0;
	    auto it_chsearch = fChannelMap.find(daqId);
	    if ( it_chsearch!=fChannelMap.end() ){
	      ch=(*it_chsearch).second;
	    }
	    else {
	      if ( ( crate_number==1 && card.getModule()==8 && (tpc_channel_number>=32 && tpc_channel_number<64) ) ||
		   ( crate_number==9 && card.getModule()==5 && (tpc_channel_number>=32 && tpc_channel_number<64) ) ) {
		continue; // do not write to data product                                                                                                                                            
	      }
	      else {
		char warn[256];
		sprintf( warn, "Warning DAQ ID not found ( %d, %d, %d )!", crate_number, card.getModule(), tpc_channel_number );
		throw std::runtime_error( warn );
	      }
	    }
	    ::larlite::raw::Compress_t compression=::larlite::raw::kNone; // as of June 19,2015 compression not used by the DAQ. Data stored is uncompressed.                                                              
	    if ( adclist.size()!=9595 ) {
	      char warn[256];
	      sprintf( warn, "Error: Number of ADCs in (crate,slot,channel)=( %d, %d, %d ) does not equal 9595!", crate_number, card.getModule(), tpc_channel_number );
	      throw std::runtime_error( warn );
	    }
	          
	    ::larlite::rawdigit rd(ch,chdsize,std::move(adclist),(::larlite::raw::Compress_t)(compression));
	    _ev_rawdigit->emplace_back(rd);
	  }
	}
      }
    }
    _mgr.set_id(globalHeader.getRunNumber(),globalHeader.getSubrunNumber(),globalHeader.getEventNumber());
    _mgr.next_event();
  }
  
}

#endif
