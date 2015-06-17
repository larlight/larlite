#ifndef ERTOOL_MANAGER_CXX
#define ERTOOL_MANAGER_CXX

#include "Manager.h"

namespace ertool {

  Manager::Manager() 
    : _filter (nullptr)
    , _algo   (nullptr)
    , _ana    (nullptr)
  { 
    _status = kIDLE; Reset(); 
    _training_mode = false;
    _profile_mode  = true;
    _mc_for_ana    = false;
    _tprof_algo.first   = _tprof_algo.second    = 0;
    _tprof_ana.first    = _tprof_ana.second     = 0;
    _tprof_filter.first = _tprof_filter.second  = 0;
    _time_algo_init     = _time_algo_finalize   = 0;
    _time_filter_init   = _time_filter_finalize = 0;
    _time_ana_init      = _time_ana_finalize    = 0;
  }

  const ertool::EventData&   Manager::EventData     () const { return _data;    }
  const ertool::EventData&   Manager::MCEventData   () const { return _mc_data; }
  const ertool::ParticleSet& Manager::ParticleSet   () const { return _ps;      }
  const ertool::ParticleSet& Manager::MCParticleSet () const { return _mc_ps;   }
  
  ertool::EventData&   Manager::EventDataWriteable     ()
  { return _data; }
  ertool::ParticleSet& Manager::ParticleSetWriteable   ()
  { return _ps; }
  ertool::EventData&   Manager::MCEventDataWriteable   ()
  { return _mc_data; }
  ertool::ParticleSet& Manager::MCParticleSetWriteable () 
  { return _mc_ps;   } 

  void Manager::SetAlgo(AlgoBase* a) 
  { 
    if(_status != kIDLE)
      throw ERException("Cannot set algorithm in non-kIDLE state...");
    _algo = a; 
  }

  void Manager::SetFilter(FilterBase* f)
  { 
    if(_status != kIDLE)
      throw ERException("Cannot set filter in non-kIDLE state...");
    _filter = f;
  }

  void Manager::SetAna(AnaBase* a)
  {
    if(_status != kIDLE)
      throw ERException("Cannot set ana in non-kIDLE state...");
    _ana = a;
  }

  void Manager::Reset() {

    switch(_status) {
    case kPROCESSING:
      std::cout << "\033[93m[WARNING]\033[00m<<"
		<< __FUNCTION__
		<< ">> Calling Reset() before calling Finalize()..."
		<< std::endl;
    default:
      if(_filter) _filter->Reset();
      if(_algo) _algo->Reset();
      _status = kIDLE;
    }
    ClearData();
  }

  void Manager::ClearData() {
    _data.Reset();
    _mc_data.Reset();
    _ps.clear();
    _mc_ps.clear();
  }
  
  void Manager::Initialize()
  {
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
    if(!_algo) {
      std::ostringstream msg;
      throw ERException("No algorithm attached (must be attached before Initialize())!");
    }
    if(_filter) {
      fWatch.Start();
      _filter->_training_mode = this->_training_mode;
      _filter->ProcessBegin();
      _time_filter_init = fWatch.RealTime();
    }
    if(_algo) {
      fWatch.Start();
      _algo->_training_mode = this->_training_mode;
      _algo->ProcessBegin();
      _time_algo_init = fWatch.RealTime();
    }
    if(_ana) {
      fWatch.Start();
      _ana->ProcessBegin();
      _time_ana_init = fWatch.RealTime();
    }
    _status = kINIT;
  }
  
  void Manager::Finalize(TFile* fout)
  {
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
    
    if(_filter) {
      fWatch.Start();
      _filter->_training_mode = this->_training_mode;
      _filter->ProcessEnd(fout);
      _time_filter_finalize = fWatch.RealTime();
    }
    if(_algo) {
      fWatch.Start();
      _algo->_training_mode = this->_training_mode;
      _algo->ProcessEnd(fout);
      _time_algo_finalize = fWatch.RealTime();
    }
    if(_ana) {
      fWatch.Start();
      _ana->ProcessEnd(fout);
      _time_ana_finalize = fWatch.RealTime();
    }

    if(_profile_mode) {
      std::cout << "  \033[95m<<" << __FUNCTION__ << ">>\033[00m Time Profile Report 4 Filter"    << std::endl
		<< "    ProcessBegin : " << _time_filter_init     << " [s] " << std::endl
		<< "    Filter       : " << _tprof_filter.second  << " [s] ... or " 
		<< _tprof_filter.second/_tprof_filter.first << " [s/event]" << std::endl
		<< "    ProcessEnd   : " << _time_filter_finalize << " [s] " << std::endl;
      std::cout << "  \033[95m<<" << __FUNCTION__ << ">>\033[00m Time Profile Report 4 Algo"    << std::endl
		<< "    ProcessBegin : " << _time_algo_init     << " [s] " << std::endl
		<< "    Reconstruct  : " << _tprof_algo.second  << " [s] ... or " 
		<< _tprof_algo.second/_tprof_algo.first << " [s/event]" << std::endl
		<< "    ProcessEnd   : " << _time_algo_finalize << " [s] " << std::endl;
      std::cout << "  \033[95m<<" << __FUNCTION__ << ">>\033[00m Time Profile Report 4 Analysis"    << std::endl
		<< "    ProcessBegin : " << _time_ana_init     << " [s] " << std::endl
		<< "    Analyze      : " << _tprof_ana.second  << " [s] ... or " 
		<< _tprof_ana.second/_tprof_algo.first << " [s/event]" << std::endl
		<< "    ProcessEnd   : " << _time_ana_finalize << " [s] " << std::endl;
    }
    _status = kFINISHED;
  }  

  bool Manager::Process()
  {
    _ps.clear();
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
    if( _filter ) _filter->_training_mode = this->_training_mode;
    if( _algo   ) _algo->_training_mode = this->_training_mode;

    if(_profile_mode) fWatch.Start();
    if(_filter) _filter->Filter(_data);
    if(_profile_mode) _tprof_filter.second += fWatch.RealTime();

    if(_profile_mode) fWatch.Start();
    if(_algo)         _ps = _algo->Reconstruct(_data);
    if(_profile_mode) _tprof_algo.second += fWatch.RealTime();

    bool status = true;
    if(_profile_mode) fWatch.Start();
    if(_ana) {
      if(_mc_for_ana) _ana->SetMCData(_mc_data,_mc_ps);
      status = _ana->Analyze(_data,_ps);
      _ana->UnsetMCData();
    }
    if(_profile_mode) _tprof_algo.second += fWatch.RealTime();

    if(_profile_mode) {
      _tprof_algo.first   += 1.;
      _tprof_filter.first += 1.;
    }

    return status;
  }

}
#endif
