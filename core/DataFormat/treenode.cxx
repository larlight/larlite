#ifndef TREENODE_CXX
#define TREENODE_CXX

#include "treenode.h"

void treenode::countNodes(int& count){

  count += 1;
  for (size_t i=0; i < _children.size(); i++)
    _children.at(i).countNodes(count);

  return;
}

#endif
