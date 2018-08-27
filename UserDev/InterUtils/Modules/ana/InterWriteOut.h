#ifndef LARLITE_INTERWRITEOUT_H
#define LARLITE_INTERWRITEOUT_H

#include "Analysis/ana_base.h"

namespace larlite {

  class InterWriteOut : public ana_base {
  
  public:

    InterWriteOut();
    ~InterWriteOut(){}

    bool initialize() { return true; }
    bool analyze(storage_manager* storage);
    bool finalize() { return true; }

  };
}
#endif

