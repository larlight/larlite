#ifndef CORRELATEDNOISEFILTER_CXX
#define CORRELATEDNOISEFILTER_CXX

#include "CorrelatedNoiseFilter.h"

namespace ub_noise_filter {

void CorrelatedNoiseFilter::reset() {
  _correlatedNoiseWaveforms.clear();
  _correlatedNoiseWaveforms.resize(_detector_properties_interface.n_planes());
  for (size_t i = 0; i < _detector_properties_interface.n_planes(); i ++) {
    _correlatedNoiseWaveforms.at(i).resize(_detector_properties_interface.correlated_noise_blocks(i).size() - 1);
  }

  _harmonicNoiseWaveforms.clear();
  _harmonicNoiseWaveforms.resize(_detector_properties_interface.n_planes());


}


void CorrelatedNoiseFilter::remove_correlated_noise(
  float * _data_arr,
  int N,
  unsigned int wire,
  unsigned int plane) {

  if (_wire_status_by_plane->at(plane)[wire] == kDead ||
      _wire_status_by_plane->at(plane)[wire] == kHighNoise) {
    // Make sure all the ticks are zeroed out:
    for (int tick = 0; tick < N; tick ++) {
      _data_arr[tick] = 0.0;
    }
    return;
  }

  // First, need to know what block this wire came from:
  size_t i_block;
  for (i_block = 0;
       i_block < _detector_properties_interface.correlated_noise_blocks(plane).size();
       i_block ++)
  {
    if (_detector_properties_interface.correlated_noise_blocks(plane).at(i_block + 1) > wire) {
      // Then the block is the correct one!
      break;
    }
  }

  // Now subtract the waveform from this wire
  int start_tick = 0;
  int end_tick = N;



  if (_chirp_info_ptr -> at(plane).find(wire) != _chirp_info_ptr -> at(plane).end()) {
    // this wire IS chirping, so only use the good range:
    // Either start or the end of the wire will be one range of the chirping.
    if (_chirp_info_ptr -> at(plane)[wire].chirp_start == 0) {
      start_tick = _chirp_info_ptr -> at(plane)[wire].chirp_stop;
      end_tick = N;
    }
    else {
      start_tick = 0.0;
      end_tick = _chirp_info_ptr -> at(plane)[wire].chirp_start;
    }
  }
  else {
    // Then this wire is not chirping, use the whole range for pedestal subtraction

  }


  for (int tick = start_tick; tick < end_tick; tick ++) {
    // Scale the harmonic noise by wire length:
    _data_arr[tick] -=
      _detector_properties_interface.wire_scale(plane, wire) *
      _harmonicNoiseWaveforms[plane][tick];
    // _data_arr[tick] -= _harmonicNoiseWaveforms[plane][tick];
    _data_arr[tick] -= _correlatedNoiseWaveforms[plane][i_block][tick];
  }

  return;

}


void CorrelatedNoiseFilter::build_noise_waveforms(
  float * _plane_data,
  int plane,
  int _n_time_ticks_data)
{

  // Make sure each noise vector is set to zero when this function is called:
  for (size_t i_block = 0; i_block < _detector_properties_interface.correlated_noise_blocks(plane).size() - 1; i_block ++) {
    _correlatedNoiseWaveforms.at(plane).at(i_block).clear();
    _correlatedNoiseWaveforms.at(plane).at(i_block).resize(_n_time_ticks_data);
  }


  _harmonicNoiseWaveforms.at(plane).clear();
  _harmonicNoiseWaveforms.at(plane).resize(_n_time_ticks_data);

  // First build a guess at the harmonic noise waveforms:
  build_harmonic_noise_waveform(_plane_data, plane, _n_time_ticks_data);

  build_coherent_noise_waveforms(_plane_data, plane, _n_time_ticks_data);


}

void CorrelatedNoiseFilter::build_coherent_noise_waveforms(
  float * _plane_data,
  int plane,
  int _n_time_ticks_data)
{


  for (size_t i_block = 0;
       i_block < _detector_properties_interface.correlated_noise_blocks(plane).size() - 1;
       i_block ++) {
    _correlatedNoiseWaveforms.at(plane).at(i_block).clear();
    _correlatedNoiseWaveforms.at(plane).at(i_block).resize(_n_time_ticks_data);


    int block_wire_start
      = _detector_properties_interface.correlated_noise_blocks(plane).at(i_block);
    int block_wire_end
      = _detector_properties_interface.correlated_noise_blocks(plane).at(i_block + 1);

    std::vector<float> _median_accumulator;

    int offset;
    for (int tick = 0; tick < _n_time_ticks_data; tick ++) {
      _median_accumulator.clear();
      _median_accumulator.reserve(block_wire_end - block_wire_start);
      for (int wire = block_wire_start; wire < block_wire_end ; wire ++) {
        if (_wire_status_by_plane->at(plane)[wire] == kNormal) {
          offset = tick + wire * _n_time_ticks_data;
          // std::cout << "Accessing at wire " << wire << ", offset " << offset << std::endl;
          float scale = _detector_properties_interface.wire_scale(plane, wire);
          _median_accumulator.push_back(_plane_data[offset] -
                                        scale * _harmonicNoiseWaveforms[plane][tick]);
          // std::cout << "Success!" << std::endl;


        }
      }

      // std::cout << "_median_accumulator.size() " << _median_accumulator.size() << std::endl;

      // Now find the median of this tick:
      if (_median_accumulator.size() < 8) {
        continue;
      }
      _correlatedNoiseWaveforms.at(plane).at(i_block).at(tick)
        = getMedian(_median_accumulator);

    }



  }



  return;
}



void CorrelatedNoiseFilter::build_harmonic_noise_waveform(
  float * _plane_data,
  int plane,
  int _n_time_ticks_data)
{

  // Only consider wires that are full length wires:
  double _min_length = _max_wire_lengths[plane];

  std::vector<float> harmonic_noise;
  harmonic_noise.resize(_n_time_ticks_data);

  // Loop over the ticks, and then the wires, and get the most probable
  // value for the harmonic noise

  for (int tick = 0; tick < _n_time_ticks_data; tick ++) {

    std::vector<float> values;
    values.reserve(_detector_properties_interface.n_wires(plane));

    float lowVal = 999;
    float highVal = -999;

    for (unsigned int wire = 0;
         wire < _detector_properties_interface.n_wires(plane);
         wire += 10)
    {

      // Need to know which correlated noise block this wire is from:
      size_t i_block;
      for (i_block = 0;
           i_block < _detector_properties_interface.correlated_noise_blocks(plane).size();
           i_block ++)
      {
        if (_detector_properties_interface.correlated_noise_blocks(plane).at(i_block + 1) > wire) {
          // Then the block is the correct one!
          break;
        }
      }

      if (_detector_properties_interface.wire_length(plane, wire) >= _min_length) {
        int offset = wire * _n_time_ticks_data;
        auto _data_val = _plane_data[offset + tick] -
                         _correlatedNoiseWaveforms[plane][i_block][tick];
        values.push_back(_data_val);
        if (_data_val < lowVal) {
          lowVal = _data_val;
        }
        if (_data_val > highVal) {
          highVal = _data_val;
        }

      }
    }

    // Get the most probable value:
    // harmonic_noise.at(tick) = getMode(values, lowVal, highVal, 200);
    harmonic_noise.at(tick) = getMedian(values);
  }

  // Merge this information into the best guess for harmonic noise:
  _harmonicNoiseWaveforms.at(plane) = harmonic_noise;

}

void CorrelatedNoiseFilter::find_correlated_noise_errors() {

  // Here's the strategy:  Look at each correlated noise waveform and
  // its correlation to every other waveform that it has a strong correlation with.
  //
  // Practically, that means every wave form needs it's correlation to 5 other boards:
  //   The other motherboard in the same plane
  //   Both motherboards in both other planes
  //
  // Also, break each correlation into pieces of 200 ticks to help find anomalous correlations

  // For right now, only looking at known locations of problems

  // Let's just look at one block to see how the block making works

  int target_plane = 2;
  int target_block = 22;

  auto correlated_blocks
    = _detector_properties_interface.service_board_block(target_plane, target_block);
  // for (size_t i_plane = 0;
  //      i_plane < correlated_blocks.size();
  //      i_plane ++)
  // {
  //   std::cout << "correlated_blocks.at(" << i_plane << ").size() "
  //             << correlated_blocks.at(i_plane).size()
  //             << std::endl;
  // for (size_t i_plane = 0;
  //      i_plane < correlated_blocks.size();
  //      i_plane ++)
  // {
  //     int this_plane = i_plane;
  //     int this_block = correlated_blocks.at(i_plane).at(i_block);

  //     std::cout << "_correlatedNoiseWaveforms["
  //               << this_plane << "][" << this_block << "].size() "
  //               << _correlatedNoiseWaveforms[this_plane][this_block].size() << std::endl;
  //     std::cout << "_correlatedNoiseWaveforms["
  //               << target_plane << "][" << target_block << "].size() "
  //               << _correlatedNoiseWaveforms[target_plane][target_block].size() << std::endl;
  //     std::cout << "correlation between (" << target_plane << ", " << target_block
  //               << ") and (" << this_plane << ", " << this_block << ") is "
  //               << getCorrelation(_correlatedNoiseWaveforms[this_plane][this_block],
  //                                 _correlatedNoiseWaveforms[target_plane][target_block])
  //               << std::endl
  //               << std::endl;
  //   }
  // }

  for (size_t i_plane = 0;
       i_plane < correlated_blocks.size();
       i_plane ++)
  {
    for (size_t i_plane = 0;
         i_plane < correlated_blocks.size();
         i_plane ++)
    {

      // Break it into blocks of 

    }
  }

}




}

#endif
