#ifndef SELECTIONTOOL_SPARTICLE_CXX
#define SELECTIONTOOL_SPARTICLE_CXX

#include "SPArticle.h"

namespace sptool {

  SPArticle::SPArticle() 
    : _pos(3)
    , _mom(3)
  { Init(); }

  void SPArticle::Init() {

    _pdg_code = kINVALID_INT;

    for(size_t i=0; i<3; ++i) {
      _pos[i] = kINVALID_DOUBLE;
      _mom[i] = kINVALID_DOUBLE;
    }

    _energy = _score = kINVALID_DOUBLE;

    _daughters.clear();
  }

}

#endif
