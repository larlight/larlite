/**
 * \file ToyDataMaker.h
 *
 * \ingroup App
 *
 * \brief Class def header for a class ToyDataMaker
 *
 * @author kazuhiro
 */

/** \addtogroup App

 @{*/
#ifndef TOYDATAMAKER_H
#define TOYDATAMAKER_H

#include <iostream>
#include "ERTool/Base/Shower.h"
#include "ERTool/Base/Track.h"
#include "ERTool/Base/UtilFunc.h"
#include "GeoAlgo/GeoVector.h"

#include "TRandom3.h"
#include "TMath.h"

namespace ertool {
/**
 \class ToyDataMaker
 User defined class ToyDataMaker ... these comments are used to generate
 doxygen documentation!
 */
class ToyDataMaker {

public:

	/// Default constructor
	ToyDataMaker() {
		SetTrajectoryParams();
	}

	/// Default destructor
	~ToyDataMaker() {
	}

	/// Track maker
	Track MakeTrack(double x, double y, double z, double px, double py,
			double pz, int pdg_code);

	/// Shower maker
	Shower MakeShower(double x, double y, double z, double px, double py,
			double pz, int pdg_code);

	/// Vtx sigma
	void SetVertexSigma(const double s) {
		_vtx_sigma = s;
	}

	/// Trajectory parameter
	void SetTrajectoryParams(const double step_size = 5, // cm
			const double step_sigma = 1, // cm
			const double angle_sigma = 5, // degree
			const double dedx = 2 // MeV/cm
			) {
		_track_step_size = step_size;
		_track_step_sigma = step_sigma;
		_track_step_angle_sigma = angle_sigma * TMath::Pi() / 180;
		_track_step_dedx = dedx;
	}

	/// Shower parameter
	void SetShowerParams(const double length, // cm
			const double dedx = 2  // MeV/cm
			) {
		_shower_length = length;
		_shower_dedx = dedx;
	}

	/// Clear added elements
	void Clear() {
		_shower_v.clear();
		_track_v.clear();
	}

private:
	/// smearing of the vector with the defined parameters
	geoalgo::Vector smearVector(::geoalgo::Vector v);

	std::vector<ertool::Shower> _shower_v;
	std::vector<ertool::Track> _track_v;

	/// vtx point smearing
	double _vtx_sigma;

	//
	// Track params
	//
	double _track_step_size;        ///< step size of a trajectory in cm
	double _track_step_sigma;       ///< that for sigma
	double _track_step_angle_sigma; ///< angle obviously in rad
	double _track_step_dedx;        ///< guess your unit

	// Shower params
	double _shower_length;
	double _shower_dedx;

	bool _debug = true;
};
}
#endif
/** @} */ // end of doxygen group
