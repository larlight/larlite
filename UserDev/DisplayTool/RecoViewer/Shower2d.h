/**
 * \file Shower2d.h
 *
 * \ingroup RecoViewer
 * 
 * \brief Class def header for a class Shower2d
 *
 * @author cadams
 */

/** \addtogroup RecoViewer

    @{*/
#ifndef SHOWER2D_H
#define SHOWER2D_H

#include <iostream>
#include "TVector2.h"

/**
   \class Shower2d
   User defined class Shower2d ... these comments are used to generate
   doxygen documentation!
 */
namespace evd {

  class Shower2d{

  public:

    /// Default constructor
    Shower2d(){}

    /// Default destructor
    ~Shower2d(){}

    int plane(){return _plane;}
    TVector2 startPoint(){return _startPoint;}
    float angleInPlane(){return _angleInPlane;}
    float openingAngle(){return _openingAngle;}
    float length(){return _length;}
    bool is_good(){return _is_good;}
    float dedx(){return _dedx;}

    // ALL OF THESE VARIABLES ARE THE PROJECTION INTO THE PLANE
    int _plane;                ///< The Plane of the shower
    TVector2 _startPoint;      ///< Wire time start point (units in cm)
    float _angleInPlane;       ///< Angle in the plane
    float _openingAngle;       ///< Opening angle
    float _length;             ///< Length in cm
    bool _is_good;             ///< Whether or not the projection succeeded
    float _dedx;                ///< dedx in collection plane, for printout
  };

} // evd
#endif
/** @} */ // end of doxygen group 

