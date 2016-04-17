#ifndef __TRACKEMUAPPLYTOTALEFF_CXX__
#define __TRACKEMUAPPLYTOTALEFF_CXX__

#include "TrackEmuApplyTotalEff.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "RecoEmulatorException.h"

namespace recoemu {

    TrackEmuApplyTotalEff::TrackEmuApplyTotalEff()
        : TrackEmulatorBase("TrackEmuApplyTotalEff")
    {
        srand( time(NULL) );
        LengthEffParam0 = std::numeric_limits<double>::max();
        LengthEffParam1 = std::numeric_limits<double>::max();
        AngleEffParam0 = std::numeric_limits<double>::max();
        AngleEffParam1 = std::numeric_limits<double>::max();
        AngleEffParam2 = std::numeric_limits<double>::max();
    }

    void TrackEmuApplyTotalEff::Configure(const ::fcllite::PSet &pset)
    {

        //((atan([1]*(x-[0]))/pi) + 0.5)
        LengthEffParam0 = pset.get<double>("LengthEffParam0");
        LengthEffParam1 = pset.get<double>("LengthEffParam1");
        // complicated, see implementation in this file
        AngleEffParam0 = pset.get<double>("AngleEffParam0");
        AngleEffParam1 = pset.get<double>("AngleEffParam1");
        AngleEffParam2 = pset.get<double>("AngleEffParam2");


    }

    recoemu::Track_t TrackEmuApplyTotalEff::Emulate(const recoemu::Track_t& mc)
    {
        if (
            LengthEffParam0 == std::numeric_limits<double>::max() ||
            LengthEffParam1 == std::numeric_limits<double>::max() ||
            AngleEffParam0 == std::numeric_limits<double>::max() ||
            AngleEffParam1 == std::numeric_limits<double>::max() ||
            AngleEffParam2 == std::numeric_limits<double>::max() 
        )
            throw RecoEmulatorException("TrackEmuApplyTotalEff not properly configured!");

        // First make a copy to modify
        Track_t result = mc;
	
	double mylength = mc.trajectory.Length();
        double lengtheff = GetLengthEff(mylength);
	double totaleff = lengtheff;

	// Hack: Only apply angle efficiency if track length is shorter than 5 cm
	if( mylength < 5. ){
	  double mytheta = (mc.trajectory.back() - mc.trajectory.front()).Theta() * 180./3.14159;
	  double myphi = (mc.trajectory.back() - mc.trajectory.front()).Phi() * 180./3.14159;
	  double angleeff = GetAngleEff(mytheta,myphi);
	  totaleff *= angleeff;
	}
        bool keep_track = _totalEff.DrawFlat(totaleff);

        // Mark the track for deletion if keep_track is false
        result.mark_for_deletion = !keep_track;

        return result;
    }

    const double TrackEmuApplyTotalEff::GetLengthEff(double length) const {
        return ((atan(LengthEffParam1 * (length - LengthEffParam0)) / 3.14159) + 0.5);
    }

    const double TrackEmuApplyTotalEff::GetAngleEff(double theta, double phi) const {
        double foftheta = ((((cos(2*theta * 3.14159/180. ) + 1.)/2.) * AngleEffParam2) * (AngleEffParam2-AngleEffParam1)/AngleEffParam2 + AngleEffParam1);
        double eff = (((cos(2*phi * 3.14159/180. + 3.14159)+1)/2) * AngleEffParam0) * ( AngleEffParam0- foftheta )/AngleEffParam0 + foftheta;
        return eff;
    }

}

#endif
