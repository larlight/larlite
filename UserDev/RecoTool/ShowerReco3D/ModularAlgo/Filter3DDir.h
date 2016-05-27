/**
 * \file EmptyModule.h
 *
 * \ingroup ModularAlgo
 *
 * \brief Class def header for a class Filter3DDir
 *
 * @author david caratelli
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef FILTER3DDIR_H
#define FILTER3DDIR_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

class Filter3DDir : ShowerRecoModuleBase {

public:

  /// Default constructor
  Filter3DDir() {_name = "Filter3DDir";}

  /// Default destructor
  ~Filter3DDir() {}


  void do_reconstruction(const ::protoshower::ProtoShower &, Shower_t &);

  void SetDotProdCut(double d) { _dot_cut = d; }

private:

  double _dot_cut;

};

} // showerreco

#endif
/** @} */ // end of doxygen group

