#ifndef TOYGEOMODULE_CXX
#define TOYGEOMODULE_CXX

#include "ToyGeoModule.h"
#include "TVector3.h"

namespace showerreco {

void ToyGeoModule::do_reconstruction(const ProtoShower & proto_shower,
                                     Shower_t& resultShower) {
  
  // This function takes the shower cluster set and computes the best fit 3D axis
  // and then assigns it to the shower

  // Calculate the length and opening angle of the shower.

  // First, make sure the axis is pointing in the right direction
  // checkAxisDir(clusters, resultShower);

  if ( proto_shower.hasCluster3D() ) {
    // Length:
    resultShower.fLength = 3. * sqrt(proto_shower.params3D().eigenvalue_principal);
    // resultShower.fLength = proto_shower.params3D().eigenvalue_principal * proto_shower.params3D().N_points;

    // Get the opening angle:
    double openAngle = 0.;
    if ( proto_shower.params3D().eigenvalue_principal != 0. && proto_shower.params3D().eigenvalue_secondary != 0. ) {
      double Theta = proto_shower.params3D().principal_dir.Angle( proto_shower.params3D().secondary_dir );
      double sinTheta = sin( Theta );
      openAngle = 2.* atan( sqrt( proto_shower.params3D().eigenvalue_secondary ) * sinTheta / 
                            sqrt( proto_shower.params3D().eigenvalue_principal ) );

    }
    resultShower.fOpeningAngle = openAngle;
  }
  else{
    throw ShowerRecoException("ToyGeoModule requires 3D cluster but has none.");
  }

}

}
#endif
