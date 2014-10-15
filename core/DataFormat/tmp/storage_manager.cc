#ifndef STORAGE_MANAGER_CC
#define STORAGE_MANAGER_CC
#include "storage_manager.hh"

namespace larlight {

  storage_manager* storage_manager::me=0;
  
  storage_manager::storage_manager(storage_manager::MODE mode)
    : larlight_base(), _treename("pmt_data")
  {
    _name="storage_manager";
    //_event_wf=0;
    _fout=0;
    _out_fname="";
    _name_in_tdirectory="";
    _name_out_tdirectory="";
    _status=INIT;
    _check_alignment=true;

    reset();
    _mode=mode;
    
  };
  
  event_base* storage_manager::get_data(DATA::DATA_TYPE type){
    
    // Read entry _index-1
    if(!_index && _mode != WRITE) {

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Call next_event() before calling %s", __FUNCTION__));

      return 0;
    }

    size_t ptr_index = (size_t)(type);
    
    // If in READ mode, here is where we read in data product
    if(_in_ch[ptr_index] && _mode == READ) {

      _in_ch[ptr_index]->GetEntry(_index-1);

      if( _check_alignment ) {

	if(_current_event_id==DATA::INVALID_INT) 

	  _current_event_id = _ptr_data_array[ptr_index]->event_id();

	else if(_ptr_data_array[ptr_index]->event_id() != _current_event_id) {
	
	  print(MSG::ERROR,__FUNCTION__,
		Form("Detected event-alignment mismatch! (%d != %d)",
		     _ptr_data_array[ptr_index]->event_id(), _current_event_id)
		);
	  
	  return 0;
	  
	}
      }
    }
    
    // If data class object does not exist, and if it's either WRITE or BOTH mode, create it.
    if(!_ptr_data_array[type] && _mode != READ){
      
      create_data_ptr(type);

      if(_ptr_data_array[(size_t)type]) {
	
	_fout->cd(_name_out_tdirectory.c_str());
	
	_out_ch[(size_t)type]=new TTree(Form("%s_tree",DATA::DATA_TREE_NAME[type].c_str()),
					Form("%s Tree",DATA::DATA_TREE_NAME[type].c_str()));
	_out_ch[(size_t)type]->SetMaxTreeSize    (1024*1024*1024);
	_out_ch[(size_t)type]->SetMaxVirtualSize (1024*1024*1024);      
	
	_out_ch[(size_t)type]->Branch(Form("%s_branch",DATA::DATA_TREE_NAME[type].c_str()),
				      _ptr_data_array[(size_t)type]->GetName(),
				      &(_ptr_data_array[(size_t)type]));
	
	Message::send(MSG::WARNING,__FUNCTION__,
		      Form("Requested tree %s which does not exists yet. Created a new one.", 
			   _out_ch[(size_t)type]->GetName())
		      );
	
	_write_data_array[(size_t)type]=true;
      }
    }
    
    return (event_base*)(_ptr_data_array[type]);
    
  }
  
  std::vector<larlight::DATA::DATA_TYPE> storage_manager::list_data_types(){
    std::vector<larlight::DATA::DATA_TYPE> list;
    for(int i=0;i<larlight::DATA::DATA_TYPE_MAX;i++) {
      if( _ptr_data_array[i]) { list.push_back((larlight::DATA::DATA_TYPE)(i)); }
    }
    return list;
  }
  
  void storage_manager::reset()
  {
    if(_verbosity[MSG::DEBUG])
      Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"called ...");
    
    switch(_status){
    case READY_IO:
    case OPENED:
    case OPERATING:
      close();
      break;
    case INIT:
    case CLOSED:
      break;
    }
    
    _current_event_id = DATA::INVALID_INT;
    _index=0;
    _nevents=0;
    _nevents_written=0;
    _nevents_read=0;
    _mode=UNDEFINED;
    _status=INIT;
    _in_fnames.clear();
    
    for(size_t i=0; i<DATA::DATA_TYPE_MAX; ++i) {
      _read_data_array[i]=true;   // Attempt to read in all data by default
      _write_data_array[i]=false; // Attemp to write no data by default 
      _in_ch[i]=0;
      _out_ch[i]=0;
      _ptr_data_array[i]=0;
    }
    
    if(_verbosity[MSG::DEBUG])
      Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"ends ...");  
  };
  
  bool storage_manager::is_open()
  {
    bool status=true;
    switch(_status){
    case OPENED:
    case READY_IO:
    case OPERATING:
      break;
    case INIT:
    case CLOSED:
      status=false;
    }
    
    return status;
  }
  
  bool storage_manager::is_ready_io()
  {
    bool status=true;
    switch(_status){
    case READY_IO:
    case OPERATING:
      break;
    case INIT:
    case OPENED:
    case CLOSED:
      status=false;
    }
    return status;
  }
  
  bool storage_manager::open()
  {
    bool status=true;
    
    if(_verbosity[MSG::DEBUG])
      Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"called ...");
    
    if((_mode==READ || _mode==BOTH) && _in_fnames.size()==0){
      Message::send(MSG::ERROR,
		    __FUNCTION__,
		    "Open attempt w/o specifing a file name!");
      return false;
    }
    
    if((_mode==WRITE || _mode==BOTH) && _out_fname.size()==0){
      Message::send(MSG::ERROR,
		    __FUNCTION__,
		    "Open attempt w/o specifing a file name!");
      return false;
    }
    
    switch(_mode){
      
    case BOTH:
    case READ:
      
      for(std::vector<std::string>::const_iterator iter(_in_fnames.begin());
	  iter!=_in_fnames.end();
	  ++iter) {
	
	TFile *fin = TFile::Open((*iter).c_str(),"READ");
	if(!fin) {
	  sprintf(_buf,"Open attempt failed for a file: %s", (*iter).c_str());
	  Message::send(MSG::ERROR,__FUNCTION__,_buf);
	  status=false;
	}else{
	  sprintf(_buf,"Opening a file in READ mode: %s", (*iter).c_str());
	  Message::send(MSG::NORMAL,__FUNCTION__,_buf);
	  fin->Close();
	}
	
      }
      
      if(_mode==READ) break;
    case WRITE:
      sprintf(_buf,"Opening a file in WRITE mode: %s",_out_fname.c_str());
      Message::send(MSG::NORMAL,__FUNCTION__,_buf);
      _fout=TFile::Open(_out_fname.c_str(),"RECREATE");
      if(!_fout) {
	sprintf(_buf,"Open attempt failed for a file: %s", _out_fname.c_str());
	Message::send(MSG::ERROR,__FUNCTION__,_buf);
	status=false;
      }else if(!_name_out_tdirectory.empty()) {
	
	_fout->mkdir(_name_out_tdirectory.c_str());
	_fout->cd(_name_out_tdirectory.c_str());

      }	
      break;
      
    case UNDEFINED:
      Message::send(MSG::ERROR,
		    __FUNCTION__,
		    "Open attempt w/o specifing I/O mode!");
      status=false;
      break;
    }
    
    if(!status) return status;
    
    if(_verbosity[MSG::DEBUG])
      Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"ends ...");
    
    _status=OPENED;
    return prepare_tree();
  };
  
  bool storage_manager::prepare_tree(){
    
    bool status=true;
    
    std::vector<size_t> nevents_array(DATA::DATA_TYPE_MAX,0);
    
    if(_verbosity[MSG::DEBUG])
      Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"called ...");
    
    if(_status!=OPENED) {
      sprintf(_buf,"Unexpected function call @ _status=%d!",_status);
      Message::send(MSG::ERROR,__FUNCTION__,_buf);
      status=false;
    }
    
    for(size_t i=0; i<DATA::DATA_TYPE_MAX; ++i){
      
      if(!_status) break;
      
      if(_mode!=WRITE && _read_data_array[i]) {
	
	std::string tree_name(Form("%s_tree",DATA::DATA_TREE_NAME[(DATA::DATA_TYPE)i].c_str()));
	
	if(_name_in_tdirectory.size()>0)
	  _in_ch[i]=new TChain(Form("%s/%s",_name_in_tdirectory.c_str(),tree_name.c_str()),
			       Form("%s Tree",DATA::DATA_TREE_NAME[(DATA::DATA_TYPE)i].c_str()));
	else
	  _in_ch[i]=new TChain(tree_name.c_str(),
			       Form("%s Tree",DATA::DATA_TREE_NAME[(DATA::DATA_TYPE)i].c_str()));
	
	for(size_t j=0; j<_in_fnames.size(); ++j)
	  
	  _in_ch[i]->AddFile(_in_fnames[j].c_str());
	
	// Ignore ROOT error message due to not finding a TTree 
	gErrorIgnoreLevel = kBreak;
	nevents_array[i]=_in_ch[i]->GetEntries();
	gErrorIgnoreLevel = kWarning;
	
	if(nevents_array[i]) { 
	  
	  print(MSG::DEBUG,__FUNCTION__,
		Form("Found %zu events found in TTree %s ...",nevents_array[i],tree_name.c_str()));
	  
	  create_data_ptr((DATA::DATA_TYPE)i);
	  
	  _in_ch[i]->SetBranchAddress(Form("%s_branch",DATA::DATA_TREE_NAME[(DATA::DATA_TYPE)i].c_str()),&(_ptr_data_array[i]));
	  
	  if(!_nevents) _nevents = nevents_array[i];
	  
	  if(_mode==BOTH) _write_data_array[i]=true;
	  
	}else{
	  delete _in_ch[i];
	  _in_ch[i]=0;
	  if(_mode==BOTH) _write_data_array[i]=false;
	}
      }
      
      if(_mode!=READ && _write_data_array[i] ) {
	
	_fout->cd(_name_out_tdirectory.c_str());
	
	/*
	if(_name_out_tdirectory.empty())
	  _out_ch[i]=new TTree(Form("%s_tree",DATA::DATA_TREE_NAME[(DATA::DATA_TYPE)i].c_str()),
			       Form("%s Tree",DATA::DATA_TREE_NAME[(DATA::DATA_TYPE)i].c_str()));
	else
	  _out_ch[i]=new TTree(Form("%s/%s_tree",_name_out_tdirectory.c_str(),DATA::DATA_TREE_NAME[(DATA::DATA_TYPE)i].c_str()),
			       Form("%s Tree",DATA::DATA_TREE_NAME[(DATA::DATA_TYPE)i].c_str()));
	*/

	_out_ch[i]=new TTree(Form("%s_tree",DATA::DATA_TREE_NAME[(DATA::DATA_TYPE)i].c_str()),
			     Form("%s Tree",DATA::DATA_TREE_NAME[(DATA::DATA_TYPE)i].c_str()));

	_out_ch[i]->SetMaxTreeSize    (1024*1024*1024);
	_out_ch[i]->SetMaxVirtualSize (1024*1024*1024);
	
	create_data_ptr((DATA::DATA_TYPE)i);
	
	_out_ch[i]->Branch(Form("%s_branch",DATA::DATA_TREE_NAME[(DATA::DATA_TYPE)i].c_str()),
			   _ptr_data_array[i]->GetName(),
			   &(_ptr_data_array[i]));
	
      }
      
      _nevents_written=0;
      _nevents_read=0;
      _index=0;
      
    }
    
    if( _mode!=WRITE && _nevents==0) {
      Message::send(MSG::ERROR,__FUNCTION__,"Did not find any relevant data tree!");
      status=false;
    }

    size_t min_nevents=_nevents;
    
    for(size_t i=0; i<DATA::DATA_TYPE_MAX; ++i) {
      
      if(nevents_array[i] && _nevents!=nevents_array[i]) {

	Message::send(MSG::WARNING,__FUNCTION__,
		      Form("Different number of entries found on tree: %s (found %ud while previous found %zu)",
			   DATA::DATA_TREE_NAME[(DATA::DATA_TYPE)i].c_str(),_nevents, nevents_array[i]));
	if( nevents_array[i] < min_nevents)
	  
	  min_nevents = nevents_array[i];
      }
      
    }

    if(_nevents!=min_nevents) {

      Message::send(MSG::WARNING,__FUNCTION__,
		    Form("Running the event loop with the smallest TTree's entries (=%zu)",min_nevents));

      _nevents = min_nevents;

    }
    
    if(status) _status=READY_IO;
    
    else close();
    
    if(_verbosity[MSG::DEBUG])
      Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"ends ...");

    return status;
  }
  
  void storage_manager::create_data_ptr(DATA::DATA_TYPE type){
    
    if(_ptr_data_array[type]) return;
    
    switch(type){
    case DATA::Event:
      _ptr_data_array[type]=(event_base*)(new event_base(type));
      break;
    case DATA::SimChannel:
      _ptr_data_array[type]=(event_base*)(new event_simch(type));
      break;
    case DATA::MCShower:
      _ptr_data_array[type]=(event_base*)(new event_mcshower(type));
      break;
    case DATA::Track:
    case DATA::Bezier:
    case DATA::Kalman3DHit:
    case DATA::Kalman3DSPS:
      _ptr_data_array[type]=(event_base*)(new event_track(type));
      break;
    case DATA::MCTruth:
      _ptr_data_array[type]=(event_base*)(new event_mctruth(type));
      break;
    case DATA::MCParticle:
      _ptr_data_array[type]=(event_base*)(new event_mcpart(type));
      break;
    case DATA::SpacePoint:
      _ptr_data_array[type]=(event_base*)(new event_sps(type));
      break;
    case DATA::UserInfo:
      _ptr_data_array[type]=(event_base*)(new event_user(type));
      break;
    case DATA::FIFO:
      _ptr_data_array[type]=(event_base*)(new event_fifo(type));
      break;
    case DATA::PMTFIFO:
      _ptr_data_array[type]=(event_base*)(new event_pmtfifo(type));
      break;
    case DATA::TPCFIFO:
      _ptr_data_array[type]=(event_base*)(new event_tpcfifo(type));
      break;
    case DATA::Pulse:
    case DATA::PMTPulse_ThresWin:
    case DATA::TPCPulse_ThresWin:
    case DATA::PMTPulse_FixedWin:
    case DATA::TPCPulse_FixedWin:
      _ptr_data_array[type]=(event_base*)(new event_pulse(type));
      break;
    case DATA::Trigger:
      _ptr_data_array[type]=(event_base*)(new trigger(type));
      break;
    case DATA::Wire:
      _ptr_data_array[type]=(event_base*)(new event_wire(type));
      break;
    case DATA::Hit:
    case DATA::MCShowerHit:
    case DATA::CrawlerHit:
    case DATA::GausHit:
    case DATA::APAHit:
    case DATA::FFTHit:
    case DATA::RFFHit:
      _ptr_data_array[type]=(event_base*)(new event_hit(type));
      break;
    case DATA::Cluster:
    case DATA::MCShowerCluster:
    case DATA::CrawlerCluster:
    case DATA::DBCluster:
    case DATA::FuzzyCluster:
    case DATA::HoughCluster:
    case DATA::ShowerAngleCluster:
    case DATA::RyanCluster:
      _ptr_data_array[type]=(event_base*)(new event_cluster(type));
      break;
    case DATA::Shower:
    case DATA::RyanShower:
      _ptr_data_array[type]=(event_base*)(new event_shower(type));
      break;
    case DATA::Vertex:
    case DATA::FeatureVertex:
    case DATA::HarrisVertex:
      _ptr_data_array[type]=(event_base*)(new event_vertex(type));
      break;
    case DATA::EndPoint2D:
    case DATA::FeatureEndPoint2D:
    case DATA::HarrisEndPoint2D:
      _ptr_data_array[type]=(event_base*)(new event_endpoint2d(type));
      break;
    case DATA::Calorimetry:
      _ptr_data_array[type]=(event_base*)(new event_calorimetry(type));
      break;
    case DATA::MCNeutrino:
      print(MSG::ERROR,__FUNCTION__,Form("MCNeutrino is stored within MCTruth! Retrieve MCTruth instead."));
      break;
    case DATA::MCTrajectory:
      print(MSG::ERROR,__FUNCTION__,Form("MCTrajectory is stored within MCParticle! Retrieve MCParticle instead."));
      break;
    case DATA::Seed:
    case DATA::CosmicTag:
      _ptr_data_array[type]=(event_base*)(new event_cosmictag());
      break;
    case DATA::OpFlash:
      _ptr_data_array[type]=(event_base*)(new event_opflash());
      break;
    case DATA::OpHit:
      _ptr_data_array[type]=(event_base*)(new event_ophit());
      break;
    case DATA::DATA_TYPE_MAX:
      print(MSG::ERROR,__FUNCTION__,Form("Data identifier not supported: %d",(int)type));
      break;
    }
    
    return;
  }
  
  void storage_manager::delete_data_ptr(DATA::DATA_TYPE type){
    
    if(!_ptr_data_array[type]) return;
    
    delete _ptr_data_array[type];
    
    _ptr_data_array[type]=0;
    
    return;
  }
  
  bool storage_manager::close(){
    
    if(_verbosity[MSG::DEBUG])
      Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"called ...");
    
    bool status=true;
    switch(_status){
    case INIT:
    case CLOSED:
      Message::send(MSG::ERROR,__FUNCTION__,"Attempt to close file while not operating I/O!");
      status=false;
      break;
    case OPENED:
    case READY_IO:
      Message::send(MSG::WARNING,__FUNCTION__,"Closing a file without any I/O operation done!");
      break;
    case OPERATING:
      if(_mode!=READ){
	
	//_fout->cd();
	
	for(size_t i=0; i<DATA::DATA_TYPE_MAX; i++) {
	  
	  if(!_out_ch[i]) {
	    
	    if(_verbosity[MSG::DEBUG])
	      
	      Message::send(MSG::DEBUG,__FUNCTION__,
			    Form("Skipping to write a Tree %s_tree...", 
				 DATA::DATA_TREE_NAME[(DATA::DATA_TYPE)i].c_str()));
	    
	    continue;
	  }
	  
	  if(_verbosity[MSG::INFO])
	    
	    Message::send(MSG::INFO,__FUNCTION__,Form("Writing TTree: %s",_out_ch[i]->GetName()));
	  
	  _fout = _out_ch[i]->GetCurrentFile();
	  _fout->cd(_name_out_tdirectory.c_str());
	  _out_ch[i]->Write();
	  
	  Message::send(MSG::NORMAL,__FUNCTION__,
			Form("TTree \"%s\" written with %lld events...",
			     _out_ch[i]->GetName(),
			     _out_ch[i]->GetEntries()));
	}
      }
      break;
    }
    
    if(status) {
      
      if(_fout) {

	Message::send(MSG::NORMAL,__FUNCTION__,Form("Closing the output: %s",_out_fname.c_str()));
	_fout->Close();

      }
      _fout=0;
      
      for(size_t i=0; i<DATA::DATA_TYPE_MAX; ++i) {
	
	if(_in_ch[i])  { delete _in_ch[i];  _in_ch[i]=0;  }
	if(_out_ch[i]) { _out_ch[i]=0; }
	if(_ptr_data_array[i]) delete_data_ptr((DATA::DATA_TYPE)i);
	
      }
      
    }
    
    if(_verbosity[MSG::DEBUG])
      Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"ends ...");
    
    _status=CLOSED;
    return status;
  }
  
  bool storage_manager::go_to(uint32_t index) {
    
    bool status=true;
    if(_mode==WRITE){
      Message::send(MSG::ERROR,__FUNCTION__,
		    "Cannot move the data pointer back/forth in WRITE mode.");
      status=false;
    }else if(!_nevents) {
      Message::send(MSG::WARNING,__FUNCTION__,"Input file empty!");
      status=false;
    }else if(!(index<_nevents)){
      Message::send(MSG::WARNING,__FUNCTION__,"Reached EOF");
      status=false;
    }else
      _index=index;
    
    if(status) status=next_event();
    
    return status;
  }
  
  bool storage_manager::next_event(){
    
    bool status=true;
    
    switch(_status){
    case READY_IO:
      // first event
      _status=OPERATING;
    case OPERATING:
      switch(_mode) {
      case READ:
	status=read_event();
	break;
      case WRITE:
	status=write_event();
	break;
      case BOTH:
	if(_nevents_read)
	  status = write_event();
	if(status) status = read_event();
	break;
      case UNDEFINED:
	break;
      }
      break;
    case INIT:
    case OPENED:
    case CLOSED:
      Message::send(MSG::ERROR,__FUNCTION__,"Cannot perform I/O on a closed file!");
      status=false;
      break;
    }

    return status;
  }
  
  bool storage_manager::read_event(){
    
    if(_index>=_nevents)

      return false;

    _current_event_id = DATA::INVALID_INT;

    // If this is BOTH mode, then read all relevant data products & check alignment here
    if( _mode == BOTH ) {

      for(size_t i=0; i<DATA::DATA_TYPE_MAX; ++i) { 
      
	if(_in_ch[i]) {

	  _in_ch[i]->GetEntry(_index);

	  if(_read_data_array[i] &&  _check_alignment) {

	    if(_current_event_id == DATA::INVALID_INT) 

	      _current_event_id = _ptr_data_array[i]->event_id();

	    else if(_current_event_id != _ptr_data_array[i]->event_id()) {

	      print(MSG::ERROR,__FUNCTION__,
		    Form("Detected event-alignment mismatch! (%d != %d)",
			 _current_event_id,
			 _ptr_data_array[i]->event_id()));
	      
	      return false;
	      
	    }
	  } // end check event alignment
	} // end read-in data @ _index
      } 
    }else if(_mode == READ) {
      
      // if DATA::Event is present, use that as absolute check
      if( _in_ch[(size_t)(DATA::Event)] ) {
	
	_in_ch[(size_t)(DATA::Event)]->GetEntry(_index);

	_current_event_id = _ptr_data_array[DATA::Event]->event_id();
      }

    }
    _index++;
    _nevents_read++;
    return true;
  }
  
  bool storage_manager::write_event(){
    
    for(size_t i=0; i<DATA::DATA_TYPE_MAX; ++i) {
      
      if(!_out_ch[i]) continue;
      
      _out_ch[i]->Fill();
      _ptr_data_array[i]->clear_data();
      
    }
    
    if(_mode==WRITE)
      _index++;
    _nevents_written++;
    //_event_wf->clear_data();
    return true;
  }
}
#endif
  
