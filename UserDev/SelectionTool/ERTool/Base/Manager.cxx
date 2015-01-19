#ifndef ERTOOL_MANAGER_CXX
#define ERTOOL_MANAGER_CXX

#include "Manager.h"

namespace ertool {

  Manager::Manager() 
    : _algo   (nullptr)
    , _filter (nullptr)
  { 
    _status = kIDLE; Reset(); 
    _training_mode = false;
    _profile_mode  = true;
    _tprof_algo.first   = _tprof_algo.second    = 0;
    _tprof_filter.first = _tprof_filter.second  = 0;
    _time_algo_init     = _time_algo_finalize   = 0;
    _time_filter_init   = _time_filter_finalize = 0;
  }

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
    _status = kINIT;
  }
  
  void Manager::Finalize(TFile* fout)
  {
    if(_status != kPROCESSING) {
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

    if(_profile_mode) {
      std::cout << "  \033[95m<<" << __FUNCTION__ << ">>\033[00m Time Profile Report 4 Filter"    << std::endl
		<< "    ProcessBegin : " << _time_filter_init     << " [s] " << std::endl
		<< "    Reconstruct  : " << _tprof_filter.second  << " [s] ... or " 
		<< _tprof_filter.second/_tprof_filter.first << " [s/event]" << std::endl
		<< "    ProcessEnd   : " << _time_filter_finalize << " [s] " << std::endl;
      std::cout << "  \033[95m<<" << __FUNCTION__ << ">>\033[00m Time Profile Report 4 Algo"    << std::endl
		<< "    ProcessBegin : " << _time_algo_init     << " [s] " << std::endl
		<< "    Reconstruct  : " << _tprof_algo.second  << " [s] ... or " 
		<< _tprof_algo.second/_tprof_algo.first << " [s/event]" << std::endl
		<< "    ProcessEnd   : " << _time_algo_finalize << " [s] " << std::endl;
    }
    _status = kFINISHED;
  }  

  ParticleSet Manager::Process(EventData &data)
  {
    ParticleSet res;
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
    if(_filter) {
      if(_profile_mode) fWatch.Start();
      _filter->_training_mode = this->_training_mode;
      _filter->EventBegin();
      if(_profile_mode) _tprof_filter.second += fWatch.RealTime();
    }
    if(_algo) {
      if(_profile_mode) fWatch.Start();
      _algo->_training_mode = this->_training_mode;
      _algo->EventBegin();
      if(_profile_mode) _tprof_algo.second += fWatch.RealTime();
    }

    if(_profile_mode) fWatch.Start();
    if(_filter)       _filter->Filter(data);
    if(_profile_mode) _tprof_filter.second += fWatch.RealTime();

    if(_profile_mode) fWatch.Start();
    if(_algo)         res = _algo->Reconstruct(data);
    if(_profile_mode) _tprof_algo.second += fWatch.RealTime();

    if(_profile_mode) fWatch.Start();
    if(_filter)       _filter->EventEnd();
    if(_profile_mode) _tprof_filter.second += fWatch.RealTime();

    if(_profile_mode) fWatch.Start();
    if(_algo)         _algo->EventEnd();
    if(_profile_mode) _tprof_algo.second += fWatch.RealTime();

    if(_profile_mode) {
      _tprof_algo.first   += 1.;
      _tprof_filter.first += 1.;
    }

    return res;
  }

}
#endif
