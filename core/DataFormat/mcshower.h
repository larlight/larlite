/**
 * \file mcshower.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for mcshower data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat
    
@{*/

#ifndef LARLITE_MCSHOWER_H
#define LARLITE_MCSHOWER_H

#include "mctrajectory.h"

namespace larlite {
  /**
     \class mcshower
     LArSoft utility class (not data container) MCShowerProfile 
     implemented as a data product in Larlite.
  */
  class mcshower : public data_base {
    
  public:

    /// Default constructor
    mcshower() : data_base(data::kMCShower) {clear_data();}

    /// Copy constructor
    mcshower(const mcshower& origin) : data_base(origin),
				       fOrigin(origin.fOrigin),
				       fG4TrackID(origin.fG4TrackID),
				       fProcess(origin.fProcess),
				       fG4Start(origin.fG4Start),
				       fG4End(origin.fG4End),
				       fMotherPDGCode(origin.fMotherPDGCode),
				       fMotherG4TrackID(origin.fMotherG4TrackID),
				       fMotherProcess(origin.fMotherProcess),
				       fMotherG4Start(origin.fMotherG4Start),
				       fMotherG4End(origin.fMotherG4End),
				       fAncestorPDGCode(origin.fAncestorPDGCode),
				       fAncestorG4TrackID(origin.fAncestorG4TrackID),
				       fAncestorProcess(origin.fAncestorProcess),
                                       fAncestorG4Start(origin.fAncestorG4Start),
				       fAncestorG4End(origin.fAncestorG4End),
                                       fDaughterTrackID(origin.fDaughterTrackID),
                                       fDetProfile(origin.fDetProfile),
                                       fPlaneCharge(origin.fPlaneCharge)
    {}

    /// Default destructor
    virtual ~mcshower(){}

    /// Clear method
    virtual void clear_data();

    //--- Getters ---//

    simb::Origin_t     Origin    () const { return fOrigin;    }

    int                PdgCode   () const { return fPDGCode;   }
    unsigned int       G4TrackID () const { return fG4TrackID; } 
    const std::string& Process   () const { return fProcess;   }
    const mcstep&      G4Start   () const { return fG4Start;   }
    const mcstep&      G4End     () const { return fG4End;     }

    int                MotherPdgCode   () const { return fMotherPDGCode;   }
    unsigned int       MotherG4TrackID () const { return fMotherG4TrackID; }
    const std::string& MotherProcess   () const { return fMotherProcess;   }
    const mcstep&      MotherG4Start   () const { return fMotherG4Start;   }
    const mcstep&      MotherG4End     () const { return fMotherG4End;     }

    int                AncestorPdgCode   () const { return fAncestorPDGCode;   }
    unsigned int       AncestorG4TrackID () const { return fAncestorG4TrackID; }
    const std::string& AncestorProcess   () const { return fAncestorProcess;   }
    const mcstep&      AncestorG4Start   () const { return fAncestorG4Start;   }
    const mcstep&      AncestorG4End     () const { return fAncestorG4End;     }

    const mcstep& DetProfile () const { return fDetProfile; }
    
    const std::vector<unsigned int>&  DaughterTrackID() const { return fDaughterTrackID; }

    double Charge(const size_t plane) const;

    //--- Setters ---//
    void Origin    ( simb::Origin_t o ) { fOrigin    = o;    }

    void PdgCode   ( int id                  ) { fPDGCode   = id;   }
    void G4TrackID ( unsigned int id         ) { fG4TrackID = id;   }
    void Process   ( const std::string &name ) { fProcess   = name; }
    void G4Start   ( const mcstep &s         ) { fG4Start   = s;    }
    void G4End     ( const mcstep &s         ) { fG4End     = s;    }

    void MotherPdgCode   ( int id                  ) { fMotherPDGCode   = id;   }
    void MotherG4TrackID ( unsigned int id         ) { fMotherG4TrackID = id;   }
    void MotherProcess   ( const std::string& name ) { fMotherProcess   = name; }
    void MotherG4Start   ( const mcstep& s         ) { fMotherG4Start   = s;    }
    void MotherG4End     ( const mcstep& s         ) { fMotherG4End     = s;    }

    void AncestorPdgCode   ( int id                  ) { fAncestorPDGCode   = id;   }
    void AncestorG4TrackID ( unsigned int id         ) { fAncestorG4TrackID = id;   }
    void AncestorProcess   ( const std::string& name ) { fAncestorProcess   = name; }
    void AncestorG4Start   ( const mcstep& s         ) { fAncestorG4Start   = s;    }
    void AncestorG4End     ( const mcstep& s         ) { fAncestorG4End     = s;    }

    void DetProfile ( const mcstep& s) { fDetProfile = s; }

    void DaughterTrackID ( const std::vector<unsigned int>& id_v ) { fDaughterTrackID = id_v; }

    void Charge (const std::vector<double>& q) { fPlaneCharge = q; }

  protected:

    //---- Origin info ----//
    simb::Origin_t fOrigin;    ///< Origin information

    //---- Shower particle info ----//
    int          fPDGCode;     ///< Shower particle PDG code
    unsigned int fG4TrackID;   ///< Shower particle G4 track ID
    std::string  fProcess;     ///< Shower particle's creation process
    mcstep       fG4Start;     ///< Shower particle's G4 start point
    mcstep       fG4End;       ///< Shower particle's G4 end point

    //---- Mother's particle info ---//
    int          fMotherPDGCode;   ///< Shower's mother PDG code   
    unsigned int fMotherG4TrackID; ///< Shower's mother G4 track ID
    std::string  fMotherProcess;   ///< Shower's mother creation process
    mcstep       fMotherG4Start;   ///< Shower's mother G4 start point
    mcstep       fMotherG4End;     ///< Shower's mother G4 end point

    //---- Ancestor's particle info ---//
    int          fAncestorPDGCode;   ///< Shower's ancestor PDG code   
    unsigned int fAncestorG4TrackID; ///< Shower's ancestor G4 track ID
    std::string  fAncestorProcess;   ///< Shower's ancestor creation process
    mcstep       fAncestorG4Start;   ///< Shower's ancestor G4 start point
    mcstep       fAncestorG4End;     ///< Shower's ancestor G4 end point

    //---- Energy deposition info ----//
    std::vector<unsigned int>  fDaughterTrackID; ///< Daughters' track ID
    mcstep                     fDetProfile;      ///< Combined energy deposition information

    //---- Charge per plane ----//
    std::vector<double> fPlaneCharge; ///< Charge deposit per plane

    ////////////////////////
    ClassDef(mcshower,6)
    ////////////////////////
      
  };

  /**
     \class event_mcshower
     A collection storage class of multiple mcshower
  */
  class event_mcshower : public std::vector<larlite::mcshower>,
			 public event_base {
    
  public:
    
    /// Default constructor
    event_mcshower(std::string name="noname") : event_base(data::kMCShower,name) {clear_data();}
    
    /// Default copy constructor
    event_mcshower(const event_mcshower& original) : std::vector<larlite::mcshower>(original),
						     event_base(original)
    {}
    
    /// Default destructor
    ~event_mcshower(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}
    
  private:
    
    //////////////////////////
    ClassDef(event_mcshower,2)
    //////////////////////////
  };
  
}
#endif

/** @} */ // end of doxygen group 
