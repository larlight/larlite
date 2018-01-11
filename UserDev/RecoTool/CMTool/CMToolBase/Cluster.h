/**
 * \file Cluster.h
 *
 * \ingroup CMToolBase
 * 
 * \brief Class def header for a class Cluster
 *
 * @author david
 */

/** \addtogroup CMToolBase

    @{*/
#ifndef CLUSTER_H
#define CLUSTER_H

#include <iostream>
#include <vector>

#include "TwoDimTools/Linearity.h"

/**
   \class Cluster
   User defined class Cluster ... these comments are used to generate
   doxygen documentation!
 */

namespace cluster {

  class pt{

  public:

    pt(){}
    ~pt(){}

    pt(float r, float a, float w, float t, float q, int pl)
      { _r = r; _a = a; _w = w; _t = t; _q = q; _pl = pl; }

    float _r; // radius [cm]
    float _a; // angle [angle degrees]
    float _w; // wire [cm]
    float _t; // time [cm]
    float _q; // charge [ADCs]
    int   _pl; // plane
    
  };

  class anglespan{

  public:

    anglespan(){ _amin = 0.; _amax = 0.;}
    ~anglespan(){}

    anglespan(float amin, float amax){ _amin = amin; _amax = amax; }

    float _amin;
    float _amax;

    bool inRange(float angle, float buffer=0.) const {

      // if span does not cross 2Pi boundary:
      if (_amin < _amax) {
	if ( (angle > (_amin - buffer) ) && (angle < (_amax + buffer) ) ) return true;
      }
      else { // if angle crosses 2Pi boundary
	if ( (angle > 180.) && (angle > (_amin - buffer) ) ) return true;
	if ( (angle < 180.) && (angle < (_amax + buffer) ) ) return true;
      }

      return false;
    }
    
  };
  
  class Cluster {
    
  public:
    
    /// Default constructor
    Cluster(){}
    
    /// Default destructor
    ~Cluster(){}

    size_t _plane;
    float  _sum_charge;
    float  _angle;
    ::cluster::anglespan _angle_span;
    float  _angle_rms;
    cluster::pt _start_pt;
    cluster::pt _end_pt;

    ::twodimtools::Linearity _lin;

    const std::vector<cluster::pt> GetHits() const { return _hits; }
    
    int SetHits(const std::vector<cluster::pt>& hits) ;

    size_t size() const { return _hits.size(); }

    float Length() const { return _end_pt._r - _start_pt._r; }

  private:

    void Clear();

    bool QuadMixup(const std::vector<::cluster::pt>& pt_v);

    std::vector<cluster::pt> _hits;
    
  };

}
#endif
/** @} */ // end of doxygen group 

