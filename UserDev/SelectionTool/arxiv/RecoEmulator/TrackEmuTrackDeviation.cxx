#ifndef __TRACKEMUTRACKDEVIATION_CXX__
#define __TRACKEMUTRACKDEVIATION_CXX__

#include "TrackEmuTrackDeviation.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "RecoEmulatorException.h"

namespace recoemu {

    TrackEmuTrackDeviation::TrackEmuTrackDeviation()
        : TrackEmulatorBase("TrackEmuTrackDeviation")
    {
        srand( time(NULL) );
    }

    void TrackEmuTrackDeviation::Configure(const ::fcllite::PSet &pset)
    {
        auto XDeviationValues = pset.get< std::vector<double> >("XDeviationValues");
        auto XDeviationMin    = pset.get<double>("XDeviationMin");
        auto XDeviationMax    = pset.get<double>("XDeviationMax");
        _xres.SetDistribution(XDeviationValues, XDeviationMin, XDeviationMax);

        auto YDeviationValues = pset.get< std::vector<double> >("YDeviationValues");
        auto YDeviationMin    = pset.get<double>("YDeviationMin");
        auto YDeviationMax    = pset.get<double>("YDeviationMax");
        _yres.SetDistribution(YDeviationValues, YDeviationMin, YDeviationMax);

        auto ZDeviationValues = pset.get< std::vector<double> >("ZDeviationValues");
        auto ZDeviationMin    = pset.get<double>("ZDeviationMin");
        auto ZDeviationMax    = pset.get<double>("ZDeviationMax");
        _zres.SetDistribution(ZDeviationValues, ZDeviationMin, ZDeviationMax);
    }

    recoemu::Track_t TrackEmuTrackDeviation::Emulate(const recoemu::Track_t& mc)
    {
        // First make a copy of the input Track_t
        Track_t result = mc;

        //Loop through the Track_t trajectory and smear each point
        for (auto &step : result.trajectory) {
            step[0] = step[0] + _xres.Draw();
            step[1] = step[1] + _yres.Draw();
            step[2] = step[2] + _zres.Draw();
        }

        return result;
    }

}

#endif
