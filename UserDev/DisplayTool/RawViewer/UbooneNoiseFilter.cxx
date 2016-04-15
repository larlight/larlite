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

namespace evd {

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

  // This function cleans the data up.
  //
  // First, do pedestal subtraction and determine if the wire is chirping:

  for (unsigned int plane = 0; plane < _detector_properties_interface.n_planes(); plane ++) {

    unsigned int offset = 0;

    std::vector<float> pedestal_vector;
    std::vector<float> rms_vector;
    size_t n_pedestal_points = 151;  // keep this an odd number!!

    // Loop over the wires within the plane
    for (unsigned int wire = 0; wire < _detector_properties_interface.n_wires(plane); wire ++) {

      pedestal_vector.clear();
      rms_vector.clear();
      size_t offset = wire * _n_time_ticks_data;

      // Very first step:  determine if this wire is chirping.

      // The wire in question is from _data_by_plane[plane][offset]
      // to _data_by_plane[plane][offset + _n_time_ticks_data]
      bool wire_is_chirping
        = is_chirping(&(_data_by_plane->at(plane).at(offset)),
                      _n_time_ticks_data,
                      wire,
                      plane);

      if ( wire_is_chirping) {
        // Calculate the pedestal and rms only in the good regions:

      }
      else {
        // Remove the pedestal with the whole wire

        ///\ TODO
        /// Replace this with code that sorts as it fills and that should speed things up.

        // Grab points along the wire:
        int step = _n_time_ticks_data / n_pedestal_points;
        // std::cout << "Step is " << step << std::endl;
        for (int i = 0; i < n_pedestal_points; i ++) {
          // std::cout << "index is " << index << std::endl;
          pedestal_vector.push_back(_data_by_plane->at(plane)[offset + i * step]);
          rms_vector.push_back(_data_by_plane->at(plane)[offset + (i+1) * step]);
        }
        // Get the nth element
        std::nth_element(pedestal_vector.begin(),
                         pedestal_vector.begin() + pedestal_vector.size() / 2,
                         pedestal_vector.end());
        _pedestal_by_plane[plane][wire] = pedestal_vector[pedestal_vector.size() / 2];

        // Now do the pedestal subtraction:
        for (int i = 0; i < _n_time_ticks_data; i ++) {
          _data_by_plane->at(plane)[offset + i] -= _pedestal_by_plane[plane][wire];
          // _rms_by_plane[plane][wire] +=
          //   _data_by_plane->at(plane)[offset + i] *
          //   _data_by_plane->at(plane)[offset + i];
        }

        // Subtract the pedestal from the RMS vector:
        for (auto & tick : rms_vector){
          // std::cout << "tick starts at " << tick;
          tick -= _pedestal_by_plane[plane][wire];
          tick = fabs(tick);
          // std::cout << " and ends at " << tick << std::endl;
        }

        // Get the nth element
        std::nth_element(rms_vector.begin(),
                         rms_vector.begin() + rms_vector.size() / 2,
                         rms_vector.end());
        _rms_by_plane[plane][wire] = rms_vector[rms_vector.size()/2];

        // std::cout << "Rms is " << _rms_by_plane[plane][wire] << std::endl;


        if (_rms_by_plane[plane][wire] > _highRMS_cutoff) {
          // Flag the wire as high RMS, and zero the wire:
          _wire_status_by_plane[plane][wire] = kHighRMS;
          for (int i = 0; i < _n_time_ticks_data; i ++) {
            _data_by_plane->at(plane)[offset + i] = 0;
          }
        }
        if (_rms_by_plane[plane][wire] < _lowRMS_cutoff) {
          // Flag the wire as high RMS, and zero the wire:
          _wire_status_by_plane[plane][wire] = kLowRMS;
          for (int i = 0; i < _n_time_ticks_data; i ++) {
            _data_by_plane->at(plane)[offset + i] = 0;
          }
        }
      }

    }
  }

  // Here, the pedestal is calculated.
  // Get the harmonic noise


  for (unsigned int plane = 0; plane < _detector_properties_interface.n_planes(); plane ++) {

    // Loop over the ticks, and take the median of central wires (500 to n_wires-500)

    std::vector<float> _harmonic_noise_waveform(_n_time_ticks_data, 0.0);
    std::cout << "_harmonic_noise_waveform.size() is " << _harmonic_noise_waveform.size() << std::endl;

    std::vector<float> _harmonic_noise_accumulator;

    for (int i_tick = 0; i_tick < _n_time_ticks_data; i_tick ++) {
      // std::cout << "Tick is " << i_tick << std::endl;
      _harmonic_noise_accumulator.clear();
      while (_harmonic_noise_accumulator.size() < 301) {
        // std::cout << "_harmonic_noise_accumulator.size(): " << _harmonic_noise_accumulator.size() << std::endl;
        int wire_index = 700 + rand() % (1700);
        if (_wire_status_by_plane[plane][wire_index] != kNormal) {
          continue;
        }
        size_t offset = wire_index * _n_time_ticks_data;
        _harmonic_noise_accumulator.push_back(_data_by_plane->at(plane).at(offset + i_tick));
      }

      std::nth_element(_harmonic_noise_accumulator.begin(),
                       _harmonic_noise_accumulator.begin() +
                       _harmonic_noise_accumulator.size() / 2,
                       _harmonic_noise_accumulator.end());
      _harmonic_noise_waveform.at(i_tick)
        = _harmonic_noise_accumulator.at(_harmonic_noise_accumulator.size() / 2);
    }

    // Now subtract out the harmonic noise:
    for (unsigned int wire = 700; wire < 1700; wire ++) {
    // for (unsigned int wire = 0; wire < _detector_properties_interface.n_wires(plane); wire ++) {

      if (_wire_status_by_plane[plane][wire] != kNormal)
        continue;
      size_t offset = wire * _n_time_ticks_data;
      for (int i = 0; i < _n_time_ticks_data; i ++) {
        _data_by_plane->at(plane).at(offset + i) -= _harmonic_noise_waveform.at(i);
      }
    }

  }


  return;
}

bool UbooneNoiseFilter::is_chirping(float * _data_arr,
                                    int N,
                                    unsigned int wire,
                                    unsigned int plane) {
  return false;
}


}

#endif
