
#ifndef LARUTILCONSTANTS_H
#define LARUTILCONSTANTS_H

#include "Base/GeoConstants.h"

namespace larutil{

  /// Geometry default data file
  const std::string kUTIL_DATA_FILENAME[larlite::geo::kDetIdMax] = 
    { "",                        // Bo
      "larutil_argoneut.root",   // ArgoNeuT
      "larutil_microboone.root", // MicroBoone
      "",                        // LBNE 10kt 
      "",                        // JPARC 250 L
      "",                        // 35t prototype
      "",                        // LBNE 34kt
      "",                        // CSU 40 L
      "",                        // LArIAT
      ""                         // ICARUS T600
    };

  const std::string kTREENAME_GEOMETRY = "scanner/Geometry";
  const std::string kTREENAME_DETECTORPROPERTIES = "scanner/DetectorProperties";
  const std::string kTREENAME_LARPROPERTIES = "scanner/LArProperties";
  const std::string kTREENAME_TIMESERVICE = "scanner/TimeService";
  //--- LArSoft parameters ---//

  // Recombination factor coefficients come from Nucl.Instrum.Meth.A523:275-286,2004
  // R = A/(1 + (dE/dx)*k/Efield)
  // dE/dx is given by the voxel energy deposition, but have to convert it to MeV/cm
  // from GeV/voxel width
  // A = 0.800 +/- 0.003 
  // k = 0.0486           needs to be scaled with Electric field.
  // Efield in kV/cm
  static const double kRecombA        = 0.8;      ///< see Nucl.Instrum.Meth.A523:275-286,2004 
  static const double kRecombk        = 0.0486;   ///< in g/(MeVcm^{2})*kV/cm
  static const double kModBoxA        = 0.930;     // Modified Box Alpha (ArgoNeuT JINST)
  static const double kModBoxB        = 0.212;     // in g/(MeVcm^{2})*kV/cm
  // static const double kWion   = 23.6e-6;  ///< ionization potenial in LAr, 23.6 eV = 1e, Wion in MeV/e

  // Conversion for energy deposited in GeV to number of ionization electrons produced
  static const double kGeVToElectrons = 4.237e7; ///< 23.6eV per ion pair, 1e9 eV/GeV 

  // More constants
  //static double kc    = 2.99792458e10;   ///< cm/s
  //static double khbar = 6.58211899e-22;  ///< MeVs

  // Conversion factors
  static const double kMeterToCentimeter = 1.e2;                  ///< 1 m = 100 cm
  static const double kCentimeterToMeter = 1./kMeterToCentimeter;
  static const double kMeterToKilometer  = 1.e-3;                 ///< 1000 m = 1 km
  static const double kKilometerToMeter  = 1./kMeterToKilometer;

  static const double keVToMeV           = 1.e-6;                 ///< 1e6 eV = 1 MeV
  static const double kMeVToeV           = 1./keVToMeV;

  static const double kBogusD            = -999.;                 ///< obviously bogus double value
  static const int    kBogusI            = -999;                  ///< obviously bogus integer value
  static const float  kBogusF            = -999.;                 ///< obviously bogus float value
  static const double quietCompiler = kBogusD*kBogusI*kBogusF*kRecombA*kRecombk*kGeVToElectrons;

  static const double kDriftVelMCC9 = 0.1098;                     ///< cm/us
  static const double kDriftVelMCC8 = 0.111;                      ///< cm/us
}

#endif
