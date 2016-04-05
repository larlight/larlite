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
  //  static const double kCalAmpConstants_UB[3] = {9.573e-4/1.79, 6.868e-4/1.79, 1.382e-3/1.79};
  //  static const double kCalAreaConstants_UB[3] = {7.488e-3/1.79, 5.110e-3/1.79, 1.008e-2/1.79};
  //static const double kCalAmpConstants_UB[3] = {9.573e-4*1.379, 6.868e-4*1.379, 1.382e-3*1.379};
  //static const double kCalAreaConstants_UB[3] = {7.488e-3*1.379, 5.110e-3*1.379, 1.008e-2*1.379};
  //area constants tuned with mean for for MCC6 v04_03_02 - A. Schukraft 3/9/15
  //#Pulse height constants for pre-MCC6. - T. Yang 3/5/15

  // MCC6 constants
  //static const double kCalAmpConstants_UB[3] =  {9.200e-4, 1.062e-3, 1.084e-03};
  //static const double kCalAreaConstants_UB[3] = { 5.0142e-3, 5.1605e-3, 5.4354e-3 };
  //static const bool kCaloUseModBox_UB = false;

  // MCC7 constants
  static const double kCalAmpConstants_UB[3] =  { 5.9e-4, 7.30e-3, 7.60e-3};
  static const double kCalAreaConstants_UB[3] = { 5.1e-3, 5.28e-3, 5.32e-3 };
  static const bool kCaloUseModBox_UB = false;
}

#endif
