/**
 * \file mcpart.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for mcpart data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat
    
@{*/

#ifndef MCPART_H
#define MCPART_H

#include "mctrajectory.h"

namespace larlite {
  /**
     \class mcpart
     LArSoft Mcpart data container
  */
  class mcpart : public data_base {
    
  public:

    /// Default constructor 
    mcpart() : data_base(data::kMCParticle) {clear_data();}
    
    /// Alternative constructor
    mcpart(const Int_t       trackId,
	   const Int_t       pdgid,
	   const std::string process,
	   const Int_t       mother = data::kINVALID_INT,
	   const Double_t    mass   = data::kINVALID_DOUBLE,
	   const Int_t       status = data::kINVALID_INT   );

    /// Default destructor
    virtual ~mcpart(){}

    ///--- Utility ---///
    virtual void clear_data();

    ///--- Setters ---///

    inline void AddDaughter     (const int trackID)   { fdaughters.insert(trackID);  }
    inline void SetPolarization (TVector3 const& p)   { fpolarization = p;           }
    inline void SetRescatter    (Int_t code)          { frescatter    = code;        }
    inline void SetWeight       (Double_t wt)         { fWeight       = wt;          }
    inline void SetTrajectory   (const mctrajectory steps) { ftrajectory   = steps;       }
    inline void AddTrajectory   (const mcstep step)   { ftrajectory.push_back(step); }
    inline void AddTrajectory   (const TLorentzVector& position,
				 const TLorentzVector& momentum)
    { ftrajectory.push_back(position,momentum); }

    inline void AddFiducialTrack(size_t start, size_t end)
    { ftrackFiducial.push_back(std::pair<size_t,size_t>(start,end)); }

    inline void AddFiducialTrack(const std::pair<size_t,size_t>& step)
    { ftrackFiducial.push_back(step); }

    ///--- Getters ---///

    Int_t                  StatusCode()   const { return fstatus;       }
    Int_t                  TrackId()      const { return ftrackId;      }
    Int_t                  PdgCode()      const { return fpdgCode;      }
    Int_t                  Mother()       const { return fmother;       }
    const std::string      Process()      const { return fprocess;      }
    const mctrajectory&    Trajectory()   const { return ftrajectory;   }
    Double_t               Mass()         const { return fmass;         }
    const TVector3&        Polarization() const { return fpolarization; }
    const std::set<Int_t>& Daughters()    const { return fdaughters;    } 
    Double_t               Weight()       const { return fWeight;       }
    TLorentzVector         GetGvtx()      const { return fGvtx;         }
    Int_t                  Rescatter()    const { return frescatter;    }

    const std::vector<std::pair<size_t,size_t> >& FiducialTrack() const 
    {return ftrackFiducial;}

  private:

    Int_t           fstatus;       ///< Status code from generator, geant, etc
    Int_t           ftrackId;      ///< TrackId
    Int_t           fpdgCode;      ///< PDG code
    Int_t           fmother;       ///< Mother
    std::string     fprocess;      ///< Detector-simulation physics process that created the particle
    mctrajectory    ftrajectory;   ///< particle trajectory (position,momentum)
    Double_t        fmass;         ///< Mass; from PDG unless overridden Should be in GeV
    TVector3        fpolarization; ///< Polarization
    std::set<Int_t> fdaughters;    ///< Sorted list of daughters of this particle.
    Double_t        fWeight;       ///< Assigned weight to this particle for MC tests
    TLorentzVector  fGvtx;         ///< Vertex needed by generater (genie) to rebuild. genie::EventRecord for event reweighting
    Int_t           frescatter;    ///< rescatter code

    std::vector<std::pair<size_t,size_t> > ftrackFiducial; ///< mctrajectory indexes for a trajectory inside fiducial volume
   
    ////////////////////////
    ClassDef(mcpart,1)
    ////////////////////////
      
  };
  
  /**
     \class event_mcpart
     A collection of MCParticles
  */
  class event_mcpart : public std::vector<larlite::mcpart>,
		       public event_base {
    
  public:
    
    /// Default constructor
    event_mcpart(std::string name="noname") : event_base(data::kMCParticle,name) {clear_data();}
    
    /// Copy constructor
    event_mcpart(const event_mcpart& original) : std::vector<larlite::mcpart>(original),
						 event_base(original)
    {};
    
    /// Default destructor
    virtual ~event_mcpart(){};
    
    /// Clear method
    virtual void clear_data() {event_base::clear_data(); clear();}

    
  private:
    
    ////////////////////////
    ClassDef(event_mcpart,1)
    ////////////////////////
      
  };
}
#endif

/** @} */ // end of doxygen group 
