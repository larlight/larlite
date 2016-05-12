#ifndef PCA3DAXISMODULE_CXX
#define PCA3DAXISMODULE_CXX

#include "PCA3DAxis.h"
#include "TMath.h"
// #include <algorithm>

namespace showerreco {

void PCA3DAxis::do_reconstruction(const ::protoshower::ProtoShower & proto_shower,
                                  Shower_t& resultShower) {

  // auto & clusters = proto_shower.params();


  // This is really simple.  Read in the 3D params and use that to
  // set the 3D axis of the shower.  Right now, no checks against the
  // 2D projection but should be added somewhere.

  // If there is no 3D params, throw an exception.
  //
  if (proto_shower.hasCluster3D()) {

    // Set the centroid
    resultShower.fCentroid.SetXYZ( proto_shower.params3D().mean_x, proto_shower.params3D().mean_y,
                                   proto_shower.params3D().mean_z );
    resultShower.fSigmaCentroid.SetXYZ( proto_shower.params3D().rms_x, proto_shower.params3D().rms_y,
                                        proto_shower.params3D().rms_z );

    resultShower.fDCosStart = proto_shower.params3D().principal_dir;
  }
  else {
    throw ShowerRecoException("PCA3DAxis requires 3D cluster but has none.");
  }
}

} //showerreco

#endif
