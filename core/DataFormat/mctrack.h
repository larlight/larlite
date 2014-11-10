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
				   fPDGCode(orig.fPDGCode),
				   fCreationProcess(orig.fCreationProcess),
				   fG4Start(orig.fG4Start),
				   fG4End(orig.fG4End),
				   fAncestorPDGCode(orig.fAncestorPDGCode),
				   fAncestorG4Start(orig.fAncestorG4Start)
    {}

    void clear_data();

    int                 PdgCode() const         { return fPDGCode;         }
    const std::string&  CreationProcess() const { return fCreationProcess; }
    const mcstep&       G4Start() const         { return fG4Start;         }
    const mcstep&       G4End()   const         { return fG4End;           }
    int                 AncestorPDGCode() const { return fAncestorPDGCode; }
    const mcstep&       AncestorG4Start() const { return fAncestorG4Start; }

    void PdgCode         ( int id           ) { fPDGCode = id;           }
    void CreationProcess ( std::string name ) { fCreationProcess = name; }
    void G4Start         ( const mcstep s   ) { fG4Start = s;            }
    void G4End           ( const mcstep s   ) { fG4End = s;              }
    void AncestorPDGCode ( int id           ) { fAncestorPDGCode = id;   }
    void AncestorG4Start ( const mcstep s   ) { fAncestorG4Start = s;    }

  protected:

    int         fPDGCode;         ///< PDG code of this track particle
    std::string fCreationProcess; ///< Creation process of this track particle
    mcstep      fG4Start;         ///< G4 start position/momentum of this track particle
    mcstep      fG4End;           ///< G4 end position/momentum of this track particle

    int    fAncestorPDGCode; ///< This particle's ancestor's PDG code
    mcstep fAncestorG4Start; ///< This particle's ancestor's start position/momentum

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
