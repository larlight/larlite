/**
 * \file OtherStartPoint3D.h
 *
 * \ingroup ModularAlgo
 *
 * \brief Class def header for a class OtherStartPoint3D
 *
 * @author cadams
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef OTHERSTARTPOINT3D_H
#define OTHERSTARTPOINT3D_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
#include "TVector3.h"

/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

class OtherStartPoint3D : ShowerRecoModuleBase {

public:

  /// Default constructor
  OtherStartPoint3D();

  /// Default destructor
  ~OtherStartPoint3D() {}

  /// Inherited/overloaded function from ShowerRecoModuleBase
  void do_reconstruction(const ProtoShower &, Shower_t &);

  /// Pass center point-- you have initDist
  std::vector<TVector3> calculatePoints(const TVector3 & centerPoint,
                                        const double & dist) ;



private:


  double _initDist ;

};

} // showerreco

#endif
/** @} */ // end of doxygen group

