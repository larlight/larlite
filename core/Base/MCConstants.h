/**
 * \file MCConstants.h
 *
 * \ingroup Base
 * 
 * \brief defines constants used for MC
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Base

    @{*/

#ifndef LARLITE_MCCONSTANTS_H
#define LARLITE_MCCONSTANTS_H
#include <string>

/// Namespace of everything in this framework
namespace larlite{


  namespace simb{
    
    enum flux_code_{
      kHistPlusFocus  = +1, ///< Flux for positive horn focus
      kHistMinusFocus = -1, ///< Flux for negative horn focus
      kGenerator      =  0, ///< A bogus flux assumed by the generator
      kNtuple         =  2, ///< Full flux simulation ntuple
      kSimple_Flux    =  3  ///< A simplified flux ntuple for quick running
    }; 
    
    /// MC generator type
    enum Origin_t{
      kUnknown,           ///< ???
      kBeamNeutrino,      ///< Beam neutrinos
      kCosmicRay,         ///< Cosmic rays
      kSuperNovaNeutrino, ///< Supernova neutrinos
      kSingleParticle     ///< single particles thrown at the detector
    };
    
    /// Neutrino interaction categories
    enum curr_type_{
      kCC,
      kNC
    };
    
    /// Neutrino interaction categories
    enum int_type_{
      kQE                        = 0,
      kRes                       = 1,
      kDIS                       = 2,
      kCoh                       = 3,
      kNuanceOffset              = 1000,                ///< offset to account for adding in Nuance codes to this enum 
      kCCQE                      = kNuanceOffset + 1,   ///< charged current quasi-elastic
      kNCQE                      = kNuanceOffset + 2,   ///< neutral current quasi-elastic
      kResCCNuProtonPiPlus       = kNuanceOffset + 3,   ///< resonant charged current, nu p -> l- p pi+
      kResCCNuNeutronPi0         = kNuanceOffset + 4,   ///< resonant charged current, nu n -> l- n pi0
      kResCCNuNeutronPiPlus      = kNuanceOffset + 5,   ///< resonant charged current, nu n -> l- n pi+
      kResNCNuProtonPi0          = kNuanceOffset + 6,   ///< resonant neutral current, nu p -> nu p pi0
      kResNCNuProtonPiPlus       = kNuanceOffset + 7,   ///< resonant neutral current, nu p -> nu p pi+
      kResNCNuNeutronPi0         = kNuanceOffset + 8,   ///< resonant neutral current, nu n -> nu n pi0
      kResNCNuNeutronPiMinus     = kNuanceOffset + 9,   ///< resonant neutral current, nu n -> nu p pi-
      kResCCNuBarNeutronPiMinus  = kNuanceOffset + 10,  ///< resonant charged current, nubar n -> l+ n pi-
      kResCCNuBarProtonPi0       = kNuanceOffset + 11,  ///< resonant charged current, nubar p -> l+ n pi0
      kResCCNuBarProtonPiMinus   = kNuanceOffset + 12,  ///< resonant charged current, nubar p -> l+ p pi-
      kResNCNuBarProtonPi0       = kNuanceOffset + 13,  ///< resonant charged current, nubar p -> nubar p pi0
      kResNCNuBarProtonPiPlus    = kNuanceOffset + 14,  ///< resonant charged current, nubar p -> nubar n pi+
      kResNCNuBarNeutronPi0      = kNuanceOffset + 15,  ///< resonant charged current, nubar n -> nubar n pi0
      kResNCNuBarNeutronPiMinus  = kNuanceOffset + 16,  ///< resonant charged current, nubar n -> nubar p pi-
      kResCCNuDeltaPlusPiPlus    = kNuanceOffset + 17,
      kResCCNuDelta2PlusPiMinus  = kNuanceOffset + 21,
      kResCCNuBarDelta0PiMinus   = kNuanceOffset + 28,
      kResCCNuBarDeltaMinusPiPlus= kNuanceOffset + 32,
      kResCCNuProtonRhoPlus      = kNuanceOffset + 39,
      kResCCNuNeutronRhoPlus     = kNuanceOffset + 41,
      kResCCNuBarNeutronRhoMinus = kNuanceOffset + 46,
      kResCCNuBarNeutronRho0     = kNuanceOffset + 48,
      kResCCNuSigmaPlusKaonPlus  = kNuanceOffset + 53,
      kResCCNuSigmaPlusKaon0     = kNuanceOffset + 55,
      kResCCNuBarSigmaMinusKaon0 = kNuanceOffset + 60,
      kResCCNuBarSigma0Kaon0     = kNuanceOffset + 62,
      kResCCNuProtonEta          = kNuanceOffset + 67,
      kResCCNuBarNeutronEta      = kNuanceOffset + 70,
      kResCCNuKaonPlusLambda0    = kNuanceOffset + 73,
      kResCCNuBarKaon0Lambda0    = kNuanceOffset + 76,
      kResCCNuProtonPiPlusPiMinus= kNuanceOffset + 79,
      kResCCNuProtonPi0Pi0       = kNuanceOffset + 80,
      kResCCNuBarNeutronPiPlusPiMinus = kNuanceOffset + 85,
      kResCCNuBarNeutronPi0Pi0   = kNuanceOffset + 86,
      kResCCNuBarProtonPi0Pi0    = kNuanceOffset + 90,
      kCCDIS                     = kNuanceOffset + 91,  ///< charged current deep inelastic scatter
      kNCDIS                     = kNuanceOffset + 92,  ///< charged current deep inelastic scatter
      kUnUsed1                   = kNuanceOffset + 93,
      kUnUsed2                   = kNuanceOffset + 94,
      kCCQEHyperon               = kNuanceOffset + 95,
      kNCCOH                     = kNuanceOffset + 96,
      kCCCOH                     = kNuanceOffset + 97,  ///< charged current coherent pion
      kNuElectronElastic         = kNuanceOffset + 98,  ///< neutrino electron elastic scatter
      kInverseMuDecay            = kNuanceOffset + 99   ///< inverse muon decay
    };
  
  }
}
#endif
/** @} */ // end of doxygen group
