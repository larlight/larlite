#ifndef LARLITE_INTERWRITEOUTLL_H
#define LARLITE_INTERWRITEOUTLL_H

#include "Analysis/ana_base.h"

namespace larlite {

  class InterWriteOutLL : public ana_base {
  
  public:

    InterWriteOutLL();
    ~InterWriteOutLL(){}

    bool initialize() { return true; }
    bool analyze(storage_manager* storage);
    bool finalize() { return true; }

  };
}
#endif

