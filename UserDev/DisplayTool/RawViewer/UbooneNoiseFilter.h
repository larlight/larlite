/**
 * \file UbooneNoiseFilter.h
 *
 * \ingroup RawViewer
 *
 * \brief Class def header for a class UbooneNoiseFilter
 *
 * This filter is a comprehensive noise filter for Microboone.
 *
 * It is optimized for speed and designed to leave signals intact
 * while removing noise waveforms.  In particular it does:
 *   - On the fly, wire by wire pedestal subtraction
 *   - Harmonic Noise removal on a plane by plane basis
 *   - Correlated noise removal on a service board by service board basis
 *     -- This subtraction uses 3 plane information and so should affect
 *        tracks that are parallel to the time ticks in a single plane.
 *
 * To do this algorithm quickly, and with flexibility, it goes over the data
 * in multiple passes.  First, it must pedestal subtract each wire or the other
 * noise effects can not be cleanly removed.  In this stage, wires are also
 * classified as one of the following:
 *   kNormal   - no observed issues with the wire
 *   kLowRMS   - the wire has lower than expected RMS, indicating problems
 *   kHighRMS  - the wire has higher than expected RMS
 *   kChirping - the wire is observed to be in a chirping state
 *
 * For chirping wires, the waveform that is chirping is zeroed while the
 * rest of the waveform is kept in tact.
 *
 * Next, it determines the noise waveforms for the correlated noise.  Since
 * this involves a median across a set of ticks, which are not quite in consecutive
 * memory, this part of the algorithm is the slowest.
 *
 * While it is possible to subtract the correlated noise waveforms from the ticks
 * on the fly, this is not done.  Instead we remove the harmonic noise component
 * from the correlated noise, and then build a correlated noise waveform across
 * the planes.
 *
 * Finally, the harmonic noise and the correlated noise are subtracted from the
 * waveforms, and the filtering can return.
 *
 * While this class doesn't save the noise waveforms, pedestals, etc, it does
 * provide access to these metrics for a calling class to save if desired.
 *
 * @author cadams
 * @author mrmooney?
 */

/** \addtogroup RawViewer

    @{*/
#ifndef UBOONENOISEFILTER_H
#define UBOONENOISEFILTER_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

#include "ChirpFilter.h"

/**
   \class UbooneNoiseFilter
   User defined class UbooneNoiseFilter ... these comments are used to generate
   doxygen documentation!
 */

namespace larlite {

class detPropFetcher {

public:

  unsigned int n_wires(unsigned int plane);
  unsigned int n_planes();

};

}

namespace ub_noise_filter {

enum wireStatus {kNormal, kDead, kHighNoise, kChirping, kNStatus};


// This chirp_info class is meant to package chirping
// data up into a small but flexible container.

class UbooneNoiseFilter {


public:

  /// Default constructor
  UbooneNoiseFilter();

  /// Default destructor
  ~UbooneNoiseFilter() {}

  // Set the pointer to the data.  Only basic integrity checks are done,
  // but it's the calling class's responsibility to manage this memory.
  // In
  /**
   * @brief Set pointer to the data to be cleaned
   * @details Pass a pointer to a vector<vector<float> > of uboone data
   *          Assumption is the index is [plane][wire1 wire2 ... wireN]
   *          That is, the interior vector is a concatenation of all the wires
   *          in a plane.
   *          This is the natural format for fast visualization in python (underlying
   *          C style array), hence the not trivial format.
   *
   * @param _wire_data A Pointer to the vector<vector<float> >.  The calling class
   *                   is responsible for the memory management of this data.
   */
  void set_data(std::vector<std::vector<float> > * _wire_data);

  /**
   * @brief This function activates the noise filter and runs the subroutines
   *        in the proper way
   * @details See the top of the header file, UbooneNoiseFilter.h, for a detailed
   *          description of the order of noise removal operations
   */
  void clean_data();

  /**
   * @brief Declare how many time ticks are in the wire data
   * @details Declare how many time ticks are in a single wire.
   *          If not called, the algorithm will assume a value of 9595
   *          which is the value in the uboone data stream.
   *
   * @param _n_time_ticks Unsigned integer representing the length, in
   *                      time ticks, of a wire.
   */
  void set_n_time_ticks(unsigned int _n_time_ticks);


private:


  /**
   * @brief Applying a moving average to this wire.
   * @details Every bin gets replaced by the average of it and it's
   *          next neighbor.  This essentially downsamples the wire.
   *          This is targeted to remove zig zag noise.
   *          Wire and plane info is used to deal with chirping wires
   *
   * @param _data_arr The array of data to smooth
   * @param N The number of ticks to smooth
   * @param wire The wire number
   * @param plane The plane number
   */
  void apply_moving_average(float * _data_arr, int N, int wire, int plane);

  /**
   * @brief Calculate the pedestal and it's width
   * @details Calculates the mean of median pedestals.
   *          This gets the median of ~20 regions of the wire,
   *          and takes the mean of them to achieve sub integer
   *          precision.
   *
   * @param _data_arr The wire data
   * @param N Number of ticks in the wire
   * @param wire Wire number
   * @param plane Plane number
   */
  void get_pedestal_info(float * _data_arr, int N, int wire, int plane);

  /**
   * @brief Reset all of the internal data to prepare for the next event.
   * @details This function clears internally stored data such as pedestals,
   *          RMS, chirping status, etc.
   */
  void reset_internal_data();


  /**
   * @brief Loop over the wires and determine the pedestal, rms, chirping
   *        status
   * @details For each wire, compute the pedestal, rms, and determine if the wire
   *          is chirping.  If the wire is chirping, the pedestal and RMS should
   *          only be calculated for the non chirping part.
   */
  void determine_wire_info();


  /**
   * @brief Get the correlation between two vectors of float
   * @details Computes the correlation using the standard method:
   *          C = sum_O^N (x_i - <x>) * (y_i - <y>) / (sigma_x * sigma_y * N)
   *
   * @param r [description]
   * @return [description]
   */
  float get_correlation(const std::vector<float> &, const std::vector<float> &);

  /**
   * @brief Get the median of a vector as possible
   * @details The median calculation is a slow task - naively it requires sorting
   *          the data, but that is a very slow process.  Instead, this function
   *          uses std::nth_element to find the median more quickly.
   *
   *          It could be optimized further.
   *
   * @param vals A vector of floating point numbers to find the median for.
   * @return  The median of the input vector.  If the input size is even, this may not
   *          be a value in the input array
   */
  float get_median(const std::vector<float> & vals);


  /**
   * @brief Determine if a waveform is chirping.
   * @details This function takes an input waveform, _data_arr,
   *          and determines if chirping occurs anywhere along it.
   *          If it does, it fills the internal data member _chirp_info_by_plane
   *          at [plane][wire], while if it's not chirping _chirp_info_by_plane
   *          is not modified.
   *
   *          The pointer to the data is the address of the start of the
   *          wire in question.  So, if the wire is chirping, this function will
   *          correct the chirping as much as possible
   *
   * @param _data_arr Pointer to array of data.  This points to the internal
   *                  data array so modify with caution.
   * @param N Length of the wire segment, nominally equal to _n_time_ticks.
   *          Remove this input variable?
   * @param wire The wire number of this waveform, used to fill _chirp_info_by_plane
   * @param plane The plane of this waveform, used to fill _chirp_info_by_plane
   * @return Boolean, true if the wire is chirping and false if it isn't.
   */
  bool is_chirping(float * _data_arr, int N, unsigned int wire, unsigned int plane);

  void rescale_by_rms(float * _data_arr, int N, unsigned int wire, unsigned int plane);


  void build_correlated_noise_waveforms(float * _plane_arr, int n_wires, int n_ticks_per_wire, int block_size)


  /*
  Below are the private data members, many of which can be accessed with getter
  functions.
   */

private:

  const float _lowRMS_cutoff = 0.4 * 0.4;
  const float _highRMS_cutoff = 10.0 * 10.0;

  // This is the chirping removal algorithm:
  ChirpFilter _chirp_filter;

  // This objects holds the correlated noise waveforms by [plane][wireblock][tick]
  std::vector<std::vector<std::vector<float> > > correlatedNoiseWaveforms;

  // All of the detector properties are encapsulated in this object
  // This allows larlite <-> larsoft transitions to be a little less painful
  larlite::detPropFetcher _detector_properties_interface;


  // This class filters noise out so it needs access to the data.  It does the
  // filtering in place so it takes a pointer to the data.
  std::vector<std::vector<float> > * _data_by_plane;

  unsigned int _n_time_ticks_data;
  unsigned int _n_planes;
  std::vector<unsigned int> _n_wires_per_plane;

  std::vector<std::vector<float> > _pedestal_by_plane;
  std::vector<std::vector<float> > _rms_by_plane;

  // This object contains the chirping info
  // Since chirping is not really a common occurrence,
  // this is store in a map to allow sparseness
  std::vector<std::map<int, ::ub_noise_filter::chirp_info> > _chirp_info_by_plane;

  // This vector deals with the wire status:
  std::vector<std::vector<wireStatus> > _wire_status_by_plane;

};

} // evd


#endif
/** @} */ // end of doxygen group

