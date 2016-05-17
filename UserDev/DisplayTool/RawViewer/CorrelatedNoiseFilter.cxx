#ifndef CORRELATEDNOISEFILTER_CXX
#define CORRELATEDNOISEFILTER_CXX

#include "CorrelatedNoiseFilter.h"

namespace ub_noise_filter {

void CorrelatedNoiseFilter::reset() {
  _correlatedNoiseWaveforms.clear();
  _correlatedNoiseWaveforms.resize(_correlated_noise_blocks.size());
  for (size_t i = 0; i < _correlated_noise_blocks.size(); i ++) {
    _correlatedNoiseWaveforms.at(i).resize(_correlated_noise_blocks.at(i).size() - 1);
  }

  _harmonicNoiseWaveforms.clear();
  _harmonicNoiseWaveforms.resize(_correlated_noise_blocks.size());


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
       i_block < _correlated_noise_blocks.at(plane).size();
       i_block ++)
  {
    if (_correlated_noise_blocks.at(plane).at(i_block + 1) > wire) {
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
      (_detector_properties_interface.wire_length(plane, wire) /
       _max_wire_lengths[plane]) *
      _harmonicNoiseWaveforms[plane][tick];
    // _data_arr[tick] -= _harmonicNoiseWaveforms[plane][tick];
    // _data_arr[tick] -= _correlatedNoiseWaveforms[plane][i_block][tick];
  }

  return;

}


void CorrelatedNoiseFilter::build_noise_waveforms(
  float * _plane_data,
  int plane,
  int _n_time_ticks_data)
{

  // // Make sure each noise vector is set to zero when this function is called:
  // for (size_t i_block = 0; i_block < _correlated_noise_blocks.at(plane).size(); i_block ++) {
  //   _correlatedNoiseWaveforms.at(plane).at(i_block).clear();
  //   _correlatedNoiseWaveforms.at(plane).at(i_block).resize(_n_time_ticks_data);
  // }


  // _harmonicNoiseWaveforms.at(plane).clear();
  // _harmonicNoiseWaveforms.at(plane).resize(_n_time_ticks_data);

  // First build a guess at the harmonic noise waveforms:
  build_harmonic_noise_waveform(_plane_data, plane, _n_time_ticks_data);

  // build_coherent_noise_waveforms(_plane_data, plane, _n_time_ticks_data);

  // build_harmonic_noise_waveform(_plane_data, plane, _n_time_ticks_data);

  // build_coherent_noise_waveforms(_plane_data, plane, _n_time_ticks_data);


}

void CorrelatedNoiseFilter::build_coherent_noise_waveforms(
  float * _plane_data,
  int plane,
  int _n_time_ticks_data)
{


  for (size_t i_block = 0; i_block < _correlated_noise_blocks.at(plane).size() - 1; i_block ++) {
    _correlatedNoiseWaveforms.at(plane).at(i_block).clear();
    _correlatedNoiseWaveforms.at(plane).at(i_block).resize(_n_time_ticks_data);


    int block_wire_start = _correlated_noise_blocks.at(plane).at(i_block);
    int block_wire_end = _correlated_noise_blocks.at(plane).at(i_block + 1);

    std::vector<float> _median_accumulator;

    int offset;
    for (int tick = 0; tick < _n_time_ticks_data; tick ++) {
      _median_accumulator.clear();
      _median_accumulator.reserve(block_wire_end - block_wire_start);
      for (int wire = block_wire_start; wire < block_wire_end ; wire ++) {
        if (_wire_status_by_plane->at(plane)[wire] == kNormal) {
          offset = tick + wire * _n_time_ticks_data;
          // std::cout << "Accessing at wire " << wire << ", offset " << offset << std::endl;
          float scale = _detector_properties_interface.wire_length(plane, wire) /
                        _max_wire_lengths[plane];
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

      // // Need to know which correlated noise block this wire is from:
      // size_t i_block;
      // for (i_block = 0;
      //      i_block < _correlated_noise_blocks.at(plane).size();
      //      i_block ++)
      // {
      //   if (_correlated_noise_blocks.at(plane).at(i_block + 1) > wire) {
      //     // Then the block is the correct one!
      //     break;
      //   }
      // }

      if (_detector_properties_interface.wire_length(plane, wire) >= _min_length) {
        int offset = wire * _n_time_ticks_data;
        auto _data_val = _plane_data[offset + tick] ;
                         // _correlatedNoiseWaveforms[plane][i_block][tick];
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


template <class T>
T CorrelatedNoiseFilter::getMode(const std::vector<T> & invec,
                                 T lowbin,
                                 T highbin,
                                 int n_bins) {

  std::vector<int> histogram;
  std::vector<T> lowerBounds;
  std::vector<T> upperBounds;

  histogram.resize(n_bins);
  lowerBounds.resize(n_bins);
  upperBounds.resize(n_bins);

  T bin_width = (highbin - lowbin) / n_bins;

  for (int i = 0; i < n_bins; i ++) {
    lowerBounds[i] = lowbin + i * bin_width;
    upperBounds[i] = lowbin + (i + 1) * bin_width;
  }

  // Fill in the values from the input vector to the histogram:
  //
  for (auto & val : invec) {
    int bin = 0;
    if (val < lowerBounds[0] || val > upperBounds.back())
      continue;
    while (bin < n_bins) {
      if (val > lowerBounds[bin] && val < upperBounds[bin]) {
        histogram.at(bin) ++;
        break;
      }
      else {
        bin ++;
      }
    }
  }


  // Get the most probable value:
  int max_count = 0;
  int max_bin = 0;
  for (size_t i = 0; i < histogram.size(); i ++) {
    if (histogram[i] > max_count) {
      max_count = histogram[i];
      max_bin = i;
    }
  }
  // for (size_t i = 0; i < histogram.size(); i ++) {
  //   std::cout << lowerBounds[i] << " - " << upperBounds[i] << ":\t"
  //             << histogram[i] << std::endl;
  // }
  // std::cout << "mode is " << (upperBounds[max_bin + 1] + lowerBounds[max_bin]) / 2 << std::endl;

  return (upperBounds[max_bin + 1] + lowerBounds[max_bin]) / 2;
}

template <class T>
T CorrelatedNoiseFilter::getMedian( std::vector<T> & _values) {


  if (_values.size() % 2 == 1) {
    std::nth_element(_values.begin(),
                     _values.begin() + _values.size() / 2,
                     _values.end());
    return  _values.at(_values.size() / 2);

  }
  else if (_values.size() > 2) {
    std::nth_element(_values.begin(),
                     _values.begin() + _values.size() / 2,
                     _values.end());
    std::nth_element(_values.begin(),
                     _values.begin() + _values.size() / 2 - 1,
                     _values.end());
    return 0.5 * (_values.at(_values.size() / 2) +
                  _values.at(_values.size() / 2 - 1));
  }
  // std::cout << "median is " << median << std::endl;
  // if (_median_accumulator.size() % 2 == 0) {

  //   median += _median_accumulator.at(_median_accumulator.size() / 2 - 1);
  //   median /= 2.0;
  // }
  // std::cout << "median is " << median << std::endl;

  return 0.0;
}

template <class T>
T getMean(const std::vector<T> &) {

  return 0.0;
}



}

#endif
