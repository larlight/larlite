#ifndef SELECTIONTOOL_ERTOOL_ROOTFILEIO_CXX
#define SELECTIONTOOL_ERTOOL_ROOTFILEIO_CXX

#include "ROOTFileIO.h"
#include "Message.h"
namespace ertool {

  ROOTFileIO(const std::string& proc_name)
    : IOBase              ( proc_name )
    , _in_chain_edata     ( nullptr   )
    , _in_chain_pgraph    ( nullptr   )
    , _in_chain_edata_mc  ( nullptr   )
    , _in_chain_pgraph_mc ( nullptr   )
    , _out_tree_edata     ( nullptr   )
    , _out_tree_pgraph    ( nullptr   )
    , _out_tree_edata_mc  ( nullptr   )
    , _out_tree_pgraph_mc ( nullptr   )
  {}

  /// Open input/output file(s)
  bool ROOTFileIO::Open() {

    for(auto const& in_fname : _in_file_name_v) {

      TFile* tmp_file = TFile::Open(in_fname.c_str(),"READ");

      if(!tmp_file) {

	msg::Message::get()->send(msg::kERROR, __FUNCTION__,
				  Form("Failed to read a file: %s", in_fname.c_str()));

	continue;
      }

      if(tmp_file->Get(Form("%s_tree",kEVENT_DATA_NAME.c_str()))) {
	if(!_in_chain_edata) {
	  _in_chain_edata = new TChain();
	}
	_in_chain_edata->AddFile(in_fname);
      }
      if(tmp_file->Get(Form("%s_tree",kGRAPH_DATA_NAME.c_str())))
	_in_chain_graph->AddFile(in_fname);
      if(tmp_file->Get(Form("%s_tree",kMC_EVENT_DATA_NAME.c_str())))
	_in_chain_edata_mc->AddFile(in_fname);
      if(tmp_file->Get(Form("%s_tree",kMC_GRAPH_DATA_NAME.c_str())))
	_in_chain_graph_mc->AddFile(in_fname);
    }
  }
  
  /// Read a specified entry (event)
  bool ROOTFileIO::ReadEntry(size_t entry);
  
  /// Write the current entry (event)
  bool ROOTFileIO::WriteEntry();
  
  /// Close input/output file(s)
  bool ROOTFileIO::Close();
  
}
#endif
