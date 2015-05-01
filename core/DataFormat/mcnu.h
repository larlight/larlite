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
    mcnu(const mcnu& origin);

    /// Default destructor
    virtual ~mcnu(){}

    /// Clear method
    virtual void clear_data();

    ///--- Getters ---///

    const  mcpart&  Nu()     const;
    const  mcpart&  Lepton() const;
    int    CCNC()            const;
    int    Mode()            const;
    int    InteractionType() const;
    int    Target()          const;
    int    HitNuc()          const;
    int    HitQuark()        const;
    double W()               const;
    double X()               const;
    double Y()               const;
    double QSqr()            const;

  private:

    mcpart   fNu;              ///< the incoming neutrino
    mcpart   fLepton;          ///< the outgoing lepton
    int    fMode;            ///< Interaction mode (QE/1-pi/DIS...) see enum list
    int    fInteractionType; ///< More detailed interaction type, see enum list below kNuanceOffset
    int    fCCNC;            ///< CC or NC interaction? 0 means CC, 1 means NC
    int    fTarget;          ///< Nuclear target, as PDG code
    int    fHitNuc;          ///< Hit nucleon (2212 (proton) or 2112 (neutron))
    int    fHitQuark;        ///< For DIS events only, as PDG code
    double fW;               ///< Hadronic invariant mass, in GeV
    double fX;               ///< Bjorken x=Q^2/(2M*(E_neutrino-E_lepton)), unitless
    double fY;               ///< Inelasticity y=1-(E_lepton/E_neutrino), unitless
    double fQSqr;            ///< Momentum transfer Q^2, in GeV^2

  };
  
}
#endif

/** @} */ // end of doxygen group 
