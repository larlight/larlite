#ifndef LARLITE_STORAGE_MANAGER_CXX
#define LARLITE_STORAGE_MANAGER_CXX
#include "storage_manager.h"
#include "storage_manager.template.cc"

#include "trigger.h"
#include "potsummary.h"
#include "hit.h"
#include "track.h"
#include "trackmom.h"
#include "mctruth.h"
#include "mctree.h"
#include "user_info.h"
#include "spacepoint.h"
#include "rawdigit.h"
#include "wire.h"
#include "hit.h"
#include "cluster.h"
#include "shower.h"
#include "mcshower.h"
#include "mctrack.h"
#include "simch.h"
#include "calorimetry.h"
#include "vertex.h"
#include "endpoint2d.h"
#include "seed.h"
#include "cosmictag.h"
#include "opflash.h"
#include "ophit.h"
#include "t0.h"
#include "mcflux.h"
#include "pfpart.h"
#include "partid.h"
#include "gtruth.h"
#include "minos.h"
#include "pcaxis.h"
#include "flashmatch.h"
#include "event_ass.h"
#include "fifo.h"
#include "opdetwaveform.h"
#include "simphotons.h"
#include "mucsdata.h"
#include "mucsreco.h"
#include "roi.h"
#include "auxsimch.h"
#include "chstatus.h"
#include "mceventweight.h"
#include "swtrigger.h"
#include "larflow3dhit.h"
#include "larflowcluster.h"
#include "pixelmask.h"
#include "crthit.h"
#include "crttrack.h"
#include "daqheadertimeuboone.h"

namespace larlite {

  storage_manager* storage_manager::me=0;
  
  storage_manager::storage_manager(storage_manager::IOMode_t mode)
    : larlite_base()
    , _ptr_data_array (data::kDATA_TYPE_MAX,std::map<std::string,larlite::event_base*>())
    , _ptr_rundata_array (data::kRUNDATA_TYPE_MAX,std::map<std::string,larlite::run_base*>())
    , _ptr_subrundata_array (data::kSUBRUNDATA_TYPE_MAX,std::map<std::string,larlite::subrun_base*>())
    , _in_id_ch  (nullptr)
    , _out_id_ch (nullptr)
    , _in_ch  (data::kDATA_TYPE_MAX,std::map<std::string,TChain*>())
    , _out_ch (data::kDATA_TYPE_MAX,std::map<std::string,TTree*>() )
    , _in_rundata_ch  (data::kRUNDATA_TYPE_MAX,std::map<std::string,TChain*>())
    , _out_rundata_ch (data::kRUNDATA_TYPE_MAX,std::map<std::string,TTree*>() )
    , _in_subrundata_ch  (data::kSUBRUNDATA_TYPE_MAX,std::map<std::string,TChain*>())
    , _out_subrundata_ch (data::kSUBRUNDATA_TYPE_MAX,std::map<std::string,TTree*>())
    , _read_data_array(data::kDATA_TYPE_MAX,std::map<std::string,bool>())
    , _read_rundata_array(data::kRUNDATA_TYPE_MAX,std::map<std::string,bool>())
    , _read_subrundata_array(data::kSUBRUNDATA_TYPE_MAX,std::map<std::string,bool>())
    , _write_data_array(data::kDATA_TYPE_MAX,std::map<std::string,bool>())
    , _write_rundata_array(data::kRUNDATA_TYPE_MAX,std::map<std::string,bool>())
    , _write_subrundata_array(data::kSUBRUNDATA_TYPE_MAX,std::map<std::string,bool>())

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

  run_base* storage_manager::get_rundata(const data::RunDataType_t type, 
					 const std::string& name)
  {
    if(!_index && _mode != kWRITE) {

      Message::send(msg::kERROR,__FUNCTION__,
		    Form("Call next_event() before calling %s", __FUNCTION__));

      return 0;
    }
    run_base* result_ptr = nullptr;    
    auto inch_ptr_iter = _in_rundata_ch[type].find(name);
    auto data_ptr_iter = _ptr_rundata_array[type].find(name);
    if(data_ptr_iter != _ptr_rundata_array[type].end())
      result_ptr = (*data_ptr_iter).second;

    // If in kREAD mode, here is where we read in data product
    if( inch_ptr_iter != _in_rundata_ch[type].end() && _mode == kREAD) {

      long current_entry = (*inch_ptr_iter).second->GetReadEntry();
      while( (*data_ptr_iter).second->run() != _run_id &&
	     current_entry < (*inch_ptr_iter).second->GetEntries() ) {
	++current_entry;
	(*inch_ptr_iter).second->GetEntry(current_entry);
      }
      
      if((*data_ptr_iter).second->run() != _run_id)
	throw DataFormatException(Form("Could not find run data (%s by %s) for run %d",
				       data::kRUNDATA_TREE_NAME[type].c_str(),
				       name.c_str(),
				       _run_id)
				  );

      result_ptr = (*data_ptr_iter).second;
    }

    // If data class object does not exist, and if it's either kWRITE or kBOTH mode, create it.
    if( (data_ptr_iter == _ptr_rundata_array[type].end() || !((*data_ptr_iter).second)) && 
       _mode != kREAD) {

      create_rundata_ptr(type,name);

      data_ptr_iter = _ptr_rundata_array[type].find(name);

      if(data_ptr_iter!=_ptr_rundata_array[type].end() && (*data_ptr_iter).second) {

	result_ptr = (*data_ptr_iter).second;

	result_ptr->set_run(_run_id);

	if(!_name_out_tdirectory.empty())
	  _fout->cd(_name_out_tdirectory.c_str());

	auto out_ch_iter = _out_rundata_ch[type].find(name);
	if(out_ch_iter == _out_rundata_ch[type].end()) {
	  _out_rundata_ch[type][name]=nullptr;
	  out_ch_iter = _out_rundata_ch[type].find(name);
	}

	if(!(*out_ch_iter).second) {

	  _fout->cd(_name_out_tdirectory.c_str());

	  (*out_ch_iter).second=new TTree(Form("%s_%s_tree",
					       data::kRUNDATA_TREE_NAME[(data::RunDataType_t)type].c_str(),
					       name.c_str()),
					  Form("%s Tree by %s",
					       data::kRUNDATA_TREE_NAME[(data::RunDataType_t)type].c_str(),
					       name.c_str())
					  );
	  
	  //(*out_ch_iter).second->SetMaxTreeSize    (1024*1024*1024);
	  //(*out_ch_iter).second->SetMaxVirtualSize (1024*1024*1024);
	  
	  (*out_ch_iter).second->Branch(Form("%s_%s_branch",
					     data::kRUNDATA_TREE_NAME[(data::RunDataType_t)type].c_str(),
					     name.c_str()),
					//result_ptr->GetName(),
					&(_ptr_rundata_array[type][name]));
	  
	  Message::send(msg::kWARNING,__FUNCTION__,
			Form("Requested tree %s by %s which does not exists yet. Created a new one.", 
			     data::kRUNDATA_TREE_NAME[type].c_str(),
			     //result_ptr->GetName(),
			     name.c_str())
			);
	}
      }
    }
    return result_ptr;
  }

  subrun_base* storage_manager::get_subrundata(const data::SubRunDataType_t type, 
					       const std::string& name)
  {
    if(!_index && _mode != kWRITE) {

      Message::send(msg::kERROR,__FUNCTION__,
		    Form("Call next_event() before calling %s", __FUNCTION__));

      return 0;
    }
    subrun_base* result_ptr = nullptr;    
    auto inch_ptr_iter = _in_subrundata_ch[type].find(name);
    auto data_ptr_iter = _ptr_subrundata_array[type].find(name);
    if(data_ptr_iter != _ptr_subrundata_array[type].end())
      result_ptr = (*data_ptr_iter).second;

    // If in kREAD mode, here is where we read in data product
    if( inch_ptr_iter != _in_subrundata_ch[type].end() && _mode == kREAD) {

      long current_entry = (*inch_ptr_iter).second->GetReadEntry();
      while( ((*data_ptr_iter).second->run() != _run_id ||
	      (*data_ptr_iter).second->subrun() != _subrun_id) &&
	     current_entry < (*inch_ptr_iter).second->GetEntries() ) {
	++current_entry;
	(*inch_ptr_iter).second->GetEntry(current_entry);
      }
      
      if( (*data_ptr_iter).second->run() != _run_id ||
	  (*data_ptr_iter).second->subrun() != _subrun_id )
	throw DataFormatException(Form("Could not find sub-run data (%s by %s) for run/subrun %d/%d",
				       data::kSUBRUNDATA_TREE_NAME[type].c_str(),
				       name.c_str(),
				       _run_id,
				       _subrun_id)
				  );

      result_ptr = (*data_ptr_iter).second;
    }

    // If data class object does not exist, and if it's either kWRITE or kBOTH mode, create it.
    if( (data_ptr_iter == _ptr_subrundata_array[type].end() || !((*data_ptr_iter).second)) && 
       _mode != kREAD) {

      create_subrundata_ptr(type,name);

      data_ptr_iter = _ptr_subrundata_array[type].find(name);

      if(data_ptr_iter!=_ptr_subrundata_array[type].end() && (*data_ptr_iter).second) {

	result_ptr = (*data_ptr_iter).second;

	result_ptr->set_run(_run_id);
	result_ptr->set_subrun(_subrun_id);

	if(!_name_out_tdirectory.empty())
	  _fout->cd(_name_out_tdirectory.c_str());

	auto out_ch_iter = _out_subrundata_ch[type].find(name);
	if(out_ch_iter == _out_subrundata_ch[type].end()) {
	  _out_subrundata_ch[type][name]=nullptr;
	  out_ch_iter = _out_subrundata_ch[type].find(name);
	}

	if(!(*out_ch_iter).second) {

	  _fout->cd(_name_out_tdirectory.c_str());

	  (*out_ch_iter).second=new TTree(Form("%s_%s_tree",
					       data::kSUBRUNDATA_TREE_NAME[(data::SubRunDataType_t)type].c_str(),
					       name.c_str()),
					  Form("%s Tree by %s",
					       data::kSUBRUNDATA_TREE_NAME[(data::SubRunDataType_t)type].c_str(),
					       name.c_str())
					  );
	  
	  //(*out_ch_iter).second->SetMaxTreeSize    (1024*1024*1024);
	  //(*out_ch_iter).second->SetMaxVirtualSize (1024*1024*1024);
	  
	  (*out_ch_iter).second->Branch(Form("%s_%s_branch",
					     data::kSUBRUNDATA_TREE_NAME[(data::SubRunDataType_t)type].c_str(),
					     name.c_str()),
					//result_ptr->GetName(),
					&(_ptr_subrundata_array[type][name]));
	  
	  Message::send(msg::kWARNING,__FUNCTION__,
			Form("Requested %s tree by %s which does not exists yet. Created a new one.", 
			     data::kSUBRUNDATA_TREE_NAME[type].c_str(),
			     //result_ptr->GetName(),
			     name.c_str())
			);
	}
      }
    }
    return result_ptr;
  }

  event_base* storage_manager::get_data(const data::DataType_t type, const std::string& name)
  {
    // Make sure io is ready
    if(!is_ready_io()){
      Message::send(msg::kERROR,__FUNCTION__,
		    Form("IO is not ready (file not opened and/or TTree has not read-in yet!"));
      throw DataFormatException("Cannot retrieve data yet.");
    }
    
    // Read entry _index-1
    if(!_index && _mode != kWRITE) {
      Message::send(msg::kERROR,__FUNCTION__,
		    Form("Call next_event() before calling %s", __FUNCTION__));
      throw DataFormatException("Cannot retrieve data yet.");
    }
    
    event_base* result_ptr = nullptr;
    auto inch_ptr_iter = _in_ch[type].find(name);
    auto data_ptr_iter = _ptr_data_array[type].find(name);
    if(data_ptr_iter != _ptr_data_array[type].end())
      result_ptr = (event_base*)((*data_ptr_iter).second);

    // If in kREAD mode, here is where we read in data product
    if( inch_ptr_iter != _in_ch[type].end() &&
	_mode == kREAD &&
	!( _event_id == (*data_ptr_iter).second->event_id() &&
	   _run_id    == (*data_ptr_iter).second->run()      &&
	   _subrun_id == (*data_ptr_iter).second->subrun()      ) ) {
      
      (*inch_ptr_iter).second->GetEntry(_index-1);

      if( _check_alignment ) {
	
	if(_event_id==data::kINVALID_UINT) 

	  _event_id = (*data_ptr_iter).second->event_id();

	else if( (*data_ptr_iter).second->event_id() != _event_id) {
	  
	  print(msg::kERROR,__FUNCTION__,
		Form("Detected event-alignment mismatch for data type %s by %s! %d (ref) != %d (%s by %s)",
		     data::kDATA_TREE_NAME[type].c_str(), name.c_str(),
		     _event_id,
		     (*data_ptr_iter).second->event_id(),
		     data::kDATA_TREE_NAME[type].c_str(),
		     (*data_ptr_iter).first.c_str())
		);
	  throw DataFormatException("Aborting event processing...");
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
	result_ptr->set_run(_run_id);
	result_ptr->set_subrun(_subrun_id);
	result_ptr->set_event_id(_event_id);
	//std::cout<<"Setting "<<_run_id<<" "<<_subrun_id<<" "<<_event_id<<std::endl;

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
	  
	  //(*out_ch_iter).second->SetMaxTreeSize    (1024*1024*1024);
	  //(*out_ch_iter).second->SetMaxVirtualSize (1024*1024*1024);
	  
	  (*out_ch_iter).second->Branch(Form("%s_%s_branch",
					     data::kDATA_TREE_NAME[(data::DataType_t)type].c_str(),
					     name.c_str()),
					//result_ptr->GetName(),
					&(_ptr_data_array[type][name]));
	  
	  Message::send(msg::kWARNING,__FUNCTION__,
			Form("Requested %s tree by %s which does not exists yet. Created a new one.", 
			     data::kDATA_TREE_NAME[type].c_str(),
			     //result_ptr->GetName(),
			     name.c_str())
			);
	}
      }
    }
    
    return result_ptr;    
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
    
    _event_id  = data::kINVALID_UINT;
    _run_id    = data::kINVALID_UINT;
    _subrun_id = data::kINVALID_UINT;
    _last_run_id = data::kINVALID_UINT;
    _last_subrun_id = data::kINVALID_UINT;
    _index=0;
    _nevents=0;
    _nevents_written=0;
    _nevents_read=0;
    _mode=kUNDEFINED;
    _status=kINIT;
    _in_fnames.clear();
    _input_product_id.clear();
    _use_read_bool = false;
    _use_write_bool = false;
    //delete _in_id_ch;
    //delete _out_id_ch;
    for(int i=0; i<data::kDATA_TYPE_MAX; ++i) {
      _in_ch[i].clear();
      _out_ch[i].clear();
      _ptr_data_array[i].clear();
      _write_data_array[i].clear();      
      _read_data_array[i].clear();      
    }
    for(int i=0; i<data::kRUNDATA_TYPE_MAX; ++i) {
      _in_rundata_ch[i].clear();
      _out_rundata_ch[i].clear();
      _ptr_rundata_array[i].clear();
      _write_rundata_array[i].clear();      
      _read_rundata_array[i].clear();      
    }
    for(int i=0; i<data::kSUBRUNDATA_TYPE_MAX; ++i) {
      _in_subrundata_ch[i].clear();
      _out_subrundata_ch[i].clear();
      _ptr_subrundata_array[i].clear();
      _write_subrundata_array[i].clear();      
      _read_subrundata_array[i].clear();      
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
	    std::set<std::string> name_candidates;
	    if(_use_read_bool) {
	      name_candidates.insert(data::kEVENT_ID_TREE);
	      for(int i=0; i<data::kDATA_TYPE_MAX; ++i) {
		
		for(auto const& name_bool : _read_data_array[i]) {
		  
		  if(name_bool.second)

		    name_candidates.insert(Form("%s_%s_tree",
						data::kDATA_TREE_NAME[i].c_str(),
						(name_bool.first).c_str()));
		}
	      }
	      for(int i=0; i<data::kRUNDATA_TYPE_MAX; ++i) {
		
		for(auto const& name_bool : _read_rundata_array[i]) {
		  
		  if(name_bool.second)

		    name_candidates.insert(Form("%s_%s_tree",
						data::kRUNDATA_TREE_NAME[i].c_str(),
						(name_bool.first).c_str()));
		}
	      }
	      for(int i=0; i<data::kSUBRUNDATA_TYPE_MAX; ++i) {
		
		for(auto const& name_bool : _read_subrundata_array[i]) {
		  
		  if(name_bool.second)

		    name_candidates.insert(Form("%s_%s_tree",
						data::kSUBRUNDATA_TREE_NAME[i].c_str(),
						(name_bool.first).c_str()));
		}
	      }
	    }
	    while(1) {
	      obj = key_iter.Next();			    
	      if(!obj) break;
	      if(name_candidates.size() && name_candidates.find(obj->GetName()) == name_candidates.end()) continue;
	      obj = fin_dir->Get(obj->GetName());
	      print(msg::kDEBUG,__FUNCTION__,Form("Found object %s (type=%s)",obj->GetName(),obj->ClassName()));
	      if(std::string(obj->ClassName())!="TTree") {
		print(msg::kDEBUG,__FUNCTION__,Form("Skipping %s ... (not TTree)",obj->GetName()));
		continue;
	      }
	      std::string obj_name = obj->GetName();
	      if(obj_name == data::kEVENT_ID_TREE) {
		if(!_in_id_ch) {
		  if(!_name_in_tdirectory.empty())
		    _in_id_ch = new TChain(Form("%s/%s",_name_in_tdirectory.c_str(),data::kEVENT_ID_TREE.c_str()),"");
		  else
		    _in_id_ch = new TChain(data::kEVENT_ID_TREE.c_str(),"");
		}
		continue;
	      }
	      char c[2] = "_";
	      if(obj_name.find_first_of(c) > obj_name.size() ||
		 obj_name.find_first_of(c) == obj_name.find_last_of(c)) {
		print(msg::kINFO,__FUNCTION__,Form("Skipping %s ... (not LArLite TTree)",obj->GetName()));
		continue;
	      }
	      std::string type_name( obj_name.substr(0,obj_name.find_first_of(c)) );
	      std::string suffix( obj_name.substr(obj_name.find_last_of(c)+1, obj_name.size()-obj_name.find_last_of(c)) );
	      std::string producer_name( obj_name.substr(obj_name.find_first_of(c)+1,obj_name.find_last_of(c)-obj_name.find_first_of(c)-1) );
	      if(suffix != "tree") {
		print(msg::kINFO,__FUNCTION__,Form("Skipping %s ... (not LArLite TTree)",obj->GetName()));
		continue;
	      }
	      bool matched=false;
	      for(int i=0; i<data::kDATA_TYPE_MAX && !matched; ++i) {
		if(data::kDATA_TREE_NAME[i] == type_name && 
		   _in_ch[(data::DataType_t)i].find(producer_name) == _in_ch[(data::DataType_t)i].end()) {
		  _in_ch[(data::DataType_t)i].insert(std::make_pair(producer_name.c_str(),(TChain*)nullptr));
		  print(msg::kINFO,__FUNCTION__,Form("Found %s tree made by %s...",type_name.c_str(),producer_name.c_str()));
		  _input_product_id.push_back(product_id((data::DataType_t)i,producer_name.c_str()));
		  matched=true;
		}
	      }
	      for(int i=0; i<data::kRUNDATA_TYPE_MAX && !matched; ++i) {
		if(data::kRUNDATA_TREE_NAME[i] == type_name && 
		   _in_rundata_ch[(data::RunDataType_t)i].find(producer_name) == _in_rundata_ch[(data::RunDataType_t)i].end()) {
		  _in_rundata_ch[(data::RunDataType_t)i].insert(std::make_pair(producer_name.c_str(),(TChain*)nullptr));
		  print(msg::kINFO,__FUNCTION__,Form("Found %s tree made by %s...",type_name.c_str(),producer_name.c_str()));
		  //_input_run_product_id.push_back(product_id((data::SubRunDataType_t)i,producer_name.c_str()));
		  matched=true;
		}
	      }
	      for(int i=0; i<data::kSUBRUNDATA_TYPE_MAX && !matched; ++i) {
		if(data::kSUBRUNDATA_TREE_NAME[i] == type_name && 
		   _in_subrundata_ch[(data::SubRunDataType_t)i].find(producer_name) == _in_subrundata_ch[(data::SubRunDataType_t)i].end()) {
		  _in_subrundata_ch[(data::SubRunDataType_t)i].insert(std::make_pair(producer_name.c_str(),(TChain*)nullptr));
		  print(msg::kINFO,__FUNCTION__,Form("Found %s tree made by %s...",type_name.c_str(),producer_name.c_str()));
		  //_input_subrun_product_id.push_back(product_id((data::SubRunDataType_t)i,producer_name.c_str()));
		  matched=true;
		}
	      }
	    }
	  }
	  fin->Close();
	}
      }
      
      if(_mode==kREAD) break;
      
    case kWRITE:
      sprintf(_buf,"Opening a file in kWRITE mode: %s",_out_fname.c_str());
      print(msg::kNORMAL,__FUNCTION__,_buf);
      _fout=TFile::Open(_out_fname.c_str(),"RECREATE");
      if(!_fout) {
	sprintf(_buf,"Open attempt failed for a file: %s", _out_fname.c_str());
	print(msg::kERROR,__FUNCTION__,_buf);
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
      print(msg::kDEBUG,__PRETTY_FUNCTION__,"ends ...");
    
    _status=kOPENED;
    return prepare_tree();
  };
  
  bool storage_manager::prepare_tree() {
    
    bool status=true;
    
    std::vector<std::map<std::string, size_t> > nevents_array(data::kDATA_TYPE_MAX,
							      std::map<std::string,size_t>());
    std::vector<std::map<std::string, size_t> > nruns_array(data::kRUNDATA_TYPE_MAX,
							    std::map<std::string,size_t>());
    std::vector<std::map<std::string, size_t> > nsubruns_array(data::kSUBRUNDATA_TYPE_MAX,
							       std::map<std::string,size_t>());
    
    for(size_t i=0; i<nevents_array.size(); ++i) {

      for(auto const& name_ptr : _in_ch[i])

	nevents_array[i].insert(std::make_pair(name_ptr.first,0));
    }
    for(size_t i=0; i<nruns_array.size(); ++i) {

      for(auto const& name_ptr : _in_rundata_ch[i])

	nruns_array[i].insert(std::make_pair(name_ptr.first,0));
    }
    for(size_t i=0; i<nsubruns_array.size(); ++i) {

      for(auto const& name_ptr : _in_subrundata_ch[i])

	nsubruns_array[i].insert(std::make_pair(name_ptr.first,0));
    }
    
    if(_verbosity[msg::kDEBUG])
      Message::send(msg::kDEBUG,__PRETTY_FUNCTION__,"called ...");
    
    if(_status!=kOPENED) {
      sprintf(_buf,"Unexpected function call @ _status=%d!",_status);
      print(msg::kERROR,__FUNCTION__,_buf);
      status=false;
    }

    if(!status) return status;
    
    if(_mode!=kWRITE) {

      // For event id tree
      if(_in_id_ch) {

	for(size_t j=0; j<_in_fnames.size(); ++j)
	    
	  _in_id_ch->AddFile(_in_fnames[j].c_str());

	gErrorIgnoreLevel = kBreak;
	bool exist=( _in_id_ch->GetEntries() > 0 );
	_in_id_ch->GetEntries();
	gErrorIgnoreLevel = kWarning;
	
	if(!exist) {
	  print(msg::kWARNING,__FUNCTION__,
		"Failed to read larlite_id_tree ... removing read pointer...");
	  delete _in_id_ch;
	  _in_id_ch=nullptr;
	  //std::cout<<"id tree does not exist... "<<std::endl;
	}else{
	  if( !_in_id_ch->GetBranch( "_run_id"    ) ||
	      !_in_id_ch->GetBranch( "_subrun_id" ) ||
	      !_in_id_ch->GetBranch( "_event_id"  ) )
	    throw DataFormatException("larlite_id_tree in an wrong format. Cannot process this file!");
	  _in_id_ch->SetBranchAddress("_run_id",    &_run_id    );
	  _in_id_ch->SetBranchAddress("_subrun_id", &_subrun_id );
	  _in_id_ch->SetBranchAddress("_event_id",  &_event_id  );
	  //std::cout<<"Set id chain addresses..."<<std::endl;
	}
      }
      // For event data tree
      for(size_t i=0; i<data::kDATA_TYPE_MAX; ++i){
      
	for(auto& name_ptr : _in_ch[i]) {
	  
	  std::string tree_name(Form("%s_%s_tree",
				     data::kDATA_TREE_NAME[(data::DataType_t)i].c_str(),
				     name_ptr.first.c_str())
				);
	  if(!_name_in_tdirectory.empty())
	    tree_name = Form("%s/%s",_name_in_tdirectory.c_str(),tree_name.c_str());
	  
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
      // For run data tree
      for(int i=0; i<data::kRUNDATA_TYPE_MAX; ++i){
      
	for(auto& name_ptr : _in_rundata_ch[i]) {
	  
	  std::string tree_name(Form("%s_%s_tree",
				     data::kRUNDATA_TREE_NAME[(data::RunDataType_t)i].c_str(),
				     name_ptr.first.c_str())
				);
	  if(!_name_in_tdirectory.empty())
	    tree_name = Form("%s/%s",_name_in_tdirectory.c_str(),tree_name.c_str());
	  
	  name_ptr.second=new TChain(tree_name.c_str(),
				     Form("%s Tree by %s",
					  data::kRUNDATA_TREE_NAME[(data::RunDataType_t)i].c_str(),
					  name_ptr.first.c_str())
				     );
	  
	  for(size_t j=0; j<_in_fnames.size(); ++j)
	    
	    name_ptr.second->AddFile(_in_fnames[j].c_str());
	  
	  // Ignore ROOT error message due to not finding a TTree 
	  gErrorIgnoreLevel = kBreak;
	  nruns_array[i][name_ptr.first] += name_ptr.second->GetEntries();
	  gErrorIgnoreLevel = kWarning;
	  
	  if(nruns_array[i][name_ptr.first]) { 
	    
	    print(msg::kDEBUG,__FUNCTION__,
		  Form("Found %zu runs found in TTree %s ...",nruns_array[i][name_ptr.first],tree_name.c_str()));
	    
	    create_rundata_ptr((data::RunDataType_t)i,name_ptr.first);
	    name_ptr.second->SetBranchAddress(Form("%s_%s_branch",
						   data::kRUNDATA_TREE_NAME[(data::RunDataType_t)i].c_str(),
						   name_ptr.first.c_str()
						   ),
					      &(_ptr_rundata_array[i][name_ptr.first])
					      );
	    
	  }else{
	    print(msg::kWARNING,__FUNCTION__,
		  Form("TTree %s contains no run... removing read pointer...",tree_name.c_str()));
	    delete name_ptr.second;
	    name_ptr.second=nullptr;
	  }
	}
      }

      // For sub-run data tree
      for(int i=0; i<data::kSUBRUNDATA_TYPE_MAX; ++i){
      
	for(auto& name_ptr : _in_subrundata_ch[i]) {
	  
	  std::string tree_name(Form("%s_%s_tree",
				     data::kSUBRUNDATA_TREE_NAME[(data::SubRunDataType_t)i].c_str(),
				     name_ptr.first.c_str())
				);
	  if(!_name_in_tdirectory.empty())
	    tree_name = Form("%s/%s",_name_in_tdirectory.c_str(),tree_name.c_str());
	  
	  name_ptr.second=new TChain(tree_name.c_str(),
				     Form("%s Tree by %s",
					  data::kSUBRUNDATA_TREE_NAME[(data::SubRunDataType_t)i].c_str(),
					  name_ptr.first.c_str())
				     );
	  
	  for(size_t j=0; j<_in_fnames.size(); ++j)
	    
	    name_ptr.second->AddFile(_in_fnames[j].c_str());
	  
	  // Ignore ROOT error message due to not finding a TTree 
	  gErrorIgnoreLevel = kBreak;
	  nsubruns_array[i][name_ptr.first] += name_ptr.second->GetEntries();
	  gErrorIgnoreLevel = kWarning;
	  
	  if(nsubruns_array[i][name_ptr.first]) { 
	    
	    print(msg::kDEBUG,__FUNCTION__,
		  Form("Found %zu sub-runs found in TTree %s ...",nsubruns_array[i][name_ptr.first],tree_name.c_str()));
	    
	    create_subrundata_ptr((data::SubRunDataType_t)i,name_ptr.first);
	    name_ptr.second->SetBranchAddress(Form("%s_%s_branch",
						   data::kSUBRUNDATA_TREE_NAME[(data::SubRunDataType_t)i].c_str(),
						   name_ptr.first.c_str()
						   ),
					      &(_ptr_subrundata_array[i][name_ptr.first])
					      );
	    
	  }else{
	    print(msg::kWARNING,__FUNCTION__,
		  Form("TTree %s contains no subrun... removing read pointer...",tree_name.c_str()));
	    delete name_ptr.second;
	    name_ptr.second=nullptr;
	  }
	}
      }
    }

    if(_mode==kWRITE) {

      _fout->cd(_name_out_tdirectory.c_str());

      // For event id tree
      _out_id_ch = new TTree(data::kEVENT_ID_TREE.c_str(),"LArLite Event ID Tree");
      _out_id_ch->Branch( "_run_id",    &_run_id,    "_run_id/i"    );
      _out_id_ch->Branch( "_subrun_id", &_subrun_id, "_subrun_id/i" );
      _out_id_ch->Branch( "_event_id",  &_event_id,  "_event_id/i"  );

      _run_id = _subrun_id = _event_id = ::larlite::data::kINVALID_UINT;
    }
    // In BOTH mode, write all that is read in
    else if(_mode==kBOTH) {
	
      _fout->cd(_name_out_tdirectory.c_str());

      // For event id tree
      _out_id_ch = new TTree(data::kEVENT_ID_TREE.c_str(),"LArLite Event ID Tree");
      _out_id_ch->Branch( "_run_id",    &_run_id,    "_run_id/i"    );
      _out_id_ch->Branch( "_subrun_id", &_subrun_id, "_subrun_id/i" );
      _out_id_ch->Branch( "_event_id",  &_event_id,  "_event_id/i"  );

      // For event data tree
      for(int i=0; i<data::kDATA_TYPE_MAX; ++i) {
	
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
	  
	  //_out_ch[i][name_ptr.first]->SetMaxTreeSize    (1024*1024*1024);
	  //_out_ch[i][name_ptr.first]->SetMaxVirtualSize (1024*1024*1024);
	  
	  _out_ch[i][name_ptr.first]->Branch(Form("%s_%s_branch",
						  data::kDATA_TREE_NAME[(data::DataType_t)i].c_str(),
						  name_ptr.first.c_str()),
					     //_ptr_data_array[i][name_ptr.first]->GetName(),
					     &(_ptr_data_array[i][name_ptr.first]));
	}
      }

      for(int i=0; i<data::kRUNDATA_TYPE_MAX; ++i) {

	for(auto& name_ptr : _in_rundata_ch[i]) {
	  
	  if(!_ptr_rundata_array[i][name_ptr.first]) continue;
	  
	  _fout->cd(_name_out_tdirectory.c_str());
	  
	  _out_rundata_ch[i][name_ptr.first]=new TTree(Form("%s_%s_tree",
							    data::kRUNDATA_TREE_NAME[(data::RunDataType_t)i].c_str(),
							    name_ptr.first.c_str()),
						       Form("%s Tree by %s",
							    data::kRUNDATA_TREE_NAME[(data::RunDataType_t)i].c_str(),
							    name_ptr.first.c_str())
						       );
	  
	  //_out_rundata_ch[i][name_ptr.first]->SetMaxTreeSize    (1024*1024*1024);
	  //_out_rundata_ch[i][name_ptr.first]->SetMaxVirtualSize (1024*1024*1024);
	  
	  _out_rundata_ch[i][name_ptr.first]->Branch(Form("%s_%s_branch",
							  data::kRUNDATA_TREE_NAME[i].c_str(),
							  name_ptr.first.c_str()),
						     //_ptr_rundata_array[i][name_ptr.first]->GetName(),
						     &(_ptr_rundata_array[i][name_ptr.first]));
	}
      }

      for(int i=0; i<data::kSUBRUNDATA_TYPE_MAX; ++i) {
	
	for(auto& name_ptr : _in_subrundata_ch[i]) {
	  
	  if(!_ptr_subrundata_array[i][name_ptr.first]) continue;
	  
	  _fout->cd(_name_out_tdirectory.c_str());
	  
	  _out_subrundata_ch[i][name_ptr.first]=new TTree(Form("%s_%s_tree",
						    data::kSUBRUNDATA_TREE_NAME[(data::SubRunDataType_t)i].c_str(),
						    name_ptr.first.c_str()),
					       Form("%s Tree by %s",
						    data::kSUBRUNDATA_TREE_NAME[(data::SubRunDataType_t)i].c_str(),
						    name_ptr.first.c_str())
					       );
	  
	  //_out_subrundata_ch[i][name_ptr.first]->SetMaxTreeSize    (1024*1024*1024);
	  //_out_subrundata_ch[i][name_ptr.first]->SetMaxVirtualSize (1024*1024*1024);
	  
	  _out_subrundata_ch[i][name_ptr.first]->Branch(Form("%s_%s_branch",
							     data::kSUBRUNDATA_TREE_NAME[(data::SubRunDataType_t)i].c_str(),
							     name_ptr.first.c_str()),
							//_ptr_subrundata_array[i][name_ptr.first]->GetName(),
							&(_ptr_subrundata_array[i][name_ptr.first]));
	}
      }
    }
      
    _nevents_written=0;
    _nevents_read=0;
    _index=0;
    
    if( _mode!=kWRITE && _nevents==0) {
      Message::send(msg::kERROR,__FUNCTION__,"Did not find any relevant data tree!");
      status=false;
    }
  
    size_t min_nevents=_nevents;
    
    for(int i=0; i<data::kDATA_TYPE_MAX; ++i) {

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
		    Form("Requested a new event-data pointer of type %s (=%d) by %s",
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
    case data::kTrigger:
      _ptr_data_array[type][name]=new trigger(name);
      break;
    case data::kGTruth:
      _ptr_data_array[type][name]=new event_gtruth(name);
      break;
    case data::kEvent:
      _ptr_data_array[type][name]=new event_base(type,name);
      break;
    case data::kRawDigit:
      _ptr_data_array[type][name]=new event_rawdigit(name);
      break;
    case data::kDAQHeaderTimeUBooNE:
      _ptr_data_array[type][name]=new event_daqheadertimeuboone(name);
      break;
    case data::kSimChannel:
      _ptr_data_array[type][name]=new event_simch(name);
      break;
    case data::kAuxDetSimChannel:
      _ptr_data_array[type][name]=new event_auxsimch(name);
      break;
    case data::kMCShower:
      _ptr_data_array[type][name]=new event_mcshower(name);
      break;
    case data::kTrack:
      _ptr_data_array[type][name]=new event_track(name);
      break;
    case data::kTrackMomentum:
      _ptr_data_array[type][name]=new event_trackmom(name);
      break;
    case data::kMCTruth:
      _ptr_data_array[type][name]=new event_mctruth(name);
      break;
    case data::kMCParticle:
      _ptr_data_array[type][name]=new event_mcpart(name);
      break;
    case data::kMCFlux:
      _ptr_data_array[type][name]=new event_mcflux(name);
      break;
    case data::kSpacePoint:
      _ptr_data_array[type][name]=new event_spacepoint(name);
      break;
    case data::kUserInfo:
      _ptr_data_array[type][name]=new event_user(name);
      break;
    case data::kWire:
      _ptr_data_array[type][name]=new event_wire(name);
      break;
    case data::kHit:
      _ptr_data_array[type][name]=new event_hit(name);
      break;
    case data::kCRTHit:
      _ptr_data_array[type][name]=new event_crthit(name);
      break;
    case data::kCRTTrack:
      _ptr_data_array[type][name]=new event_crttrack(name);
      break;
    case data::kCluster:
      _ptr_data_array[type][name]=new event_cluster(name);
      break;
    case data::kShower:
      _ptr_data_array[type][name]=new event_shower(name);
      break;
    case data::kVertex:
      _ptr_data_array[type][name]=new event_vertex(name);
      break;
    case data::kEndPoint2D:
      _ptr_data_array[type][name]=new event_endpoint2d(name);
      break;
    case data::kSeed:
      _ptr_data_array[type][name]=new event_seed(name);
      break;
    case data::kCalorimetry:
      _ptr_data_array[type][name]=new event_calorimetry(name);
      break;
    case data::kMCNeutrino:
      print(msg::kERROR,__FUNCTION__,Form("MCNeutrino is stored within MCTruth! Retrieve MCTruth instead."));
      break;
    case data::kMCTrajectory:
      print(msg::kERROR,__FUNCTION__,Form("MCTrajectory is stored within MCParticle! Retrieve MCParticle instead."));
      break;
    case data::kCosmicTag:
      _ptr_data_array[type][name]=new event_cosmictag(name);
      break;
    case data::kOpFlash:
      _ptr_data_array[type][name]=new event_opflash(name);
      break;
    case data::kOpHit:
      _ptr_data_array[type][name]=new event_ophit(name);
      break;
    case data::kT0:
      _ptr_data_array[type][name]=new event_t0(name);
      break;
    case data::kPFParticle:
      _ptr_data_array[type][name]=new event_pfpart(name);
      break;
    case data::kParticleID:
      _ptr_data_array[type][name]=new event_partid(name);
      break;
    case data::kMCTrack:
      _ptr_data_array[type][name]=new event_mctrack(name);
      break;
    case data::kROI:
      _ptr_data_array[type][name]=new event_roi(name);
      break;
    case data::kMCTree:
      _ptr_data_array[type][name]=new event_mctree(name);
      break;
    case data::kMinos:
      _ptr_data_array[type][name]=new event_minos(name);
      break;
    case data::kAssociation:
      _ptr_data_array[type][name]=new event_ass(name);
      break;
    case data::kPCAxis:
      _ptr_data_array[type][name]=new event_pcaxis(name);
      break;
    case data::kFlashMatch:
      _ptr_data_array[type][name]=new event_flashmatch(name);
      break;
    case data::kFIFO:
      _ptr_data_array[type][name]=(event_fifo*)(new event_fifo(name));
      break;
    case data::kOpDetWaveform:
      _ptr_data_array[type][name]=new event_opdetwaveform(name);
      break;
    case data::kSimPhotons:
      _ptr_data_array[type][name]=new event_simphotons(name);
      break;
    case data::kMuCSData:
      _ptr_data_array[type][name]=new event_mucsdata(name);
      break;
    case data::kMuCSReco:
      _ptr_data_array[type][name]=new event_mucsreco(name);
      break;
    case data::kChStatus:
      _ptr_data_array[type][name]=new event_chstatus(name);
      break;
    case data::kMCEventWeight:
      _ptr_data_array[type][name]=new event_mceventweight(name);
      break;
    case data::kSWTrigger:
      _ptr_data_array[type][name]=new swtrigger(name);
      break;
    case data::kLArFlow3DHit:
      _ptr_data_array[type][name]=new event_larflow3dhit(name);
      break;
    case data::kLArFlowCluster:
      _ptr_data_array[type][name]=new event_larflowcluster(name);
      break;
    case data::kPixelMask:
      _ptr_data_array[type][name]=new event_pixelmask(name);
      break;
    default:
      print(msg::kERROR,__FUNCTION__,Form("Event-data identifier not supported: %d",(int)type));
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

  void storage_manager::create_rundata_ptr(data::RunDataType_t const type, std::string const& name){

    if(get_verbosity() <= msg::kINFO)

      Message::send(msg::kINFO,__FUNCTION__,
		    Form("Requested a new run-data pointer of type %s (=%d) by %s",
			 data::kRUNDATA_TREE_NAME[type].c_str(),
			 type,
			 name.c_str())
		    );

    auto name_ptr = _ptr_rundata_array[type].find(name);
    if(name_ptr == _ptr_rundata_array[type].end()) {
      _ptr_rundata_array[type][name]=nullptr;
      name_ptr = _ptr_rundata_array[type].find(name);
    }
    if((*name_ptr).second) return;
    
    switch(type){
    default:
      print(msg::kERROR,__FUNCTION__,Form("Run-data identifier not supported: %d",(int)type));
      break;
    }
    
    return;
  }
  
  void storage_manager::delete_rundata_ptr(data::RunDataType_t const type,std::string const& name){
    
    auto name_ptr = _ptr_rundata_array[type].find(name);

    if(name_ptr == _ptr_rundata_array[type].end()) return;

    if(!((*name_ptr).second)) return;
    
    delete ((*name_ptr).second);

    (*name_ptr).second = nullptr;

    return;
  }

  void storage_manager::create_subrundata_ptr(data::SubRunDataType_t const type, std::string const& name){

    if(get_verbosity() <= msg::kINFO)

      Message::send(msg::kINFO,__FUNCTION__,
		    Form("Requested a new subrun-data pointer of type %s (=%d) by %s",
			 data::kSUBRUNDATA_TREE_NAME[type].c_str(),
			 type,
			 name.c_str())
		    );

    auto name_ptr = _ptr_subrundata_array[type].find(name);
    if(name_ptr == _ptr_subrundata_array[type].end()) {
      _ptr_subrundata_array[type][name]=nullptr;
      name_ptr = _ptr_subrundata_array[type].find(name);
    }
    if((*name_ptr).second) return;
    
    switch(type){
    case data::kPOTSummary:
      _ptr_subrundata_array[type][name]=new potsummary(name);
      break;
    default:
      print(msg::kERROR,__FUNCTION__,Form("SubRun-data identifier not supported: %d",(int)type));
      break;
    }
    
    return;
  }
  
  void storage_manager::delete_subrundata_ptr(data::SubRunDataType_t const type,std::string const& name){
    
    auto name_ptr = _ptr_subrundata_array[type].find(name);

    if(name_ptr == _ptr_subrundata_array[type].end()) return;

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
      if(_in_id_ch) {
	delete _in_id_ch;
	_in_id_ch = nullptr;
      }
      if(_mode!=kREAD){

	if(_out_id_ch) {
	  if(_verbosity[msg::kINFO])
	    Message::send(msg::kINFO,__FUNCTION__,Form("Writing TTree: %s",data::kEVENT_ID_TREE.c_str()));
						       
	  _fout = _out_id_ch->GetCurrentFile();
	  _fout->cd(_name_out_tdirectory.c_str());
	  _out_id_ch->Write();
	  _out_id_ch=nullptr;
	}

	for(int i=0; i<data::kDATA_TYPE_MAX; i++) {
	  
	  if(!_out_ch[i].size()) continue;

	  //if(_use_write_bool && !_write_data_array[i].size()) continue;

	  for(auto& name_ptr : _out_ch[i]) {

	    if(_use_write_bool && 
	       _write_data_array[i].find(name_ptr.first) == _write_data_array[i].end())
	      continue;

	    if(_verbosity[msg::kINFO])
	    
	      Message::send(msg::kINFO,__FUNCTION__,Form("Writing TTree: %s",
							 name_ptr.second->GetName()));
	    
	    _fout = name_ptr.second->GetCurrentFile();
	    _fout->cd(_name_out_tdirectory.c_str());
	    name_ptr.second->Write();
	  
	    Message::send(msg::kNORMAL,__FUNCTION__,
			  Form("TTree \"%s\" for %s written with %lld events...",
			       name_ptr.second->GetName(),
			       data::kDATA_TREE_NAME[i].c_str(),
			       name_ptr.second->GetEntries())
			  );
	  }
	}
	for(int i=0; i<data::kRUNDATA_TYPE_MAX; i++) {
	  
	  if(!_out_rundata_ch[i].size()) continue;

	  if(_use_write_bool && !_write_rundata_array[i].size()) continue;

	  for(auto& name_ptr : _out_rundata_ch[i]) {

	    if(_use_write_bool && 
	       _write_rundata_array[i].find(name_ptr.first) == _write_rundata_array[i].end())
	      continue;
	    
	    if(_verbosity[msg::kINFO])
	    
	      Message::send(msg::kINFO,__FUNCTION__,Form("Writing TTree: %s",
							 name_ptr.second->GetName()));
	    
	    _fout = name_ptr.second->GetCurrentFile();
	    _fout->cd(_name_out_tdirectory.c_str());
	    name_ptr.second->Write();
	  
	    Message::send(msg::kNORMAL,__FUNCTION__,
			  Form("TTree \"%s\" for %s written with %lld events...",
			       name_ptr.second->GetName(),
			       data::kRUNDATA_TREE_NAME[i].c_str(),
			       name_ptr.second->GetEntries())
			  );
	  }
	}
	for(int i=0; i<data::kSUBRUNDATA_TYPE_MAX; i++) {
	  
	  if(!_out_subrundata_ch[i].size()) continue;

	  //if(_use_write_bool && !_write_subrundata_array[i].size()) continue;

	  for(auto& name_ptr : _out_subrundata_ch[i]) {

	    if(_use_write_bool && 
	       _write_subrundata_array[i].find(name_ptr.first) == _write_subrundata_array[i].end())
	      continue;

	    if(_verbosity[msg::kINFO])
	    
	      Message::send(msg::kINFO,__FUNCTION__,Form("Writing TTree: %s",
							 name_ptr.second->GetName()));
	    
	    _fout = name_ptr.second->GetCurrentFile();
	    _fout->cd(_name_out_tdirectory.c_str());
	    name_ptr.second->Write();
	  
	    Message::send(msg::kNORMAL,__FUNCTION__,
			  Form("TTree \"%s\" for %s written with %lld events...",
			       name_ptr.second->GetName(),
			       data::kSUBRUNDATA_TREE_NAME[i].c_str(),
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
      
      for(int i=0; i<data::kDATA_TYPE_MAX; ++i) {
	if(_in_ch[i].size())  { _in_ch[i].clear(); }
	if(_out_ch[i].size()) { _out_ch[i].clear(); }
	for(auto const& name_ptr : _ptr_data_array[i])
	  delete_data_ptr((data::DataType_t)i, name_ptr.first);
	_ptr_data_array[i].clear();
      }

      for(int i=0; i<data::kRUNDATA_TYPE_MAX; ++i) {
	if(_in_rundata_ch[i].size())  { _in_rundata_ch[i].clear(); }
	if(_out_rundata_ch[i].size()) { _out_rundata_ch[i].clear(); }
	for(auto const& name_ptr : _ptr_rundata_array[i])
	  delete_rundata_ptr((data::RunDataType_t)i, name_ptr.first);
	_ptr_rundata_array[i].clear();
      }

      for(int i=0; i<data::kSUBRUNDATA_TYPE_MAX; ++i) {
	if(_in_subrundata_ch[i].size())  { _in_subrundata_ch[i].clear(); }
	if(_out_subrundata_ch[i].size()) { _out_subrundata_ch[i].clear(); }
	for(auto const& name_ptr : _ptr_subrundata_array[i])
	  delete_subrundata_ptr((data::SubRunDataType_t)i, name_ptr.first);
	_ptr_subrundata_array[i].clear();
      }
      
    }
    
    if(_verbosity[msg::kDEBUG])
      Message::send(msg::kDEBUG,__PRETTY_FUNCTION__,"ends ...");
    
    _status=kCLOSED;
    return status;
  }
  
  bool storage_manager::go_to(uint32_t index,bool store) {
    
    bool status=true;
    if(_mode==kWRITE) {
      if( index && index >= _nevents_written ){
	Message::send(msg::kERROR,__FUNCTION__,
		      "Cannot move the data pointer back/forth in kWRITE mode.");
	status=false;
      }
    }else if(!_nevents) {
      Message::send(msg::kWARNING,__FUNCTION__,"Input file empty!");
      status=false;
    }else if(!(index<_nevents)){
      Message::send(msg::kWARNING,__FUNCTION__,"Reached EOF");
      status=false;
    }else
      _index=index;
    
    if(status) status=next_event(store);
    
    return status;
  }

  bool storage_manager::go_to( const unsigned int run_id, const unsigned int subrun_id, const unsigned int event_id ) {
    // we cannot assume that the above indices are in order in a given tree (or chain)
    // this makes smart random access routines impossible
    // instead, we search sequentially through the index tree. however, we are usually coordinating between two files
    // whose (rse) numbers are in the same order -- though one file might have entries missing.
    // for that use, this isn't a disaster, as we start the sequential search from the last index and loop
    //  back only once in the beginning to search
    // for the coordination between truly randomly ordered files, the only thing we can do is compile an index map
    //  as we go along, then using that to search after its been completely populated.
    // also, we assume we have only one instance of r,s,e per file here.    
    // we can do that in the future, maybe fill vector< struct index_t { run, subrun, event, instance, entry } >, sort it, then
    //  freeze it after we've filled _nevents. Then we can use binary search. (not bothering to implement now)

    bool status=true;
    
    if(_mode==kWRITE) {
      Message::send(msg::kERROR,__FUNCTION__,
		    "Cannot do look up with run_id, subrun_id, event_id in kWRITE mode.");
      status=false;
    }else if(!_nevents) {
      Message::send(msg::kWARNING,__FUNCTION__,"Input file empty!");
      status=false;
    }

    if ( run_id==data::kINVALID_UINT || subrun_id==data::kINVALID_UINT || event_id==data::kINVALID_UINT ) {
      Message::send(msg::kERROR,__FUNCTION__,
		    "Invalid run_id, subrun_id, event_id given.");
      status = false;
    }
    
    if (!status)
      return status;
    
    size_t current_index = _index;    
    size_t search_index  = current_index;
    if ( _index==data::kINVALID_UINT )
      search_index = 0;
    if ( search_index>=1 ) search_index--;
    
    ULong_t bytes = 1;
    bool found = false;
    bool end_reached = false;
    while ( (current_index!=search_index || !end_reached)  && !found ) {
      bytes = _in_id_ch->GetEntry( search_index );
      if ( bytes==0 ) {
	// end of file, go to beginning
	search_index = 0;
        end_reached = true;
      }
      else {
	if ( _run_id==run_id && _subrun_id==subrun_id && _event_id==event_id ) {
	  // found
	  found = true;
          break;
	}
        search_index++;        
      }
    }
    
    // return result of search
    if ( !found ) {
      // reset the index tree state, return status = false;
      _in_id_ch->GetEntry( current_index );
      status = false;
    }
    else {
      // found a matching entry
      go_to( search_index, false );
      status = true;
    }
    return status;
  }
  
  bool storage_manager::next_event(bool store){
    
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
	if(store)
	  status=write_event();
	break;
      case kBOTH:
	if(_nevents_read && store)
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

    if(_mode == kREAD) {
      _last_run_id    = _run_id;
      _last_subrun_id = _subrun_id;
    }

    if(_in_id_ch) _in_id_ch->GetEntry(_index);
    //std::cout<<_index<<" ID chain raed-in: "<<_run_id<<" "<<_subrun_id<<" "<<_event_id<<std::endl;

    // If this is kBOTH mode, then read all relevant data products & check alignment here
    if( _mode == kBOTH ) {

      for(int i=0; i<data::kDATA_TYPE_MAX; ++i) { 

	if(_use_read_bool && !(_read_data_array[i].size()))
	  continue;

	for(auto& name_ptr : _in_ch[i]) {
      
	  if((name_ptr.second)) {

	    if(_use_read_bool && _read_data_array[i].find(name_ptr.first) == _read_data_array[i].end())
	      continue;
	    
	    (name_ptr.second)->GetEntry(_index);

	    if( _check_alignment ) {
	      
	      if(_event_id == data::kINVALID_UINT) {
		_event_id  = _ptr_data_array[i][name_ptr.first]->event_id();
		_run_id    = _ptr_data_array[i][name_ptr.first]->run();
		_subrun_id = _ptr_data_array[i][name_ptr.first]->subrun();
	      }
	      
	      else if(_event_id  != _ptr_data_array[i][name_ptr.first]->event_id() ||
		      _run_id    != _ptr_data_array[i][name_ptr.first]->run()   ||
		      _subrun_id != _ptr_data_array[i][name_ptr.first]->subrun() ) {
		
		print(msg::kERROR,__FUNCTION__,
		      Form("Detected event-alignment mismatch! %d (ref) != %d (%s by %s)",
			   _event_id,
			   _ptr_data_array[i][name_ptr.first]->event_id(),
			   data::kDATA_TREE_NAME[i].c_str(),
			   name_ptr.first.c_str()));

		return false;
		
	      }
	      //std::cout<<_run_id<<" : "<<_subrun_id<<" : "<<_event_id<<" "<<name_ptr.first<<std::endl;
	    } // end check event alignment
	  } // ptr exists
	} // loop over all trees of type i
      } // loop over all types

      for(int i=0; i<data::kRUNDATA_TYPE_MAX; ++i) { 

	if(_use_read_bool && !(_read_rundata_array[i].size()))
	  continue;

	for(auto& name_ptr : _in_rundata_ch[i]) {
      
	  if((name_ptr.second)) {

	    if(_use_read_bool && _read_rundata_array[i].find(name_ptr.first) == _read_rundata_array[i].end())
	      continue;

	    auto data_ptr_iter = _ptr_rundata_array[i].find(name_ptr.first);

	    if(name_ptr.second->GetReadEntry()<0) name_ptr.second->GetEntry(0);

	    if(_run_id == (*data_ptr_iter).second->run()) continue;

	    while( _run_id != (*data_ptr_iter).second->run() && 
		   name_ptr.second->GetReadEntry()<name_ptr.second->GetEntries()) {

	      name_ptr.second->GetEntry(name_ptr.second->GetReadEntry()+1);

	      if(_check_alignment && _run_id != (*data_ptr_iter).second->run()) {
		throw DataFormatException(Form("Detected run-id mis-alignment! %d (ref) != %d (%s by %s) ",
					       _run_id,
					       (*data_ptr_iter).second->run(),
					       data::kRUNDATA_TREE_NAME[i].c_str(),
					       name_ptr.first.c_str())
					  );
		return false;
	      }
	    }

	    if(_run_id != (*data_ptr_iter).second->run()) {
	      throw DataFormatException(Form("Could not find the run-id %d for %s by %s ",
					     _run_id,
					     data::kRUNDATA_TREE_NAME[i].c_str(),
					     name_ptr.first.c_str())
					);
	      return false;
	    } // end checking validity
	  } // ptr exists
	} // loop over all trees of type i
      } // loop over all types

      for(int i=0; i<data::kSUBRUNDATA_TYPE_MAX; ++i) { 

	if(_use_read_bool && !(_read_subrundata_array[i].size()))
	  continue;

	for(auto& name_ptr : _in_subrundata_ch[i]) {
      
	  if((name_ptr.second)) {

	    if(_use_read_bool && _read_subrundata_array[i].find(name_ptr.first) == _read_subrundata_array[i].end())
	      continue;

	    auto data_ptr_iter = _ptr_subrundata_array[i].find(name_ptr.first);

	    if(name_ptr.second->GetReadEntry()<0) name_ptr.second->GetEntry(0);

	    if(_run_id == (*data_ptr_iter).second->run() &&
	       _subrun_id == (*data_ptr_iter).second->subrun()) continue;

	    while( (_run_id != (*data_ptr_iter).second->run() ||
		    _subrun_id != (*data_ptr_iter).second->subrun()) &&
		   name_ptr.second->GetReadEntry()<name_ptr.second->GetEntries()) {

	      name_ptr.second->GetEntry(name_ptr.second->GetReadEntry()+1);
	      
	      if(_check_alignment && 
		 (_run_id != (*data_ptr_iter).second->run() ||
		  _subrun_id != (*data_ptr_iter).second->subrun()) ) {
		throw DataFormatException(Form("Detected run/subrun-id mis-alignment! %d:%d (ref) != %d:%d (%s by %s) ",
					       _run_id,
					       _subrun_id,
					       (*data_ptr_iter).second->run(),
					       (*data_ptr_iter).second->subrun(),
					       data::kRUNDATA_TREE_NAME[i].c_str(),
					       name_ptr.first.c_str())
					  );
		return false;
	      }
	    }

	    if(_run_id != (*data_ptr_iter).second->run() ||
	       _subrun_id != (*data_ptr_iter).second->subrun()) {
	      throw DataFormatException(Form("Could not find the run/subrun-id %d:%d for %s by %s ",
					     _run_id,
					     _subrun_id,
					     data::kRUNDATA_TREE_NAME[i].c_str(),
					     name_ptr.first.c_str())
					);
	      return false;
	    } // end checking validity
	  } // ptr exists
	} // loop over all trees of type i
      } // loop over all types

    }

    _index++;
    _nevents_read++;
    return true;
  }

  void storage_manager::set_id(const unsigned int run_id,
			       const unsigned int subrun_id,
			       const unsigned int event_id)
  { 
    //if(_run_id == run_id && _subrun_id == subrun_id && _event_id == event_id)
    //  return;

    _run_id = run_id; 
    _subrun_id = subrun_id; 
    _event_id = event_id; 

    for(int i=0; i<data::kDATA_TYPE_MAX; ++i) {

      for(auto& name_ptr : _out_ch[i]) {
      
	if(!(name_ptr.second)) continue;

	auto& ptr = _ptr_data_array[i][name_ptr.first];
	ptr->set_run(_run_id);
	ptr->set_subrun(_subrun_id);
	ptr->set_event_id(_event_id);
      }
    }

    for(int i=0; i<data::kSUBRUNDATA_TYPE_MAX; ++i) {
      
      for(auto& name_ptr : _out_subrundata_ch[i]) {
	
	if(!(name_ptr.second)) continue;
	
	auto& ptr = _ptr_subrundata_array[i][name_ptr.first];
	ptr->set_run(_run_id);
	ptr->set_subrun(_subrun_id);
      }
    }
	
    for(int i=0; i<data::kRUNDATA_TYPE_MAX; ++i) {
	  
      for(auto& name_ptr : _out_rundata_ch[i]) {
	    
	if(!(name_ptr.second)) continue;

	auto& ptr = _ptr_rundata_array[i][name_ptr.first];
	ptr->set_run(_run_id);
      }
    }
  }
  
  bool storage_manager::write_event(){
    
    if(_out_id_ch) _out_id_ch->Fill();

    for(int i=0; i<data::kDATA_TYPE_MAX; ++i) {

      //if(_use_write_bool && !(_write_data_array[i].size())) continue;

      for(auto& name_ptr : _out_ch[i]) {
      
	if(!(name_ptr.second)) continue;
	
	if( !_use_write_bool ||
	    _write_data_array[i].find((name_ptr.first)) != _write_data_array[i].end())
	  name_ptr.second->Fill();

	auto& data_ptr = _ptr_data_array[i][name_ptr.first];

	data_ptr->clear_data();
	data_ptr->set_run(_run_id);
	data_ptr->set_subrun(_subrun_id);
	data_ptr->set_event_id(_event_id);
      }
    }

    if(_run_id != _last_run_id || _subrun_id != _last_subrun_id) {

      for(int i=0; i<data::kSUBRUNDATA_TYPE_MAX; ++i) {
	
	//if(_use_write_bool && !(_write_subrundata_array[i].size())) continue;
	
	for(auto& name_ptr : _out_subrundata_ch[i]) {
	  
	  if(!(name_ptr.second)) continue;
	  
	  if( !_use_write_bool ||
	      _write_subrundata_array[i].find((name_ptr.first)) != _write_subrundata_array[i].end())
	    name_ptr.second->Fill();

	  auto& data_ptr = _ptr_subrundata_array[i][name_ptr.first];
	
	  data_ptr->clear_data();
	  data_ptr->set_run(_run_id);
	  data_ptr->set_subrun(_subrun_id);
	}
      }

      if(_run_id != _last_run_id) {
	
	for(int i=0; i<data::kRUNDATA_TYPE_MAX; ++i) {
	  
	  //if(_use_write_bool && !(_write_rundata_array[i].size())) continue;
	  
	  for(auto& name_ptr : _out_rundata_ch[i]) {
	    
	    if(!(name_ptr.second)) continue;
	    
	    if( !_use_write_bool ||
		_write_rundata_array[i].find((name_ptr.first)) != _write_rundata_array[i].end())
	      name_ptr.second->Fill();

	    auto& data_ptr = _ptr_rundata_array[i][name_ptr.first];
	    
	    data_ptr->clear_data();
	    data_ptr->set_run(_run_id);
	  }
	}
      }

      _last_run_id    = _run_id;
      _last_subrun_id = _subrun_id;
    }

    if(_mode==kWRITE) {
      _index++;
    }

    _nevents_written++;
    //_event_wf->clear_data();
    return true;
  }
  /*  
  const storage_manager::AssInfo_t
  storage_manager::find_one_assid(const data::DataType_t type_a,
				  const data::DataType_t type_b) const
  {
    auto const& ev_ass_m = _ptr_data_array[data::kAssociation];
    for(auto const& ev_ass_p : ev_ass_m) {
      auto const& ev_ass = (larlite::event_ass*)(ev_ass_p.second);
      auto id = ev_ass->find_one_assid(type_a,type_b);
      if( id != kINVALID_ASS )
	return std::make_pair((const larlite::event_ass*)(ev_ass),id);
    }
    return std::make_pair((const larlite::event_ass*)nullptr,kINVALID_ASS);
  }
  */
}
#endif
  
