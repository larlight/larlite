#ifndef ERTOOL_EMPTYINPUT_CXX
#define ERTOOL_EMPTYINPUT_CXX

#include "ERException.h"
#include "EmptyInput.h"

namespace ertool {

  namespace io {

    EmptyInput::EmptyInput(const std::string& name)
      : IStreamBase(name)
      , _graph    (nullptr)
      , _mc_graph (nullptr)
      , _data     (nullptr)
      , _mc_data  (nullptr)
    {}
    
    void EmptyInput::SetID(unsigned int evID,
			   unsigned int runID,
			   unsigned int subrunID)
    {
      _data->SetID(evID,runID,subrunID);
      _mc_data->SetID(evID,runID,subrunID);
    }

    size_t EmptyInput::NumEntries() const
    { return 0; }
      
    void EmptyInput::Prepare( ::ertool::EventData     *event_ptr,
			      ::ertool::ParticleGraph *graph_ptr,
			      bool mc)
    {
      if(mc) {
	_mc_graph = graph_ptr;
	_mc_data  = event_ptr;
      }else{
	_graph = graph_ptr;
	_data  = event_ptr;
      }
      return;
    }
    
    void EmptyInput::AddFile( const std::string& fname )
    {
      throw ERException("EmptyInput does not take an input file ");
    }
      
    bool EmptyInput::Open( ::ertool::Provenance& in_prov)
    {
      return true;
    }
      
    bool EmptyInput::ReadParticleGraph(const size_t entry)
    {
      return true;
    }

    bool EmptyInput::ReadEventData(const size_t entry)
    {
      return true;
    }

    bool EmptyInput::Close()
    {
      return true;
    }

    void EmptyInput::ReserveShowerArray(const size_t n,const bool mc)
    {
      if(mc) _mc_data->_shower_v.reserve(n);
      else _data->_shower_v.reserve(n);
    }

    void EmptyInput::ReserveTrackArray(const size_t n,const bool mc)
    {
      if(mc) _mc_data->_track_v.reserve(n);
      else _data->_track_v.reserve(n);
    }

    NodeID_t EmptyInput::Add(const ertool::Shower& obj,
			     const ertool::RecoInputID_t& input_id,
			     const bool mc) 
    {
      if(!mc) {
	_data->Add(obj,input_id);
	return _graph->CreateParticle(_data->Shower().back()).ID();
      }else{
	_mc_data->Add(obj,input_id);
	return _mc_graph->CreateParticle(_mc_data->Shower().back()).ID();
      }
      
    }
    
    NodeID_t EmptyInput::Add(const ::ertool::Track& obj,
			     const ertool::RecoInputID_t& input_id,
			     const bool mc) 
    {
      if(!mc) {
	_data->Add(obj,input_id);
	return _graph->CreateParticle(_data->Track().back()).ID();
      }else{
	_mc_data->Add(obj,input_id);
	return _mc_graph->CreateParticle(_mc_data->Track().back()).ID();
      }
    }

    void EmptyInput::Add(const ::ertool::Flash& obj,
			 const ertool::RecoInputID_t& input_id)
    {
      _data->Add(obj,input_id);
    }
    
    NodeID_t EmptyInput::Emplace(const ::ertool::Shower&& obj,
				 const ertool::RecoInputID_t&& input_id,
				 const bool mc)
    {
      if(!mc) {
	_data->Emplace(std::move(obj),std::move(input_id));
	return _graph->CreateParticle(_data->Shower().back()).ID();
      }else{
	_mc_data->Emplace(std::move(obj),std::move(input_id));
	return _mc_graph->CreateParticle(_data->Shower().back()).ID();
      }
    }
    
    NodeID_t EmptyInput::Emplace(const ::ertool::Track&& obj,
				 const ertool::RecoInputID_t&& input_id,
				 const bool mc)
    {
      if(!mc) {
	_data->Emplace(std::move(obj),std::move(input_id));
	return _graph->CreateParticle(_data->Track().back()).ID();
      }else{
	_mc_data->Emplace(std::move(obj),std::move(input_id));
	return _mc_graph->CreateParticle(_data->Track().back()).ID();
      }
    }

    void EmptyInput::Emplace(const ::ertool::Flash&& obj,
			     const ertool::RecoInputID_t&& input_id)
    {
      _data->Emplace(std::move(obj),std::move(input_id));
    }
  }

  ::ertool::ParticleGraph& io::EmptyInput::GetParticleGraphWriteable(bool mc)
  {
    if(mc) return *_mc_graph;
    else return *_graph;
  }
}

#endif
