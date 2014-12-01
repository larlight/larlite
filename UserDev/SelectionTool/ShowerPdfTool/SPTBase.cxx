#ifndef SELECTIONTOOL_SPTBASE_CXX
#define SELECTIONTOOL_SPTBASE_CXX

#include "SPTBase.h"

namespace sptool {

  void SPTBase::LoadParams(std::string fname, size_t version)
  {
    // If file name is empty, use the default
    if(fname.empty())
      
      fname = Form("%s/SelectionTool/ShowerPdfTool/dat/%s",getenv("LARLITE_USERDEVDIR"),kSPTDataFileName.c_str());

    // Create data receivers
    TChain* ch = new TChain(Form("sptool_data_%s",_name.c_str()));
    ::larlite::user_info* tmp_data = new ::larlite::user_info;
    ch->SetBranchAddress(Form("sptool_data_%s_br",_name.c_str()),
			 &tmp_data);

    // Read in data tree
    ch->AddFile(fname.c_str());
    gErrorIgnoreLevel = kBreak;
    size_t nentries = ch->GetEntries();
    gErrorIgnoreLevel = kWarning;

    // Check if data is found
    if(!nentries) throw SPAException(Form("Failed to load params for algo: %s",_name.c_str()));

    // Check if specified version is available
    if(nentries < version) throw SPAException(Form("Cannot fetch version %zu (latest version %zu)",version,(nentries-1)));

    // Retrieve data
    if(version)
      ch->GetEntry(version);
    else
      ch->GetEntry(nentries-1);

    // Copy
    _params = *tmp_data;

    // Cleanup
    delete ch;
    delete tmp_data;

    return;
  }

  void SPTBase::StoreParams(std::string fname)
  {
    // If file name is empty, use the default
    if(fname.empty())
      
      fname = Form("%s/SelectionTool/ShowerPdfTool/dat/%s",getenv("LARLITE_USERDEVDIR"),kSPTDataFileName.c_str());

    // Create data containers
    TFile* fout = TFile::Open(fname.c_str(),"UPDATE");
    if(!fout) throw SPAException(Form("Failed to create a storage file: %s",fname.c_str()));
    TTree* tree = (TTree*)(fout->Get(Form("sptool_data_%s",_name.c_str())));
    ::larlite::user_info* tmp_data = new ::larlite::user_info;

    // Copy data
    (*tmp_data) = _params;

    // Set TTree branch address or create a branch if tree does not exist yet.
    if(tree)

      tree->SetBranchAddress(Form("sptool_data_%s_br",_name.c_str()),&tmp_data);

    else {
      tree = new TTree(Form("sptool_data_%s",_name.c_str()),Form("sptool data for algo %s",_name.c_str()));
      tree->Branch(Form("sptool_data_%s_br",_name.c_str()),
		   _params.GetName(),
		   &tmp_data);
    }
    
    // Store
    tree->Fill();
    tree->Write();
    fout->Save();
    // Purge the older TObject cycles
    fout->Purge();
    // Close
    fout->Close();
  }

}

#endif
