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

#ifndef LARLITE_MCPART_H
#define LARLITE_MCPART_H

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
    mcpart(const int       trackId,
	   const int       pdgid,
	   const std::string process,
	   const int       mother = data::kINVALID_INT,
	   const double    mass   = data::kINVALID_DOUBLE,
	   const int       status = data::kINVALID_INT   );

    /// Copy constructor
    mcpart(const mcpart& origin) : data_base(origin),
				   fstatus(origin.fstatus),
				   ftrackId(origin.ftrackId),
				   fpdgCode(origin.fpdgCode),
				   fmother(origin.fmother),
				   fprocess(origin.fprocess),
				   ftrajectory(origin.ftrajectory),
				   fmass(origin.fmass),
				   fpolarization(origin.fpolarization),
				   fdaughters(origin.fdaughters),
				   fWeight(origin.fWeight),
				   fGvtx(origin.fGvtx),
				   frescatter(origin.frescatter),
				   ftrackFiducial(origin.ftrackFiducial)
    {}

    /// Default destructor
    virtual ~mcpart(){}

    ///--- Utility ---///
    virtual void clear_data();

    ///--- Setters ---///

    inline void AddDaughter     (const int trackID)   { fdaughters.insert(trackID);  }
    inline void SetPolarization (TVector3 const& p)   { fpolarization = p;           }
    inline void SetRescatter    (int code)            { frescatter    = code;        }
    inline void SetWeight       (double wt)           { fWeight       = wt;          }
    inline void SetEndProcess   (std::string s)       { fendprocess   = s;           }
    inline void SetTrajectory   (const mctrajectory steps) { ftrajectory   = steps;  }
    inline void AddTrajectory   (const mcstep step)   { ftrajectory.push_back(step); }
    inline void AddTrajectory   (const TLorentzVector& position,
				 const TLorentzVector& momentum)
    { ftrajectory.push_back(position,momentum); }

    inline void AddFiducialTrack(size_t start, size_t end)
    { ftrackFiducial.push_back(std::pair<size_t,size_t>(start,end)); }

    inline void AddFiducialTrack(const std::pair<size_t,size_t>& step)
    { ftrackFiducial.push_back(step); }

    ///--- Getters ---///

    int                  StatusCode()   const { return fstatus;       }
    int                  TrackId()      const { return ftrackId;      }
    int                  PdgCode()      const { return fpdgCode;      }
    int                  Mother()       const { return fmother;       }
    const std::string    Process()      const { return fprocess;      }
    const std::string    EndProcess()   const { return fendprocess;   }

    const mctrajectory&  Trajectory()   const { return ftrajectory;   }
    double               Mass()         const { return fmass;         }
    const TVector3&      Polarization() const { return fpolarization; }
    const std::set<int>& Daughters()    const { return fdaughters;    } 
    double               Weight()       const { return fWeight;       }
    TLorentzVector       GetGvtx()      const { return fGvtx;         }
    int                  Rescatter()    const { return frescatter;    }

    const std::vector<std::pair<size_t,size_t> >& FiducialTrack() const 
    {return ftrackFiducial;}

  private:

    int            fstatus;       ///< Status code from generator, geant, etc
    int            ftrackId;      ///< TrackId
    int            fpdgCode;      ///< PDG code
    int            fmother;       ///< Mother
    std::string    fprocess;      ///< Detector-simulation physics process that created the particle
    std::string    fendprocess;   ///< end process for the particle  
    mctrajectory   ftrajectory;   ///< particle trajectory (position,momentum)
    double         fmass;         ///< Mass; from PDG unless overridden Should be in GeV
    TVector3       fpolarization; ///< Polarization
    std::set<int>  fdaughters;    ///< Sorted list of daughters of this particle.
    double         fWeight;       ///< Assigned weight to this particle for MC tests
    TLorentzVector fGvtx;         ///< Vertex needed by generater (genie) to rebuild. genie::EventRecord for event reweighting
    int            frescatter;    ///< rescatter code

    std::vector<std::pair<size_t,size_t> > ftrackFiducial; ///< mctrajectory indexes for a trajectory inside fiducial volume
   
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
    
  };
}
#endif

/** @} */ // end of doxygen group 
