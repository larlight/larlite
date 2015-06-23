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

    // ALL OF THESE VARIABLES ARE THE PROJECTION INTO THE PLANE
    int plane;                ///< The Plane of the shower
    TVector2 startPoint;      ///< Wire time start point (units in cm)
    float angleInPlane;       ///< Angle in the plane
    float openingAngle;       ///< Opening angle
    float length;             ///< Length in cm
  };

} // evd
#endif
/** @} */ // end of doxygen group 

