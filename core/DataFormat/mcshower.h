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
#include <TVector3.h>
#include "Base/MCConstants.h"
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
                                       fPDGCode(origin.fPDGCode),
				       fTrackID(origin.fTrackID),
				       fProcess(origin.fProcess),
				       fStart(origin.fStart),
				       fEnd(origin.fEnd),
				       fMotherPDGCode(origin.fMotherPDGCode),
				       fMotherTrackID(origin.fMotherTrackID),
				       fMotherProcess(origin.fMotherProcess),
				       fMotherStart(origin.fMotherStart),
				       fMotherEnd(origin.fMotherEnd),
				       fAncestorPDGCode(origin.fAncestorPDGCode),
				       fAncestorTrackID(origin.fAncestorTrackID),
				       fAncestorProcess(origin.fAncestorProcess),
                                       fAncestorStart(origin.fAncestorStart),
				       fAncestorEnd(origin.fAncestorEnd),
                                       fDaughterTrackID(origin.fDaughterTrackID),
                                       fDetProfile(origin.fDetProfile),
                                       fPlaneCharge(origin.fPlaneCharge),
                                       fStartDir(origin.fStartDir),
                                       fdEdx(origin.fdEdx),
                                       fdQdx(origin.fdQdx)
    {}

    /// Default destructor
    virtual ~mcshower(){}

    /// Clear method
    virtual void clear_data();

    //--- Getters ---//

    simb::Origin_t     Origin  () const;

    int                PdgCode () const;
    unsigned int       TrackID () const;
    const std::string& Process () const;
    const mcstep&      Start   () const;
    const mcstep&      End     () const;

    int                MotherPdgCode () const;
    unsigned int       MotherTrackID () const;
    const std::string& MotherProcess () const;
    const mcstep&      MotherStart   () const;
    const mcstep&      MotherEnd     () const;

    int                AncestorPdgCode () const;
    unsigned int       AncestorTrackID () const;
    const std::string& AncestorProcess () const;
    const mcstep&      AncestorStart   () const;
    const mcstep&      AncestorEnd     () const;

    const mcstep& DetProfile () const;
    
    const std::vector<unsigned int>&  DaughterTrackID() const;

    double Charge(const size_t plane) const;
    double dQdx(const size_t plane) const;

    const std::vector<double>& Charge() const;
    const std::vector<double>& dQdx() const;

    const TVector3& StartDir() const;

    const double& dEdx() const;

    //--- Setters ---//
    void Origin  ( simb::Origin_t o ) { fOrigin    = o;    }

    void PdgCode ( int id                  ) { fPDGCode   = id;   }
    void TrackID ( unsigned int id         ) { fTrackID = id;     }
    void Process ( const std::string &name ) { fProcess   = name; }
    void Start   ( const mcstep &s         ) { fStart   = s;      }
    void End     ( const mcstep &s         ) { fEnd     = s;      }



    void MotherPdgCode ( int id                  ) { fMotherPDGCode   = id;   }
    void MotherTrackID ( unsigned int id         ) { fMotherTrackID = id;     }
    void MotherProcess ( const std::string& name ) { fMotherProcess   = name; }
    void MotherStart   ( const mcstep& s         ) { fMotherStart   = s;      }
    void MotherEnd     ( const mcstep& s         ) { fMotherEnd     = s;      }

    void AncestorPdgCode ( int id                  ) { fAncestorPDGCode   = id;   }
    void AncestorTrackID ( unsigned int id         ) { fAncestorTrackID = id;     }
    void AncestorProcess ( const std::string& name ) { fAncestorProcess   = name; }
    void AncestorStart   ( const mcstep& s         ) { fAncestorStart   = s;      }
    void AncestorEnd     ( const mcstep& s         ) { fAncestorEnd     = s;      }

    void DetProfile ( const mcstep& s) { fDetProfile = s; }

    void DaughterTrackID ( const std::vector<unsigned int>& id_v ) { fDaughterTrackID = id_v; }

    void Charge (const std::vector<double>& q) { fPlaneCharge = q; }
    void dQdx (const std::vector<double>& dqdx) { fdQdx = dqdx; }

    void StartDir ( const TVector3 &sdir) { fStartDir = sdir; }
    void dEdx    (const double& dedx) {fdEdx = dedx;}


  protected:

    //---- Origin info ----//
    simb::Origin_t fOrigin;    ///< Origin information

    //---- Shower particle info ----//
    int          fPDGCode;     ///< Shower particle PDG code
    unsigned int fTrackID;   ///< Shower particle G4 track ID
    std::string  fProcess;     ///< Shower particle's creation process
    mcstep       fStart;     ///< Shower particle's G4 start point
    mcstep       fEnd;       ///< Shower particle's G4 end point


    //---- Mother's particle info ---//
    int          fMotherPDGCode;   ///< Shower's mother PDG code   
    unsigned int fMotherTrackID; ///< Shower's mother G4 track ID
    std::string  fMotherProcess;   ///< Shower's mother creation process
    mcstep       fMotherStart;   ///< Shower's mother G4 start point
    mcstep       fMotherEnd;     ///< Shower's mother G4 end point

    //---- Ancestor's particle info ---//
    int          fAncestorPDGCode;   ///< Shower's ancestor PDG code   
    unsigned int fAncestorTrackID; ///< Shower's ancestor G4 track ID
    std::string  fAncestorProcess;   ///< Shower's ancestor creation process
    mcstep       fAncestorStart;   ///< Shower's ancestor G4 start point
    mcstep       fAncestorEnd;     ///< Shower's ancestor G4 end point

    //---- Energy deposition info ----//
    std::vector<unsigned int>  fDaughterTrackID; ///< Daughters' track ID
    mcstep                     fDetProfile;      ///< Combined energy deposition information

    //---- Charge per plane ----//
    std::vector<double> fPlaneCharge; ///< Charge deposit per plane


    TVector3                   fStartDir;        ///< Shower Starting Direction, within the first 2.4cm                   
    double                     fdEdx;            ///< Shower True dEdx
    std::vector<double> fdQdx; ///< Charge deposit per plane


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
    
  };
  
}
#endif

/** @} */ // end of doxygen group 
