#ifndef ERTOOL_MANAGER_CXX
#define ERTOOL_MANAGER_CXX

#include "Manager.h"
#include "Shower.h"
#include "Track.h"
#include <sstream>
#include <iostream>

namespace ertool {

  Manager::Manager() 
    : _algo   (nullptr)
    , _ana    (nullptr)
  { 
    _status = kIDLE; Reset(); 
    _training_mode = false;
    _profile_mode  = true;
    _mc_for_ana    = false;
    _tprof_algo.first   = _tprof_algo.second    = 0;
    _tprof_ana.first    = _tprof_ana.second     = 0;
    _time_algo_init     = _time_algo_finalize   = 0;
    _time_ana_init      = _time_ana_finalize    = 0;
  }
  
  const ertool::EventData&     Manager::EventData     () const { return _data;    }
  const ertool::EventData&     Manager::MCEventData   () const { return _mc_data; }
  ertool::ParticleGraph& Manager::ParticleGraph   () { return _graph;      }
  ertool::ParticleGraph& Manager::MCParticleGraph () { return _mc_graph;   }

  /*
  ertool::EventData&   Manager::EventDataWriteable     ()
  { return _data; }
  ertool::ParticleGraph& Manager::ParticleGraphWriteable   ()
  { return _graph; }
  ertool::EventData&   Manager::MCEventDataWriteable   ()
  { return _mc_data; }
  ertool::ParticleGraph& Manager::MCParticleGraphWriteable () 
  { return _mc_graph;   } 
  */
  void Manager::SetAlgo(AlgoBase* a) 
  { 
    if(_status != kIDLE)
      throw ERException("Cannot set algorithm in non-kIDLE state...");
    _algo = a; 
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
      if(_algo) _algo->Reset();
      _status = kIDLE;
    }
    ClearData();
  }

  NodeID_t Manager::Add(const ertool::Shower& obj,
			const ertool::RecoInputID_t& input_id,
			const bool mc) 
  {
    if(!mc) {
      _data.Add(obj,input_id);
      return _graph.CreateParticle(_data.Shower().back()).ID();
    }else{
      _mc_data.Add(obj,input_id);
      return _mc_graph.CreateParticle(_mc_data.Shower().back()).ID();
    }
  }

  NodeID_t Manager::Add(const ::ertool::Track& obj,
			const ertool::RecoInputID_t& input_id,
			const bool mc) 
  {
    if(!mc) {
      _data.Add(obj,input_id);
      return _graph.CreateParticle(_data.Track().back()).ID();
    }else{
      _mc_data.Add(obj,input_id);
      return _mc_graph.CreateParticle(_mc_data.Track().back()).ID();
    }
  }

  NodeID_t Manager::Emplace(const ::ertool::Shower&& obj,
			    const ertool::RecoInputID_t&& input_id,
			    const bool mc)
  {
    if(!mc) {
      _data.Emplace(std::move(obj),std::move(input_id));
      return _graph.CreateParticle(_data.Shower().back()).ID();
    }else{
      _mc_data.Emplace(std::move(obj),std::move(input_id));
      return _mc_graph.CreateParticle(_data.Shower().back()).ID();
    }
  }

  NodeID_t Manager::Emplace(const ::ertool::Track&& obj,
			    const ertool::RecoInputID_t&& input_id,
			    const bool mc)
  {
    if(!mc) {
      _data.Emplace(std::move(obj),std::move(input_id));
      return _graph.CreateParticle(_data.Track().back()).ID();
    }else{
      _mc_data.Emplace(std::move(obj),std::move(input_id));
      return _mc_graph.CreateParticle(_data.Track().back()).ID();
    }
  }

  void Manager::ClearData() {
    _data.Reset();
    _mc_data.Reset();
    _graph.Reset();
    _mc_graph.Reset();
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
    _graph.Reset();
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
    if( _algo   ) _algo->_training_mode = this->_training_mode;

    if(_profile_mode) fWatch.Start();
    if(_algo)         _algo->Reconstruct(_data,_graph);
    if(_profile_mode) _tprof_algo.second += fWatch.RealTime();

    bool status = true;
    if(_profile_mode) fWatch.Start();
    if(_ana) {
      if(_mc_for_ana) _ana->SetMCData(_mc_data,_mc_graph);
      status = _ana->Analyze(_data,_graph);
      _ana->UnsetMCData();
    }
    if(_profile_mode) _tprof_algo.second += fWatch.RealTime();

    if(_profile_mode) {
      _tprof_algo.first   += 1.;
    }

    return status;
  }

}
#endif
