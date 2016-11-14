#ifndef LARLITE_TESTSPLINE_CXX
#define LARLITE_TESTSPLINE_CXX

#include "TestSpline.h"
#include "DataFormat/mctruth.h"
#include "DataFormat/mctrack.h"

namespace larlite {

    bool TestSpline::initialize() {

        std::string histtitle;
        histtitle = _testing_protons ?
                    "Spline Computed E vs. Perfect Reco Proton Deposited E;Deposited E;Spline E" :
                    "Spline Computed E vs. Perfect Reco Muon Deposited E;Deposited E;Spline E";

        _h = new TH2D("h", histtitle.c_str(), 200, 0, 2, 200, 0, 2);

        myspline = new TrackMomentumSplines();

        //Box here is TPC
	//        _myGeoAABox = FidVolBox();
	double fidvol_dist_x = 5.;
	double fidvol_dist_y = 5;
	double fidvol_dist_zmin = 5.;
	double fidvol_dist_z = 5;
	
	// DetHalfHeight is 116.5
	// DetHalfWidth  is 128.175
	// DetLength     is 1036.8
	
	//Box here is TPC
	_myGeoAABox.Min( 0 + fidvol_dist_x,
			 -(::larutil::Geometry::GetME()->DetHalfHeight()) + fidvol_dist_y,
			 0 + fidvol_dist_zmin);
	
	_myGeoAABox.Max( 2 * (::larutil::Geometry::GetME()->DetHalfWidth()) - fidvol_dist_x,
			 ::larutil::Geometry::GetME()->DetHalfHeight() - fidvol_dist_y,
			 ::larutil::Geometry::GetME()->DetLength() - fidvol_dist_z);
	

        return true;
    }

    bool TestSpline::analyze(storage_manager* storage) {

        auto ev_mctruth = storage->get_data<event_mctruth>("generator");
        if (!ev_mctruth) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, mctruth!"));
            return false;
        }
        if (ev_mctruth->size() != 1) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("MCTruth size is not equal to 1... it equals %lu!", ev_mctruth->size()));
            return false;
        }


        auto ev_mctrack = storage->get_data<event_mctrack>("mcreco");
        if (!ev_mctrack) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, mctrack!"));
            return false;
        }


        const mcnu neutrino = ev_mctruth->at(0).GetNeutrino();
        bool _is_numuCC = !neutrino.CCNC() && neutrino.Nu().PdgCode() == 14 && neutrino.Lepton().PdgCode() == 13;
        if (!_is_numuCC) return false;

        size_t n_relevant_mctracks = 0;
        mctrack thetrack;
        for (auto const& mct : *ev_mctrack) {
            if (!_testing_protons) {
                if (mct.PdgCode() == 13 && mct.size() >= 2 ) {
                    thetrack = mct;
                    n_relevant_mctracks++;
                }
            }
            else {
                if (mct.PdgCode() == 2212 && mct.size() >= 2) {
                    thetrack = mct;
                    n_relevant_mctracks++;
                }
            }
        }

        if (thetrack.size() < 2) return false;

        if ( !_myGeoAABox.Contain(::geoalgo::Vector(thetrack.front().Position().Vect())) ||
                !_myGeoAABox.Contain(::geoalgo::Vector(thetrack.back().Position().Vect()))) return false;
        if (n_relevant_mctracks > 1) return false;

        double tracklen = ::geoalgo::Vector(thetrack.front().Position().Vect()).Dist(
                              ::geoalgo::Vector(thetrack.back().Position().Vect()));

        double depE = (thetrack.front().E() - thetrack.back().E()) / 1000.;
        double splineE = _testing_protons ?
                         myspline->GetPMomentum(tracklen) / 1000. : myspline->GetMuMomentum(tracklen) / 1000.;

        _h->Fill(depE, splineE);


        return true;
    }

    bool TestSpline::finalize() {

        if (_fout) {
            _h->Write();
        }
        return true;
    }

}
#endif
