/**
 * \file gtruth.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for gtruth data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef LARLITE_GTRUTH_H
#define LARLITE_GTRUTH_H

#include "data_base.h"
#include <vector>
#include <TLorentzVector.h>

namespace larlite{
  /**
     \class gtruth
     LArSoft (nutools) GTruth class equivalent data container.
     This class stores/retrieves the additional information needed
     (and not in MCTruth) to recreate a genie::EventRecord
     for genie based event reweighting. 
  */
  class gtruth : public data_base {
    
  public:
    
    /// Default constructor
    gtruth();
    
    /// Default destructor
    virtual ~gtruth(){}

    void clear_data() { data_base::clear_data(); }

    //interactions info
    int            fGint;         ///< interaction code
    int            fGscatter;     ///< neutrino scattering code 

    //Event info                                                                                                                                  
    double         fweight;       ///< event interaction weight (genie internal)
    double         fprobability;  ///< interaction probability
    double         fXsec;         ///< cross section of interaction
    double         fDiffXsec;     ///< differential cross section of interaction

    //genie::XclsTag info
    int            fNumPiPlus;    ///< number of pi pluses in the final state
    int            fNumPiMinus;   ///< number of pi minuses in the final state
    int            fNumPi0;       ///< number of pi0 in the final state 
    int            fNumProton;    ///< number of protons in the final state
    int            fNumNeutron;   ///< number of neutrons in the final state
    bool           fIsCharm;      ///< did the interaction produce a charmed hadron
    int            fResNum;       ///< resonance number 

    //kinematics info 
    ///<these are for the interal (on shell) genie kinematics
    double         fgQ2;
    double         fgq2;
    double         fgW;
    double         fgT;
    double         fgX;
    double         fgY;
    TLorentzVector fFShadSystP4;

    //Target info
    bool           fIsSeaQuark;
    TLorentzVector fHitNucP4;
    int            ftgtZ;
    int            ftgtA;
    int            ftgtPDG;       ///< Target Nucleous(?) PDG

    //Initial State info 
    int            fProbePDG;
    TLorentzVector fProbeP4;
    TLorentzVector fVertex;

    //Flag for values that might not have been set 
    static const double kUndefinedValue;
 
  private:
      
  };
  
  /**
     \class event_gtruth
     A collection storage class of multiple gtruth.
  */
  class event_gtruth : public std::vector<larlite::gtruth>, 
		       public event_base {
    
  public:
    
    /// Default constructor
    event_gtruth(std::string name="noname") : event_base(data::kGTruth,name) { clear_data(); }
    
    /// Default copy constructor
    event_gtruth(const event_gtruth& original) : std::vector<larlite::gtruth>(original), event_base(original)
    {}
    
    /// Default destructor
    ~event_gtruth(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}

  private:
    
  };
}
#endif

/** @} */ // end of doxygen group 
