/**
 * \file ChirpFilter.h
 *
 * \ingroup RawViewer
 *
 * \brief Class def header for a class ChirpFilter
 *
 * @author Mike Mooney
 */

///////////
// NOTES //
///////////

// The correct way to run the following bits of code is as follows:
//
// vector<float> myWaveform = <code to grab single waveform>;
// ChirpFilterAlg(myWaveform);
// ZigZagFilterAlg(myWaveform);
// RawAdapativeBaselineAlg(myWaveform);
// RemoveChannelFlags(myWaveform);
//
// After this, you should then flag noisy (high RMS) channels, and proceed with harmonic/low-frequency noise removal.
//
// Note that the mean is adjusted in ZigZagFilterAlg.  If you don't want to use that filter, then you might still want
//   to reset the mean elsewhere, but in the same point in the sequence above.  It might work without doing that, but
//     I haven't tested it.
//
// Also note that you need to make a couple of additions in the code below.  I've explicitly pointed out where you need
//   to do this.  And note that there are dead channels that are still not flagged by the below code, that should also
//     be targeted before the harmonic/low-frequency noise removal.  These are the FT 4 Y plane channels at FT 7-9 U
//       plane channels that are slightly higher in RMS, and thus don't get flagged via the ChirpFilterAlg.  I think
//         these are easy to hit after we find chirping channels by looking at a wire-length-dependent low RMS cut.
//           Let's talk about that later, I can help advise that step.
//
// Let me know if you have any questions.
//
// --Mike

/** \addtogroup RawViewer

    @{*/
#ifndef CHIRPFILTER_H
#define CHIRPFILTER_H

#include <iostream>
#include <vector>
#include <math.h>

/**
   \class ChirpFilter
   User defined class ChirpFilter ... these comments are used to generate
   doxygen documentation!
 */

namespace ub_noise_filter {

class chirp_info {

public:
  unsigned int _chirp_multiplicity;
  std::vector<unsigned int> _transition_ticks;
  bool _start_section_is_chirping;
};



class ChirpFilter {

public:

  /// Default constructor
  ChirpFilter() {}

  /// Default destructor
  ~ChirpFilter() {}

  void ChirpFilterAlg(std::vector<float> &wf);
  void ZigzagFilterAlg(std::vector<float> &wf);

  void RawAdaptiveBaselineAlg(std::vector<float> &wf);
  void RemoveChannelFlags(std::vector<float> &wf);

};

}
#endif
/** @} */ // end of doxygen group

