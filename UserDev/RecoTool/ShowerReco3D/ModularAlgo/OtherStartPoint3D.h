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
/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

class OtherStartPoint3D : ShowerRecoModuleBase {

public:

  /// Default constructor
  OtherStartPoint3D() {_name = "OtherStartPoint3D"; _coords.resize(7); _initDist = 50; }

  /// Default destructor
  ~OtherStartPoint3D() {}

  /// Inherited/overloaded function from ShowerRecoModuleBase
  void do_reconstruction(const ProtoShower &, Shower_t &);

  /// Pass center point-- you have initDist
  std::vector<std::vector<double>> calculatePoints(const std::vector<double> & centerPoint, const double & dist) ;

  bool inTPC( const TVector3 & pt );


private:

  std::vector<double> _xCoords ;
  std::vector<double> _yCoords ;
  std::vector<double> _zCoords ;

  std::vector<std::vector<double>> _coords ;

  double _initDist ;

};

} // showerreco

#endif
/** @} */ // end of doxygen group

