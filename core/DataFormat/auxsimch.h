////////////////////////////////////////////////////////////////////////
/// \file  AuxDetSimChannel.h
///
/// \brief object containing MC truth information necessary for making RawDigits 
/// and doing back tracking
///
/// \author  miceli@fnal.gov
////////////////////////////////////////////////////////////////////////

#ifndef LARLITE_SIM_AUXDETSIMCHANNEL_H
#define LARLITE_SIM_AUXDETSIMCHANNEL_H

// C/C++ standard libraries
//#include <stdint.h> // C header (need to be compatible with Reflex)
#include <vector>

// LArSoft libraries
//#include "larcore/SimpleTypesAndConstants/geo_types.h"
#include "Base/GeoTypes.h"

#include "data_base.h"

namespace larlite {
  
  /**
   * @brief MC truth information to make RawDigits and do back tracking
   *
   * This structure describes the true position of momentum of a MC particle
   * entering and exiting a scintillator cell (channel) of an auxiliary
   * scintillator detector.
   */
  class auxide {
    
  public:
    auxide();
    
    //constructor for IDEs applying G4 offset...
    auxide(auxide const&, int);

    int   trackID;               ///< Geant4 supplied track ID
    float energyDeposited;       ///< total energy deposited for this track ID and time
    float entryX;                ///< Entry position X of particle
    float entryY;                ///< Entry position Y of particle
    float entryZ;                ///< Entry position Z of particle
    float entryT;                ///< Entry time of particle
    float exitX;                 ///< Exit position X of particle
    float exitY;                 ///< Exit position Y of particle
    float exitZ;                 ///< Exit position Z of particle
    float exitT;                 ///< Exit time of particle
    float exitMomentumX;         ///< Exit X-Momentum of particle
    float exitMomentumY;         ///< Exit Y-Momentum of particle
    float exitMomentumZ;         ///< Exit Z-Momentum of particle

    //#ifndef __GCCXML__
    bool operator<  (const auxide& other) const;
    bool operator== (const auxide& other) const;
    //#endif
  }; // class auxide
  
  /**
   * @brief Collection of particles crossing one auxiliary detector cell
   *
   * This structure collects information (as larlite::auxide) from all the MC
   * particles crossing a single auxiliary detector cell (channel).
   */
  class auxsimch : public data_base {
    
  public:
    /// Default constructor (invalid, empty data)
    auxsimch();

  private:
    unsigned int                    fAuxDetID;          ///< geo->AuxDet(auxDetID), integer used to retrieve AuxDetGeo objec   
    unsigned int                    fAuxDetSensitiveID; ///< integer used to retrieve AuxDetSensitiveGeo object
    std::vector<larlite::auxide> fAuxDetIDEs;        ///< one larlite::auxide for each G4 track id

    //#ifndef __GCCXML__
  public:

    auxsimch(unsigned int inputAuxDetID, 
	     unsigned int inputAuxDetSensitiveID);
    
    /// Constructor: copies from the specified IDE vector
    auxsimch(unsigned int inputAuxDetID, 
	     const std::vector<larlite::auxide>& inputAuxDetIDEs,
	     unsigned int inputAuxDetSensitiveID=0);
    
    /// Constructor: moves data from the specified IDE vector
    auxsimch(unsigned int inputAuxDetID, 
	     std::vector<larlite::auxide>&& inputAuxDetIDEs,
	     unsigned int inputAuxDetSensitiveID=0);
    
    std::pair<int,int> MergeAuxDetSimChannel(const auxsimch&, int);

    ///@name Getters
    ///@{
    unsigned int AuxDetID()          const;
    unsigned int AuxDetSensitiveID() const;

    bool operator<  (const auxsimch& other)     const;
    bool operator== (const auxsimch& other)     const;

    std::vector<larlite::auxide> const& auxides() const;
    ///@}
    
    //#endif
		
		
  }; // class auxsimch

  /**
     \class event_auxsimch
     A collection storage class of multiple auxsimch
  */
  class event_auxsimch : public std::vector<larlite::auxsimch>,
			 public event_base {
    
  public:
    
    /// Default constructor
    event_auxsimch(std::string name="noname") : event_base(data::kAuxDetSimChannel,name) {clear_data();}
    
    /// Default copy constructor
    event_auxsimch(const event_auxsimch& original)
      : std::vector<larlite::auxsimch>(original)
      , event_base(original)
    {}
    
    /// Default destructor
    ~event_auxsimch(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}
    
  private:
    
  };

} // namespace sim

//#ifndef __GCCXML__

inline bool      larlite::auxide::operator<  (const auxide& other) const { return trackID < other.trackID;  }
inline bool      larlite::auxide::operator== (const auxide& other) const { return other.trackID == trackID; }
inline unsigned int  larlite::auxsimch::AuxDetID()                   const { return fAuxDetID;                }
inline unsigned int  larlite::auxsimch::AuxDetSensitiveID()          const { return fAuxDetSensitiveID;       }
inline std::vector<larlite::auxide> const& larlite::auxsimch::auxides() const { return fAuxDetIDEs; }
//#endif

#endif // SIM_AUXDETSIMCHANNEL_H

////////////////////////////////////////////////////////////////////////
