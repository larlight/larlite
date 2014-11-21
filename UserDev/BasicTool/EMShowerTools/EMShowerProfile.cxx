#ifndef BASICTOOL_EMSHOWERPROFILE_CXX
#define BASICTOOL_EMSHOWERPROFILE_CXX

#include "EMShowerProfile.h"
#include <cmath>

double EMShowerProfile::Length(const double energy) const {

  /// This formula taken from Andrzej

  double rad_length_cm = 48.; //Assumed energy independent (roughly correct)
  //this should be 14cm, but studying mcshowers tells me g4 is using 
  //47cm (fit)

  double epsilon_mev = 5715.; //Parameter fit from geant4 output apparently
  
  double shower_length = rad_length_cm * ( log( energy/epsilon_mev ) - 1 + (0.08*18) + 9.6 );

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

  double rad_length_cm = 48.; //Assumed energy independent (roughly correct)
  //this should be 14cm, but studying mcshowers tells me g4 is using 
  //47cm (fit)
  
  // If you're outside of the TPC, no energy is contained
  if(dist < 0) return 0;

  return (1 - pow(2.71828,(dist*(-1)/rad_length_cm)));
}

#endif
