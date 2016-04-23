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
#include "AnalysisAlg/CalorimetryAlg.h"
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

  void do_reconstruction(const ::protoshower::ProtoShower &, Shower_t &);

  void initialize();

protected:
  
  //This variable will contain the average dQdx from the start of a shower projected into the 3 planes.
  //it is inteded to be filled such that element 0 contains the average dQdx for the U-plane,
  //element 1 contains the same for the V-plane, and element 2 will be for the Y-plane.
  std::vector<double> fdQdx;
  int    _pl_best;// Now set as Y plane
  //electron life time correction
  double _tau; // electron lifetime in usec
  double _timetick; // sampling size in usec

  /// Calorimetry algorithm
  ::calo::CalorimetryAlg _caloAlg;

  // debugging tree
  double _dqdx, _dqdx_u, _dqdx_v, _dqdx_y;
  int    _n_hits_u, _n_hits_v, _n_hits_y;
  double _pitch;
  double _trunk_l_u, _trunk_l_v, _trunk_l_y;
  double _px, _py, _pz;

};

} // showerreco

#endif
/** @} */ // end of doxygen group

