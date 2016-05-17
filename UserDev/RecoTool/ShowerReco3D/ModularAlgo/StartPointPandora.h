/**
 * \file StartPointPandora.h
 *
 * \ingroup ModularAlgo
 *
 * \brief Class def header for a class StartPointPandora
 *
 * @author cadams
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef STARTPOINTPANDORAMODULE_H
#define STARTPOINTPANDORAMODULE_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

class StartPointPandora : ShowerRecoModuleBase {

public:

  /// Default constructor
  StartPointPandora();

  /// Default destructor
  ~StartPointPandora() {}

  void do_reconstruction(const ::protoshower::ProtoShower &, Shower_t &);

  TVector3 PointProjectedToLine( const TVector3& point, const TVector3& line );
  double DCAPointToLine( const TVector3& p, const TVector3& c, const TVector3& m );

  void SetRange( double range ) { _range = range; }

private:

  double _range;
};

} // showerreco

#endif
/** @} */ // end of doxygen group

