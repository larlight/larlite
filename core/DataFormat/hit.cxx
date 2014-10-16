#ifndef LARLITE_HIT_CXX
#define LARLITE_HIT_CXX

#include "hit.h"

namespace larlite {

  //#################################################
  hit::hit() : data_base(data::kHit), 
	       fHitSignal() 
  //#################################################
  {
    clear_data();
  }

  //##########################################################################
  void hit::clear_data()
  //##########################################################################
  {
    data_base::clear_data();
    fHitSignal.clear();
    fStartTime = fPeakTime = fEndTime = -1;
    fSigmaStartTime = fSigmaPeakTime = fSigmaEndTime = -1;
    fCharge = fMaxCharge = -1;
    fSigmaCharge = fSigmaMaxCharge = -1;
    fMultiplicity = -1;
    fGoodnessOfFit = -1;
    fView = geo::kUnknown;
    fSignalType = geo::kMysteryType;
    fChannel = 0xffffffff;
    fWire    = 0xffffffff;
  }

}
#endif
  
