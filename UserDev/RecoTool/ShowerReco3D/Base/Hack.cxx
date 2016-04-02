//by vic

#ifndef HACK_CXX
#define HACK_CXX

#include "Hack.h"

namespace showerreco {

  Hack::Hack() {
    _toffset = 3200;
    _w2cm = 0.3;
    _t2cm = 0.05;

  }

  double Hack::_povtx(unsigned char plane) {
    auto _plane = int{plane};

    if ( _plane == 0 ) return  0.0;
    if ( _plane == 1 ) return -0.3;
    if ( _plane == 2 ) return -0.6;

    printf("plane not 0,1,2");
    throw std::exception();
  }

  
  ::larutil::PxPoint Hack::convertCSP(const ::larlite::cluster& c) {

    auto plane = c.Plane().Plane;
    auto sW    = c.StartWire();
    auto sT    = c.StartTick();

    sW *= _w2cm;
    sT  = ( sT - _toffset ) * _t2cm + _povtx(plane);
    
    return ::larutil::PxPoint(plane,sW,sT);
  }

  ::larutil::PxPoint Hack::convertCEP(const ::larlite::cluster& c) {

    auto plane = c.Plane().Plane;
    auto eW    = c.EndWire();
    auto eT    = c.EndTick();

    eW *= _w2cm;
    eT  = ( eT - _toffset ) * _t2cm + _povtx(plane);
    
    return ::larutil::PxPoint(plane,eW,eT);
  }

  
 
}

#endif
