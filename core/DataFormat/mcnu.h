/**
 * \file mcnu.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for mcnu data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat
    
@{*/

#ifndef LARLITE_MCNU_H
#define LARLITE_MCNU_H

#include "mcpart.h"

namespace larlite {
  /**
     \class mcnu
     Simplified version of MCParticle data container
  */
  class mcnu : public data_base {
    
  public:

    /// Default constructor
    mcnu() : data_base(data::kMCNeutrino) {clear_data();}

    /// Alternative constructor
    mcnu(mcpart &nu,
	 mcpart &lep,
	 Int_t CCNC,
	 Int_t mode,
	 Int_t interactionType,
	 Int_t target,
	 Int_t nucleon,
	 Int_t quark,
	 Double_t w,
	 Double_t x,
	 Double_t y,
	 Double_t qsqr);
    
    /// Default destructor
    virtual ~mcnu(){}

    /// Clear method
    virtual void clear_data();

    ///--- Getters ---///
    inline const  mcpart&  Nu()              const { return fNu;              }
    inline const  mcpart&  Lepton()          const { return fLepton;          }
    inline        Int_t    CCNC()            const { return fCCNC;            }
    inline        Int_t    Mode()            const { return fMode;            }
    inline        Int_t    InteractionType() const { return fInteractionType; }
    inline        Int_t    Target()          const { return fTarget;          }
    inline        Int_t    HitNuc()          const { return fHitNuc;          }
    inline        Int_t    HitQuark()        const { return fHitQuark;        }
    inline        Double_t W()               const { return fW;               }
    inline        Double_t X()               const { return fX;               }
    inline        Double_t Y()               const { return fY;               }
    inline        Double_t QSqr()            const { return fQSqr;            }

  private:

    mcpart   fNu;              ///< the incoming neutrino
    mcpart   fLepton;          ///< the outgoing lepton
    Int_t    fMode;            ///< Interaction mode (QE/1-pi/DIS...) see enum list
    Int_t    fInteractionType; ///< More detailed interaction type, see enum list below kNuanceOffset
    Int_t    fCCNC;            ///< CC or NC interaction? see enum list
    Int_t    fTarget;          ///< Nuclear target, as PDG code
    Int_t    fHitNuc;          ///< Hit nucleon (2212 (proton) or 2112 (neutron))
    Int_t    fHitQuark;        ///< For DIS events only, as PDG code
    Double_t fW;               ///< Hadronic invariant mass, in GeV
    Double_t fX;               ///< Bjorken x=Q^2/(2M*(E_neutrino-E_lepton)), unitless
    Double_t fY;               ///< Inelasticity y=1-(E_lepton/E_neutrino), unitless
    Double_t fQSqr;            ///< Momentum transfer Q^2, in GeV^2
    
    ////////////////////////
    ClassDef(mcnu,1)
    ////////////////////////
      
  };
  
}
#endif

/** @} */ // end of doxygen group 
