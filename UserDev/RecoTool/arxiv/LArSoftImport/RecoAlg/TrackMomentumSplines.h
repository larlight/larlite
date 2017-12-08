/**
 * \file TrackMomentumSplines.h
 *
 * \ingroup CCInclusive
 *
 * \brief This is kaleko copying some stuff to compute a muon momentum from a range table. Copied from here:
 * https://cdcvs.fnal.gov/redmine/projects/larreco/repository/revisions/develop/entry/larreco/RecoAlg/TrackMomentumCalculator.cxx
 *
 * @author davidkaleko
 */

/** \addtogroup CCInclusive

    @{*/
#ifndef TRACKMOMENTUMSPLINES_H
#define TRACKMOMENTUMSPLINES_H

#include <iostream>
#include "TGraph.h"
#include "TSpline.h"

/**
   \class TrackMomentumSplines
   User defined class TrackMomentumSplines ... these comments are used to generate
   doxygen documentation!
 */
namespace larlite {

	class TrackMomentumSplines {

	public:

		/// Default constructor
		TrackMomentumSplines();

		/// Default destructor
		~TrackMomentumSplines() {}

		double GetMuMomentum(const double muon_range);

		double GetPMomentum(const double p_range);


	private:

		TGraph *KEvsR_muon;
		TSpline3 *KEvsR_muon_spline3;
		TGraph *KEvsR_proton;
		TSpline3 *KEvsR_proton_spline3;
	};

}
#endif
/** @} */ // end of doxygen group

