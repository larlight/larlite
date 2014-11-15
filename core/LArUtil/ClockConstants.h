
#ifndef LARLITE_CLOCKCONSTANTS_H
#define LARLITE_CLOCKCONSTANTS_H

#include <limits>
#include <climits>

namespace larutil {

  /// Maximum time in nano-second
  const double kTIME_MAX = std::numeric_limits<double>::max();

  /// Default Frequency in MHz
  const double kDEFAULT_FREQUENCY = 1.e-6;

  /// Default Frame period in micro-second
  const double kDEFAULT_FRAME_PERIOD = 1.6e3;

  /// Default Optical clock speed in MHz
  const double kDEFAULT_FREQUENCY_OPTICAL = 64.;

  /// Default TPC clock speed in MHz
  const double kDEFAULT_FREQUENCY_TPC = 2.;

  /// Default Trigger clock speed in MHz
  const double kDEFAULT_FREQUENCY_TRIGGER = 16.;

  /// Default G4 reference time in micro-second
  const double kDEFAULT_MC_CLOCK_T0 = 0;

  /// Default TPC readout start time offset from trigger in micro-second
  const double kDEFAULT_TRIG_OFFSET_TPC = -1600.;

}

#endif
