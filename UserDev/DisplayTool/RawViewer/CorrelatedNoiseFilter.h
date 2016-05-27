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


  void fix_medium_angle_tracks(float * _plane_data,
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


  void find_correlated_noise_errors();


public:
  // These functions are temporary to give access to the correlated noise info:
  const std::vector<std::vector<std::vector<float> > >&
  getCorrelatedNoiseWaveforms() const {return _correlatedNoiseWaveforms;}

  std::vector<std::vector<float > >
  getCorrelatedNoiseBlocks() const {
    std::vector<std::vector<float> > _blocks;
    _blocks.push_back(_detector_properties_interface.correlated_noise_blocks(0));
    _blocks.push_back(_detector_properties_interface.correlated_noise_blocks(1));
    _blocks.push_back(_detector_properties_interface.correlated_noise_blocks(2));
    return _blocks;
  }




private:

  /*some private functions here are used in correcting the correlated noise waveforms*/



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





  const std::vector<float> _max_wire_lengths = { 456.9, 465.9, 233.0};

};

}

#endif
/** @} */ // end of doxygen group

