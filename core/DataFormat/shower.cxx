#ifndef LARLITE_SHOWER_CXX
#define LARLITE_SHOWER_CXX

#include <exception>
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
    fTotalEnergy = -1;
    fTotalEnergy_v.clear();
    fTotalEnergy_v.resize(3);
    fSigmaTotalEnergy_v.clear();
    fSigmaTotalEnergy_v.resize(3);
    fTotalMIPEnergy_v.clear();
    fTotalMIPEnergy_v.resize(3);
    fSigmaTotalMIPEnergy_v.clear();
    fSigmaTotalMIPEnergy_v.resize(3);
    fdQdx = -1;
    fSigmadQdx = -1;
    fdQdx_v.clear();
    fdQdx_v.resize(3);
    fSigmadQdx_v.clear();
    fSigmadQdx_v.resize(3);
    fdEdx = -1;
    fSigmadQdx = -1;
    fdEdx_v.clear();
    fdEdx_v.resize(3);
    fSigmadEdx_v.clear();
    fSigmadEdx_v.resize(3);
    fLength = -1;
    fWidth[0] = fWidth[1] = -1.;
    fBestPlane = -1;
  }
  
  int   shower::ID()                     const { return fID;             }
  const TVector3& shower::Direction()    const { return fDCosStart;      }
  const TVector3& shower::DirectionErr() const { return fSigmaDCosStart; }
  
  const TVector3& shower::ShowerStart()    const { return fXYZstart;      }
  const TVector3& shower::ShowerStartErr() const { return fSigmaXYZstart;  }

  const double& shower::Energy(int pl)  const {
    if ( pl == -1 ) return fTotalEnergy;
    else if ( pl <= 2  ) return fTotalEnergy_v[pl];
    else {
      std::cerr<< "shower.cxx : Plane number invalid!";
      throw std::exception();
    }
    return fTotalEnergy;
  }

  const double& shower::EnergyErr(int pl)  const {
    if ( pl == -1 ) return fSigmaTotalEnergy;
    else if ( pl <= 2  ) return fSigmaTotalEnergy_v[pl];
    else {
      std::cerr<< "shower.cxx : Plane number invalid!";
      throw std::exception();
    }
    return fSigmaTotalEnergy;
  }
  const std::vector< double >& shower::Energy_v()    const { return fTotalEnergy_v;      }
  const std::vector< double >& shower::EnergyErr_v() const { return fSigmaTotalEnergy_v; }
  const std::vector< double >& shower::MIPEnergy_v()    const { return fTotalMIPEnergy_v;      }
  const std::vector< double >& shower::MIPEnergyErr_v() const { return fSigmaTotalMIPEnergy_v; }

  int    shower::best_plane()               const { return fBestPlane;    }
  double shower::Length()                   const { return fLength;       }
  const double* shower::Width()             const { return fWidth;        }
  double shower::OpeningAngle()             const { return fOpeningAngle; }

  const  double& shower::dEdx(int pl)  const {
    if ( pl == -1 ) return fdEdx;
    else if ( pl <= 2  ) return fdEdx_v[pl];
    else {
      std::cerr<< "shower.cxx : Plane number invalid!";
      throw std::exception();
    }
    return fdEdx;
  }

  const  double& shower::dEdxErr(int pl)  const {
    if ( pl == -1 ) return fSigmadEdx;
    else if ( pl <= 2  ) return fSigmadEdx_v[pl];
    else {
      std::cerr<< "shower.cxx : Plane number invalid!";
      throw std::exception();
    }
    return fSigmadEdx;
  }
  const  std::vector< double >& shower::dEdx_v()    const { return fdEdx_v;      }
  const  std::vector< double >& shower::dEdxErr_v() const { return fSigmadEdx_v; }

  const  double& shower::dQdx(int pl)  const {
    if ( pl == -1 ) return fdQdx;
    else if ( pl <= 2  ) return fdQdx_v[pl];
    else {
      std::cerr<< "shower.cxx : Plane number invalid!";
      throw std::exception();
    }
    return fdQdx;
  }

  const  double& shower::dQdxErr(int pl)  const {
    if ( pl == -1 ) return fSigmadQdx;
    else if ( pl <= 2  ) return fSigmadQdx_v[pl];
    else {
      std::cerr<< "shower.cxx : Plane number invalid!";
      throw std::exception();
    }
    return fSigmadQdx;
  }
  const  std::vector< double >& shower::dQdx_v()    const { return fdQdx_v;      }
  const  std::vector< double >& shower::dQdxErr_v() const { return fSigmadQdx_v; }
  
}

#endif
