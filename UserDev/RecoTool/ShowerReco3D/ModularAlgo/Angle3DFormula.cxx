#ifndef ANGLE3DFORMULA_CXX
#define ANGLE3DFORMULA_CXX

#include "Angle3DFormula.h"
#include "LArUtil/GeometryHelper.h"

namespace showerreco {

void Angle3DFormula::do_reconstruction(const ::protoshower::ProtoShower & proto_shower,
                                       Shower_t& resultShower) {


  //if the module does not have 2D cluster info -> fail the reconstruction
  if (!proto_shower.hasCluster2D()){
    std::stringstream ss;
    ss << "Fail @ algo " << this->name() << " due to missing 2D cluster";
    throw ShowerRecoException(ss.str());
  }

  auto & clusters = proto_shower.params();

  auto geomHelper = larutil::GeometryHelper::GetME();

  // use geometry helper function to go from 2 2D angles to 1 3D angle
  // do this for all plane combinations available

  if (_verbose)
    std::cout << std::endl;

  // store theta & phi angles
  std::vector<double> theta_v;
  std::vector<double> phi_v;

  for (size_t i = 0; i < clusters.size(); i++) {
    for (size_t j = i + 1; j < clusters.size(); j++) {

      auto const& cl1 = clusters[i];
      auto const& cl2 = clusters[j];

      double theta, phi;

      if (_verbose)
        std::cout << "expamining cluster pair (" << i << ", " << j << ")" << std::endl;


      if (_verbose) {
        std::cout << "input angles : " << std::endl
                  << "\t " << cl1.angle_2d << " @ plane " << cl1.plane_id.Plane << std::endl
                  << "\t " << cl2.angle_2d << " @ plane " << cl2.plane_id.Plane << std::endl;
      }

      geomHelper->Get3DAxisN((int)cl1.plane_id.Plane, (int)cl2.plane_id.Plane,
                             cl1.angle_2d, cl2.angle_2d,
                             phi, theta);

      if (_verbose)
        std::cout << "return angle 3D : phi -> " << phi << ", theta -> " << theta << std::endl;

      // if we are returned the garbage values that the function returns...
      // very ugly...
      if ( (phi == 0) or (theta == -999) )
        continue;

      phi_v.push_back(phi);
      theta_v.push_back(theta);

    }// for index j
  }// for index i

  // get the average theta & phi
  double theta, phi = 0.;
  double theta_err, phi_err = 0.;
  // if no values actually used -> return
  if (theta_v.size() == 0)
    return;
  // if only 1 pair of clusters was used -> use their return values
  else if (theta_v.size() == 1) {
    theta = theta_v[0];
    phi   = phi_v[0];
  }
  // if 2 pairs were used -> average
  else if (theta_v.size() == 2) {
    // average the 2 results
    theta = (theta_v[0] + theta_v[1]) / 2.;
    phi   = (phi_v[0] + phi_v[1]) / 2.;
    theta_err = sqrt ( (theta_v[0] - theta) * (theta_v[0] - theta) +
                       (theta_v[1] - theta) * (theta_v[1] - theta) );
    phi_err   = sqrt ( (phi_v[0] - phi) * (phi_v[0] - phi) +
                       (phi_v[1] - phi) * (phi_v[1] - phi) );
  }// if 2 pairs used
  else if (theta_v.size() == 3) {
    // create new vectors where to store the angles measured (if they are good)
    std::vector<double> theta_good;
    std::vector<double> phi_good;
    std::vector<double> theta_err_good;
    std::vector<double> phi_err_good;
    for (size_t i = 0; i < theta_v.size(); i++) {
      for (size_t j = i + 1; j < theta_v.size(); j++) {
        // average the 2 results
        theta = (theta_v[i] + theta_v[j]) / 2.;
        phi   = (phi_v[i] + phi_v[j]) / 2.;
        theta_err = sqrt ( (theta_v[i] - theta) * (theta_v[i] - theta) +
                           (theta_v[j] - theta) * (theta_v[j] - theta) );
        phi_err   = sqrt ( (phi_v[i] - phi) * (phi_v[i] - phi) +
                           (phi_v[j] - phi) * (phi_v[j] - phi) );
        // if the angle errors are within tolerance
        if ( (theta_err < _max_err) and (phi_err < _max_err) ) {
          theta_good.push_back(theta);
          phi_good.push_back(phi);
          theta_err_good.push_back(theta_err);
          phi_err_good.push_back(phi_err);
        }// if the angle errors are within tolerance
      }// for all j elements
    }// for all i elements
    // if the new vectors are empty -> return
    if (theta_good.size() == 0)
      return;
    // otherwise, average the results
    theta = phi = theta_err = phi_err = 0.;
    for (size_t n = 0; n < theta_good.size(); n++) {
      theta += theta_good[n];
      phi   += phi_good[n];
    }
    theta /= theta_good.size();
    phi   /= phi_good.size();
    for (size_t n = 0; n < theta_good.size(); n++) {
      theta_err += (theta_good[n] - theta) * (theta_good[n] - theta);
      phi_err += (phi_good[n] - phi) * (phi_good[n] - phi);
    }
    theta_err = sqrt(theta_err);
    phi_err   = sqrt(phi_err);
  }// if using 3 pairs

  if (_verbose)
    std::cout << "final angles : phi -> " << phi << ", theta -> " << theta << std::endl;

  resultShower.fDCosStart[1] = sin(theta);
  resultShower.fDCosStart[0] = cos(theta) * sin(phi);
  resultShower.fDCosStart[2] = cos(theta) * cos(phi);

  return;
}


} //showerreco

#endif
