#ifndef LARLITE_SHOWER_CXX
#define LARLITE_SHOWER_CXX

#include "shower.h"

namespace larlite {

  //*******************************************
  void shower::clear_data()
  //******************************************* 
  {
    data_base::clear_data();

    fID = -1;
    fDCosStart[0] = fDCosStart[1] = fDCosStart[2] = -1;
    fSigmaDCosStart[0] = fSigmaDCosStart[1] = fSigmaDCosStart[2] = -1;
    fTotalEnergy.clear();           ///< Calculated Energy per each plane
    fSigmaTotalEnergy.clear();           ///< Calculated Energy per each plane
    fdEdx.clear();           ///< Calculated dEdx per each plane
    fSigmadEdx.clear();           ///< Calculated dEdx per each plane
    fLength = -1;
    fBestPlane = -1;
 //   fMaxWidthX = fMaxWidthY = -1;
 //   fDistanceMaxWidth=-1;
 //   fTotalCharge=-1;
  }

}

#endif
