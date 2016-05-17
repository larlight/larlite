#ifndef NOISEFILTER_TYPES_H
#define NOISEFILTER_TYPES_H

#include "LArUtil/Geometry.h"


namespace ub_noise_filter {

class detPropFetcher {

public:

  detPropFetcher();

  unsigned int n_wires(unsigned int plane);
  unsigned int n_planes();
  double wire_length(unsigned int plane, unsigned int wire);

private:

  // The function for wire lengths is called A LOT
  // and so the values of the function are cached
  // (To avoid calls to sqrt)
  std::vector<std::vector<double> > _wire_lengths;

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
