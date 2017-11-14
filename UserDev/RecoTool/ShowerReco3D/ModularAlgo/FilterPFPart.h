/**
 * \file FilterPFPart.h
 *
 * \ingroup ModularAlgo
 *
 * \brief Class def header for a class FilterPFPart
 *
 * @author david caratelli
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef FILTERPFPART_H
#define FILTERPFPART_H

#include <iostream>
#include "ShowerRecoModuleBase.h"

#include <sstream>

/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

class FilterPFPart : ShowerRecoModuleBase {

public:

  /// Default constructor
  FilterPFPart() {_name = "FilterPFPart";}

  /// Default destructor
  ~FilterPFPart() {}


  void do_reconstruction(const ::protoshower::ProtoShower &, Shower_t &);

  void setMinNHitsAbsolute(int n) { _min_nhits_absolute = n; }
  void setMinNHitsLargest (int n) { _min_nhits_largest  = n; }

private:

  size_t _min_nhits_absolute, _min_nhits_largest;

};

} // showerreco

#endif
/** @} */ // end of doxygen group

