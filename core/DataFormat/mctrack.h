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
#include "Base/MCConstants.h"
#include <vector>
namespace larlite{

  typedef size_t SegmentID_t; ///< Segment ID                                                                                                                                                          
  const SegmentID_t InvalidSegmentID = -1;

  /**
     \class mctrack
     LArSoft MCTrack class equivalent data container
  */
  class mctrack : public std::vector<larlite::mcstep>,
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
				   fTrackID(orig.fTrackID),
				   fProcess(orig.fProcess),
				   fStart(orig.fStart),
				   fEnd(orig.fEnd),
                                   fdQdx(orig.fdQdx),
                                   fdEdx(orig.fdEdx),
                                   fMotherPDGCode(orig.fMotherPDGCode),
				   fMotherTrackID(orig.fMotherTrackID),
				   fMotherProcess(orig.fMotherProcess),
				   fMotherStart(orig.fMotherStart),
				   fMotherEnd(orig.fMotherEnd),
				   fAncestorPDGCode(orig.fAncestorPDGCode),
				   fAncestorTrackID(orig.fAncestorTrackID),
                                   fAncestorProcess(orig.fAncestorProcess),
                                   fAncestorStart(orig.fAncestorStart),
				   fAncestorEnd(orig.fAncestorEnd)

    {}

    void clear_data();

    simb::Origin_t      Origin   () const;
    int                 PdgCode  () const;
    unsigned int        TrackID  () const;
    const std::string&  Process  () const;
    const mcstep&       Start    () const;
    const mcstep&       End      () const;
    const std::vector< std::vector<double> >& dQdx() const;  // dQdx[# of MCSteps - 1][# of plane]
    const std::vector<double>& dEdx() const; // dEdx[# of MCSteps - 1]


    int                MotherPdgCode () const;
    unsigned int       MotherTrackID () const;
    const std::string& MotherProcess () const;
    const mcstep&      MotherStart   () const;
    const mcstep&      MotherEnd     () const;

    int                AncestorPdgCode      () const;
    unsigned int       AncestorTrackID      () const;
    const std::string& AncestorProcess      () const;
    const mcstep&      AncestorStart        () const;
    const mcstep&      AncestorEnd          () const;
    

    void Origin          ( simb::Origin_t o ) { fOrigin    = o;      }
    void PdgCode         ( int id           ) { fPDGCode   = id;     }
    void TrackID         ( unsigned int id  ) { fTrackID = id;       }
    void Process         ( std::string name ) { fProcess   = name;   }
    void Start           ( const mcstep s   ) { fStart   = s;        }
    void End             ( const mcstep s   ) { fEnd     = s;        }
    void dEdx            ( const std::vector<double> s) { fdEdx = s;}
    void dQdx            ( const std::vector<std::vector<double> > s) { fdQdx = s;}


    void MotherPdgCode   ( int id               ) { fMotherPDGCode  = id; }
    void MotherTrackID   ( unsigned int id      ) { fMotherTrackID  = id; }
    void MotherProcess   ( const std::string& n ) { fMotherProcess  = n;  }
    void MotherStart     ( const mcstep& s      ) { fMotherStart    = s;  }
    void MotherEnd       ( const mcstep& s      ) { fMotherEnd      = s;  }

    void AncestorPdgCode   ( int id               ) { fAncestorPDGCode = id; }
    void AncestorTrackID   ( unsigned int id      ) { fAncestorTrackID = id; }
    void AncestorProcess   ( const std::string& n ) { fAncestorProcess = n;  }
    void AncestorStart     ( const mcstep& s      ) { fAncestorStart   = s;  }
    void AncestorEnd       ( const mcstep& s      ) { fAncestorEnd     = s;  }

    //    double   dEdx(const SegmentID_t id) const;
    //    double   dX  (const SegmentID_t id) const ;
    //    const std::vector<double> dEdx()    const;   
    //    const std::vector<TLorentzVector>  SegmentCenter() const;


    
  protected:

    simb::Origin_t fOrigin;    ///< Origin of this particle (see simb::Origin_t)
    int            fPDGCode;   ///< PDG code of this track particle
    unsigned int   fTrackID;   ///< G4 track ID
    std::string    fProcess;   ///< Creation process of this track particle
    mcstep         fStart;     ///< G4 start position/momentum of this track particle
    mcstep         fEnd;       ///< G4 end position/momentum of this track particle
    std::vector<std::vector<double> > fdQdx; //< the G4 electron yeild at each wire plane between each step
    std::vector<double> fdEdx;//< the G4 energy loss between each step

    int            fMotherPDGCode;   ///< This particle's mother's PDG code
    unsigned int   fMotherTrackID;   ///< This particle's mother's G4 track ID
    std::string    fMotherProcess;   ///< This particle's mother's process name
    mcstep         fMotherStart;     ///< This particle's mother's start position/momentum
    mcstep         fMotherEnd;       ///< This particle's mother's end position/momentum

    int            fAncestorPDGCode;   ///< This particle's ancestor's PDG code
    unsigned int   fAncestorTrackID;   ///< This particle's ancestor's G4 track ID
    std::string    fAncestorProcess;   ///< This particle's ancestor's process name
    mcstep         fAncestorStart;     ///< This particle's ancestor's start position/momentum
    mcstep         fAncestorEnd;       ///< This particle's ancestor's start position/momentum

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
    
  };
}
#endif

/** @} */ // end of doxygen group 
