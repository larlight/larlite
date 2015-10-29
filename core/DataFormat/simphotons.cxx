// SimPhotons, OnePhoton and SimPhotonsCollection implementation.
//
// These objects are primarily storage containers, so not much
// function code is actually required.
//
// See comments at head of SimPhotons.h for more details.
//
// Ben Jones, MIT, 06/04/2010
//
/// \version $Id: ParticleHistory.cxx,v 1.1 2010/04/29 15:38:01 seligman Exp $

#include "simphotons.h"
#include <iostream>

namespace larlite
{

  onephoton::onephoton()
  {}

  simphotons::simphotons() : data_base(data::kSimPhotons)
  {}

  simphotons & simphotons::operator+=(const simphotons &rhs)
  {
    for(simphotons::const_iterator it = rhs.begin(); it!=rhs.end(); it++){
      push_back(*it);
    }
    return *this;
  }

  //----------------------------------------------------------------------------
  const simphotons simphotons::operator+(const simphotons &rhs) const
  {
    return simphotons(*this)+=rhs;
  }

}
