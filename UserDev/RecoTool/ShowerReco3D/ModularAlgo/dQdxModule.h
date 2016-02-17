/**
 * \file dQdxModule.h
 *
 * \ingroup ModularAlgo
 *
 * \brief Class def header for a class dQdx2DModule
 *
 * @authors David Caratelli, Nevis, dcaratelli@nevis.columbia.edu
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef DQDXMODULE_H
#define DQDXMODULE_H
#include <iostream>
#include "ShowerRecoModuleBase.h"

//#include "AnalysisAlg/CalorimetryAlg.h"

/**
   \class dQdxModule : ShowerRecoModuleBase
   This is meant to compute the 2D dQdx along the start of the shower.
 */
namespace showerreco {

class dQdxModule : ShowerRecoModuleBase {

public:

  /// Default constructor
  dQdxModule();

  /// Default destructor
  ~dQdxModule() {}

  void do_reconstruction(const ProtoShower &, Shower_t &);

  void initialize();

private:
  //This variable will contain the average dQdx from the start of a shower projected into the 3 planes.
  //it is inteded to be filled such that element 0 contains the average dQdx for the U-plane,
  //element 1 contains the same for the V-plane, and element 2 will be for the Y-plane.
  std::vector<double> fdQdx;
  int    _pl_best;// Now set as Y plane
  // ADC -> e- conversion factors
  double _fC_to_e;
  double _ADC_to_mV;
  double _shp_time;
  double _asic_gain;
  double _charge_conversion;
  //electron life time correction
  double _tau; // electron lifetime in usec
  double _timetick; // sampling size in usec

};

} // showerreco

#endif
/** @} */ // end of doxygen group

