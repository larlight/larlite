#ifndef __TRACKEMUSMEARING_CXX__
#define __TRACKEMUSMEARING_CXX__

#include "TrackEmuApplyLengthEff.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "RecoEmulatorException.h"

namespace recoemu {

    TrackEmuApplyLengthEff::TrackEmuApplyLengthEff()
        : TrackEmulatorBase("TrackEmuApplyLengthEff")
          //, _lengthEff(nullptr)
    {
        srand( time(NULL) );
        LengthEffParam0 = std::numeric_limits<double>::max();
        LengthEffParam1 = std::numeric_limits<double>::max();
        LengthEffParam2 = std::numeric_limits<double>::max();
    }

    void TrackEmuApplyLengthEff::Configure(const ::fcllite::PSet &pset)
    {

        //((atan([1]*(x-[0]))/pi) + 0.5)*[2]
        LengthEffParam0 = pset.get<double>("LengthEffParam0");
        LengthEffParam1 = pset.get<double>("LengthEffParam1");
        LengthEffParam2 = pset.get<double>("LengthEffParam2");

    }

    recoemu::Track_t TrackEmuApplyLengthEff::Emulate(const recoemu::Track_t& mc)
    {
        if (
            LengthEffParam0 == std::numeric_limits<double>::max() ||
            LengthEffParam1 == std::numeric_limits<double>::max() ||
            LengthEffParam2 == std::numeric_limits<double>::max()
        )
            throw RecoEmulatorException("TrackEmuApplyLengthEff not properly configured!");

        Track_t result;

        double lengtheff = ((atan(LengthEffParam1 * (mc.trajectory.Length() - LengthEffParam0)) / 3.14159) + 0.5) * LengthEffParam2;
        bool keep_track = _lengthEff.DrawFlat(lengtheff);

        // Mark the track for deletion if keep_track is false
        result.mark_for_deletion = !keep_track;

        result.trajectory = mc.trajectory;
        result.energy = mc.energy;
        result.dedx = mc.dedx;
        result.pdg  = mc.pdg;
        result.time = mc.time;

        return result;
    }

}

#endif
