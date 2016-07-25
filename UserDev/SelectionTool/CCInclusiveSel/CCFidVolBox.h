/**
 * \file CCFidVolBox.h
 *
 * \ingroup CCInclusive
 * 
 * \brief General fiducial volume box object used by lots of other things, 
 *        best to define it all in one place so as not to copy/paste a lot
 *
 * @author davidkaleko
 */

/** \addtogroup CCInclusive

    @{*/
#ifndef LARLITE_CCFIDVOLBOX_H
#define LARLITE_CCFIDVOLBOX_H

#include <iostream>
#include "GeoAlgo/GeoAABox.h"
#include "LArUtil/Geometry.h"

/**
   \class CCFidVolBox
   User defined class CCFidVolBox ... these comments are used to generate
   doxygen documentation!
 */
namespace larlite{

class CCFidVolBox : public ::geoalgo::AABox {

public:

  /// Default constructor
  CCFidVolBox();

  /// Default destructor
  ~CCFidVolBox(){}

};
}
#endif
/** @} */ // end of doxygen group 

