#ifndef SELECTIONTOOL_SPALGOPI0_CXX
#define SELECTIONTOOL_SPALGOPI0_CXX

#include "SPAlgoPi0.h"

namespace sptool {

  SPAlgoPi0::SPAlgoPi0() : SPAlgoBase()
  {
    _name     = "SPAlgoPi0";
    _nshowers = 1;
  }

  float SPAlgoPi0::Select(const SPAData &data)
  { return 1.0; }

  void SPAlgoPi0::Fill(const SPAData &data)
  {}

  void SPAlgoPi0::Fit(TFile* fout)
  {}

}

#endif
