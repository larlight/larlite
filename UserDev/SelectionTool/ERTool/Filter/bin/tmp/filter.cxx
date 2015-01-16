#ifndef SELECTIONTOOL_FILTER_CLASS_NAME_CXX
#define SELECTIONTOOL_FILTER_CLASS_NAME_CXX

#include "Filter_Class_Name.h"

namespace sptool {

  Filter_Class_Name::Filter_Class_Name() : SPAFilterBase()
  {
    _name = "Filter_Class_Name";
  }

  SPAOrder Filter_Class_Name::Select(const SPAData &data)
  { return SPAOrder(); }

}

#endif
