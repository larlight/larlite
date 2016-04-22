#ifndef SHOWERANABASE_CXX
#define SHOWERANABASE_CXX

#include "ShowerAnaBase.h"

namespace showerreco {

ShowerAnaBase::ShowerAnaBase() : _tree(nullptr)
{
}

TTree* ShowerAnaBase::GetTree()
{
  return _tree;
}

}
#endif
