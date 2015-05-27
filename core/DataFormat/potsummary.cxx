#ifndef POTSUMMARY_CXX
#define POTSUMMARY_CXX

#include "potsummary.h"

namespace larlite {

  void potsummary::clear_data()
  { 
    subrun_base::clear_data();
    //totpot = totgoodpot = 0;
    //totspills = goodspills = 0;
  }

}

std::ostream& operator<< (std::ostream &o, larlite::potsummary const& a)
{
  o << "This sub run has " << a.totspills
    << " total spills with an exposure of "
    << a.totpot << " POT" <<std::endl
    <<" with cuts on beam quality, there are " << a.goodspills
    <<" good spills with an exposure of " << a.totgoodpot
    <<std::endl;

  return o;
}

#endif
