#ifndef LARLITE_ANA_PROCESSOR_CXX
#define LARLITE_ANA_PROCESSOR_CXX

#include "ana_processor.h"

namespace larlite {
  
  ana_processor::ana_processor(){
    _name="ana_processor";
    _ofile_name="";
    _storage=storage_manager::get();
    _fout=0;
    reset();
    _filter_enable = false;
  }
  
  void ana_processor::set_verbosity(msg::Level level){
    
    larlite_base::set_verbosity(level);
    if(_storage)
      _storage->set_verbosity(level);
    
    for(std::vector<ana_base*>::iterator iter(_analyzers.begin());
	iter!=_analyzers.end();
	++iter)

      (*iter)->set_verbosity(level);
    
  }
  
  void ana_processor::reset(){
    
    if(_verbosity[msg::kDEBUG])
      Message::send(msg::kDEBUG,__PRETTY_FUNCTION__,"called...");
    
    if(_fout){
      _fout->Close();
      _fout=0;
    }
    
    if(_storage)
      _storage->reset();
    
    _analyzers.clear();
    _ana_status.clear();
    _ana_index.clear();
    _nevents=0;
    _index=0;
    
    _process=kINIT;
  }
  
  Bool_t ana_processor::initialize(){
    
    set_verbosity(_verbosity_level);
    
    if(_verbosity[msg::kDEBUG])
      Message::send(msg::kDEBUG,__PRETTY_FUNCTION__,"called...");
    
    if(_process!=kINIT){
      Message::send(msg::kERROR,__FUNCTION__,
		    "Logic error: the function should not be called.");
      return false;
    }
    
    //_storage->set_io_mode(storage_manager::READ);
    if(!_storage->open()) {
      
      Message::send(msg::kERROR,__FUNCTION__,"File I/O failure...");
      
      return false;
      
    }
    
    if(_ofile_name.size()==0)
      
      Message::send(msg::kWARNING,__FUNCTION__,
		    "Analysis output file will not be created for this time...");

    else
      
      _fout=TFile::Open(_ofile_name.c_str(),"RECREATE");
    

    Bool_t status = true;

    _ana_status.resize(_analyzers.size(),false);
    
    for(size_t i=0; i<_analyzers.size(); ++i) {
      
      _analyzers[i]->set_output_file(_fout);

      _ana_status[i] = _analyzers[i]->initialize();

      if(!_ana_status[i]) {
	
	Message::send(msg::kERROR,__PRETTY_FUNCTION__,
		      Form("Failed to initialize: %s",_analyzers[i]->class_name().c_str()));
	
	status = false;
      }
      
    }    
    _process=kREADY;
    _index=0;
    _nevents=0;
    return status;
  }
  
  Bool_t ana_processor::process_event(UInt_t index){

    if(index == data::kINVALID_UINT) index = _index;

    _index = index;
    
    if(_process==kINIT) {
      
      if(!initialize()) {
	Message::send(msg::kERROR,__FUNCTION__,"Aborting.");
	return false;
      }
      else
	_process=kPROCESSING;
    }
    
    if(_process==kREADY)
      _process=kPROCESSING;
    
    bool event_found = _storage->go_to(index);
    
    if(event_found){

      for(size_t i=0; i<_analyzers.size(); ++i){

	_ana_status[i] = false;

	_ana_status[i] = _analyzers[i]->analyze(_storage);

	if(!_ana_status[i] && _filter_enable) break;
	
      }
      
      _index++;
      _nevents++;
      
      return true;
    }
    else
      return finalize();
  }
  
  Bool_t ana_processor::run(UInt_t start_index, UInt_t nevents){
    
    if(_verbosity[msg::kDEBUG])
      Message::send(msg::kDEBUG,__PRETTY_FUNCTION__,"called...");
    
    Bool_t status=true;
    
    if(_process==kINIT) status = initialize();
    
    if(!status){
      
      Message::send(msg::kERROR,__PRETTY_FUNCTION__,"Aborting.");
      
      return false;
    }
    
    _index=start_index;
    
    if(!nevents)
      nevents=_storage->get_entries();
    if(nevents > (_storage->get_entries() - _index))
      nevents=_storage->get_entries() - _index;
    
    sprintf(_buf,"Processing %d events from entry %d...",nevents, _index);
    Message::send(msg::kNORMAL,__FUNCTION__,_buf);
    
    int ten_percent_ctr=0;
    
    while(status){
      
      status=process_event(_index);

      if( nevents >= 10 && (_nevents >= ten_percent_ctr * nevents/10.) ) {

	if(ten_percent_ctr) {
	  sprintf(_buf," ... %3d%% done ...",ten_percent_ctr*10);
	  Message::send(msg::kNORMAL,__FUNCTION__,_buf);
	}
	ten_percent_ctr++;
      }
      
      if(nevents && nevents == _nevents){
	Message::send(msg::kNORMAL,__FUNCTION__,Form("Processed %d/%d events! Aborting...",_nevents,nevents));

	_storage->next_event();

	break;
      }
            
      if(_process!=kPROCESSING) break;
      
    }

    if(_process != kFINISHED)
      status = finalize();

    return status;
    
  }
  
  Bool_t ana_processor::finalize() {

    if(_verbosity[msg::kDEBUG])
      Message::send(msg::kDEBUG,__PRETTY_FUNCTION__,"called...");
    
    if(_process!=kPROCESSING && _process!=kREADY) {
      Message::send(msg::kERROR,__FUNCTION__,
		    "Logic error: the function should not be called.");
      return false;
    }
    
    Bool_t status=true;

    for(size_t i=0; i<_analyzers.size(); ++i){
      
      _ana_status[i] = _analyzers[i]->finalize() && _ana_status[i];
      
      status = status && _ana_status[i];
    }
    
    _process=kFINISHED;
    reset();
    return status;  
  }
  
  Bool_t ana_processor::get_ana_status(ana_base* ptr) const{
    
    std::map<ana_base*,size_t>::const_iterator iter(_ana_index.find(ptr));
    if(iter==_ana_index.end()) return false;
    
    else return _ana_status[(*iter).second];
    
  }
}
#endif
