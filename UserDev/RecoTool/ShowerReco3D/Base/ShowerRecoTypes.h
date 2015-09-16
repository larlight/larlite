#ifndef SHOWERRECOTYPE_H
#define SHOWERRECOTYPE_H

#include <TVector3.h>
#include <vector>
#include "LArUtil/PxUtils.h"
#include "Base/GeoTypes.h"
#include "ClusterRecoUtil/Base/ClusterParams.h"

namespace showerreco {


  /// Input matched cluster set representation for shower reco algorithms
  typedef std::vector<cluster::cluster_params> ShowerClusterSet_t;

  /// Output shower representation from shower reco algorithms
  struct Shower_t {

    TVector3 fDCosStart;                        ///< direction cosines at start of shower
    TVector3 fSigmaDCosStart;                   ///< uncertainty on initial direction cosines
    TVector3 fXYZStart;                         ///< 3D start point of shower
    TVector3 fSigmaXYZStart;                    ///< uncertainty on 3D start point
    double   fLength;                           ///< 3D length of a shower
    double   fOpeningAngle;                     ///< 3D opening angle of a shower
    std::vector< double > fTotalEnergy;         ///< Calculated Energy per each plane
    std::vector< double > fSigmaTotalEnergy;    ///< Calculated Energy per each plane
    std::vector< double > fdEdx;                ///< Calculated dEdx per each plane
    std::vector< double > fShoweringLength;     ///< Calculates the distance from start to shower points [in cm]
    std::vector< double > fSigmadEdx;           ///< Calculated dEdx per each plane
    std::vector< double > fTotalMIPEnergy;      ///< Calculated Energy per each plane
    std::vector< double > fSigmaTotalMIPEnergy; ///< Calculated Energy per each plane
    ::larlite::geo::PlaneID fBestPlane;         ///< "Best" plane used for geometrical interpretation

    std::vector< ::larlite::geo::PlaneID > fPlaneIDs;    ///< List of PlaneIDs in the order aligned w.r.t. other vectors
    
    Shower_t()
    { Reset(); }

    void Reset() {
      
      fDCosStart[0] = fDCosStart[1] = fDCosStart[2] = -1;
      fSigmaDCosStart[0] = fSigmaDCosStart[1] = fSigmaDCosStart[2] = -1;

      fXYZStart[0] = fXYZStart[1] = fXYZStart[2] = -1;
      fSigmaXYZStart[0] = fSigmaXYZStart[1] = fSigmaXYZStart[2] = -1;

      fLength = -1;
      fOpeningAngle = 0;
      
      fTotalEnergy.clear();
      fSigmaTotalEnergy.clear();
      fdEdx.clear();
      fShoweringLength.clear();
      fSigmadEdx.clear();
      fTotalMIPEnergy.clear();
      fSigmaTotalMIPEnergy.clear();

      fBestPlane = ::larlite::geo::PlaneID();
      fPlaneIDs.clear();      
    }
  };
  
}
#endif
