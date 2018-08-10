#ifndef __TRACKEMUTRACKDEVIATIONPARAMETRIZED_CXX__
#define __TRACKEMUTRACKDEVIATIONPARAMETRIZED_CXX__

#include "TrackEmuTrackDeviationParametrized.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

namespace recoemu {

  TrackEmuTrackDeviationParametrized::TrackEmuTrackDeviationParametrized()
    : TrackEmulatorBase("TrackEmuTrackDeviationParametrized")
  {
    srand( time(NULL) );
  }

  void TrackEmuTrackDeviationParametrized::Configure(const ::fcllite::PSet &pset)
  {

    auto SmearFunc   = pset.get< std::string >("SmearFunc");
    auto XSmearParams = pset.get< std::vector<double> >("XSmearParams");
    auto XSmearMin    = pset.get< double >("XSmearMin");
    auto XSmearMax    = pset.get< double >("XSmearMax");
    auto YSmearParams = pset.get< std::vector<double> >("YSmearParams");
    auto YSmearMin    = pset.get< double >("YSmearMin");
    auto YSmearMax    = pset.get< double >("YSmearMax");
    auto ZSmearParams = pset.get< std::vector<double> >("ZSmearParams");
    auto ZSmearMin    = pset.get< double >("ZSmearMin");
    auto ZSmearMax    = pset.get< double >("ZSmearMax");

    _fXSmear = new TF1("xsmear", SmearFunc.c_str(), XSmearMin, XSmearMax);
    for (size_t i = 0; i < XSmearParams.size(); i++)
      _fXSmear->SetParameter(i, XSmearParams[i]);
    _fYSmear = new TF1("ysmear", SmearFunc.c_str(), YSmearMin, YSmearMax);
    for (size_t i = 0; i < YSmearParams.size(); i++)
      _fYSmear->SetParameter(i, YSmearParams[i]);
    _fZSmear = new TF1("zsmear", SmearFunc.c_str(), ZSmearMin, ZSmearMax);
    for (size_t i = 0; i < ZSmearParams.size(); i++)
      _fZSmear->SetParameter(i, ZSmearParams[i]);

  }

  recoemu::Track_t TrackEmuTrackDeviationParametrized::Emulate(const recoemu::Track_t& mc)
  {
    // First make a copy of the input Track_t
    Track_t result = mc;

    //Loop through the Track_t trajectory and smear each point
    for (auto &step : result.trajectory) {
      step[0] = step[0] + _fXSmear->GetRandom();
      step[1] = step[1] + _fYSmear->GetRandom();
      step[2] = step[2] + _fZSmear->GetRandom();
    }

    return result;
  }

}

#endif
