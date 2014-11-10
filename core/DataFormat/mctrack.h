/**
 * \file mctrack.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for mctrack data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef LARLITE_MCTRACK_H
#define LARLITE_MCTRACK_H

#include "mctrajectory.h"
#include <vector>
namespace larlite{

  /**
     \class mctrack
     LArSoft MCTrack class equivalent data container
  */
  class mctrack : std::vector<larlite::mcstep>,
		  public data_base {
    
  public:
    
    /// Default constructor
    mctrack() : data_base(data::kMCTrack)
    { clear_data(); }
    
    /// Default destructor
    virtual ~mctrack(){}

    /// copy ctor
    mctrack(const mctrack& orig) : std::vector<larlite::mcstep>(orig),
				   data_base(orig),
				   fOrigin(orig.fOrigin),
				   fPDGCode(orig.fPDGCode),
				   fG4TrackID(orig.fG4TrackID),
				   fProcess(orig.fProcess),
				   fG4Start(orig.fG4Start),
				   fG4End(orig.fG4End),
				   fMotherPDGCode(orig.fMotherPDGCode),
				   fMotherG4TrackID(orig.fMotherG4TrackID),
				   fMotherG4Start(orig.fMotherG4Start),
				   fMotherG4End(orig.fMotherG4End),
				   fAncestorPDGCode(orig.fAncestorPDGCode),
				   fAncestorG4TrackID(orig.fAncestorG4TrackID),
                                   fAncestorG4Start(orig.fAncestorG4Start),
				   fAncestorG4End(orig.fAncestorG4End)
    {}

    void clear_data();

    simb::Origin_t      Origin     () const { return fOrigin;            }
    int                 PdgCode    () const { return fPDGCode;           }
    unsigned int        G4TrackID  () const { return fG4TrackID;         }
    const std::string&  Process    () const { return fProcess;           }
    const mcstep&       G4Start    () const { return fG4Start;           }
    const mcstep&       G4End      () const { return fG4End;             }

    int           MotherPDGCode    () const { return fMotherPDGCode;     }
    unsigned int  MotherG4TrackID  () const { return fMotherG4TrackID;   }
    const mcstep& MotherG4Start    () const { return fMotherG4Start;     }
    const mcstep& MotherG4End      () const { return fMotherG4End;       }

    int           AncestorPDGCode  () const { return fAncestorPDGCode;   }
    unsigned int  AncestorG4TrackID() const { return fAncestorG4TrackID; }
    const mcstep& AncestorG4Start  () const { return fAncestorG4Start;   }
    const mcstep& AncestorG4End    () const { return fAncestorG4End;     }

    
    void Origin          ( simb::Origin_t o ) { fOrigin    = o;        }
    void PdgCode         ( int id           ) { fPDGCode   = id;       }
    void G4TrackID       ( unsigned int id  ) { fG4TrackID = id;       }
    void Process         ( std::string name ) { fProcess   = name;     }
    void G4Start         ( const mcstep s   ) { fG4Start   = s;        }
    void G4End           ( const mcstep s   ) { fG4End     = s;        }

    void MotherPDGCode   ( int id           ) { fMotherPDGCode   = id; }
    void MotherG4TrackID ( unsigned int id  ) { fMotherG4TrackID = id; }
    void MotherG4Start   ( const mcstep s   ) { fMotherG4Start   = s;  }
    void MotherG4End     ( const mcstep s   ) { fMotherG4End     = s;  }

    void AncestorPDGCode   ( int id           ) { fAncestorPDGCode   = id; }
    void AncestorG4TrackID ( unsigned int id  ) { fAncestorG4TrackID = id; }
    void AncestorG4Start   ( const mcstep s   ) { fAncestorG4Start   = s;  }
    void AncestorG4End     ( const mcstep s   ) { fAncestorG4End     = s;  }

  protected:

    simb::Origin_t fOrigin;    ///< Origin of this particle (see simb::Origin_t)
    int            fPDGCode;   ///< PDG code of this track particle
    unsigned int   fG4TrackID; ///< G4 track ID
    std::string    fProcess;   ///< Creation process of this track particle
    mcstep         fG4Start;   ///< G4 start position/momentum of this track particle
    mcstep         fG4End;     ///< G4 end position/momentum of this track particle

    int            fMotherPDGCode;   ///< This particle's mother's PDG code
    unsigned int   fMotherG4TrackID; ///< This particle's mother's G4 track ID
    mcstep         fMotherG4Start;   ///< This particle's mother's start position/momentum
    mcstep         fMotherG4End;     ///< This particle's mother's end position/momentum

    int            fAncestorPDGCode;   ///< This particle's ancestor's PDG code
    unsigned int   fAncestorG4TrackID; ///< This particle's ancestor's G4 track ID
    mcstep         fAncestorG4Start;   ///< This particle's ancestor's start position/momentum
    mcstep         fAncestorG4End;     ///< This particle's ancestor's start position/momentum

    ////////////////////////
    ClassDef(mctrack,1)
    ////////////////////////
      
  };
  
  /**
     \class event_mctrack
     A collection storage class of multiple mctracks.
  */
  class event_mctrack : public std::vector<larlite::mctrack>, 
			public event_base {
    
  public:
    
    /// Default constructor
    event_mctrack(std::string name="noname") : event_base(data::kMCTrack,name) { clear_data(); }
    
    /// Default copy constructor
    event_mctrack(const event_mctrack& original) : std::vector<larlite::mctrack>(original), event_base(original)
    {}
    
    /// Default destructor
    ~event_mctrack(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}

  private:
    
    ////////////////////////
    ClassDef(event_mctrack,1)
    ////////////////////////
  };
}
#endif

/** @} */ // end of doxygen group 
