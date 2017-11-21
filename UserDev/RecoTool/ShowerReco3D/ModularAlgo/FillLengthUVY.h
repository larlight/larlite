/**
 * \file FillLengthUVY.h
 *
 * \ingroup ModularAlgo
 *
 * \brief Class def header for a class FillLengthUVY
 *
 * @author me
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef FILLLENGTHUVY_H
#define FILLLENGTHUVY_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
*/
namespace showerreco {

  class FillLengthUVY : ShowerRecoModuleBase {

  public:

    /// Default constructor
    FillLengthUVY();

    /// Default destructor
    ~FillLengthUVY() {}

    void do_reconstruction(const ::protoshower::ProtoShower &, Shower_t &);

  private:

  };

} // showerreco

#endif
/** @} */ // end of doxygen group

