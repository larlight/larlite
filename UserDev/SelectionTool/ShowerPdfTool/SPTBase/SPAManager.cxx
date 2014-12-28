#ifndef SPAMANAGER_CXX
#define SPAMANAGER_CXX

#include "SPAManager.h"

namespace sptool {

  SPAManager::SPAManager() 
    : _algo   (nullptr)
    , _filter (nullptr)
  { 
    _status = kIDLE; Reset(); 
    _training_mode = false;
  }

  void SPAManager::SetSPAlgo(SPAlgoBase* a) 
  { 
    if(_status != kIDLE)
      throw SPAException("Cannot set algorithm in non-kIDLE state...");
    _algo = a; 
  }

  void SPAManager::SetFilter(SPAFilterBase* f)
  { 
    if(_status != kIDLE)
      throw SPAException("Cannot set filter in non-kIDLE state...");
    _filter = f;
  }

  void SPAManager::Reset() {

    switch(_status) {
    case kINIT:
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
  
  void SPAManager::Initialize()
  {
    if(_status != kIDLE) {
      std::ostringstream msg;
      msg <<"Cannot call Initialize() while status ("
	  << _status
	  << ") is not kIDLE ("
	  << kIDLE
	  << ")!";
      throw SPAException(msg.str());
    }
    if(!_algo) {
      std::ostringstream msg;
      throw SPAException("No algorithm attached (must be attached before Initialize())!");
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
  }
  
  void SPAManager::Finalize(TFile* fout)
  {
    if(_status != kPROCESSING) {
      std::ostringstream msg;
      msg <<"Cannot call Finalize() while status ("
	  << _status
	  << ") is not kPROCESSING ("
	  << kPROCESSING
	  << ")!";
      throw SPAException(msg.str());
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

  SPArticleSet SPAManager::Process(SPAData &data)
  {
    SPArticleSet res;
    if(_status != kPROCESSING && _status != kINIT) {
      std::ostringstream msg;
      msg <<"Cannot call Process() while status ("
	  << _status
	  << ") is not kPROCESSING or kINIT ("
	  << kINIT << " or " << kPROCESSING
	  << ")!";
      throw SPAException(msg.str());
    }
    _status = kPROCESSING;
    if(_filter) {
      _filter->_training_mode = this->_training_mode;
      _filter->EventBegin();
    }
    if(_algo) {
      _algo->_training_mode = this->_training_mode;
      _algo->EventBegin();
    }

    if(_filter) {
      auto const order = _filter->Select(data);
      data.ApplyOrder(order);
    }

    if(_algo)
      res = _algo->Reconstruct(data);

    if(_filter) _filter->EventEnd();
    if(_algo) _algo->EventEnd();
    return res;
  }

}
#endif
