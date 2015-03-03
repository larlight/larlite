#ifndef BASICTOOL_EMSHOWERPROFILE_CXX
#define BASICTOOL_EMSHOWERPROFILE_CXX

#include "EMShowerProfile.h"
#include <cmath>

double EMShowerProfile::Length(const double energy) const {

  /*
  /// This formula taken from Andrzej

  double rad_length_cm = 48.; //Assumed energy independent (roughly correct)
  //this should be 14cm, but studying mcshowers tells me g4 is using 
  //47cm (fit)

  double epsilon_mev = 5715.; //Parameter fit from geant4 output apparently
  
  double shower_length = rad_length_cm * ( log( energy/epsilon_mev ) - 1 + (0.08*18) + 9.6 );
  */

  /// This formula from fitting single electron merge-all plot of reco shower energy vs. mcshower
  /// DetProfile energy, assuming >95% energy containment, fit from 100 MEV --> 1000 MEV
  /// Fit results for pol2 fit:
  /// Chi2                      =      129.971
  /// NDf                       =           93
  /// p0                        =      13.8874   +/-   0.713199    
  /// p1                        =     0.121734   +/-   0.00356722  
  /// p2                        = -3.75571e-05   +/-   3.80609e-06 

  ///Note: this formula only valid up to ~1.5 GEV. After that, shower length actually
  /// decreases as a function of shower energy.
  double shower_length = 13.8874 + 0.121734*energy - (3.75571e-05)*energy*energy;

  if(shower_length < 5) return 5.;

  return shower_length;
}

double EMShowerProfile::EnergyContainment(const double dist) const {

  // This formula taken from my understanding of electromagnetic showers
  // They deposit (1- 1/e) of their energy in each radiation length, X
  // So E(length of shower, d) = E_o * (1-e^(-d/X))
  // This is assuming radiation length is energy independent.
  // Ex: plug in d = 0: No energy deposited
  // Ex: plug in d = inf: All of initial energy E_o is deposited
  // Ex: plug in d = X: All but 1/e of energy E_o is deposited

  double rad_length_cm = 20.; //Assumed energy independent (roughly correct)
  /// 20 comes from fitting single gamma radiation lengths with MCShowers.
  
  // If you're outside of the TPC, no energy is contained
  if(dist < 0) return 0;

  return (1 - pow(2.71828,(dist*(-1)/rad_length_cm)));
}

double EMShowerProfile::OpeningAngle(const double energy) const {

  //          /                ^ 
  //         /                 |R
  //        /)(1/2)*o_angle    |
  //      <- - - - - - - - - - - - - - - 
  //        \    <-- length -->
  //         \
  //          \

  //(1/2)*opening angle = arctan(R/L)

  return 2*atan(ShowerRadius()/Length(energy));
  
}

double EMShowerProfile::ShowerRadius() const {

  //Moleiere radius in LAr is 10.1cm, according to Wikipedia
  return 10.1;

}
#endif
