/**
 * \file GetAverages3D.h
 *
 * \ingroup ClusterRecoUtil
 *
 * \brief Class def header for a class GetAverages3D
 *
 * @author cadams
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef GETAVERAGES3D_H
#define GETAVERAGES3D_H

#include <iostream>
#include "Cluster3DRecoUtil/Base/Params3DAlgBase.h"

namespace cluster3D {

/**
   \class GetAverages3D
   User defined class GetAverages3D ... these comments are used to generate
   doxygen documentation!
 */
class GetAverages3D : public ParamsAlgBase {

public:

  /// Default constructor
  GetAverages3D() {_name = "GetAverages3D";}

  /// Default destructor
  ~GetAverages3D() {}

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

