#ifndef __TRACKEMUMASTER_CXX__
#define __TRACKEMUMASTER_CXX__

#include "TrackEmuMaster.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "RecoEmulatorException.h"

namespace recoemu {

    TrackEmuMaster::TrackEmuMaster()
        : TrackEmulatorBase("TrackEmuMaster")
    {
        srand( time(NULL) );
    }

    void TrackEmuMaster::Configure(const ::fcllite::PSet &pset)
    {
        _trackdeviation.Configure(pset.get_pset("TrackEmuTrackDeviationParametrized"));
        _trackflip.Configure(pset.get_pset("TrackEmuTrackFlip"));
        _tracktotaleff.Configure(pset.get_pset("TrackEmuApplyTotalEff"));
    }

    recoemu::Track_t TrackEmuMaster::Emulate(const recoemu::Track_t& mc)
    {
        // First make a copy of the input Track_t
        Track_t result = mc;

        // Run each emulation from sub- emulator modules
        // order matters! Should think about this more...
        result = _trackdeviation.Emulate(result);
        result = _trackflip.Emulate(result);
        result = _tracktotaleff.Emulate(result);

        return result;
    }

}

#endif
