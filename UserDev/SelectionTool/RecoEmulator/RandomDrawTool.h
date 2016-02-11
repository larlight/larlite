/**
 * \file RandomDrawTool.h
 *
 * \ingroup RecoEmulator
 * 
 * \brief Class def header for a class RandomDrawTool
 *
 * @author david caratelli
 */

/** \addtogroup RecoEmulator

    @{*/
#ifndef RANDOMDRAWTOOL_H
#define RANDOMDRAWTOOL_H

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

/**
   \class RandomDrawTool
   * @brief tool that, given a distribution or parametrized function, samples randomly from it
   */
class RandomDrawTool{
  
public:

  /// Default constructor
  RandomDrawTool(){}

  /**
   * @brief constructor that sets the distribution values
   * @input values -> vector of values for parametrized function
   * @input min -> lower bound of x-axis for distrib values
   * @input max -> upper bound of x-axis for distrib values
   */
  RandomDrawTool(const std::vector<double>& vec,
		 const double& min, const double& max);

  /// Default destructor
  ~RandomDrawTool(){}

  /**
   * @brief set the distribution to be used
   * @input values -> vector of values for parametrized function
   * @input min -> lower bound of x-axis for distrib values
   * @input max -> upper bound of x-axis for distrib values
   */
  void SetDistribution(const std::vector<double>& values,
		       const double& min, const double& max);
  

  /**
   *@brief function to sample randomly from the distribution
   */
  double Draw();

 private:

  // distribution of values for the smearing function
  std::vector<double> _distribution;

  // value of bounds for the distribution
  double _xmin;
  double _xmax;

  /**
   * @brief function to normalize the distribution values
   */
  void Normalize(std::vector<double>& values);

  

};

#endif
/** @} */ // end of doxygen group 

