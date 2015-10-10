#ifndef TREEELEMENTREADER_H_
#define TREEELEMENTREADER_H_

#include <string>
#include <Rtypes.h>

#include "TTree.h"
#include "TBranchElement.h"
#include "TVirtualCollectionProxy.h"

// This code is borrowed from Johnny Ho who borrowed it from Nathaniel Tagg

// class TTree;
// class TBranchElement;
// class TVirtualCollectionProxy;
namespace evd {

class TreeElementReader {
public:

  TreeElementReader(TTree * tree, const std::string & branch_name);
  ~TreeElementReader();

  unsigned int entries();
  bool ok();

  template <typename T> const T * get(unsigned int row) {
    return (const T *) get_address(row);
  }

private:

  char * get_address(unsigned int row);

  TTree * tree_;
  std::string branch_name_;
  TBranchElement * branch_element_;
  TVirtualCollectionProxy * proxy_;
  int element_offset_;
  int pointer_offset_;
  unsigned int entries_;
  bool ok_;

};

} // evd

#endif  // TREEELEMENTREADER_H_