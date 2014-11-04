#ifndef RECOTOOL_ANALYSISALGCONSTANTS_HH
#define RECOTOOL_ANALYSISALGCONSTANTS_HH

#include <vector>
namespace calo {

  static const double kCalAmpConstants_ARGONEUT[2]  = {0.588726e-3 , 1.18998e-3};
  static const double kCalAreaConstants_ARGONEUT[2] = {0.773887e-2 , 1.78186e-2};
  static const bool kCaloUseModBox_ARGONEUT = false;

  // MCC4 constants
  //static const double kCalAmpConstants_UB[3]  = {2.677e-3, 2.804e-3, 5.716e-3};
  //static const double kCalAreaConstants_UB[3] = { 1.2093e-2, 1.2831e-2, 2.934e-2};

  // MCC5 constants
  static const double kCalAmpConstants_UB[3] = {9.573e-4, 6.868e-4, 1.382e-3};
  static const double kCalAreaConstants_UB[3] = {7.488e-3, 5.110e-3, 1.008e-2};
  static const bool kCaloUseModBox_UB = false;
}

#endif
