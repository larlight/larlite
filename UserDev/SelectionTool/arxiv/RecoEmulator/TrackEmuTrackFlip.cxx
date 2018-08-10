#ifndef __TRACKEMUTRACKFLIP_CXX__
#define __TRACKEMUTRACKFLIP_CXX__

#include "TrackEmuTrackFlip.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "RecoEmulatorException.h"

namespace recoemu {

    TrackEmuTrackFlip::TrackEmuTrackFlip()
        : TrackEmulatorBase("TrackEmuTrackFlip")
    {
        srand( time(NULL) );
        _flip_probability = 0.;
    }

    void TrackEmuTrackFlip::Configure(const ::fcllite::PSet &pset)
    {        
        _flip_probability = pset.get<double>("FlipProbability");
    }

    recoemu::Track_t TrackEmuTrackFlip::Emulate(const recoemu::Track_t& mc)
    {
        // First make a copy of the input Track_t
        Track_t result = mc;

        // Decide whether to flip the track
        bool reverse_the_track = _flipeff.DrawFlat(_flip_probability);

        // Reverse the track trajectory points ordering if necessary
        if(reverse_the_track)
            std::reverse(result.trajectory.begin(),result.trajectory.end());

        return result;
    }

}

#endif
