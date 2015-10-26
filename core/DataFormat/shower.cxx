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
    fdQdx.clear();
    fSigmadEdx.clear();           ///< Calculated dEdx per each plane
    fLength = -1;
    fBestPlane = -1;
 //   fMaxWidthX = fMaxWidthY = -1;
 //   fDistanceMaxWidth=-1;
 //   fTotalCharge=-1;
  }
  
  int   shower::ID()                     const { return fID;               }
  const TVector3& shower::Direction()    const { return fDCosStart;          }
  const TVector3& shower::DirectionErr() const { return fSigmaDCosStart;     }
  
  const TVector3& shower::ShowerStart()    const { return fXYZstart;          }
  const TVector3& shower::ShowerStartErr() const { return fSigmaXYZstart;     }
  
  const std::vector< double >& shower::Energy()    const { return fTotalEnergy;          }
  const std::vector< double >& shower::EnergyErr() const { return fSigmaTotalEnergy;     }
  
  const std::vector< double >& shower::MIPEnergy()    const { return fTotalMIPEnergy;          }
  const std::vector< double >& shower::MIPEnergyErr() const { return fSigmaTotalMIPEnergy;     }
  int    shower::best_plane()               const { return fBestPlane;               }
  double shower::Length() const { return fLength; }
  double shower::OpeningAngle() const { return fOpeningAngle; }

  const  std::vector< double >& shower::dEdx()    const { return fdEdx;          }
  const  std::vector< double >& shower::dEdxErr() const { return fSigmadEdx;     }
  const  std::vector< double >& shower::dQdx()    const { return fdQdx;          }
  
}

#endif
