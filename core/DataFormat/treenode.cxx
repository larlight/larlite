#ifndef TREENODE_CXX
#define TREENODE_CXX

#include "treenode.h"

void treenode::countNodes(int& count){

  count += 1;
  for (size_t i=0; i < _children.size(); i++)
    _children.at(i).countNodes(count);

  return;
}

std::string treenode::getTreeDiagram() const {

  std::string ret;

  ret += std::to_string(_index) + "\n";
  
  std::string prefix = "\t";
  
  getTreeDiagram(ret,prefix);

  return ret;
}

void treenode::getTreeDiagram(std::string& res, std::string prefix) const {

  for (auto& child : _children){

    res += prefix + std::to_string(child.getNodeIndex()) + "\n";
    prefix += "\t";
    child.getTreeDiagram(res,prefix);

  }// for all children
    

  return;  
}

#endif
