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
    int pdg; // used for artificial track PID in ertoolhelper
    double time; // needed for xshift in ertoolhelper
  };

  struct Shower_t {
    ::geoalgo::Cone_t cone;
    double energy;
    double dedx; // [MeV/cm]
    int pdg; // used for artificially filling dedx for poorly reconstructed mcshowers
    double time; // needed for xshift in ertoolhelper
  };

}

#endif
/** @} */ // end of doxygen group
