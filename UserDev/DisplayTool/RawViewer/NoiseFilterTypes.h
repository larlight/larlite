#ifndef NOISEFILTER_TYPES_H
#define NOISEFILTER_TYPES_H

#include "LArUtil/Geometry.h"


namespace ub_noise_filter {


/**
 * @brief Returns the approximate mode of a list
 * @details Bins the input into a histogram and returns the bin with the
 *          largest number of counts
 *
 * @param _input input vector of data
 * @param lowbin lowest bin to use
 * @param highbin highest bin to use
 * @param n_bins number of bins to use
 * @return Approximate mode of the values
 */
float getMode(const std::vector<float> & _input, float lowbin, float highbin, int n_bins);


/**
 * @brief Returns the median of a vector
 * @details Finds the nth_element of a vector.  This function can and will change the input.
 *
 * @tparam _input input list of values
 * @return The median of the vector
 */
float getMedian( std::vector<float> & _input);


float getMean(const std::vector<float> &);

float getCorrelation(const std::vector<float> & _input1, const std::vector<float> & _input2);

float getCorrelation(const float * _input1, const float * _input2, unsigned int N);

class detPropFetcher {

public:

  detPropFetcher();

  unsigned int n_wires(unsigned int plane);
  unsigned int n_planes();
  double wire_length(unsigned int plane, unsigned int wire);
  double wire_scale(unsigned int plane, unsigned int wire);

  const std::vector<float> & correlated_noise_blocks(int plane) const {
    return _correlated_noise_blocks.at(plane);
  }

  int same_plane_pair(int plane, int block);

  std::vector<std::vector<float> > service_board_block(int plane, int block);

private:

  // The function for wire lengths is called A LOT
  // and so the values of the function are cached
  // (To avoid calls to sqrt)
  std::vector<std::vector<double> > _wire_lengths;
  std::vector<std::vector<double> > _wire_scales;

  // This defines what blocks to use for correlated noise removal.
  // It's every 48 wires in induction, 96 in collection
  // That corresponds to motherboards in the TPC
  const std::vector<std::vector<float> > _correlated_noise_blocks = {
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



};


enum wireStatus {kNormal, kDead, kHighNoise, kChirping, kNStatus};


class chirp_info {

public:
  size_t chirp_start;
  size_t chirp_stop;
  float chirp_frac;
};

}

#endif
