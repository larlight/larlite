#ifndef LARLITE_DLRAWCLUSTERASS_H
#define LARLITE_DLRAWCLUSTERASS_H

#include "Analysis/ana_base.h"

namespace larlite {

  class DLRawClusterAss : public ana_base {
  
  public:
    DLRawClusterAss()
      { _name="DLRawClusterAss"; _fout=0; _threshold=-1; _debug=false; }
    ~DLRawClusterAss(){}

    bool initialize() { return true; }
    bool analyze(storage_manager* storage);
    bool finalize() { return true; }
    
    void SetThreshold(int threshold) { _threshold = threshold; }
    void SetDebug(bool debug) { _debug = debug; }
    
  private:

    bool Equals(const larlite::hit& h1, const larlite::hit& h2);
    int _threshold;
    bool _debug;

  };
}
#endif
