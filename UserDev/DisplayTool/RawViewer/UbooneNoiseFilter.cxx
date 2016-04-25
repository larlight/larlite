#ifndef UBOONENOISEFILTER_CXX
#define UBOONENOISEFILTER_CXX

#include "UbooneNoiseFilter.h"

namespace larlite {

unsigned int detPropFetcher::n_wires(unsigned int plane) {
  if (plane == 0)
    return 2400;
  if (plane == 1)
    return 2400;
  if (plane == 2)
    return 3456;
}
unsigned int detPropFetcher::n_planes() {
  return 3;
}

}

namespace ub_noise_filter {

UbooneNoiseFilter::UbooneNoiseFilter() {

  _pedestal_by_plane.resize(_detector_properties_interface.n_planes());
  _rms_by_plane.resize(_detector_properties_interface.n_planes());
  _wire_status_by_plane.resize(_detector_properties_interface.n_planes());
  _chirp_info_by_plane.resize(_detector_properties_interface.n_planes());

  for (int plane = 0; plane < _detector_properties_interface.n_planes(); plane ++) {
    _pedestal_by_plane.at(plane).resize(_detector_properties_interface.n_wires(plane));
    _rms_by_plane.at(plane).resize(_detector_properties_interface.n_wires(plane));
    _wire_status_by_plane.at(plane).resize(_detector_properties_interface.n_wires(plane));
  }
}

void UbooneNoiseFilter::set_data(std::vector<std::vector<float> > * _input_data) {

  if (!_input_data) {
    throw std::runtime_error("NoiseFilterException: Bad pointer to data set provided.");
  }
  else {
    _data_by_plane = _input_data;
  }

}

void UbooneNoiseFilter::reset_internal_data() {
  // Clear the internal data:

  // Clear the pedestal data without needing a reallocation:
  for (auto & _ped_plane : _pedestal_by_plane) {
    for (auto & _wire_ped : _ped_plane) {
      _wire_ped = 0;
    }
  }

  // Similarly, clear the rms data:
  for (auto & _rms_plane : _rms_by_plane) {
    for (auto & _wire_rms : _rms_plane) {
      _wire_rms = 0;
    }
  }

  // Similarly, clear the rms data:
  for (auto & _wire_status_plane : _wire_status_by_plane) {
    for (auto & _status : _wire_status_plane) {
      _status = kNormal;
    }
  }


  // Clear the chirping data, but since it's a map
  // and it's sparse delete the past data.
  for (auto & _chirp_map : _chirp_info_by_plane) {
    _chirp_map.clear();
  }


}

void UbooneNoiseFilter::set_n_time_ticks(unsigned int _n_time_ticks) {
  this->_n_time_ticks_data = _n_time_ticks;
}


void UbooneNoiseFilter::clean_data() {

  // Make sure everything is cleaned up:
  reset_internal_data();

  // This function cleans the data up.
  //
  // First, do pedestal subtraction and determine if the wire is chirping:

  for (unsigned int plane = 0; plane < _detector_properties_interface.n_planes(); plane ++) {

    // Loop over the wires within the plane
    for (unsigned int wire = 0; wire < _detector_properties_interface.n_wires(plane); wire ++) {


      size_t offset = wire * _n_time_ticks_data;

      // Very first step:  determine if this wire is chirping.

      // The wire in question is from _data_by_plane[plane][offset]
      // to _data_by_plane[plane][offset + _n_time_ticks_data]
      float * _wire_arr = &(_data_by_plane->at(plane).at(offset));
      bool wire_is_chirping = is_chirping(_wire_arr,
                                          _n_time_ticks_data,
                                          wire,
                                          plane);

      // Do the full pedestal subtraction:
      // get_pedestal_info(_wire_arr, _n_time_ticks_data, wire, plane);

      // apply_moving_average(_wire_arr, _n_time_ticks_data, wire, plane);

      // rescale_by_rms(_wire_arr, _n_time_ticks_data, wire, plane);

    }


  }

  // Print out the chirping info for a known wire:
  std::cout << "Wire 1696 chirp start: " << _chirp_info_by_plane[1][1696].chirp_start << std::endl;
  std::cout << "Wire 1696 chirp stop: " << _chirp_info_by_plane[1][1696].chirp_stop << std::endl;
  std::cout << "Wire 1696 chirp frac: " << _chirp_info_by_plane[1][1696].chirp_frac << std::endl;


  return;
}

void UbooneNoiseFilter::get_pedestal_info(float * _data_arr, int N, int wire, int plane) {

  int min_subrange_n = 500;
  int min_submedian_n = 101; ///KEEP THIS NUMBER ODD!

  int start_tick = 0;
  int end_tick = N;


  if (_chirp_info_by_plane[plane].find(wire) != _chirp_info_by_plane[plane].end()) {
    // this wire IS chirping, so only use the good range:
    start_tick = _chirp_info_by_plane[plane][wire].chirp_start;
    end_tick = _chirp_info_by_plane[plane][wire].chirp_stop;
  }
  else {
    // Then this wire is not chirping, use the whole range for pedestal subtraction

  }

  int total_ticks = end_tick - start_tick;

  int n_ranges = total_ticks / min_subrange_n;

  // If there are less than the min number of ticks, set pedestal and rms to zero
  // and declare this wire dead.

  if (total_ticks < min_subrange_n) {
    _pedestal_by_plane[plane][wire] = 0.0;
    _rms_by_plane[plane][wire] = 0.0;
    _wire_status_by_plane[plane][wire] = kDead;
    return;
  }


  // Loop over the n ranges and sample evenly within each range to
  // accumulate a list of medians

  std::vector<float> median_list;


  for (int i = 0; i < n_ranges; i ++) {
    std::vector<float> _median_accumulator;
    _median_accumulator.reserve(min_submedian_n);
    int step_size = min_subrange_n / min_submedian_n;
    int this_start_tick = start_tick + min_subrange_n * i;
    int this_end_tick = start_tick + min_subrange_n * (i + 1);
    for (int tick = this_start_tick; tick < this_end_tick; tick += step_size) {
      _median_accumulator.push_back(_data_arr[tick]);
    }
    // Now get the median:
    std::nth_element(
      _median_accumulator.begin(),
      _median_accumulator.begin() +
      _median_accumulator.size() / 2,
      _median_accumulator.end() );
    median_list.push_back(_median_accumulator.at(_median_accumulator.size() / 2) );
  }

  // Now, take the mean of the medians as the pedestal:
  _pedestal_by_plane[plane][wire] = std::accumulate(median_list.begin(), median_list.end(), 0.0) / (float) n_ranges;


  // Now, go through and do the pedestal subtraction.
  // Calculate the rms as it goes:


  if (_pedestal_by_plane[plane][wire] != _pedestal_by_plane[plane][wire]) {
    std::cout << "nan encountered in plane " << plane << " on wire " << wire << "!" << std::endl;
    exit(-1);
  }

  float rms_accumulator = 0;
  int n_rms;
  int n_rms_max = 400;
  int step_size = total_ticks / n_rms_max;
  for (int tick = start_tick; tick < end_tick; tick ++) {
    _data_arr[tick] -= _pedestal_by_plane[plane][wire];

    // Only take a few hundred points for the rms,
    // and skip around so that they aren't all from the same spot
    if (tick % step_size == 0) {
      rms_accumulator += _data_arr[tick] * _data_arr[tick];
      n_rms ++;
    }
  }

  _rms_by_plane[plane][wire] = sqrt(rms_accumulator / n_rms);

  // Here, do the cuts on being dead, high RMS, etc.

  if (_rms_by_plane[plane][wire] < _lowRMS_cutoff) {
    _wire_status_by_plane[plane][wire] = kDead;
  }
  if (_rms_by_plane[plane][wire] > _highRMS_cutoff) {
    _wire_status_by_plane[plane][wire] = kHighNoise;
    // Zero out the wire:
    for (int tick = start_tick; tick < end_tick; tick ++) {
      _data_arr[tick] = 0.0;
    }
  }



}


void UbooneNoiseFilter::rescale_by_rms(float * _data_arr, int N, unsigned int wire, unsigned int plane) {

  // If the wire status is normal or chirping, rescale the wire by the rms to set the RMS to 1.0


  if (_wire_status_by_plane[plane][wire] == kNormal) {
    float scale = 1.0 / _rms_by_plane[plane][wire];
    for (int tick = 0; tick < N; tick ++) {
      _data_arr[tick] *= scale;
    }
  }
}

// void UbooneNoiseFilter::build_correlated_noise_waveforms(
//   float * _plane_arr,
//   int n_wires,
//   int n_ticks_per_wire,
//   int block_size)
// {

// }


void UbooneNoiseFilter::apply_moving_average(
  float * _data_arr,
  int N,
  int wire,
  int plane)
{

  // // Skip if the wire is dead
  // if (_wire_status_by_plane[plane][wire] == kDead) {
  //   return;
  // }

  int start_tick = 0;
  int end_tick = N;


  if (_chirp_info_by_plane[plane].find(wire) != _chirp_info_by_plane[plane].end()) {
    // this wire IS chirping, so only use the good range:
    start_tick = _chirp_info_by_plane[plane][wire].chirp_start;
    end_tick = _chirp_info_by_plane[plane][wire].chirp_stop;
  }
  else {
    // Then this wire is not chirping, use the whole range for zig zag removal

  }

  // Each bin is replaced by the average of it and the next bin;
  // Default is 2 bins to remove zig zag noise.

  int i;
  for (int tick = start_tick; tick < end_tick - 1 ; tick ++) {

    _data_arr[tick] = 0.5 * (_data_arr[tick] + _data_arr[tick + 1]);

  }


}



bool UbooneNoiseFilter::is_chirping(float * _data_arr,
                                    int N,
                                    unsigned int wire,
                                    unsigned int plane) {
  // Run through the chirp filter stuff here.
  //

  // This function comes first, and determines if the waveform is chirping or not.
  if (_chirp_filter.ChirpFilterAlg(_data_arr, N)) {

    _chirp_info_by_plane[plane][wire] = _chirp_filter.get_current_chirp_info();
    _wire_status_by_plane[plane][wire] = kChirping;
    // then this channel is chirping, and we deal with it.
    // _chirp_filter.ZigzagFilterAlg(_data_arr, N);
    _chirp_filter.RawAdaptiveBaselineAlg(_data_arr, N);
    _chirp_filter.RemoveChannelFlags(_data_arr, N);
    return true;
  } else {
    // _chirp_filter.ZigzagFilterAlg(_data_arr, N);
    return false;

  }

}


}

#endif
