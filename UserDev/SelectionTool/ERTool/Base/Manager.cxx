#ifndef ERTOOL_MANAGER_CXX
#define ERTOOL_MANAGER_CXX

#include "Manager.h"
#include "UtilFunc.h"
#include "Shower.h"
#include "Track.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
namespace ertool {

  Manager::Manager( const io::StreamType_t in_strm,
		    const io::StreamType_t out_strm)
    : _io_handle("ertool",in_strm,out_strm)
    , _cfg_mgr("ERToolConfig")
  {
    Reset(); 
    _status = kIDLE; 
    _training_mode = false;
    _profile_mode  = true;
    _mc_for_ana    = false;
    _verbosity     = msg::kNORMAL;
    _ana_v.clear();
    _algo_v.clear();
    _name_v.clear();
    _time_ana_v.clear();
    _time_algo_v.clear();

  }

  void Manager::AddCfgFile(const std::string cfg_fname)
  {
    _cfg_file_v.insert(cfg_fname);
  }

  void Manager::ClearCfgFile()
  {
    if(_verbosity<=msg::kDEBUG) msg::send(msg::kDEBUG,__FUNCTION__,"called");
    _cfg_file_v.clear();
    if(_verbosity<=msg::kDEBUG) msg::send(msg::kDEBUG,__FUNCTION__,"ends");
  }
  
  const EventData&     Manager::EventData       () const { return _io_handle.GetEventData     (    ); }
  const EventData&     Manager::MCEventData     () const { return _io_handle.GetEventData     (true); }
  const ParticleGraph& Manager::ParticleGraph   () const { return _io_handle.GetParticleGraph (    ); }
  const ParticleGraph& Manager::MCParticleGraph () const { return _io_handle.GetParticleGraph (true); }

  void Manager::AddAlgo(AlgoBase* a) 
  { 
    if(_status != kIDLE)
      throw ERException("Cannot set algorithm in non-kIDLE state...");
    if(_name_v.find(a->Name()) != _name_v.end()){
      std::stringstream msg;
      msg << "Duplicate analysis/algorithm w/ the name " << a->Name().c_str();
      throw ERException(msg.str());
    }
    _name_v.insert(a->Name());
    _algo_v.push_back(a);
  }

  void Manager::AddAna(AnaBase* a)
  {
    if(_status != kIDLE)
      throw ERException("Cannot set ana in non-kIDLE state...");
    if(_name_v.find(a->Name()) != _name_v.end()){
      std::stringstream msg;
      msg << "Duplicate analysis/algorithm w/ the name " << a->Name().c_str();
      throw ERException(msg.str());
    }
    _name_v.insert(a->Name());
    _ana_v.push_back(a);
  }

  void Manager::Reset() {
    if(_verbosity<=msg::kDEBUG) msg::send(msg::kDEBUG,__FUNCTION__,"called");
    switch(_status) {
    case kPROCESSING:
      std::cout << "\033[93m[WARNING]\033[00m<<"
		<< __FUNCTION__
		<< ">> Calling Reset() before calling Finalize()..."
		<< std::endl;
    default: 
      _status = kIDLE;
    }
    ClearData();
    for(auto& ptr : _algo_v) ptr->Reset();
    for(auto& ptr : _ana_v ) ptr->Reset();
    for(auto& v : _time_ana_v  ) v.Reset();
    for(auto& v : _time_algo_v ) v.Reset();
    _cfg_file_v.clear();
    _cfg_mgr.Reset();
    _cfg_file_v.insert(kDefaultConfigFileName);
    if(_verbosity<=msg::kDEBUG) msg::send(msg::kDEBUG,__FUNCTION__,"ends");
  }

  void Manager::ClearData() {
    _io_handle.ClearData();
  }

  void Manager::Initialize()
  {
    if(_verbosity<=msg::kDEBUG) msg::send(msg::kDEBUG,__FUNCTION__,"called");
    //fWatch.Start();
    if(_status != kIDLE) {
      std::ostringstream msg;
      msg <<"Cannot call Initialize() while status ("
	  << _status
	  << ") is not kIDLE ("
	  << kIDLE
	  << ")!";
      throw ERException(msg.str());
    }
    if(!_algo_v.size() && !_ana_v.size()) {
      std::ostringstream msg;
      throw ERException("No algorithm/analysis attached (must be attached before Initialize())!");
    }

    //
    // Apply PSet to algorithms/analysis instances
    //
    for(auto const& fname : _cfg_file_v) {
      if(_verbosity <= msg::kINFO) {
	std::stringstream ss;
	ss << "Reading in configuration: " << fname.c_str();
	msg::send(msg::kINFO,__FUNCTION__,ss.str());
      }
      _cfg_mgr.AddCfgFile(fname);
    }
    
    auto const& main_cfg = _cfg_mgr.Config();

    // Loop & apply (create if not existing)
    for(auto const& ptr : _algo_v) {
      if(_verbosity <= msg::kINFO) {
	std::stringstream ss;
	ss << "Configuring algorithm: " << ptr->Name().c_str();
	msg::send(msg::kINFO,__FUNCTION__,ss.str());
      }      
      ptr->AcceptPSet(main_cfg);
    }

    // Loop & apply (create if not existing)
    for(auto const& ptr : _ana_v) {
      if(_verbosity <= msg::kINFO) {
	std::stringstream ss;
	ss << "Configuring analysis: " << ptr->Name().c_str();
	msg::send(msg::kINFO,__FUNCTION__,ss.str());
      }      
      ptr->AcceptPSet(main_cfg);
    }

    //
    // Initialize units
    //
    
    // Initialize time profile containers
    _time_algo_v.clear  (              );
    _time_algo_v.resize (_algo_v.size());
    _time_ana_v.clear   (              );
    _time_ana_v.resize  (_ana_v.size() );

    // Initialize algorithms
    for(size_t i=0; i<_algo_v.size(); ++i) {
      if(_verbosity <= msg::kINFO) {
	std::stringstream ss;
	ss << "Initializing algorithm: " << _algo_v[i]->Name().c_str();
	msg::send(msg::kINFO,__FUNCTION__,ss.str());
      }      
      _algo_v[i]->_training_mode = this->_training_mode;
      fWatch.Start();
      _algo_v[i]->ProcessBegin();
      _time_algo_v[i]._time_start = fWatch.RealTime();
    }
    // Initialize analyses
    for(size_t i=0; i<_ana_v.size(); ++i) {
      if(_verbosity <= msg::kINFO) {
	std::stringstream ss;
	ss << "Initializing analysis: " << _ana_v[i]->Name().c_str();
	msg::send(msg::kINFO,__FUNCTION__,ss.str());
      }      
      fWatch.Start();
      _ana_v[i]->ProcessBegin();
      _time_ana_v[i]._time_start = fWatch.RealTime();
    }
    _status = kINIT;
    if(_verbosity<=msg::kDEBUG) msg::send(msg::kDEBUG,__FUNCTION__,"ends");
  }

  void Manager::StorePSet(const std::string& fname) const
  {
    if(_verbosity<=msg::kDEBUG) msg::send(msg::kDEBUG,__FUNCTION__,"called");
    if (FILE *file = fopen(fname.c_str(), "r")) {
      fclose(file);
      std::stringstream msg;
      msg << "File " << fname.c_str() << " already exists and cannot be over-written...";
      throw ERException(msg.str());
    }

    std::cout<<_cfg_mgr.Config().name()<<std::endl;
    ::fcllite::PSet out_cfg(_cfg_mgr.Config().name());

    // Loop over alogirthms, check PSet should be stored or not
    for(size_t i=0; i<_algo_v.size(); ++i) {
      auto const& unit_conf = _algo_v[i]->OutputPSet();
      if(!(unit_conf.size()))
	continue;

      if( _cfg_mgr.Config().contains_pset(unit_conf.name()) &&
	  _cfg_mgr.Config().get_pset(unit_conf.name()) == unit_conf )
	continue;
      out_cfg.add_pset(unit_conf);
    }

    // Loop over analyses, check PSet should be stored or not
    for(size_t i=0; i<_ana_v.size(); ++i) {
      auto const& unit_conf = _ana_v[i]->OutputPSet();
      if(!(unit_conf.size()))
	continue;
      if( _cfg_mgr.Config().contains_pset(unit_conf.name()) &&
	  _cfg_mgr.Config().get_pset(unit_conf.name()) == unit_conf )
	continue;
      out_cfg.add_pset(unit_conf);
    }

    if(out_cfg.size()) {

      for(auto const& key : _cfg_mgr.Config().pset_keys()) {

	if(!out_cfg.contains_pset(key))

	  out_cfg.add_pset(_cfg_mgr.Config().get_pset(key));

	else {
	  std::stringstream ss;
	  ss << "  New Config Found: " << key.c_str();
	  msg::send(msg::kNORMAL,__FUNCTION__,ss.str());
	}
      }
      std::fstream fout;
      fout.open (fname.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
      for(auto const& key : out_cfg.pset_keys())
	fout << out_cfg.get_pset(key).dump().c_str() << std::endl;
      fout.close();
      std::stringstream ss;
      ss << "  Stored: " << fname.c_str();
      msg::send(msg::kNORMAL,__FUNCTION__,ss.str());
      
    }else

      msg::send(msg::kNORMAL,__FUNCTION__,"  No new configuration to be stored... ");

    if(_verbosity<=msg::kDEBUG) msg::send(msg::kDEBUG,__FUNCTION__,"ends");
  }
  
  void Manager::Finalize(TFile* fout)
  {
    if(_verbosity<=msg::kDEBUG) msg::send(msg::kDEBUG,__FUNCTION__,"called");
    if(_status == kINIT) {
      std::ostringstream msg;
      msg << "It seems Process() was never called ... (no problem but nothing to formally finalize)..."
	  << std::endl;
    }
    else if(_status != kPROCESSING) {
      std::ostringstream msg;
      msg <<"Cannot call Finalize() while status ("
	  << _status
	  << ") is not kPROCESSING ("
	  << kPROCESSING
	  << ")!";
      throw ERException(msg.str());
    }

    //
    // Finalize units
    //

    // Finalize algorithms
    for(size_t i=0; i<_algo_v.size(); ++i) {
      _algo_v[i]->_training_mode = this->_training_mode;
      fWatch.Start();
      _algo_v[i]->ProcessEnd(fout);
      _time_algo_v[i]._time_end = fWatch.RealTime();
    }

    // Finalize analyses
    for(size_t i=0; i<_ana_v.size(); ++i) {
      fWatch.Start();
      _ana_v[i]->ProcessEnd(fout);
      _time_ana_v[i]._time_end = fWatch.RealTime();
    }

    if(_profile_mode) {
      std::cout << std::endl << "  \033[95m<<" << __FUNCTION__ << ">>\033[00m Time Profile Report"  << std::endl;
      std::cout << " ";
      for(size_t i=0; i<68; ++i)
	std::cout<<"_";
      std::cout << std::endl;
      
      std::cout << "| "
		<< std::left << std::setw(20) << "Name"     << " | "
		<< std::left << std::setw(12) << "Begin [s]"    << " | "
		<< std::left << std::setw(12) << "Process [s]"  << " | "
		<< std::left << std::setw(12) << "End [s]"      << " | "
		<< std::endl;

      std::cout << " ";
      for(size_t i=0; i<68; ++i)
	std::cout<<"_";
      std::cout << std::flush;

      for(size_t i=0; i<_algo_v.size(); ++i) {
	std::cout << std::endl
		  << "| "
		  << std::left << std::setw(20) << _algo_v[i]->Name() << " | "
		  << std::left << std::setw(12) << _time_algo_v[i]._time_start << " | "
		  << std::left << std::setw(12) << _time_algo_v[i]._time_proc  << " | "
		  << std::left << std::setw(12) << _time_algo_v[i]._time_end   << " | ";
      }
      for(size_t i=0; i<_ana_v.size(); ++i) {
	std::cout << std::endl
		  << "| "
		  << std::left << std::setw(20) << _ana_v[i]->Name() << " | "
		  << std::left << std::setw(12) << _time_ana_v[i]._time_start << " | "
		  << std::left << std::setw(12) << _time_ana_v[i]._time_proc  << " | "
		  << std::left << std::setw(12) << _time_ana_v[i]._time_end   << " | ";
      }

      std::cout << std::endl;
      std::cout << " ";
      for(size_t i=0; i<68; ++i)
	std::cout <<  "_";
      std::cout << std::endl << std::endl;
    }

    _status = kFINISHED;
    if(_verbosity<=msg::kDEBUG) msg::send(msg::kDEBUG,__FUNCTION__,"ends");
  }  

  bool Manager::Process()
  {
    if(_verbosity<=msg::kDEBUG) msg::send(msg::kDEBUG,__FUNCTION__,"called");
    //_graph.Reset();
    if(_status != kPROCESSING && _status != kINIT) {
      std::ostringstream msg;
      msg <<"Cannot call Process() while status ("
	  << _status
	  << ") is not kPROCESSING or kINIT ("
	  << kINIT << " or " << kPROCESSING
	  << ")!";
      throw ERException(msg.str());
    }
    //fWatch.Start();
    _status = kPROCESSING;
    for(size_t i=0; i<_algo_v.size(); ++i) {
      auto& algo = _algo_v[i];
      algo->_training_mode = this->_training_mode;
      if(_verbosity <= msg::kDEBUG) {
	std::stringstream ss;
	ss << "Processing algorithm: " << algo->Name().c_str();
	msg::send(msg::kDEBUG,__FUNCTION__,ss.str());
      }
      if(_profile_mode) fWatch.Start();
      algo->Reconstruct(_io_handle.GetEventData(),_io_handle.GetParticleGraphWriteable());
      if(_profile_mode) _time_algo_v[i]._time_proc += fWatch.RealTime();
    }
    
    bool status = true;
    for(size_t i=0; i<_ana_v.size(); ++i) {
      auto& ana = _ana_v[i];
      if(_verbosity <= msg::kDEBUG) {
	std::stringstream ss;
	ss << "Processing analysis: " << ana->Name().c_str();
	msg::send(msg::kDEBUG,__FUNCTION__,ss.str());
      }
      if(_profile_mode) fWatch.Start();
      if(_mc_for_ana) ana->SetMCData(_io_handle.GetEventData(true),
				     _io_handle.GetParticleGraph(true)
				     );
      status = ana->Analyze(_io_handle.GetEventData(),_io_handle.GetParticleGraph());
      ana->UnsetMCData();
      if(_profile_mode) _time_ana_v[i]._time_proc += fWatch.RealTime();
    }

    if(_verbosity<=msg::kDEBUG) msg::send(msg::kDEBUG,__FUNCTION__,"ends");
    return status;
  }

}
#endif
