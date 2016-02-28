#ifndef LARLITE_ERTOOLHELPERUTIL_CXX
#define LARLITE_ERTOOLHELPERUTIL_CXX

#include "DataFormat/mctruth.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/mctrack.h"
#include "ERToolHelperUtil.h"

namespace larlite {


	TLorentzVector ERToolHelperUtil::getXShift(const mctrack & mct) const {

		// Calculates for each mc track, based on the time of the event, the corresponding shift in x-direction
		TLorentzVector shift;

		//Check if track as no energy deposition points in the detector, return 0 or something if so
		if (!mct.size()) {
			//Beware: this block is satisfied a LOT for cosmics events.
			//Any cosmic that doesn't go through the detector has a Start() and End() point, but has zero size.
			//Hopefully setting the xshift as zero means we won't shift the cosmic INTO the detector,
			//So this should be fine.
			shift.SetXYZT(0., 0., 0., 0.);
			return shift;
		}

		// double event_time = mct.End().T();
		///Use first energy deposition point in the detector, because for cosmics,
		///Start().T() is way in the upper atmosphere, End().T() is at bottom of world volume
		double event_time = mct.at(0).T();
		double shift_x = (event_time / _DetFramePeriod) * _DetWidth;
		shift.SetXYZT(shift_x, 0., 0., 0.);
		if ( _disable_xshift )
			shift.SetXYZT(0., 0., 0., 0.);

		return shift;
	}

	TLorentzVector ERToolHelperUtil::getXShift(const mcshower & mcs) const {
		// Calculates for each mc shower, based on the time of the event, the corresponding shift in x-direction
		TLorentzVector shift;

		//Check if mcshower has no energy deposition points in the detector
		// (it seems mcshower.DetProfile().T() is 1e308 when there are none ... this should be initialized better)
		// consider this a hotfix!
		if (abs(mcs.DetProfile().T()) > 1e10  || abs(mcs.DetProfile().T()) < 1e-10) {
			//Beware: this block is satisfied a LOT for cosmics events.
			//Any mcshower (from a cosmic) that doesn't go through the detector has a Start() and End() point,
			//but has a weirdly undefined DetProfile(), which I try to identify with the above if() statement.
			//Hopefully setting the xshift as zero means we won't shift the cosmic INTO the detector,
			//So this should be fine.
			shift.SetXYZT(0., 0., 0., 0.);
			return shift;
		}
		double event_time = mcs.DetProfile().T();//End().T(); <--- old usage (wrong)
		double shift_x = (event_time / _DetFramePeriod) * _DetWidth;
		shift.SetXYZT(shift_x, 0., 0., 0.);
		if ( _disable_xshift )
			shift.SetXYZT(0., 0., 0., 0.);
		return shift;
	}

	TLorentzVector ERToolHelperUtil::getXShift(const mcpart & mcp) const {
		// Implementation of position shift due to the electron drift in the TPC for mcparticles
		TLorentzVector shift;

		double event_time = mcp.Trajectory().front().T();
		double shift_x = (event_time / _DetFramePeriod) * _DetWidth;
		shift.SetXYZT(shift_x, 0., 0., 0.);
		if ( _disable_xshift )
			shift.SetXYZT(0., 0., 0., 0.);

		return shift;
	}

	bool ERToolHelperUtil::isViable(const mcshower & mcs) const {
		return mcs.DetProfile().Momentum().E() >= _minEDep && mcs.StartDir().Mag2();
	}

	double ERToolHelperUtil::getMCShowerdEdX(const mcshower & mcs) const {
		// if ( (mcs.dEdx() < 0.5 ) ||  (mcs.dEdx() > 100) )
		// return (mcs.PdgCode() == 22 ? gRandom->Gaus(4, 4 * 0.03) : gRandom->Gaus(2, 2 * 0.03));
		return mcs.dEdx();
	}
}


#endif
