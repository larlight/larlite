#ifndef LARLITE_PARTICLEID_CXX
#define LARLITE_PARTICLEID_CXX

#include "ParticleID.h"

namespace ertool_helper {

  // Particle ID default constructor
  ParticleID::ParticleID(){
    _pdg = -1;
    _mom = TLorentzVector();
    _pos = TLorentzVector();
  }
  
  ParticleID::ParticleID(const int pdg,
			 const TLorentzVector& pos,
			 const TLorentzVector& mom)
    : _pdg(pdg)
    , _pos(pos)
    , _mom(mom)
  {}
  
  // Particle ID constructor from MCShower
  ParticleID::ParticleID(const ::larlite::mcshower& mcs){
    _pdg = mcs.PdgCode();
    _mom = mcs.Start().Momentum();
    _pos = mcs.Start().Position();
  }

  // Particle ID constructor from MCTrack
  ParticleID::ParticleID(const ::larlite::mctrack& mct){
    _pdg = mct.PdgCode();
    _mom = mct.Start().Momentum();
    _pos = mct.Start().Position();
  }
  
  // Particle ID constructor from MCTruth
  ParticleID::ParticleID(const ::larlite::mcpart& mcp){
    _pdg = mcp.PdgCode();
    _mom = mcp.Trajectory()[0].Momentum();
    _pos = mcp.Trajectory()[0].Position();
  }

}

#endif  
