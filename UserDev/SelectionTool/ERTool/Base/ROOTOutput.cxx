#ifndef SELECTIONTOOL_ERTOOL_ROOTOUTPUT_CXX
#define SELECTIONTOOL_ERTOOL_ROOTOUTPUT_CXX

#include "ROOTOutput.h"
#include "UtilFunc.h"
namespace ertool {
  namespace io {
    
    ROOTOutput::ROOTOutput(const std::string& name)
      : OStreamBase(name)
      , _fout       ( nullptr )
      , _event_tree ( nullptr )
      , _graph_tree ( nullptr )
    {}
    
    void ROOTOutput::Prepare( ::ertool::EventData     *event_ptr,
			      ::ertool::ParticleGraph *graph_ptr,
			      bool mc)
    {
      if(!event_ptr && !graph_ptr)
	throw ::ertool::ERException("Neither EventData & ParticleGraph provided (nothing to read!)");
      
      if(_event_tree || _graph_tree) 
	throw ertool::ERException("Output file seems to be open!");

      std::string event_tree_name = kEVENT_DATA_NAME;
      std::string graph_tree_name = kGRAPH_DATA_NAME;
      if(mc) {
	event_tree_name += "_mc";
	graph_tree_name += "_mc";
      }
      
      if(event_ptr) {
	_event_tree = new TTree(Form("%s_tree",event_tree_name.c_str()),"");
	_event_tree->Branch(Form("%s_branch",event_tree_name.c_str()),
			    &event_ptr);
      }
      if(graph_ptr) {
	_graph_tree = new TTree(Form("%s_tree",graph_tree_name.c_str()),"");
	_graph_tree->Branch(Form("%s_branch",graph_tree_name.c_str()),
			    &graph_ptr);
      }
      return;
    }
    
    bool ROOTOutput::Open(const std::string& fname)
    {
      if(_fout) {
	ertool::msg::send(ertool::msg::kERROR,
			  "Output file handle is already open!");
	return false;
      }

      if(fname.empty()) {
	ertool::msg::send(ertool::msg::kERROR,
			  "Cannot open an output with an empty name!");
	return false;
      }

      if(!_event_tree && !_graph_tree) {
	ertool::msg::send(ertool::msg::kERROR,
			  "Neither EventData nor ParticleGraph trees exist. Call Prepare() before Open!");
	return false;
      }

      _fout = TFile::Open(fname.c_str(),"RECREATE");
	  
      return _fout;
    }
    
    bool ROOTOutput::WriteEntry()
    {
      if(_event_tree) _event_tree->Fill();
      if(_graph_tree) _graph_tree->Fill();
      return true;
    }
    
    bool ROOTOutput::Close(const ::ertool::Provenance& prov)
    {
      if(_fout) {
	_fout->cd();
	if(_event_tree) _event_tree->Write();
	if(_graph_tree) _graph_tree->Write();
	_fout->Close();
	delete _fout;
      }
      _fout       = nullptr;
      _event_tree = nullptr;
      _graph_tree = nullptr;
      return true;
    }
  }
}
   
#endif
