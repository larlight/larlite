#ifndef UBOONENOISEFILTER_CXX
#define UBOONENOISEFILTER_CXX

#include "UbooneNoiseFilter.h"

namespace evd{

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
  for(auto & _ped_plane : _pedestal_by_plane){
    for (auto & _wire_ped : _ped_plane){
      _wire_ped = 0;
    }
  }

  // Similarly, clear the rms data:
  for(auto & _rms_plane : _rms_by_plane){
    for (auto & _wire_rms : _rms_plane){
      _wire_rms = 0;
    }
  }

  // Clear the chirping data, but since it's a map
  // and it's sparse delete the past data.
  for (auto & _chirp_map : _chirp_info_by_plane){
    _chirp_map.clear();
  }


}

void UbooneNoiseFilter::set_n_time_ticks(unsigned int _n_time_ticks){
  this->_n_time_ticks = _n_time_ticks;
}

}

#endif
