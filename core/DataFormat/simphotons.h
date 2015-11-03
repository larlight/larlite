////////////////////////////////////////////////////////////////////////
/// \file  SimPhotons.h
/// \brief contains objects relating to OpDet hits
///
/// \version $Id: ParticleList.h,v 1.13 2010/05/13 16:12:20 seligman Exp $
/// \author  Ben Jones
////////////////////////////////////////////////////////////////////////
// This file contains the definitions of the classes which
// are stored in the event representing OpDet hits.
//
// A OpDet Hit stores data for each photon which steps inside the OpDet 
// volume.  Currently the quantities stored are 4 potition, 4 momentum
// and TrackID.  A SimPhotonsCollection is a set of SimPhotonss, one per OpDet
// in the collection. 
// 
// The SimPhotons is filled in by the OpDetSensitiveDetector class in LArG4 
// and will be used to generate the OpDet response later in the simulation
// chain.
//
// OnePhoton, SimPhotons and SimPhotonsCollection are all persistent under
// ROOT I/O.  For compilation to succeed, the relevant pragma lines
// must be present in LinkDef.h.
//
// The current implementation resembles that of an STL container in 
// some respects but needs more work before it is polished product.
//
// Ben Jones, MIT, 06/04/2010
//

#ifndef LARLITE_SIMPHOTONS_H
#define LARLITE_SIMPHOTONS_H

#include <TLorentzVector.h>
#include <vector>

#include "data_base.h"

namespace larlite
{

  // This structure contains all the information per photon
  // which entered the sensitive OpDet volume.

  class onephoton
  {
  public:
    onephoton();
    bool           SetInSD;
    TVector3       InitialPosition;
    TVector3       FinalLocalPosition; // in cm
    float          Time;
    float          Energy;
  };
  
  class simphotons : public data_base,
		     public std::vector<onephoton> 
  {
  public:
    simphotons();
    simphotons(int chan)
      : data_base(data::kSimPhotons)
      , fOpChannel(chan)
    {}
    
    int  fOpChannel;  /// volume number for the OpDet
    
  public:
    
    // define addition operators for combining hits
    //   (add all photons to one vector)
    simphotons& operator+=(const simphotons &rhs);
    const simphotons operator+(const simphotons &rhs) const;
    
    inline int   OpChannel() const  { return fOpChannel;      }
    inline void  SetChannel(int ch) { fOpChannel = ch;        }
    
    inline bool operator==(const simphotons& other) const { return fOpChannel == other.OpChannel(); }
    
  };
  
  class event_simphotons : public std::vector<larlite::simphotons>,
			   public event_base {

  public:

    /// Default constructor
    event_simphotons(std::string name="noname") : event_base(data::kSimPhotons,name) { clear_data(); }

    /// Default copy constructor
    event_simphotons(const event_simphotons& original) : std::vector<larlite::simphotons>(original), event_base(original)
    {}

    /// Default destructor
    ~event_simphotons(){}

    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}

  private:
  };
  
}
#endif
