/**
 * \file RecoEmulatorTypes.h
 *
 * \ingroup RecoEmulator
 *
 * \brief Class def header for a class RecoEmulatorTypes
 *
 * @author davidkaleko
 */

/** \addtogroup RecoEmulator

    @{*/
#ifndef RECOEMULATORTYPES_H
#define RECOEMULATORTYPES_H

#include "GeoAlgo/GeoTrajectory.h"
#include "GeoAlgo/GeoCone.h"
#include <iostream>

namespace recoemu {

  struct Track_t {
    ::geoalgo::Trajectory trajectory;
    double energy;
    std::vector<double> dedx;
  };

  struct Shower_t {
    ::geoalgo::Cone_t cone;
    double energy;
  };

}

#endif
/** @} */ // end of doxygen group
