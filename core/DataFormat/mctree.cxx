#ifndef LARLITE_MCTREE_CXX
#define LARLITE_MCTREE_CXX

#include "mctree.h"

namespace larlite {

  //********************************************
  mctree::mctree(int trackId)
    : data_base(data::kMCTree), treenode::treenode(trackId)
  //********************************************
  {
    clear_data();
  }
    
  //***********************
  void mctree::clear_data() 
  //***********************
  {}

}

#endif
