/**
 * \file FillLength.h
 *
 * \ingroup ModularAlgo
 *
 * \brief Class def header for a class FillLength
 *
 * @author david caratelli
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef FILLLENGTH_H
#define FILLLENGTH_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

class FillLength : ShowerRecoModuleBase {

public:

  /// Default constructor
  FillLength();

  /// Default destructor
  ~FillLength() {}


  void do_reconstruction(const ::protoshower::ProtoShower &, Shower_t &);

private:

};

} // showerreco

#endif
/** @} */ // end of doxygen group

