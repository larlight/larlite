#ifndef LARLITE_STORAGE_MANAGER_CXX
#define LARLITE_STORAGE_MANAGER_CXX
#include "storage_manager.h"
//#include "storage_manager.template.cc"

namespace larlite {

  storage_manager* storage_manager::me=0;
  
  storage_manager::storage_manager(storage_manager::IOMode_t mode)
    : larlite_base()
    , _ptr_data_array (data::kDATA_TYPE_MAX,std::map<std::string,larlite::event_base*>())
    , _in_ch  (data::kDATA_TYPE_MAX,std::map<std::string,TChain*>())
    , _out_ch (data::kDATA_TYPE_MAX,std::map<std::string,TTree*>() )
  {
    _name="storage_manager";
    //_event_wf=0;
    _fout=0;
    _out_fname="";
    _name_in_tdirectory="";
    _name_out_tdirectory="";
    _status=kINIT;
    _check_alignment=true;

    reset();
    _mode=mode;
    
  };

  event_base* storage_manager::get_data(data::DataType_t const type, std::string const name)
  {
    // Read entry _index-1
    if(!_index && _mode != kWRITE) {

      Message::send(msg::kERROR,__FUNCTION__,
		    Form("Call next_event() before calling %s", __FUNCTION__));

      return 0;
    }
    event_base* result_ptr = nullptr;
    auto inch_ptr_iter = _in_ch[type].find(name);
    auto data_ptr_iter = _ptr_data_array[type].find(name);
    if(data_ptr_iter != _ptr_data_array[type].end())
      result_ptr = (event_base*)((*data_ptr_iter).second);

    // If in kREAD mode, here is where we read in data product
    if( inch_ptr_iter != _in_ch[type].end() && _mode == kREAD) {

      (*inch_ptr_iter).second->GetEntry(_index-1);

      if( _check_alignment ) {

	if(_current_event_id==data::kINVALID_INT) 

	  _current_event_id = (*data_ptr_iter).second->event_id();

	else if( (int)((*data_ptr_iter).second->event_id()) != _current_event_id) {
	  
	  print(msg::kERROR,__FUNCTION__,
		Form("Detected event-alignment mismatch! (%d != %d)",
		     (*data_ptr_iter).second->event_id(), _current_event_id)
		);
	  
	  return 0;
	  
	}
      }
    }
    
    // If data class object does not exist, and if it's either kWRITE or kBOTH mode, create it.
    if( (data_ptr_iter == _ptr_data_array[type].end() || !((*data_ptr_iter).second)) && 
       _mode != kREAD) {

      create_data_ptr(type,name);

      data_ptr_iter = _ptr_data_array[type].find(name);

      if(data_ptr_iter!=_ptr_data_array[type].end() && (*data_ptr_iter).second) {

	result_ptr = (*data_ptr_iter).second;

	if(!_name_out_tdirectory.empty())
	  _fout->cd(_name_out_tdirectory.c_str());

	auto out_ch_iter = _out_ch[type].find(name);
	if(out_ch_iter == _out_ch[type].end()) {
	  _out_ch[type][name]=nullptr;
	  out_ch_iter = _out_ch[type].find(name);
	}

	if(!(*out_ch_iter).second) {

	  _fout->cd(_name_out_tdirectory.c_str());

	  (*out_ch_iter).second=new TTree(Form("%s_%s_tree",
					       data::kDATA_TREE_NAME[(data::DataType_t)type].c_str(),
					       name.c_str()),
					  Form("%s Tree by %s",
					       data::kDATA_TREE_NAME[(data::DataType_t)type].c_str(),
					       name.c_str())
					  );
	  
	  (*out_ch_iter).second->SetMaxTreeSize    (1024*1024*1024);
	  (*out_ch_iter).second->SetMaxVirtualSize (1024*1024*1024);
	  
	  (*out_ch_iter).second->Branch(Form("%s_%s_branch",
					     data::kDATA_TREE_NAME[(data::DataType_t)type].c_str(),
					     name.c_str()),
					result_ptr->GetName(),
					&(_ptr_data_array[type][name]));
	  
	  Message::send(msg::kWARNING,__FUNCTION__,
			Form("Requested tree %s by %s which does not exists yet. Created a new one.", 
			     result_ptr->GetName(),
			     name.c_str())
			);
	}
      }
    }
    
    return result_ptr;    
  }
  
  std::map<larlite::data::DataType_t,std::set<std::string> > storage_manager::list_data_types() const {

    std::map<larlite::data::DataType_t,std::set<std::string> > result;
    for(int i=0;i<larlite::data::kDATA_TYPE_MAX; i++) {

      for(auto const& name_ptr : _ptr_data_array[i]) {

	if(result.find((data::DataType_t)i)==result.end()) 

	  result.insert(std::make_pair((data::DataType_t)i,std::set<std::string>()));

	result[(data::DataType_t)i].insert(name_ptr.first);

      }

    }
    return result;
  }
  
  void storage_manager::reset()
  {
    if(_verbosity[msg::kDEBUG])
      Message::send(msg::kDEBUG,__PRETTY_FUNCTION__,"called ...");
    
    switch(_status){
    case kREADY_IO:
    case kOPENED:
    case kOPERATING:
      close();
      break;
    case kINIT:
    case kCLOSED:
      break;
    }
    
    _current_event_id = data::kINVALID_INT;
    _index=0;
    _nevents=0;
    _nevents_written=0;
    _nevents_read=0;
    _mode=kUNDEFINED;
    _status=kINIT;
    _in_fnames.clear();
    
    for(size_t i=0; i<data::kDATA_TYPE_MAX; ++i) {
      //_read_data_array[i]=true;   // Attempt to read in all data by default
      //_write_data_array[i]=false; // Attemp to write no data by default 
      _in_ch[i].clear();
      _out_ch[i].clear();
      _ptr_data_array[i].clear();
    }
    
    if(_verbosity[msg::kDEBUG])
      Message::send(msg::kDEBUG,__PRETTY_FUNCTION__,"ends ...");  
  };
  
  bool storage_manager::is_open()
  {
    bool status=true;
    switch(_status){
    case kOPENED:
    case kREADY_IO:
    case kOPERATING:
      break;
    case kINIT:
    case kCLOSED:
      status=false;
    }
    
    return status;
  }
  
  bool storage_manager::is_ready_io()
  {
    bool status=true;
    switch(_status){
    case kREADY_IO:
    case kOPERATING:
      break;
    case kINIT:
    case kOPENED:
    case kCLOSED:
      status=false;
    }
    return status;
  }
  
  bool storage_manager::open()
  {
    bool status=true;
    
    if(_verbosity[msg::kDEBUG])
      Message::send(msg::kDEBUG,__PRETTY_FUNCTION__,"called ...");
    
    if((_mode==kREAD || _mode==kBOTH) && _in_fnames.size()==0){
      Message::send(msg::kERROR,
		    __FUNCTION__,
		    "Open attempt w/o specifing a file name!");
      return false;
    }
    
    if((_mode==kWRITE || _mode==kBOTH) && _out_fname.size()==0){
      Message::send(msg::kERROR,
		    __FUNCTION__,
		    "Open attempt w/o specifing a file name!");
      return false;
    }

    switch(_mode){
      
    case kBOTH:
    case kREAD:
      
      for(std::vector<std::string>::const_iterator iter(_in_fnames.begin());
	  iter!=_in_fnames.end();
	  ++iter) {
	
	TFile *fin = TFile::Open((*iter).c_str(),"READ");
	if(!fin) {
	  sprintf(_buf,"Open attempt failed for a file: %s", (*iter).c_str());
	  Message::send(msg::kERROR,__FUNCTION__,_buf);
	  status=false;
	}else{
	  sprintf(_buf,"Opening a file in READ mode: %s", (*iter).c_str());
	  Message::send(msg::kNORMAL,__FUNCTION__,_buf);
	  TDirectoryFile* fin_dir = 0;
	  if(_name_in_tdirectory.empty()) fin_dir = fin;
	  else {
	    TObject* obj = fin->Get(_name_in_tdirectory.c_str());
	    
	    if(obj && std::string(obj->ClassName())!="TDirectoryFile")

	      fin_dir = (TDirectoryFile*)obj;
	    
	  }
	  if(fin_dir) {
	    TList* key_list = fin_dir->GetListOfKeys();
	    TIter key_iter(key_list);
	    TObject* obj = nullptr;
	    while(1) {
	      obj = key_iter.Next();			    
	      if(!obj) break;
	      obj = fin_dir->Get(obj->GetName());
	      Message::send(msg::kINFO,__FUNCTION__,Form("Found object %s (type=%s)",obj->GetName(),obj->ClassName()));
	      if(std::string(obj->ClassName())!="TTree") {
		Message::send(msg::kINFO,__FUNCTION__,Form("Skipping %s ... (not TTree)",obj->GetName()));
		continue;
	      }
	      std::string obj_name = obj->GetName();
	      char c[2] = "_";
	      if(obj_name.find_first_of(c) > obj_name.size() ||
		 obj_name.find_first_of(c) == obj_name.find_last_of(c)) {
		Message::send(msg::kINFO,__FUNCTION__,Form("Skipping %s ... (not LArLite TTree)",obj->GetName()));
		continue;
	      }
	      std::string type_name( obj_name.substr(0,obj_name.find_first_of(c)) );
	      std::string suffix( obj_name.substr(obj_name.find_last_of(c)+1, obj_name.size()-obj_name.find_last_of(c)) );
	      std::string producer_name( obj_name.substr(obj_name.find_first_of(c)+1,obj_name.find_last_of(c)-obj_name.find_first_of(c)-1) );
	      if(suffix != "tree") {
		Message::send(msg::kINFO,__FUNCTION__,Form("Skipping %s ... (not LArLite TTree)",obj->GetName()));
		continue;
	      }
	      for(size_t i=0; i<data::kDATA_TYPE_MAX; ++i)
		if(data::kDATA_TREE_NAME[i] == type_name) {
		  _in_ch[(data::DataType_t)i].insert(std::make_pair(producer_name.c_str(),nullptr));
		  Message::send(msg::kINFO,__FUNCTION__,Form("Found %s tree made by %s...",type_name.c_str(),producer_name.c_str()));
		  break;
		}
	    }
	  }
	  fin->Close();
	}
      }
      
      if(_mode==kREAD) break;

    case kWRITE:
      sprintf(_buf,"Opening a file in kWRITE mode: %s",_out_fname.c_str());
      Message::send(msg::kNORMAL,__FUNCTION__,_buf);
      _fout=TFile::Open(_out_fname.c_str(),"RECREATE");
      if(!_fout) {
	sprintf(_buf,"Open attempt failed for a file: %s", _out_fname.c_str());
	Message::send(msg::kERROR,__FUNCTION__,_buf);
	status=false;
      }else if(!_name_out_tdirectory.empty()) {
	
	_fout->mkdir(_name_out_tdirectory.c_str());
	_fout->cd(_name_out_tdirectory.c_str());

      }	
      break;
      
    case kUNDEFINED:
      Message::send(msg::kERROR,
		    __FUNCTION__,
		    "Open attempt w/o specifing I/O mode!");
      status=false;
      break;
    }
    
    if(!status) return status;
    
    if(_verbosity[msg::kDEBUG])
      Message::send(msg::kDEBUG,__PRETTY_FUNCTION__,"ends ...");
    
    _status=kOPENED;
    return prepare_tree();
  };
  
  bool storage_manager::prepare_tree() {
    
    bool status=true;
    
    std::vector<std::map<std::string, size_t> > nevents_array(data::kDATA_TYPE_MAX,
							      std::map<std::string,size_t>());
    for(size_t i=0; i<nevents_array.size(); ++i) {

      for(auto const& name_ptr : _in_ch[i])

	nevents_array[i].insert(std::make_pair(name_ptr.first,0));
    }
    
    if(_verbosity[msg::kDEBUG])
      Message::send(msg::kDEBUG,__PRETTY_FUNCTION__,"called ...");
    
    if(_status!=kOPENED) {
      sprintf(_buf,"Unexpected function call @ _status=%d!",_status);
      Message::send(msg::kERROR,__FUNCTION__,_buf);
      status=false;
    }
    
    for(size_t i=0; i<data::kDATA_TYPE_MAX; ++i){
      
      if(!_status) break;
      
      //if(_mode!=kWRITE && _read_data_array[i]) {
      if(_mode!=kWRITE && _in_ch[i].size()) {

	for(auto& name_ptr : _in_ch[i]) {

	  std::string tree_name(Form("%s_%s_tree",
				     data::kDATA_TREE_NAME[(data::DataType_t)i].c_str(),
				     name_ptr.first.c_str())
				);
	
	  if(_name_in_tdirectory.size()>0)
	    name_ptr.second=new TChain(Form("%s/%s",_name_in_tdirectory.c_str(),tree_name.c_str()),
				       Form("%s Tree by %s",
					    data::kDATA_TREE_NAME[(data::DataType_t)i].c_str(),
					    name_ptr.first.c_str())
				       );
	  else
	    name_ptr.second=new TChain(tree_name.c_str(),
				       Form("%s Tree by %s",
					    data::kDATA_TREE_NAME[(data::DataType_t)i].c_str(),
					    name_ptr.first.c_str())
				       );
	
	  for(size_t j=0; j<_in_fnames.size(); ++j)
	    
	    name_ptr.second->AddFile(_in_fnames[j].c_str());

	  // Ignore ROOT error message due to not finding a TTree 
	  gErrorIgnoreLevel = kBreak;
	  nevents_array[i][name_ptr.first] += name_ptr.second->GetEntries();
	  gErrorIgnoreLevel = kWarning;
	  
	  if(nevents_array[i][name_ptr.first]) { 
	  
	    print(msg::kDEBUG,__FUNCTION__,
		  Form("Found %zu events found in TTree %s ...",nevents_array[i][name_ptr.first],tree_name.c_str()));

	    create_data_ptr((data::DataType_t)i,name_ptr.first);
	    name_ptr.second->SetBranchAddress(Form("%s_%s_branch",
						   data::kDATA_TREE_NAME[(data::DataType_t)i].c_str(),
						   name_ptr.first.c_str()
						   ),
					      &(_ptr_data_array[i][name_ptr.first])
					      );
	    
	  if(!_nevents) _nevents = nevents_array[i][name_ptr.first];
	  
	  }else{
	    print(msg::kWARNING,__FUNCTION__,
		  Form("TTree %s contains no event... removing read pointer...",tree_name.c_str()));
	    delete name_ptr.second;
	    name_ptr.second=nullptr;
	  }
	}
      }

      // In BOTH mode, write all that is read in
      if(_mode==kBOTH) {
	
	_fout->cd(_name_out_tdirectory.c_str());

	for(size_t i=0; i<data::kDATA_TYPE_MAX; ++i) {

	  for(auto& name_ptr : _in_ch[i]) {

	    if(!_ptr_data_array[i][name_ptr.first]) continue;

	    _fout->cd(_name_out_tdirectory.c_str());

	    _out_ch[i][name_ptr.first]=new TTree(Form("%s_%s_tree",
						      data::kDATA_TREE_NAME[(data::DataType_t)i].c_str(),
						      name_ptr.first.c_str()),
						 Form("%s Tree by %s",
						      data::kDATA_TREE_NAME[(data::DataType_t)i].c_str(),
						      name_ptr.first.c_str())
						 );

	    _out_ch[i][name_ptr.first]->SetMaxTreeSize    (1024*1024*1024);
	    _out_ch[i][name_ptr.first]->SetMaxVirtualSize (1024*1024*1024);

	    _out_ch[i][name_ptr.first]->Branch(Form("%s_%s_branch",
						    data::kDATA_TREE_NAME[(data::DataType_t)i].c_str(),
						    name_ptr.first.c_str()),
					       _ptr_data_array[i][name_ptr.first]->GetName(),
					       &(_ptr_data_array[i][name_ptr.first]));
	  }
	}
      }
      
      _nevents_written=0;
      _nevents_read=0;
      _index=0;
      
    }
    
    if( _mode!=kWRITE && _nevents==0) {
      Message::send(msg::kERROR,__FUNCTION__,"Did not find any relevant data tree!");
      status=false;
    }
  
    size_t min_nevents=_nevents;
    
    for(size_t i=0; i<data::kDATA_TYPE_MAX; ++i) {

      for(auto const& name_ctr : nevents_array[i]) {

	if(name_ctr.second && _nevents!=name_ctr.second) {

	  Message::send(msg::kWARNING,__FUNCTION__,
			Form("Different number of entries found on tree: %s by %s (found %zu while previous found %ud)",
			     data::kDATA_TREE_NAME[(data::DataType_t)i].c_str(),
			     name_ctr.first.c_str(),
			     name_ctr.second,
			     _nevents)
			);

	  if( name_ctr.second < min_nevents)
	  
	    min_nevents = name_ctr.second;
	}
      }
    }

    if(_nevents!=min_nevents) {

      Message::send(msg::kWARNING,__FUNCTION__,
		    Form("Running the event loop with the smallest TTree's entries (=%zu)",min_nevents));

      _nevents = min_nevents;

    }
    
    if(status) _status=kREADY_IO;
    
    else close();
    
    if(_verbosity[msg::kDEBUG])
      Message::send(msg::kDEBUG,__PRETTY_FUNCTION__,"ends ...");

    return status;
  }
  
  void storage_manager::create_data_ptr(data::DataType_t const type, std::string const& name){

    if(get_verbosity() <= msg::kINFO)

      Message::send(msg::kINFO,__FUNCTION__,
		    Form("Requested a new data pointer of type %s (=%d) by %s",
			 data::kDATA_TREE_NAME[type].c_str(),
			 type,
			 name.c_str())
		    );

    auto name_ptr = _ptr_data_array[type].find(name);
    if(name_ptr == _ptr_data_array[type].end()) {
      _ptr_data_array[type][name]=nullptr;
      name_ptr = _ptr_data_array[type].find(name);
    }
    if((*name_ptr).second) return;
    
    switch(type){
    case data::kGTruth:
      _ptr_data_array[type][name]=(event_base*)(new event_gtruth(name));
      break;
    case data::kEvent:
      _ptr_data_array[type][name]=(event_base*)(new event_base(type,name));
      break;
    case data::kRawDigit:
      _ptr_data_array[type][name]=(event_base*)(new event_rawdigit(name));
      break;
    case data::kSimChannel:
      _ptr_data_array[type][name]=(event_base*)(new event_simch(name));
      break;
    case data::kMCShower:
      _ptr_data_array[type][name]=(event_base*)(new event_mcshower(name));
      break;
    case data::kTrack:
      _ptr_data_array[type][name]=(event_base*)(new event_track(name));
      break;
    case data::kMCTruth:
      _ptr_data_array[type][name]=(event_base*)(new event_mctruth(name));
      break;
    case data::kMCParticle:
      _ptr_data_array[type][name]=(event_base*)(new event_mcpart(name));
      break;
    case data::kMCFlux:
      _ptr_data_array[type][name]=(event_base*)(new event_mcflux(name));
      break;
    case data::kSpacePoint:
      _ptr_data_array[type][name]=(event_base*)(new event_spacepoint(name));
      break;
    case data::kUserInfo:
      _ptr_data_array[type][name]=(event_base*)(new event_user(name));
      break;
    case data::kWire:
      _ptr_data_array[type][name]=(event_base*)(new event_wire(name));
      break;
    case data::kHit:
      _ptr_data_array[type][name]=(event_base*)(new event_hit(name));
      break;
    case data::kCluster:
      _ptr_data_array[type][name]=(event_base*)(new event_cluster(name));
      break;
    case data::kShower:
      _ptr_data_array[type][name]=(event_base*)(new event_shower(name));
      break;
    case data::kVertex:
      _ptr_data_array[type][name]=(event_base*)(new event_vertex(name));
      break;
    case data::kEndPoint2D:
      _ptr_data_array[type][name]=(event_base*)(new event_endpoint2d(name));
      break;
    case data::kSeed:
      _ptr_data_array[type][name]=(event_base*)(new event_seed(name));
      break;
    case data::kCalorimetry:
      _ptr_data_array[type][name]=(event_base*)(new event_calorimetry(name));
      break;
    case data::kMCNeutrino:
      print(msg::kERROR,__FUNCTION__,Form("MCNeutrino is stored within MCTruth! Retrieve MCTruth instead."));
      break;
    case data::kMCTrajectory:
      print(msg::kERROR,__FUNCTION__,Form("MCTrajectory is stored within MCParticle! Retrieve MCParticle instead."));
      break;
    case data::kCosmicTag:
      _ptr_data_array[type][name]=(event_base*)(new event_cosmictag(name));
      break;
    case data::kOpFlash:
      _ptr_data_array[type][name]=(event_base*)(new event_opflash(name));
      break;
    case data::kOpHit:
      _ptr_data_array[type][name]=(event_base*)(new event_ophit(name));
      break;
    case data::kPFParticle:
      _ptr_data_array[type][name]=(event_base*)(new event_pfpart(name));
      break;
    case data::kParticleID:
      _ptr_data_array[type][name]=(event_base*)(new event_partid(name));
      break;
    default:
      print(msg::kERROR,__FUNCTION__,Form("Data identifier not supported: %d",(int)type));
      break;
    }
    
    return;
  }
  
  void storage_manager::delete_data_ptr(data::DataType_t const type,std::string const& name){
    
    auto name_ptr = _ptr_data_array[type].find(name);

    if(name_ptr == _ptr_data_array[type].end()) return;

    if(!((*name_ptr).second)) return;
    
    delete ((*name_ptr).second);

    (*name_ptr).second = nullptr;

    return;
  }
  
  bool storage_manager::close(){
    
    if(_verbosity[msg::kDEBUG])
      Message::send(msg::kDEBUG,__PRETTY_FUNCTION__,"called ...");
    
    bool status=true;
    switch(_status){
    case kINIT:
    case kCLOSED:
      Message::send(msg::kERROR,__FUNCTION__,"Attempt to close file while not operating I/O!");
      status=false;
      break;
    case kOPENED:
    case kREADY_IO:
      Message::send(msg::kWARNING,__FUNCTION__,"Closing a file without any I/O operation done!");
      break;
    case kOPERATING:
      if(_mode!=kREAD){
	
	//_fout->cd();
	
	for(size_t i=0; i<data::kDATA_TYPE_MAX; i++) {
	  
	  if(!_out_ch[i].size()) {
	    
	    if(_verbosity[msg::kDEBUG])
	      
	      Message::send(msg::kDEBUG,__FUNCTION__,
			    Form("Skipping to write a Tree %s_tree...", 
				 data::kDATA_TREE_NAME[(data::DataType_t)i].c_str()));
	    
	    continue;
	  }

	  for(auto& name_ptr : _out_ch[i]) {

	    if(_verbosity[msg::kINFO])
	    
	      Message::send(msg::kINFO,__FUNCTION__,Form("Writing TTree: %s",
							 name_ptr.second->GetName()));
	    
	    _fout = name_ptr.second->GetCurrentFile();
	    _fout->cd(_name_out_tdirectory.c_str());
	    name_ptr.second->Write();
	  
	    Message::send(msg::kNORMAL,__FUNCTION__,
			  Form("TTree \"%s\" written with %lld events...",
			       name_ptr.second->GetName(),
			       name_ptr.second->GetEntries())
			  );
	  }
	}
      }
      break;
    }
    
    if(status) {
      
      if(_fout) {

	Message::send(msg::kNORMAL,__FUNCTION__,Form("Closing the output: %s",_out_fname.c_str()));
	_fout->Close();

      }
      _fout=0;
      
      for(size_t i=0; i<data::kDATA_TYPE_MAX; ++i) {
	
	if(_in_ch[i].size())  { _in_ch[i].clear(); }
	if(_out_ch[i].size()) { _out_ch[i].clear(); }
	for(auto const& name_ptr : _ptr_data_array[i])
	  delete_data_ptr((data::DataType_t)i, name_ptr.first);
	_ptr_data_array[i].clear();
      }
      
    }
    
    if(_verbosity[msg::kDEBUG])
      Message::send(msg::kDEBUG,__PRETTY_FUNCTION__,"ends ...");
    
    _status=kCLOSED;
    return status;
  }
  
  bool storage_manager::go_to(uint32_t index) {
    
    bool status=true;
    if(_mode==kWRITE){
      Message::send(msg::kERROR,__FUNCTION__,
		    "Cannot move the data pointer back/forth in kWRITE mode.");
      status=false;
    }else if(!_nevents) {
      Message::send(msg::kWARNING,__FUNCTION__,"Input file empty!");
      status=false;
    }else if(!(index<_nevents)){
      Message::send(msg::kWARNING,__FUNCTION__,"Reached EOF");
      status=false;
    }else
      _index=index;
    
    if(status) status=next_event();
    
    return status;
  }
  
  bool storage_manager::next_event(){
    
    bool status=true;
    
    switch(_status){
    case kREADY_IO:
      // first event
      _status=kOPERATING;
    case kOPERATING:
      switch(_mode) {
      case kREAD:
	status=read_event();
	break;
      case kWRITE:
	status=write_event();
	break;
      case kBOTH:
	if(_nevents_read)
	  status = write_event();
	if(status) status = read_event();
	break;
      case kUNDEFINED:
	break;
      }
      break;
    case kINIT:
    case kOPENED:
    case kCLOSED:
      Message::send(msg::kERROR,__FUNCTION__,"Cannot perform I/O on a closed file!");
      status=false;
      break;
    }

    return status;
  }
  
  bool storage_manager::read_event(){
    
    if(_index>=_nevents)

      return false;

    _current_event_id = data::kINVALID_INT;

    // If this is kBOTH mode, then read all relevant data products & check alignment here
    if( _mode == kBOTH ) {

      for(size_t i=0; i<data::kDATA_TYPE_MAX; ++i) { 

	for(auto& name_ptr : _in_ch[i]) {
      
	  if((name_ptr.second)) {

	    (name_ptr.second)->GetEntry(_index);

	    if( _check_alignment ) {
	      
	      if(_current_event_id == data::kINVALID_INT) 
		
		_current_event_id = _ptr_data_array[i][name_ptr.first]->event_id();
	      
	      else if(_current_event_id != (int)(_ptr_data_array[i][name_ptr.first]->event_id())) {
		
		print(msg::kERROR,__FUNCTION__,
		      Form("Detected event-alignment mismatch! (%d != %d)",
			   _current_event_id,
			   _ptr_data_array[i][name_ptr.first]->event_id()));
		
		return false;
		
	      }
	    } // end check event alignment
	  } // ptr exists
	} // loop over all trees of type i
      } // loop over all types
    }else if(_mode == kREAD) {
      
      // if data::kEvent is present, use that as absolute check
      if( _in_ch[(size_t)(data::kEvent)].find("main") != _in_ch[(size_t)(data::kEvent)].end() &&
	  _in_ch[(size_t)(data::kEvent)]["main"] ) {
	
	_in_ch[(size_t)(data::kEvent)]["main"]->GetEntry(_index);
	
	_current_event_id = _ptr_data_array[data::kEvent]["main"]->event_id();
      }

    }
    _index++;
    _nevents_read++;
    return true;
  }
  
  bool storage_manager::write_event(){
    
    for(size_t i=0; i<data::kDATA_TYPE_MAX; ++i) {

      for(auto& name_ptr : _out_ch[i]) {
      
	if(!(name_ptr.second)) continue;

	name_ptr.second->Fill();
	_ptr_data_array[i][name_ptr.first]->clear_data();
	
      }
    }
    
    if(_mode==kWRITE)
      _index++;
    _nevents_written++;
    //_event_wf->clear_data();
    return true;
  }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_gtruth> () const
  { return data::kGTruth; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_mctruth> () const
  { return data::kMCTruth; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_mcpart> () const
  { return data::kMCParticle; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_mcflux> () const
  { return data::kMCFlux; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_simch> () const
  { return data::kSimChannel; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_mcshower> () const
  { return data::kMCShower; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_rawdigit> () const
  { return data::kRawDigit; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_wire> () const
  { return data::kWire; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_hit> () const
  { return data::kHit; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_ophit> () const
  { return data::kOpHit; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_opflash> () const
  { return data::kOpFlash; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_cluster> () const
  { return data::kCluster; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_seed> () const
  { return data::kSeed; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_spacepoint> () const
  { return data::kSpacePoint; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_track> () const
  { return data::kTrack; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_shower> () const
  { return data::kShower; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_vertex> () const
  { return data::kVertex; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_endpoint2d> () const
  { return data::kEndPoint2D; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_calorimetry> () const
  { return data::kCalorimetry; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_partid> () const
  { return data::kParticleID; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_pfpart> () const
  { return data::kPFParticle; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_user> () const
  { return data::kUserInfo; }

  template <class T>
  const ::larlite::data::DataType_t storage_manager::data_type() const
  { 
    Message::send(msg::kERROR,
		  __PRETTY_FUNCTION__,
		  "No corresponding data::DataType_t enum value found!");
    throw std::exception();
    return data::kUndefined;
  }

  //template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_trigger> ()
  //{ return data::kTrigger; }

  template <class T>
  T* storage_manager::get_data(std::string const name)
  {
    auto type = data_type<T>();
    return (T*)(get_data(type,name));
  }

}
#endif
  
