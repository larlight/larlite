/**
 * \file EMShowerContainment.h
 *
 * \ingroup EMShowerContainment
 * 
 * \brief Class def header for a class EMShowerContainment
 *
 * @author everyone
 */

/** \addtogroup EMShowerContainment

    @{*/
#ifndef BASICTOOL_EMSHOWERCONTAINMENT_H
#define BASICTOOL_EMSHOWERCONTAINMENT_H

// C++ includes
#include <iostream>

// ROOT includes
#include "TVector3.h"

/**
   \class EMShowerContainment
   User custom class made by SHELL_USER_NAME
 */
class EMShowerContainment {

  // constructor
  EMShowerContainment(const double detwidth = 256.35,
		      const double detheight = 233.0,
		      const double detlength = 1036.8);

  // destructor
  virtual ~EMShowerContainment() {}

  // returns true if a 3D position [cm] is inside the TPC
  bool IsInsideTPC(double const& x, double const& y, double const& z);

  // returns vector from the given position [cm] to TPC boundary along the given direction
  TVector3 VectorToTPCBoundary(TVector3 const& position, TVector3 const& direction);

  // returns corrected energy given an uncorrected energy and distance [cm] to TPC boundary from start point of shower
  double CorrectedEnergy(double const& energy, TVector3 const& distance);

 protected:

  //larutil::Geometry* geom = (larutil::Geometry*) (larutil::Geometry::GetME());

  double _detector_length;
  double _detector_width;
  double _detector_height;

  double _tpc_boundary_lower_x;
  double _tpc_boundary_lower_y;
  double _tpc_boundary_lower_z;

  double _tpc_boundary_upper_x;
  double _tpc_boundary_upper_y;
  double _tpc_boundary_upper_z;

  std::vector< double > _poly_coefficients;
  double _distance_threshold;

  double _polynomial(std::vector< double > const& coefficients, double const& x);

};

#endif
/** @} */ // end of doxygen group 

