//by vic

#ifndef HACK_H
#define HACK_H

#include "DataFormat/cluster.h"
#include "LArUtil/PxUtils.h"

namespace showerreco {

  struct Hack  {

    Hack();
    
    double _povtx(unsigned char plane);
    int _toffset;
    float _w2cm;
    float _t2cm;
    
    ::larutil::PxPoint convertCSP(const ::larlite::cluster& c);
    ::larutil::PxPoint convertCEP(const ::larlite::cluster& c);
    
  };
}

#endif
