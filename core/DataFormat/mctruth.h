/**
 * \file mctruth.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for mctruth data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat
    
@{*/

#ifndef LARLITE_MCTRUTH_H
#define LARLITE_MCTRUTH_H

#include "mcnu.h"
#include "Base/MCConstants.h"
namespace larlite {
  /**
     \class mctruth
     Simplified version of MCParticle data container
  */
  class mctruth : public data_base {
    
  public:

    /// Default constructor
    mctruth() : data_base(data::kMCTruth) {clear_data();}

    mctruth(const mctruth& origin);

    /// Default destructor
    virtual ~mctruth(){}

    /// Clear method
    virtual void clear_data();

    ///--- Getters ---///
    size_t                               NParticles()          const;
    const larlite::mcpart&               GetParticle(size_t i) const;
    const std::vector<larlite::mcpart>&  GetParticles() const;
    const mcnu&                          GetNeutrino()  const;
    simb::Origin_t                       Origin()       const;
    Bool_t                               NeutrinoSet()  const;

    ///--- Setters/Adders ---///
    void Add(mcpart part)              { fPartList.push_back(part); }
    void SetOrigin( simb::Origin_t orig) { fOrigin = orig;            }
    void SetNeutrino(Int_t CCNC,
		     Int_t mode,
		     Int_t interactionType,
		     Int_t target,
		     Int_t nucleon,
		     Int_t quark,
		     Double_t w,
		     Double_t x,
		     Double_t y,
		     Double_t qsqr);

  private:
    
    std::vector<larlite::mcpart>  fPartList;    ///< list of particles in this event
    mcnu                          fMCNeutrino;  ///< reference to neutrino info - null if not a neutrino
    simb::Origin_t                fOrigin;      ///< origin for this event
    Bool_t                        fNeutrinoSet; ///< flag for whether the neutrino information has been set

  };
  
  /**
     \class event_mctruth
     LArSoft MCTruth class equivalent data container
  */
  class event_mctruth : public std::vector<larlite::mctruth>,
			public event_base {
    
  public:
    
    /// Default constructor
    event_mctruth(std::string name="noname") : event_base(data::kMCTruth,name) {clear_data();}
    
    /// Copy constructor
    event_mctruth(const event_mctruth& original) : std::vector<larlite::mctruth>(original),
						   event_base   (original)
    {}
    
    /// Default destructor
    virtual ~event_mctruth(){}
    
    /// Clear method
    virtual void clear_data() {event_base::clear_data(); clear();}
    
  private:
    
  };
}
#endif

/** @} */ // end of doxygen group 
