#ifndef LARLITE_MCTRAJECTORY_CXX
#define LARLITE_MCTRAJECTORY_CXX

#include "mctrajectory.h"

namespace larlite{

  mcstep::mcstep(const TLorentzVector& p, const TLorentzVector& m)
    : _position(p), 
      _momentum(m)
  {}
  
  mcstep::mcstep(const mcstep& orig) : _position(orig._position),
				       _momentum(orig._momentum)
  {}
  
  const TLorentzVector& mcstep::Momentum() const {return _momentum;}
  
  const TLorentzVector& mcstep::Position() const {return _position;}
  
  double mcstep::X()  const {return _position.X();  }
  double mcstep::Y()  const {return _position.Y();  }
  double mcstep::Z()  const {return _position.Z();  }
  double mcstep::T()  const {return _position.T();  }
  double mcstep::Px() const {return _momentum.Px(); }
  double mcstep::Py() const {return _momentum.Py(); }
  double mcstep::Pz() const {return _momentum.Pz(); }
  double mcstep::E()  const {return _momentum.E();  }

  void mctrajectory::push_back(const larlite::mcstep& step){ std::vector<larlite::mcstep>::push_back(step);}
  
  void mctrajectory::push_back(const TLorentzVector p,
			       const TLorentzVector m)
  { std::vector<larlite::mcstep>::push_back(larlite::mcstep(p,m));}

  
}

#endif
