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

void CorrelatedNoiseFilter::fix_medium_angle_tracks(float * _plane_data,
    int plane,
    int _n_time_ticks_data)
{

  // Loop over every same plane pair of correlated waveforms.
  // For the sides of the TPC, do something different.
  //
  // measure the correlation between small blocks of waveforms that *ought*
  // to be very highly correlated
  //
  // If there is a big drop in the correlation, recalculate the correlated noise
  // waveform in that region

  std::vector< int > windows_to_fix;

  std::vector< int > windows_to_investigate;
  int windowsize = 200;
  int n_windows = _correlatedNoiseWaveforms[0][0].size() / windowsize;


  for (size_t i_block = 0;
       i_block < _detector_properties_interface.correlated_noise_blocks(plane).size();
       i_block ++)
  {

    int current_block = i_block;
    int matched_block = _detector_properties_interface.same_plane_pair(plane, i_block);

    auto & waveform1 = _correlatedNoiseWaveforms[plane][current_block];
    auto & waveform2 = _correlatedNoiseWaveforms[plane][matched_block];

    // Break it into blocks of N ticks
    float windowsize = 200;
    int n_windows = waveform1.size() / windowsize;

    // For each block, get the correlation of this block to the corresponding block
    // on the other cross correlated waveforms

    // keep track of which windows need to be investigated:


    std::vector<int> windows_to_investigate;
    std::vector<float> correlations;

    for (int i_window = 0; i_window < n_windows - 1; i_window ++ ) {
      float * _this_block_data = &(waveform1[i_window * windowsize]);
      float * _cross_data      = &(waveform2[i_window * windowsize]);
      float _corr = getCorrelation(_this_block_data, _cross_data, windowsize);

      if (_corr < 0.7) {

        windows_to_investigate.push_back(i_window);
        correlations.push_back(_corr);
      }
    }

    // Now fix the windows that are broken
    // This involves remaking the median estimate, for this window, using
    for (int i_window = 0; i_window < windows_to_investigate.size(); i_window ++ ) {
      int window = windows_to_investigate[i_window];
      // std::cout << "Looking at block of wires from "
      //           << _detector_properties_interface.correlated_noise_blocks(plane)[i_block]
      //           << " to "
      //           << _detector_properties_interface.correlated_noise_blocks(plane)[i_block + 1]
      //           << std::endl;
      // std::cout << "Ticks  " << windowsize*window <<  " to " << windowsize*(window + 1)
      //           << ", the correlation between "
      //           << "(" << plane << ", " << current_block << ") and "
      //           << "(" << plane << ", " << matched_block << ") is " << correlations[i_window] << std::endl;

      // Have to loop over every tick in the combined range, and recalculate the median.  Naturally,
      // that also involves subtracting harmonic noise too.

      int wire_start = std::min(_detector_properties_interface.correlated_noise_blocks(plane)[current_block],
                                _detector_properties_interface.correlated_noise_blocks(plane)[matched_block]);

      int wire_end = std::max(_detector_properties_interface.correlated_noise_blocks(plane)[current_block + 1],
                              _detector_properties_interface.correlated_noise_blocks(plane)[matched_block + 1]);


      // Actually break this into 4 different medians, and then we'll take the median of THAT
      std::vector<std::vector<float> > _median_accumulator;


      int offset;
      for (int tick = windowsize * window; tick < windowsize * (window + 1); tick ++) {
        _median_accumulator.clear();
        _median_accumulator.resize(5);
        // _median_accumulator.reserve(wire_end - wire_start);
        for (int wire = wire_start; wire < wire_end ; wire ++) {
          int n = (5 * (wire - wire_start)) / (wire_end  - wire_start);

          if (_wire_status_by_plane->at(plane)[wire] == kNormal) {
            offset = tick + wire * _n_time_ticks_data;
            // std::cout << "Accessing at wire " << wire << ", offset " << offset << std::endl;
            float scale = _detector_properties_interface.wire_scale(plane, wire);
            _median_accumulator.at(n).push_back(_plane_data[offset] -
                                                scale * _harmonicNoiseWaveforms[plane][tick]);
            // std::cout << "Success!" << std::endl;


          }

        }

        // std::cout << "_median_accumulator.size() " << _median_accumulator.size() << std::endl;

        // Now find the median of this tick:
        // if (_median_accumulator.size() < 8) {
        // continue;
        // }
        std::vector<float > medians;
        for (auto & vec : _median_accumulator) {
          if (_median_accumulator.size() > 4)
            medians.push_back(getMedian(vec));
        }

        std::sort (medians.begin(), medians.end());

        float _final_median = getMedian(medians);

        // Some care needs to be taken here.  Because there are clearly so many points
        // that have high charge, the median is already sure to be offset.

        // So, we can compute (approximately) the most probable value
        // in this list.  Then, compute the rms, and exclude all points
        // that are more than 1.5 sigma away from the mode.
        //
        // Then, compute the median, and use that.


        if (_final_median != 0.0) {
          _correlatedNoiseWaveforms.at(plane).at(current_block).at(tick)
            = _final_median;
          _correlatedNoiseWaveforms.at(plane).at(matched_block).at(tick)
            = _final_median;
        }

      }



    }

  }




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

  int target_plane = 0;
  int target_block = 22;

  auto correlated_blocks
    = _detector_properties_interface.service_board_block(target_plane, target_block);

  std::vector< int > windows_to_fix;

  std::vector< int > windows_to_investigate;
  int windowsize = 390;
  int n_windows = _correlatedNoiseWaveforms[target_plane][target_block].size() / windowsize;

  windows_to_investigate.resize(n_windows);

  for (size_t i_plane = 0;
       i_plane < correlated_blocks.size();
       i_plane ++)
  {
    for (size_t i_block = 0;
         i_block < correlated_blocks.at(i_plane).size();
         i_block ++)
    {

      int current_block = correlated_blocks.at(i_plane).at(i_block);

      if (i_plane == target_plane && current_block == target_block) {
        continue;
      }

      // Break it into blocks of 195 ticks

      // For each block, get the correlation of this block to the corresponding block
      // on the other cross correlated waveforms

      // keep track of which windows need to be investigated:

      for (int i_window = 0; i_window < n_windows; i_window ++ ) {
        float * _this_block_data = &(_correlatedNoiseWaveforms[target_plane][target_block][i_window * windowsize]);
        float * _cross_data = &(_correlatedNoiseWaveforms[i_plane][current_block][i_window * windowsize]);
        float _corr = getCorrelation(_this_block_data, _cross_data, windowsize);
        // std::cout << "Window " << i_window << ", the correlation between "
        //           << "(" << target_plane << ", " << target_block << ") and "
        //           << "(" << i_plane << ", " << current_block << ") is " << _corr << std::endl;
        if (_corr < 0.5) {
          windows_to_investigate.at(i_window) ++ ;
        }
      }



    }
  }


  // Print out what regions of this block are under revision:
  // std::cout << "Comparing at plane " << target_plane << " wires "
  //           << _detector_properties_interface.correlated_noise_blocks(target_plane).at(target_block)
  //           << " to "
  //           << _detector_properties_interface.correlated_noise_blocks(target_plane).at(target_block + 1)
  //           << " TO \n plane " << i_plane << " wires "
  //           << _detector_properties_interface.correlated_noise_blocks(i_plane).at(current_block)
  //           << " to "
  //           << _detector_properties_interface.correlated_noise_blocks(i_plane).at(current_block + 1)
  //           << std::endl;


  std::cout << "In plane " << target_plane << " wires "
            << _detector_properties_interface.correlated_noise_blocks(target_plane).at(target_block)
            << " to "
            << _detector_properties_interface.correlated_noise_blocks(target_plane).at(target_block + 1)
            << ": \n";
  for (int i_window = 0; i_window < windows_to_investigate.size(); i_window ++ ) {
    if (windows_to_investigate.at(i_window) > 1) {
      std::cout << "  Examine ticks " << i_window*windowsize
                << " to " << (i_window + 1)*(windowsize)
                << " (strength == " << windows_to_investigate.at(i_window) << ")"
                << std::endl;
      float prev_rms = 0.0;
      float rms = 0.0;
      float next_rms = 0.0;
      for (int tick = 0; tick < windowsize; tick ++) {
        if (i_window != 0) {
          prev_rms
          += _correlatedNoiseWaveforms[target_plane][target_block][windowsize * (i_window - 1) + tick] *
             _correlatedNoiseWaveforms[target_plane][target_block][windowsize * (i_window - 1) + tick];
        }
        rms
        += _correlatedNoiseWaveforms[target_plane][target_block][windowsize * (i_window) + tick] *
           _correlatedNoiseWaveforms[target_plane][target_block][windowsize * (i_window) + tick];

        if (i_window != n_windows - 1) {
          next_rms
          += _correlatedNoiseWaveforms[target_plane][target_block][windowsize * (i_window + 1) + tick] *
             _correlatedNoiseWaveforms[target_plane][target_block][windowsize * (i_window + 1) + tick];
        }
      }
      prev_rms /= (float) windowsize;
      rms /= (float) windowsize;
      next_rms /= (float) windowsize;
      std::cout << "    RMS  of previous window: " << prev_rms << "\n"
                << "    RMS  of this window: " << rms << "\n"
                << "    RMS  of next window: " << next_rms << "\n";

      if (
        ( i_window != 0 && rms > 3 * prev_rms) ||
        ( i_window != n_windows - 1 && rms > 3 * next_rms)
      ) {
        windows_to_fix.push_back(i_window);
        std::cout << "this window tagged to be fixed." << std::endl;
      }

    }
  }


}




}

#endif
