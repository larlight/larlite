#ifndef SELECTIONTOOL_ERTOOL_ROOTINPUT_CXX
#define SELECTIONTOOL_ERTOOL_ROOTINPUT_CXX

#include "ROOTInput.h"
#include "UtilFunc.h"
namespace ertool {
  namespace io {
    ROOTInput::ROOTInput(const std::string& name)
      : IStreamBase  ( name    )
      , _event_chain ( nullptr )
      , _graph_chain ( nullptr )
      , _event_entry ( ::ertool::kINVALID_SIZE )
      , _graph_entry ( ::ertool::kINVALID_SIZE )
    {}
    
    void ROOTInput::Prepare( ::ertool::EventData     *event_ptr,
			     ::ertool::ParticleGraph *graph_ptr,
			     bool mc)
    {
      if(!event_ptr && !graph_ptr)
	throw ::ertool::ERException("Neither EventData & ParticleGraph provided (nothing to read!)");
      
      if(_event_chain || _graph_chain) {
	ertool::msg::send(ertool::msg::kWARNING,__FUNCTION__,
			  "Looks Prepare/Open called beforehand. Re-setting before preparation...");
	if(!(this->Close()))
	  throw ertool::ERException("Failed to close a file!");
      }
      
      std::string event_chain_name = kEVENT_DATA_NAME;
      std::string graph_chain_name = kGRAPH_DATA_NAME;
      if(mc) {
	event_chain_name += "_mc";
	graph_chain_name += "_mc";
      }
      
      if(event_ptr) {
	_event_chain = new TChain(Form("%s_tree",event_chain_name.c_str()),"");
	_event_chain->SetBranchAddress(Form("%s_branch",event_chain_name.c_str()),
				       &event_ptr);
      }
      if(graph_ptr) {
	_graph_chain = new TChain(Form("%s_tree",graph_chain_name.c_str()),"");
	_graph_chain->SetBranchAddress(Form("%s_branch",graph_chain_name.c_str()),
				       &graph_ptr);
      }
    }
    
    void ROOTInput::AddFile( const std::string& fname )
    {
      if(!_event_chain && !_graph_chain)
	throw ertool::ERException("Call Prepare before AddFile!");
      if(_event_chain) _event_chain->AddFile(fname.c_str());
      if(_graph_chain) _graph_chain->AddFile(fname.c_str());
    }

    bool ROOTInput::IsOpen() const
    { return (_n_entries); }
    
    bool ROOTInput::Open(Provenance& in_prov)
    {
      if(!_event_chain && !_graph_chain)
	throw ertool::ERException("Call Prepare & AddFile before Open!");
      
      long long int entries = 0;

      if(_event_chain) {
	entries = _event_chain->GetEntries();
	if(entries>0)
	  _n_entries = entries;
	else
	  delete _event_chain;
      }
      
      if(_graph_chain) {
	entries = _graph_chain->GetEntries();
	if(entries>0) {
	  if(_n_entries && entries != _n_entries) {
	    ertool::msg::send(ertool::msg::kWARNING, __FUNCTION__,
			      Form("Found mis-match in # entries (EventData %zu vs. ParticleGraph %lld)",
				   _n_entries,entries)
			      );
	    if(_n_entries > entries) _n_entries = entries;
	    ertool::msg::send(ertool::msg::kWARNING, __FUNCTION__,
			      Form("Setting the smaller one (%zu) to be used as the max # events...",_n_entries)
			      );
	  }  
	}
	else
	  delete _graph_chain;
      }
      
      return IsOpen();
    }
    
    bool ROOTInput::ReadParticleGraph(const size_t entry)
    {
      if(_graph_entry == entry) return true;
      if(entry >= _n_entries) {
	ertool::msg::send(ertool::msg::kERROR,__FUNCTION__,
			  Form("Cannot read entry %zu (does not exist!)",entry));
	return false;
      }
      if(!_graph_chain) {
	ertool::msg::send(ertool::msg::kERROR,__FUNCTION__,"EventData is not registered!");
	return false;
      }
      _graph_chain->GetEntry(entry);
      _graph_entry = entry;
      return true;
    }
    
    bool ROOTInput::ReadEventData(const size_t entry)
    {
      if(_event_entry == entry) return true;
      if(entry >= _n_entries) {
	ertool::msg::send(ertool::msg::kERROR,__FUNCTION__,
			  Form("Cannot read entry %zu (does not exist!)",entry));
	return false;
      }
      if(!_event_chain) {
	ertool::msg::send(ertool::msg::kERROR,__FUNCTION__,"EventData is not registered!");
	return false;
      }
      _event_chain->GetEntry(entry);
      _event_entry = entry;
      return true;
    }
    
    /// Function to "close" the output stream
    bool ROOTInput::Close()
    {
      delete _event_chain;
      delete _graph_chain;
      _n_entries   = 0;
      _event_entry = ertool::kINVALID_SIZE;
      _graph_entry = ertool::kINVALID_SIZE;
      return true;
    }
    
  }
}
#endif
