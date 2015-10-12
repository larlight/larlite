#ifndef DATA_FETCHER_CXX
#define DATA_FETCHER_CXX





#include "DataFetcher.h"

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TLeaf.h>
#include <TLorentzVector.h>

#include "TreeElementReader.h"

namespace evd {

DataFetcher::DataFetcher() {};

DataFetcher::~DataFetcher() {}

void DataFetcher::set_file_path(std::string s) {

  file_path_ = s;
  gROOT->ProcessLine("gErrorIgnoreLevel = kError;");

  const std::string tree_name = "Events";

  TFile * file = new TFile(file_path_.c_str(), "READ");
  tree_ = (TTree *) file->Get(tree_name.c_str());
  entries_ = tree_->GetEntries();
  // tree_->SetMakeClass(1);
  // b_raw__DAQHeader_daq__Swizzler_obj_fRun = (TBranchElement* )    tree_ -> GetBranch("raw::DAQHeader_daq__Swizzler.obj.fRun");
  // b_raw__DAQHeader_daq__Swizzler_obj_fSubRun = (TBranchElement* ) tree_ -> GetBranch("raw::DAQHeader_daq__Swizzler.obj.fSubRun");
  // b_raw__DAQHeader_daq__Swizzler_obj_fEvent = (TBranchElement* )  tree_ -> GetBranch("raw::DAQHeader_daq__Swizzler.obj.fEvent");
  // b_raw__DAQHeader_daq__Swizzler_obj_fRun    -> SetAddress(&run_);
  // b_raw__DAQHeader_daq__Swizzler_obj_fSubRun -> SetAddress(&subrun_);
  // b_raw__DAQHeader_daq__Swizzler_obj_fEvent  -> SetAddress(&event_);
  // tree_->SetMakeClass(0);

}


void DataFetcher::get_entry(unsigned int entry) {

  std::cout << "entry: " << entry << std::endl;

  adc_.clear();
  channel_.clear();
  pedestal_.clear();

  // Set to get the
  tree_ -> SetMakeClass(1);
  TBranchElement * b_raw__DAQHeader_daq__Swizzler_obj_fRun
    = (TBranchElement* )    tree_ -> GetBranch("raw::DAQHeader_daq__Swizzler.obj.fRun");
  TBranchElement * b_raw__DAQHeader_daq__Swizzler_obj_fSubRun
    = (TBranchElement* ) tree_ -> GetBranch("raw::DAQHeader_daq__Swizzler.obj.fSubRun");
  TBranchElement * b_raw__DAQHeader_daq__Swizzler_obj_fEvent
    = (TBranchElement* )  tree_ -> GetBranch("raw::DAQHeader_daq__Swizzler.obj.fEvent");
  b_raw__DAQHeader_daq__Swizzler_obj_fRun    -> SetAddress(&run_);
  b_raw__DAQHeader_daq__Swizzler_obj_fSubRun -> SetAddress(&subrun_);
  b_raw__DAQHeader_daq__Swizzler_obj_fEvent  -> SetAddress(&event_);
  b_raw__DAQHeader_daq__Swizzler_obj_fRun -> GetEntry(entry);
  b_raw__DAQHeader_daq__Swizzler_obj_fSubRun -> GetEntry(entry);
  b_raw__DAQHeader_daq__Swizzler_obj_fEvent -> GetEntry(entry);
  tree_ -> SetMakeClass(0);

  tree_ -> GetEntry(entry);

  std::vector<std::string>
  raw_digit_leaf_names = FindLeavesOfType("vector<raw::RawDigit>");

  for (size_t i = 0; i < raw_digit_leaf_names.size(); ++i) {
    std::string name = raw_digit_leaf_names[i];

    TreeElementReader adc_branch(tree_, name + "obj.fADC");

    if (!adc_branch.ok()) continue;

    adc_rows_ = 0;
    adc_cols_ = 0;
    for (size_t j = 0; j < adc_branch.entries(); ++j) {
      const std::vector<short> *
      adc_ptr = adc_branch.get< std::vector<short> >(j);
      adc_.insert(adc_.end(), adc_ptr->begin(), adc_ptr->end());
      adc_cols_ = adc_ptr->size();
      adc_rows_ += 1;
    }


    TreeElementReader channel_branch(tree_, name + "obj.fChannel");
    if (!channel_branch.ok()) continue;

    for (size_t j = 0; j < channel_branch.entries(); ++j) {
      const uint32_t * channel_ptr = channel_branch.get<uint32_t>(j);
      channel_.push_back(* channel_ptr);
    }

    TreeElementReader pedestal_branch(tree_, name + "obj.fPedestal");
    if (!pedestal_branch.ok()) continue;

    for (size_t j = 0; j < pedestal_branch.entries(); ++j) {
      const double * pedestal_ptr = pedestal_branch.get<double>(j);
      pedestal_.push_back(* pedestal_ptr);
    }

    TreeElementReader pedestal_sigma_branch(tree_, name + "obj.fSigma");
    if (!pedestal_sigma_branch.ok()) continue;
  }


}

// This piece of code is borrowed from Argo written by Nathaniel Tagg
std::vector<std::string> DataFetcher::FindLeavesOfType(std::string pattern) {
  /// Look in the tree and try to find a leaf element that matches 'pattern'.
  /// Return the full name of that leaf.
  std::vector<std::string> leaf_names;

  // Strip whitespace from pattern.
  pattern.erase(std::remove_if(pattern.begin(), pattern.end(), ::isspace),
                pattern.end());

  TObjArray * list = tree_->GetListOfLeaves();
  for (int i = 0; i < list->GetEntriesFast(); ++i) {
    TObject * o = list->At(i);
    TLeaf * leaf = (TLeaf *) o;
    std::string name = leaf->GetTypeName();
    // std::cout  << name << std::endl;
    // Strip whitespace from pattern.
    name.erase(std::remove_if(name.begin(), name.end(), ::isspace),
               name.end());
    size_t found = name.find(pattern);
    if (found != std::string::npos) {
      // Return this match.
      leaf_names.push_back(leaf->GetName());
    }
  }
  return leaf_names;
}

} // evd

#endif // DATA_FETCHER_CXX
