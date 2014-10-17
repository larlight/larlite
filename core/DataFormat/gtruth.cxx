#ifndef LARLITE_GTRUTH_CXX
#define LARLITE_GTRUTH_CXX

#include "gtruth.h"

namespace larlite {

  const double gtruth::kUndefinedValue = -99999;

  //#################################################
  gtruth::gtruth() 
  : data_base(data::kGTruth)
  , fGint(-1)
  , fGscatter(-1)

  , fweight(0)
  , fprobability(0)
  , fXsec(0)
  , fDiffXsec(0)

  , fNumPiPlus(-1)
  , fNumPiMinus(-1)
  , fNumPi0(-1)
  , fNumProton(-1)
  , fNumNeutron(-1)
  , fIsCharm(false)
  , fResNum(-1)

  , fgQ2(kUndefinedValue)
  , fgq2(kUndefinedValue)
  , fgW(kUndefinedValue)
  , fgT(kUndefinedValue)
  , fgX(kUndefinedValue)
  , fgY(kUndefinedValue)

  , fIsSeaQuark(false)
  , ftgtZ(0)
  , ftgtA(0)
  , ftgtPDG(0)

  , fProbePDG(-1)
  {
    fFShadSystP4.SetXYZT(0, 0, 0, 0);
    fHitNucP4.SetXYZT(0, 0, 0, 0);
    fProbeP4.SetXYZT(0, 0, 0, 0);
    fVertex.SetXYZT(0, 0, 0, 0);
  }

}
#endif
  
