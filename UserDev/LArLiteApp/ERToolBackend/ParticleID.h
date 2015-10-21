#ifndef LARLITE_PARTICLEID_H
#define LARLITE_PARTICLEID_H

#include "DataFormat/mctruth.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/mctrack.h"
#include "GeoAlgo/GeoVector.h"

namespace ertool_helper {

  class ParticleID {
    friend class ParticleID;

  public:
    /// Particle ID default constructor
    ParticleID();
    
    ParticleID(const int pdg,
	       const TLorentzVector& pos,
	       const TLorentzVector& mom);

    ParticleID(const int pdg,
      const geoalgo::Vector& pos,
      const geoalgo::Vector& mom);
    
    /// Particle ID constructor from MCShower
    ParticleID(const ::larlite::mcshower& mcs);
    
    /// Particle ID constructor from MCTrack
    ParticleID(const ::larlite::mctrack& mct);
    
    /// Particle ID constructor from MCTruth
    ParticleID(const ::larlite::mcpart& mcp);

    /// PDGCode
    int PdgCode() const { return _pdg; }
    /// Position
    const TLorentzVector& Position() const { return _pos; }
    /// Momentum
    const TLorentzVector& Momentum() const { return _mom; }
    
    /// Default destructor
    virtual ~ParticleID(){};

    inline bool operator== (const ParticleID& rhs) const
    {
      return ( _pdg == rhs._pdg && _pos == rhs._pos && _mom == rhs._mom ); 
    }

    inline bool operator!= (const ParticleID& rhs) const
    {
      return !((*this) == rhs);
    }

    inline bool operator< (const ParticleID& rhs) const
    {
      if( _pdg         < rhs._pdg     ) return true;
      if( rhs._pdg     < _pdg         ) return false;

      if( _pos.X()     < rhs._pos.X() ) return true;
      if( rhs._pos.X() < _pos.X()     ) return false;
      if( _pos.Y()     < rhs._pos.Y() ) return true;
      if( rhs._pos.Y() < _pos.Y()     ) return false;
      if( _pos.Z()     < rhs._pos.Z() ) return true;
      if( rhs._pos.Z() < _pos.Z()     ) return false;

      if( _mom.X()     < rhs._mom.X() ) return true;
      if( rhs._mom.X() < _mom.X()     ) return false;
      if( _mom.Y()     < rhs._mom.Y() ) return true;
      if( rhs._mom.Y() < _mom.Y()     ) return false;
      if( _mom.Z()     < rhs._mom.Z() ) return true;
      if( rhs._mom.Z() < _mom.Z()     ) return false;

      return false;
    }
  private:
    
    int _pdg;              ///< PDG code of particle
    TLorentzVector _pos;
    TLorentzVector _mom;
    void Approx();
  };

}

#endif
