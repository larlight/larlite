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
      _filter->_training_mode = this->_training_mode;
      _filter->ProcessBegin();
    }
    if(_algo) {
      _algo->_training_mode = this->_training_mode;
      _algo->ProcessBegin();
    }
    _status = kINIT;
    //std::cout<<"Initialize: "<<fWatch.RealTime()<<std::endl;
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
      _filter->_training_mode = this->_training_mode;
      _filter->ProcessEnd(fout);
    }
    if(_algo) {
      _algo->_training_mode = this->_training_mode;
      _algo->ProcessEnd(fout);
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
      _filter->_training_mode = this->_training_mode;
      _filter->EventBegin();
    }
    if(_algo) {
      _algo->_training_mode = this->_training_mode;
      _algo->EventBegin();
    }
    //std::cout<<"EventBegin: "<<fWatch.RealTime()<<std::endl;

    //fWatch.Start();
    if(_filter) _filter->Filter(data);
    //std::cout<<"Filter: "<<fWatch.RealTime()<<std::endl;

    //fWatch.Start();
    if(_algo) res = _algo->Reconstruct(data);
    //std::cout<<"Reconstruct: "<<fWatch.RealTime()<<std::endl;

    //fWatch.Start();
    if(_filter) _filter->EventEnd();
    if(_algo) _algo->EventEnd();
    //std::cout<<"EventEnd: "<<fWatch.RealTime()<<std::endl;

    return res;
  }

}
#endif
