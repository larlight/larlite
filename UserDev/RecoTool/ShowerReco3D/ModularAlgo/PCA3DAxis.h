/**
 * \file PCA3DAxis.h
 *
 * \ingroup ModularAlgo
 *
 * \brief Class def header for a class PCA3DAxis
 *
 * @author cadams
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef PCA3DMODULE_H
#define PCA3DMODULE_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

class PCA3DAxis : ShowerRecoModuleBase {

public:

  /// Default constructor
  PCA3DAxis() {_name = "PCA3DAxis";}

  /// Default destructor
  ~PCA3DAxis() {}


  void do_reconstruction(const ProtoShower &, Shower_t &);

  TVector3 PointProjectedToLine( const TVector3& point, const TVector3& line );

private:

};

} // showerreco

#endif
/** @} */ // end of doxygen group

