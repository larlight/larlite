#ifndef SELECTIONTOOL_ERTOOL_OSTREAMBASE_CXX
#define SELECTIONTOOL_ERTOOL_OSTREAMBASE_CXX

#include "OStreamBase.h"

namespace ertool {
  namespace io {

    OStreamBase::OStreamBase(const std::string& name)
      : _name(name)
    {}
    
    const std::string& OStreamBase::Name() const
    { return _name; }
  }
}
#endif
