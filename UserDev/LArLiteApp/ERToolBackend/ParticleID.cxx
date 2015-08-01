#ifndef LARLITE_PARTICLEID_CXX
#define LARLITE_PARTICLEID_CXX

#include "ParticleID.h"

namespace ertool_helper {

  // Particle ID default constructor
  ParticleID::ParticleID()
    : _pdg (0)
    , _pos()
    , _mom()
  {Approx();}
  
  ParticleID::ParticleID(const int pdg,
			 const TLorentzVector& pos,
			 const TLorentzVector& mom)
    : _pdg(pdg)
    , _pos(pos)
    , _mom(mom)
  {Approx();}
  
  // Particle ID constructor from MCShower
  ParticleID::ParticleID(const ::larlite::mcshower& mcs)
    : _pdg(mcs.PdgCode())
    , _pos(mcs.Start().Position())
    , _mom(mcs.Start().Momentum())
  {Approx();}

  // Particle ID constructor from MCTrack
  ParticleID::ParticleID(const ::larlite::mctrack& mct)
    : _pdg (mct.PdgCode())
    , _pos (mct.Start().Position())
    , _mom (mct.Start().Momentum())
  {Approx();}
  
  // Particle ID constructor from MCTruth
  ParticleID::ParticleID(const ::larlite::mcpart& mcp)
    : _pdg(mcp.PdgCode())
    , _pos(mcp.Trajectory()[0].Position())
    , _mom(mcp.Trajectory()[0].Momentum())
  {
    _mom[0] *= 1.e3;
    _mom[1] *= 1.e3;
    _mom[2] *= 1.e3;

    Approx();
  }

  void ParticleID::Approx()
  {
    for(size_t i=0; i<4; ++i) {
      _pos[i] = (double)( ((signed long long)(_pos[i]*1.e12)) * 1.e-12 );
    }
    for(size_t i=0; i<4; ++i) {
      _mom[i] = (double)( ((signed long long)(_mom[i]*1.e12)) * 1.e-12 );
    }    
  }

}

#endif  
