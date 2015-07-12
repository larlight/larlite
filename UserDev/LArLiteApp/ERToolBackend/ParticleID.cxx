#ifndef LARLITE_PARTICLEID_CXX
#define LARLITE_PARTICLEID_CXX

#include "ParticleID.h"

namespace larlite {

  // Particle ID default constructor
  ParticleID::ParticleID(){
    _pdg = -1;
    _mom = TLorentzVector();
    _pos = TLorentzVector();
  }

  // Particle ID constructor from MCShower
  ParticleID::ParticleID(const mcshower& mcs){
    _pdg = mcs.PdgCode();
    _mom = mcs.Start().Momentum();
    _pos = mcs.Start().Position();
  }

  // Particle ID constructor from MCTrack
  ParticleID::ParticleID(const mctrack& mct){
    _pdg = mct.PdgCode();
    _mom = mct[0].Momentum();
    _pos = mct[0].Position();
  }
  
  // Particle ID constructor from MCTruth
  ParticleID::ParticleID(const mcpart& mcp){
    _pdg = mcp.PdgCode();
    _mom = mcp.Trajectory()[0].Momentum();
    _pos = mcp.Trajectory()[0].Position();
  }

  bool operator== (const ParticleID& p1, const ParticleID& p2){

    if (p1._pdg != p1._pdg)
      return false;
    if (p1._mom != p2._mom)
      return false;
    if (p1._pos != p2._pos)
      return false;

    return true;
  }

  bool operator< (const ParticleID& p1, const ParticleID& p2){

    double p1mom = p1._mom.Mag();
    double p2mom = p2._mom.Mag();

    if (p1mom < p2mom)
      return true;
    
    return false;
  }

}

#endif  
