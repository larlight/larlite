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
	 int CCNC,
	 int mode,
	 int interactionType,
	 int target,
	 int nucleon,
	 int quark,
	 double w,
	 double x,
	 double y,
	 double qsqr);
    
    /// Copy constructor
    mcnu(const mcnu& origin) : data_base(origin),
			       fNu(origin.fNu),
			       fLepton(origin.fLepton),
			       fMode(origin.fMode),
			       fInteractionType(origin.fInteractionType),
			       fCCNC(origin.fCCNC),
			       fTarget(origin.fTarget),
			       fHitNuc(origin.fHitNuc),
			       fHitQuark(origin.fHitQuark),
			       fW(origin.fW),
			       fX(origin.fX),
			       fY(origin.fY),
			       fQSqr(origin.fQSqr)
    {}

    /// Default destructor
    virtual ~mcnu(){}

    /// Clear method
    virtual void clear_data();

    ///--- Getters ---///
    inline const  mcpart&  Nu()              const { return fNu;              }
    inline const  mcpart&  Lepton()          const { return fLepton;          }
    inline        int    CCNC()            const { return fCCNC;            }
    inline        int    Mode()            const { return fMode;            }
    inline        int    InteractionType() const { return fInteractionType; }
    inline        int    Target()          const { return fTarget;          }
    inline        int    HitNuc()          const { return fHitNuc;          }
    inline        int    HitQuark()        const { return fHitQuark;        }
    inline        double W()               const { return fW;               }
    inline        double X()               const { return fX;               }
    inline        double Y()               const { return fY;               }
    inline        double QSqr()            const { return fQSqr;            }

  private:

    mcpart   fNu;              ///< the incoming neutrino
    mcpart   fLepton;          ///< the outgoing lepton
    int    fMode;            ///< Interaction mode (QE/1-pi/DIS...) see enum list
    int    fInteractionType; ///< More detailed interaction type, see enum list below kNuanceOffset
    int    fCCNC;            ///< CC or NC interaction? see enum list
    int    fTarget;          ///< Nuclear target, as PDG code
    int    fHitNuc;          ///< Hit nucleon (2212 (proton) or 2112 (neutron))
    int    fHitQuark;        ///< For DIS events only, as PDG code
    double fW;               ///< Hadronic invariant mass, in GeV
    double fX;               ///< Bjorken x=Q^2/(2M*(E_neutrino-E_lepton)), unitless
    double fY;               ///< Inelasticity y=1-(E_lepton/E_neutrino), unitless
    double fQSqr;            ///< Momentum transfer Q^2, in GeV^2
    
    ////////////////////////
    ClassDef(mcnu,1)
    ////////////////////////
      
  };
  
}
#endif

/** @} */ // end of doxygen group 
