#ifndef RANDOMDRAWTOOL_CXX
#define RANDOMDRAWTOOL_CXX

#include "RandomDrawTool.h"


RandomDrawTool::RandomDrawTool(const std::vector<double>& values,
				const double& min, const double& max){

  SetDistribution(values,min,max);

  srand( time(NULL) );

}

void RandomDrawTool::SetDistribution(const std::vector<double>& values,
				     const double& min, const double& max){
  
  _distribution.clear();
  
  _distribution = values;

  Normalize(_distribution);
  
  _xmin = min;
  _xmax = max;

  _bin_width = (_xmax-_xmin) / ( (float) _distribution.size() );

  return;  
}

void RandomDrawTool::Normalize(std::vector<double>& values){

  double max = 0.;

  for (auto& val : values)
    if (val > max) { max = val; }

  for (auto& val : values)
    val /= max;

  return;
}

double RandomDrawTool::Draw(){

  // has a value been chosen yet?
  bool drawn = false;

  // draw result
  double result = 0.;

  // number of samples to draw from
  int samples = _distribution.size();
  
  while (!drawn){

    // draw a random x-value to try and sample
    int xrand = rand() % samples;

    // draw a random value between [0,1]
    double yrand = ( (double) rand() ) / RAND_MAX;

    if (_distribution[xrand] > yrand){
      
      result = _xmin + (_xmax-_xmin) * ((float)xrand/(float)samples);
      
      // finally, smear by randomly samling within this bin
      double binsmear = ( (double) rand() ) / RAND_MAX - 1.;
      binsmear *= _bin_width;

      result += binsmear;

      drawn = true;
    }

  }// while we are trying to find a value
  
  return result;
}

bool RandomDrawTool::DrawFlat(double myval){

  // Draw a random value between [0,1].
  // If this random value is less than "myval", return true

  if ( myval < 0. || myval > 1. )
    throw recoemu::RecoEmulatorException("RandomDrawTool::DrawFlat handed an efficiency less than zero or greater than 1!");

  return ( (double) rand() ) / RAND_MAX < myval ? true : false;
}


#endif
