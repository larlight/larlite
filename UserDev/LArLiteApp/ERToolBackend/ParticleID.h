#ifndef LARLITE_PARTICLEID_H
#define LARLITE_PARTICLEID_H

#include "DataFormat/mctruth.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/mctrack.h"

namespace larlite {

  class ParticleID {

    // Particle ID default constructor
    ParticleID();
    
    // Particle ID constructor from MCShower
    ParticleID(const mcshower& mcs);
    
    // Particle ID constructor from MCTrack
    ParticleID(const mctrack& mct);

    // Particle ID constructor from MCTruth
    ParticleID(const mcpart& mcp);
    
    // Default destructor
    virtual ~ParticleID(){};

    friend bool operator== (const ParticleID& p1, const ParticleID& p2);
    friend bool operator<  (const ParticleID& p1, const ParticleID& p2);

  private:
    
    int _pdg;              ///< PDG code of particle
    TLorentzVector _mom;   ///< Momentum @ creation (1st step)
    TLorentzVector _pos;   ///< Position @ creation (1st step)
    
  };

}

#endif
