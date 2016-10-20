#ifndef BASICTOOL_EMSHOWERCONTAINMENT_CXX
#define BASICTOOL_EMSHOWERCONTAINMENT_CXX

// class include
#include "EMShowerContainment.h"

// C++ includes
#include <cmath>

EMShowerContainment::EMShowerContainment(const double detwidth, const double detheight, const double detlength)
{

  // get dimensions of TPC
  _detector_length = detlength;
  _detector_width  = detwidth;
  _detector_height = detheight;

  // get lower boundaries of TPC
  _tpc_boundary_lower_x = 0;
  _tpc_boundary_lower_y = -_detector_height / 2.;
  _tpc_boundary_lower_z = 0;

  // get upper boundaries of TPC
  _tpc_boundary_upper_x = _detector_width;
  _tpc_boundary_upper_y = _detector_height / 2.;
  _tpc_boundary_upper_z = _detector_length;

  // parameters for the EM shower containment energy correction template
  // TODO: put this into a FHiCL file
  _poly_coefficients.push_back(7.01085709e-02);
  _poly_coefficients.push_back(3.36567579e-02);
  _poly_coefficients.push_back(-6.10882420e-04);
  _poly_coefficients.push_back(6.51185937e-06);
  _poly_coefficients.push_back(-4.14749121e-08);
  _poly_coefficients.push_back(1.53825311e-10);
  _poly_coefficients.push_back(-3.05252080e-13);
  _poly_coefficients.push_back(2.49971865e-16);

  // EM showers that have a distance to TPC boundary greater than this value will be assumed to have 100% containment
  // TODO: put this into a FHiCL file
  _distance_threshold = 300; // [cm]

}

bool EMShowerContainment::IsInsideTPC(double const& x, double const& y, double const& z)
{
  if (x >= _tpc_boundary_lower_x &&
      x <= _tpc_boundary_upper_x &&
      y >= _tpc_boundary_lower_y &&
      y <= _tpc_boundary_upper_y &&
      z >= _tpc_boundary_lower_z &&
      z <= _tpc_boundary_upper_z)
    return true;

  return false;
} // EMShowerContainment::IsInsideTPC()

TVector3 EMShowerContainment::VectorToTPCBoundary(TVector3 const& start, TVector3 const& direction)
{

  // normalize direction vector
  //TVector3 v(direction.X() / direction.Mag(), direction.Y() / direction.Mag(), direction.Z() / direction.Mag());
  //TVector3 v = direction.Unit();

  // parameters for parametric line
  std::vector< double > parameters;

  //////////////////////////////////////////////////////////
  // start getting parameter candidates for parametric line
  //////////////////////////////////////////////////////////
  try
  {
    double parameter = (_tpc_boundary_lower_x - start.X()) / direction.Unit().X();
    parameters.push_back(parameter);
  }
  catch (const std::exception& e)
  {
    std::cout << "Could not get parameter candidate for parametric line. Skipping coordinate..." << std::endl;
  }

  try
  {
    double parameter = (_tpc_boundary_upper_x - start.X()) / direction.Unit().X();
    parameters.push_back(parameter);
  }
  catch (const std::exception& e)
  {
    std::cout << "Could not get parameter candidate for parametric line. Skipping coordinate..." << std::endl;
  }

  try
  {
    double parameter = (_tpc_boundary_lower_y - start.Y()) / direction.Unit().Y();
    parameters.push_back(parameter);
  }
  catch (const std::exception& e)
  {
    std::cout << "Could not get parameter candidate for parametric line. Skipping coordinate..." << std::endl;
  }

  try
  {
    double parameter = (_tpc_boundary_upper_y - start.Y()) / direction.Unit().Y();
    parameters.push_back(parameter);
  }
  catch (const std::exception& e)
  {
    std::cout << "Could not get parameter candidate for parametric line. Skipping coordinate..." << std::endl;
  }

  try
  {
    double parameter = (_tpc_boundary_lower_z - start.Z()) / direction.Unit().Z();
    parameters.push_back(parameter);
  }
  catch (const std::exception& e)
  {
    std::cout << "Could not get parameter candidate for parametric line. Skipping coordinate..." << std::endl;
  }

  try
  {
    double parameter = (_tpc_boundary_upper_z - start.Z()) / direction.Unit().Z();
    parameters.push_back(parameter);
  }
  catch (const std::exception& e)
  {
    std::cout << "Could not get parameter candidate for parametric line. Skipping coordinate..." << std::endl;
  }
  //////////////////////////////////////////////////////////
  // end getting parameter candidates for parametric line
  //////////////////////////////////////////////////////////

  // initialize vector for TPC exit point
  TVector3 exit(0, 0, 0);

  // flag
  bool exit_point_found = false;

  // loop over parameter candidates
  for (auto const& parameter : parameters)
  {
    double const x = start.X() + parameter * direction.Unit().X();
    double const y = start.Y() + parameter * direction.Unit().Y();
    double const z = start.Z() + parameter * direction.Unit().Z();

    // check to see if this point lies within the TPC
    if (this->IsInsideTPC(x, y, z))
    {
      exit_point_found = true;
      exit.SetXYZ(x, y, z);
    } // if inside TPC
  } // loop over parameters

  TVector3 vector = exit - start;

  return vector;

} // EMShowerContainment::VectorToTPCBoundary()

double EMShowerContainment::CorrectedEnergy(double const& energy, TVector3 const& distance)
{

  // assume 100% containment if distance is greater than threshold
  if (distance.Mag() > _distance_threshold)
    return energy;

  double corrected_energy = 0;

  // get containment from parameterized function
  double containment = this->_polynomial(_poly_coefficients, distance.Mag());

  // attempt to correct energy
  try
  {
    corrected_energy = energy / containment;
  }
  catch (const std::exception& e)
  {
    std::cout << "Could not correct for containment..." << std::endl;
    corrected_energy = energy;
  }

  return corrected_energy;

} // EMShowerContainment::CorrectedEnergy()

double EMShowerContainment::_polynomial(std::vector< double > const& coefficients, double const& x)
{

  double result = 0;

  for (size_t i = 0; i < coefficients.size(); ++i)
  {
    result += coefficients[i] * std::pow(x, i);
  }

  return result;

} // EMShowerContainment::_polynomial()

#endif
