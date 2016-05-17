/**
 * \file CorrelatedNoiseFilter.h
 *
 * \ingroup RawViewer
 *
 * \brief Class def header for a class CorrelatedNoiseFilter
 *
 *  This class manages both the correlated and harmonic noise removals.
 *
 * @author cadams
 */

/** \addtogroup RawViewer

    @{*/
#ifndef CORRELATEDNOISEFILTER_H
#define CORRELATEDNOISEFILTER_H

#include <iostream>
#include <vector>
#include <map>

#include "NoiseFilterTypes.h"

/**
   \class CorrelatedNoiseFilter
   User defined class CorrelatedNoiseFilter ... these comments are used to generate
   doxygen documentation!
 */

namespace ub_noise_filter {

class CorrelatedNoiseFilter {

public:

  /// Default constructor
  CorrelatedNoiseFilter() {}

  /// Default destructor
  ~CorrelatedNoiseFilter() {}


  /**
   * @brief Remove the correlated and harmonic noise from this wire.
   * @details Uses the correlated and harmonic noise waveforms to clean up this wire
   *
   * @param _data_arr Input data array
   * @param N Number of ticks on this wire
   * @param int wire number
   * @param int plane number
   */
  void remove_correlated_noise(float * _data_arr,
                               int N,
                               unsigned int wire,
                               unsigned int plane);


  /**
   * @brief Reset the algorithm
   * @details Clear internal stored data for correlated and harmonic noise
   */
  void reset();

  /**
   * @brief Constructs the correlate noise waveforms
   * @details Iterates over each plane and build up the harmonic and correlated noise waveforms
   *
   * @param _plane_data Array of data in each plane
   * @param plane plane number
   * @param _n_time_ticks_data length of each wire in the data, in ticks
   */
  void build_noise_waveforms(float * _plane_data,
                             int plane,
                             int _n_time_ticks_data);


  /**
   * @brief Builds the harmonic noise waveform for this plane
   * @details Iterates over the wires in this plane to construct the base
   *          harmonic noise waveform.  Only uses constant length wires.
   *
   * @param _plane_data Array of data in each plane
   * @param plane plane number
   * @param _n_time_ticks_data length of each wire in the data, in ticks
   */
  void build_harmonic_noise_waveform(float * _plane_data,
                                     int plane,
                                     int _n_time_ticks_data);

  /**
   * @brief Builds the coherent/correlated noise waveform for this plane
   * @details Iterates over the wires in this plane, broken into motherboard-wide
   *          segments, to construct the base coherent/correlated noise waveform.
   *
   * @param _plane_data Array of data in each plane
   * @param plane plane number
   * @param _n_time_ticks_data length of each wire in the data, in ticks
   */
  void build_coherent_noise_waveforms(float * _plane_data,
                                      int plane,
                                      int _n_time_ticks_data);



  void set_wire_status_pointer(std::vector<std::vector<wireStatus> > * _ptr) {
    _wire_status_by_plane = _ptr;
  }


  void set_chirp_info_pointer(std::vector<std::map<int, ::ub_noise_filter::chirp_info> >  * _ptr) {
    _chirp_info_ptr = _ptr;
  }

public:
  // These functions are temporary to give access to the correlated noise info:
  const std::vector<std::vector<std::vector<float> > >&
  getCorrelatedNoiseWaveforms() const {return _correlatedNoiseWaveforms;}

  const std::vector<std::vector<float> >
  getCorrelatedNoiseBlocks() const {return _correlated_noise_blocks;}




private:

  template <class T>
  T getMode(const std::vector<T> &, T lowbin, T highbin, int n_bins);


  /**
   * @brief Returns the median of a vector
   * @details Finds the nth_element of a vector.  This function can and will change the input.
   *
   * @tparam T input list of values
   * @return The median of the vector
   */
  template <class T>
  T getMedian( std::vector<T> &);

  template <class T>
  T getMean(const std::vector<T> &);

  // All of the detector properties are encapsulated in this object
  // This allows larlite <-> larsoft transitions to be a little less painful
  ub_noise_filter::detPropFetcher _detector_properties_interface;


  // This algorithm needs to store the accumulated data for the correlated noise subtraction.

  std::vector<std::vector<std::vector<float> > > _correlatedNoiseWaveforms;

  // Also store the harmonic noise waveforms for each plane:
  std::vector<std::vector<float> > _harmonicNoiseWaveforms;

  std::vector<std::map<int, ::ub_noise_filter::chirp_info> >  * _chirp_info_ptr;


  // This is a pointer to the wire status vector from the main noise filter algorithm
  std::vector<std::vector<wireStatus> >  * _wire_status_by_plane;


  // This defines what blocks to use for correlated noise removal.
  // It's every 48 wires in induction, 96 in collection
  std::vector<std::vector<float> > _correlated_noise_blocks = {
    {
      0, 48, 96, 144, 192, 240, 288, 336, 384, 432, 480,
      528, 576, 624, 672, 720, 768, 816, 864, 912, 960,
      1008, 1056, 1104, 1152, 1200, 1248, 1296, 1344, 1392,
      1440, 1488, 1536, 1584, 1632, 1680, 1728, 1776, 1824,
      1872, 1920, 1968, 2016, 2064, 2112, 2160, 2208,
      2256, 2304, 2352, 2400
    },
    {
      0, 48, 96, 144, 192, 240, 288, 336, 384, 432, 480,
      528, 576, 624, 672, 720, 768, 816, 864, 912, 960,
      1008, 1056, 1104, 1152, 1200, 1248, 1296, 1344, 1392,
      1440, 1488, 1536, 1584, 1632, 1680, 1728, 1776, 1824,
      1872, 1920, 1968, 2016, 2064, 2112, 2160, 2208,
      2256, 2304, 2352, 2400
    },
    {
      0, 96, 192, 288, 384, 480, 576, 672, 768, 864, 960,
      1056, 1152, 1248, 1344, 1440, 1536, 1632, 1728, 1824,
      1920, 2016, 2112, 2208, 2304, 2400, 2496, 2592, 2688,
      2784, 2880, 2976, 3072, 3168, 3264, 3360, 3456
    }
  };


  const std::vector<float> _max_wire_lengths = { 456.9, 465.9, 233.0};

};

}

#endif
/** @} */ // end of doxygen group

