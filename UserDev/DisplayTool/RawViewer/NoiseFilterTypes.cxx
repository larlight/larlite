#ifndef NOISEFILTER_TYPES_CXX
#define NOISEFILTER_TYPES_CXX

#include "NoiseFilterTypes.h"

namespace ub_noise_filter {

detPropFetcher::detPropFetcher() {
  _wire_lengths.resize(3);
  _wire_lengths[0].resize(2400);
  _wire_lengths[1].resize(2400);
  _wire_lengths[2].resize(3456);

  auto geom = larutil::Geometry::GetME();


  for (int wire = 0; wire < 2400; wire ++) {
    double xyzStart[3];
    double xyzEnd[3];


    geom ->WireEndPoints(0, wire,
                         xyzStart,
                         xyzEnd) ;

    _wire_lengths[0][wire] = sqrt(
                               (xyzStart[0] - xyzEnd[0]) * (xyzStart[0] - xyzEnd[0]) +
                               (xyzStart[1] - xyzEnd[1]) * (xyzStart[1] - xyzEnd[1]) +
                               (xyzStart[2] - xyzEnd[2]) * (xyzStart[2] - xyzEnd[2])
                             );

    geom ->WireEndPoints(1, wire,
                         xyzStart,
                         xyzEnd) ;

    _wire_lengths[1][wire] = sqrt(
                               (xyzStart[0] - xyzEnd[0]) * (xyzStart[0] - xyzEnd[0]) +
                               (xyzStart[1] - xyzEnd[1]) * (xyzStart[1] - xyzEnd[1]) +
                               (xyzStart[2] - xyzEnd[2]) * (xyzStart[2] - xyzEnd[2])
                             );

  }

  for (int wire = 0; wire < 3456; wire ++) {
    double xyzStart[3];
    double xyzEnd[3];


    geom ->WireEndPoints(2, wire,
                         xyzStart,
                         xyzEnd) ;

    _wire_lengths[2][wire] = sqrt(
                               (xyzStart[0] - xyzEnd[0]) * (xyzStart[0] - xyzEnd[0]) +
                               (xyzStart[1] - xyzEnd[1]) * (xyzStart[1] - xyzEnd[1]) +
                               (xyzStart[2] - xyzEnd[2]) * (xyzStart[2] - xyzEnd[2])
                             );
  }

}

unsigned int detPropFetcher::n_wires(unsigned int plane) {
  if (plane == 0)
    return 2400;
  else if (plane == 1)
    return 2400;
  else
    return 3456;
}
unsigned int detPropFetcher::n_planes() {
  return 3;
}

double detPropFetcher::wire_length(unsigned int plane, unsigned int wire) {

  return _wire_lengths[plane][wire];
}

}




#endif
