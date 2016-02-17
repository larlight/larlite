/**
 * \file Find3DShoweringPoint.h
 *
 * \ingroup ClusterRecoUtil
 *
 * \brief Class def header for a class Find3DShoweringPoint
 *
 * @author david caratelli
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef FIND3DSHOWERINGPOINT_H
#define FIND3DSHOWERINGPOINT_H

#include <iostream>
#include "Cluster3DRecoUtil/Base/Params3DAlgBase.h"

namespace cluster3D {

/**
   \class Find3DShoweringPoint
   This algorithm aims to calculate the 3D showering point of a shower
   tis is the point where a shower branches out.
 */
class Find3DShoweringPoint : public ParamsAlgBase {

public:

  /// Default constructor
  Find3DShoweringPoint() {_name = "Find3DShoweringPoint";}

  /// Default destructor
  ~Find3DShoweringPoint() {}

  /**
     Calculates the following variables:
     mean_x
     mean_y
     mean_z
     rms_x
     rms_y
     rms_z
     eigenvalue_principal
     eigenvalue_secondary
     eigenvalue_tertiary
     principal_dir
     secondary_dir
     tertiary_dir
     N_points
    */
  void do_params_fill(cluster3D_params &);

};

} // cluster3D


#endif
/** @} */ // end of doxygen group

