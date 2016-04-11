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

    ///--- Getters ---///
    int                  StatusCode()   const;
    int                  TrackId()      const;
    int                  PdgCode()      const;
    int                  Mother()       const;
    const std::string    Process()      const;
    const std::string    EndProcess()   const;

    const mctrajectory&  Trajectory()   const;
    double               Mass()         const;
    const TVector3&      Polarization() const;
    const std::set<int>& Daughters()    const;
    double               Weight()       const;
    TLorentzVector       GetGvtx()      const;
    int                  Rescatter()    const;
    const std::vector<std::pair<size_t,size_t> >& FiducialTrack() const;

    const TLorentzVector& Position(size_t i=0) const;
    const TLorentzVector& Momentum(size_t i=0) const;

    ///--- Setters ---///
    void AddDaughter     (const int trackID);
    void SetPolarization (TVector3 const& p);
    void SetRescatter    (int code);
    void SetWeight       (double wt);
    void SetEndProcess   (std::string s);
    void SetTrajectory   (const mctrajectory steps);
    void AddTrajectory   (const mcstep step);
    void AddTrajectory   (const TLorentzVector& position,
			  const TLorentzVector& momentum);

    void AddFiducialTrack(size_t start, size_t end);
    void AddFiducialTrack(const std::pair<size_t,size_t>& step);

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
   
    /// Status code definitions:
    /// -1 => Undefined
    /// 0  => Initial State (generator level)
    /// 1  => Stable Final State (generator level, particles to be tracked by detector-level MC)
    /// 2  => Intermediate State
    /// 3  => Decayed State
    /// 10 => Correlated Nucleon
    /// 11 => Nucleon Target
    /// 12 => DIS Prefragmented Hadronic State
    /// 13 => Predecay Resonant State
    /// 14 => Hadron in the Nucleus (marked for hadron transport modules to act on)
    /// 15 => Final State Nuclear Remnant (low energy nuclear fragments entering the record collectively as a 'hadronic blob' pseudo-particle)
    /// 16 => Nucleon Cluster Target

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
