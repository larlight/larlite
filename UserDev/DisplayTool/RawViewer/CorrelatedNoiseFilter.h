/**
 * \file CorrelatedNoiseFilter.h
 *
 * \ingroup RawViewer
 * 
 * \brief Class def header for a class CorrelatedNoiseFilter
 *
 * @author cadams
 */

/** \addtogroup RawViewer

    @{*/
#ifndef CORRELATEDNOISEFILTER_H
#define CORRELATEDNOISEFILTER_H

#include <iostream>

/**
   \class CorrelatedNoiseFilter
   User defined class CorrelatedNoiseFilter ... these comments are used to generate
   doxygen documentation!
 */
class CorrelatedNoiseFilter{

public:

  /// Default constructor
  CorrelatedNoiseFilter(){}

  /// Default destructor
  ~CorrelatedNoiseFilter(){}

  /**
   * @brief Calculate the median tick of this block of wires
   * @details Loops over the block of wires and evaluates what the median tick is.
   * 
   * @param _data_arr A set of wires, one after the other in a 1D array
   * @param n_wires Total number of wires in the block
   * @param n_ticks_per_wire Number of ticks per wire
   * @param plane Plane
   * @param start_wire The wire number of the first wire in this block
   */
  void store_median_tick(float * _data_arr, int n_wires, int n_ticks_per_wire, int plane, int start_wire);

private:

  // This algorithm needs to store the accumulated data for the correlated noise subtraction.

  // std::vector < std::vector <float> >

};

#endif
/** @} */ // end of doxygen group 

