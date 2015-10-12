#include "TreeElementReader.h"

#include <iostream>
#include <vector>

#include <TTree.h>
#include <TBranch.h>
#include <TBranchElement.h>
#include <TLorentzVector.h>
#include <TStreamerInfo.h>
#include "TVirtualCollectionProxy.h"

namespace evd {

// This piece of code is borrowed from Argo written by Nathaniel Tagg
TreeElementReader::TreeElementReader(TTree * tree,
                                     const std::string & branch_name)
  : tree_(tree),
    branch_name_(branch_name),
    branch_element_(0),
    ok_(false) {

  TBranch * branch = tree_->GetBranch(branch_name_.c_str());
  if (!branch) {
    std::cerr << "Couldn't find branch " + branch_name_ << std::endl;
    return;
  }

  branch_element_ = dynamic_cast<TBranchElement *> (branch);
  //TBranchElement * branch_element_ = tree_->GetBranch(branch_name_.c_str());
  if (!branch_element_) {
    std::cerr << "Branch " + branch_name_ +
              " couldn't be cast to TBranchElement" << std::endl;
    return;
  }

  entries_ = branch_element_->GetNdata();

  proxy_ = branch_element_->GetCollectionProxy();
  if (!proxy_) {
    std::cerr << "Couldn't GetCollectionProxy() on " + branch_name_
              << std::endl;
    return;
  }

  proxy_->PushProxy(branch_element_->GetObject());

  if (!(branch_element_->GetInfo())) {
    std::cerr << "Couldn't GetInfo() in " + branch_name_ << std::endl;
    return;
  }

  element_offset_ = branch_element_->GetOffset();


#if ROOT_VERSION_CODE >= ROOT_VERSION(5,34,20)

  pointer_offset_ = element_offset_ +
                    // TODO: Figure out a way to detect which version of ROOT is
                    //       being used. The following line of code is dependent on
                    //       the version of ROOT being used. Once the version is
                    //       detected, figure out a way to choose the appropriate
                    //       line for compilation.
                    // NOTE: This line stopped working as of ROOT 5.34/20
                    // branch_element_->GetInfo()->GetOffsets()[branch_element_->GetID()];
                    // NOTE: This line should work for ROOT 5.34/20+, but does not
                    //       work for previous versions of ROOT.
                    branch_element_->GetInfo()->GetElementOffset(branch_element_->GetID());

#else

  pointer_offset_ = element_offset_ +
                    // TODO: Figure out a way to detect which version of ROOT is
                    //       being used. The following line of code is dependent on
                    //       the version of ROOT being used. Once the version is
                    //       detected, figure out a way to choose the appropriate
                    //       line for compilation.
                    // NOTE: This line stopped working as of ROOT 5.34/20
                    branch_element_->GetInfo()->GetOffsets()[branch_element_->GetID()];
                    // NOTE: This line should work for ROOT 5.34/20+, but does not
                    //       work for previous versions of ROOT.
                    // branch_element_->GetInfo()->GetElementOffset(branch_element_->GetID());
#endif

  ok_ = true;

};

TreeElementReader::~TreeElementReader() {
  if (proxy_ && ok_) proxy_->PopProxy();
}

char * TreeElementReader::get_address(unsigned int row) {
  if (!ok_) return 0;
  char * pointer = (char *) proxy_->At(row);
  char * ladd = pointer + pointer_offset_;
  return ladd;
}

unsigned int TreeElementReader::entries() {
  return entries_;
}

bool TreeElementReader::ok() {
  return ok_;
}

template const std::vector<float> *
TreeElementReader::get< std::vector<float> >(unsigned int row);
template const std::vector<short> *
TreeElementReader::get< std::vector<short> >(unsigned int row);
template const std::vector< std::pair<TLorentzVector, TLorentzVector> > *
TreeElementReader::get< std::vector< std::pair<TLorentzVector,
                  TLorentzVector> > >(unsigned int row);

} // evd