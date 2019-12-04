
/**
 * \file LArProperties.h
 *
 * \ingroup LArUtil
 * 
 * \brief Class def header for a class LArProperties
 *
 * @author kazuhiro
 */

/** \addtogroup LArUtil

    @{*/
#ifndef LARLITE_LARPROPERTIES_H
#define LARLITE_LARPROPERTIES_H

#include "LArUtilBase.h"
#include "LArUtilException.h"
#include "LArUtilConstants.h"
#include <algorithm>
#include <cmath>

namespace larutil {
  /**
     \class LArProperties

  */
  class LArProperties : public LArUtilBase{

  private:

    /// Singleton constructor
    LArProperties(bool default_load=true);

    /// Default destructor
    virtual ~LArProperties(){};

    static LArProperties* _me;

  protected:

    virtual void ClearData();
    virtual bool ReadTree();
    
  public:

    /// Singleton getter
    static const LArProperties* GetME(bool default_load=true)
    {
      if(!_me) _me = new LArProperties(default_load);
      return _me;
    }
    
    //--- LArSoft Implementation ---//

    Double_t Density(Double_t temperature=0.) const;                          ///< g/cm^3
    Double_t DriftVelocity(Double_t efield=0., Double_t temperature=0.) const;  ///< cm/us
    
    /// dQ/dX in electrons/cm, returns dE/dX in MeV/cm.
    Double_t BirksCorrection(Double_t dQdX) const;
    Double_t BirksInverse(Double_t dEdX) const;
    Double_t ModBoxCorrection(Double_t dQdX) const;
    Double_t ModBoxInverse(Double_t dEdX) const;
    
    Double_t Efield(UInt_t planegap=0) const; ///< kV/cm

    Double_t Temperature()             const  ///< kelvin
    {return fTemperature;}

    Double_t Vd()                      const  ///< cm/us
    {return fVd;}
    
    Double_t ElectronLifetime()        const  ///< microseconds
    {return fElectronlifetime;}
    
    Double_t RadiationLength()         const  ///< g/cm^2
    { return fRadiationLength; }
    
    Double_t Argon39DecayRate()        const  ///< decays per cm^3 per second
    { return fArgon39DecayRate; }
    
    /// Restricted mean dE/dx energy loss (MeV/cm).
    Double_t Eloss(Double_t mom, Double_t mass, Double_t tcut) const;
    
    /// Energy loss fluctuation (sigma_E^2 / length in MeV^2/cm).
    Double_t ElossVar(Double_t mom, Double_t mass) const;
    
    Double_t ScintResolutionScale() const { return fScintResolutionScale; }
    Double_t ScintFastTimeConst()   const { return fScintFastTimeConst;   }
    Double_t ScintSlowTimeConst()   const { return fScintSlowTimeConst;   }
    Double_t ScintBirksConstant()   const { return fScintBirksConstant;   }
    
    bool ScintByParticleType()      const { return fScintByParticleType;  }
    
    Double_t ScintYield()           const { return fScintYield;           }
    Double_t ScintYieldRatio()      const { return fScintYieldRatio;      }

    Double_t ProtonScintYield()        const { return fProtonScintYield;         }
    Double_t ProtonScintYieldRatio()   const { return fProtonScintYieldRatio;    }
    Double_t MuonScintYield()          const { return fMuonScintYield;           }
    Double_t MuonScintYieldRatio()     const { return fMuonScintYieldRatio;      }
    Double_t KaonScintYield()          const { return fKaonScintYield;           }
    Double_t KaonScintYieldRatio()     const { return fKaonScintYieldRatio;      }
    Double_t PionScintYield()          const { return fPionScintYield;           }
    Double_t PionScintYieldRatio()     const { return fPionScintYieldRatio;      }
    Double_t ElectronScintYield()      const { return fElectronScintYield;       }
    Double_t ElectronScintYieldRatio() const { return fElectronScintYieldRatio;  }
    Double_t AlphaScintYield()         const { return fAlphaScintYield;          }
    Double_t AlphaScintYieldRatio()    const { return fAlphaScintYieldRatio;     }
    
    bool CerenkovLightEnabled()        const { return fEnableCerenkovLight;      }
    
    std::map<Double_t, Double_t>  SlowScintSpectrum() const;
    std::map<Double_t, Double_t>  FastScintSpectrum() const;
    std::map<Double_t, Double_t>  RIndexSpectrum() const;
    std::map<Double_t, Double_t>  AbsLengthSpectrum() const;
    std::map<Double_t, Double_t>  RayleighSpectrum() const;
    
    std::map<std::string, std::map<Double_t, Double_t> >  SurfaceReflectances() const;
    std::map<std::string, std::map<Double_t, Double_t> >  SurfaceReflectanceDiffuseFractions() const;

  private:
    
    std::vector< Double_t >          fEfield;           ///< kV/cm 
    Double_t                         fTemperature;      ///< kelvin
    Double_t                         fVd;               ///< cm/us
    Double_t                         fElectronlifetime; ///< microseconds
    Double_t                         fRadiationLength;  ///< g/cm^2
    
    Double_t                         fArgon39DecayRate; ///<  decays per cm^3 per second
    
    // Following parameters are for use in Bethe-Bloch formula for dE/dx.
    Double_t fZ;                ///< Ar atomic number
    Double_t fA;                ///< Ar atomic mass (g/mol)
    Double_t fI;                ///< Ar mean excitation energy (eV)
    Double_t fSa;               ///< Sternheimer parameter a
    Double_t fSk;               ///< Sternheimer parameter k
    Double_t fSx0;              ///< Sternheimer parameter x0
    Double_t fSx1;              ///< Sternheimer parameter x1
    Double_t fScbar;            ///< Sternheimer parameter Cbar
    
    
    // Optical parameters for LAr 
    std::vector<Double_t> fFastScintSpectrum;
    std::vector<Double_t> fFastScintEnergies;
    std::vector<Double_t> fSlowScintSpectrum;
    std::vector<Double_t> fSlowScintEnergies;
    std::vector<Double_t> fRIndexSpectrum;
    std::vector<Double_t> fRIndexEnergies;
    std::vector<Double_t> fAbsLengthSpectrum;
    std::vector<Double_t> fAbsLengthEnergies;
    std::vector<Double_t> fRayleighSpectrum;
    std::vector<Double_t> fRayleighEnergies;
    
    bool fScintByParticleType;
    
    Double_t fProtonScintYield;
    Double_t fProtonScintYieldRatio;
    Double_t fMuonScintYield;
    Double_t fMuonScintYieldRatio;
    Double_t fPionScintYield;
    Double_t fPionScintYieldRatio;
    Double_t fKaonScintYield;
    Double_t fKaonScintYieldRatio;
    Double_t fElectronScintYield;
    Double_t fElectronScintYieldRatio;
    Double_t fAlphaScintYield;
    Double_t fAlphaScintYieldRatio;
    
    Double_t fScintYield;
    Double_t fScintResolutionScale;
    Double_t fScintFastTimeConst;
    Double_t fScintSlowTimeConst;
    Double_t fScintYieldRatio;
    Double_t fScintBirksConstant;
    
    bool fEnableCerenkovLight;
    
    std::vector<std::string>            fReflectiveSurfaceNames;
    std::vector<Double_t>               fReflectiveSurfaceEnergies;
    std::vector<std::vector<Double_t> > fReflectiveSurfaceReflectances;
    std::vector<std::vector<Double_t> > fReflectiveSurfaceDiffuseFractions;
    
  };
}
#endif
/** @} */ // end of doxygen group 

