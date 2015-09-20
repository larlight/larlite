#ifndef SHOWERRECOTYPE_H
#define SHOWERRECOTYPE_H

#include <TVector3.h>
#include <vector>
#include "LArUtil/PxUtils.h"
#include "Base/GeoTypes.h"
#include "ClusterRecoUtil/Base/ClusterParams.h"

namespace showerreco {


  /// Utility: maximum value for double 
  const double kDOUBLE_MAX = std::numeric_limits<double>::max();
  
  /// Utility: minimum value for double
  const double kDOUBLE_MIN = std::numeric_limits<double>::min();

  /// Utility: maximum value for int
  const int    kINT_MAX    = std::numeric_limits<int>::max();
  
  /// Utility: maximum value for unsigned int
  const unsigned int kUINT_MAX    = std::numeric_limits<unsigned int>::max();

  /// Utility: maximum value for size_t
  const size_t kSIZE_MAX   = std::numeric_limits<size_t>::max();


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
    std::vector< double > fdQdx;                ///< Calculated dQdx per each plane [ADC/cm]
    size_t fBestdQdxPlane;                      ///< Best plane for dQdx calculation
    std::vector< std::vector < double > > fHitdQdx_v;      ///< Hit-by-hit dQdx per each plane [ADC/cm]
    double fBestdQdx;                           ///< Selects dQdx with the longest ShoweringLength [ADC/cm]
    std::vector< double > fShoweringLength;     ///< Calculates the distance from start to shower points [in cm]
    std::vector< double > fSigmadEdx;           ///< Calculated dEdx per each plane
    std::vector< double > fTotalMIPEnergy;      ///< Calculated Energy per each plane
    std::vector< double > fSigmaTotalMIPEnergy; ///< Calculated Energy per each plane
    ::larlite::geo::PlaneID fBestPlane;         ///< "Best" plane used for geometrical interpretation

    std::vector< ::larlite::geo::PlaneID > fPlaneIDs;    ///< List of PlaneIDs in the order aligned w.r.t. other vectors
    
    std::vector< bool > fPlaneIsBad;            ///< Matches number of planes, gets flagged if a plane is bad


    Shower_t()
    { Reset(); }

    void Reset() {
      
      fDCosStart[0] = fDCosStart[1] = fDCosStart[2] = kDOUBLE_MIN;
      fSigmaDCosStart[0] = fSigmaDCosStart[1] = fSigmaDCosStart[2] = kDOUBLE_MIN;

      fXYZStart[0] = fXYZStart[1] = fXYZStart[2] = kDOUBLE_MIN;
      fSigmaXYZStart[0] = fSigmaXYZStart[1] = fSigmaXYZStart[2] = kDOUBLE_MIN;

      fLength = kDOUBLE_MIN;
      fOpeningAngle = 0;
      
      fTotalEnergy.clear();
      fTotalEnergy = {kDOUBLE_MIN,kDOUBLE_MIN,kDOUBLE_MIN};
      fSigmaTotalEnergy.clear();
      fdEdx.clear();
      fdEdx = {kDOUBLE_MIN,kDOUBLE_MIN,kDOUBLE_MIN};
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
