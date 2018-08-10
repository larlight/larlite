#ifndef LARLITE_SAMPLESELIIAANA_CXX
#define LARLITE_SAMPLESELIIAANA_CXX

#include "SampleSelIIAAna.h"
#include "DataFormat/opflash.h"
#include "DataFormat/mctruth.h"
#include "DataFormat/mcflux.h"

namespace larlite {

    bool SampleSelIIAAna::initialize() {

        _nu_finder = SelIIA();

        if (_filetype == kINPUT_FILE_TYPE_MAX) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("DID NOT SET INPUT FILE TYPE!"));
            return false;
        }
        _nu_finder.setInputType(_filetype);
        _nu_finder.setVtxSphereRadius(_vtx_sphere_radius);
        setBGWTimes();

        total_events = 0;
        passed_events = 0;

        // Fiducial Volume Box
        _fidvolBox = CCFidVolBox();

        //Box here is TPC
        _tpcBox.Min( 1,
                     -(::larutil::Geometry::GetME()->DetHalfHeight()) + 1,
                     1);

        _tpcBox.Max( 2 * (::larutil::Geometry::GetME()->DetHalfWidth()) - 1,
                     ::larutil::Geometry::GetME()->DetHalfHeight() - 1,
                     ::larutil::Geometry::GetME()->DetLength() - 1);

        _hcorrect_ID = new TH1F("hcorrectID", "Was Neutrino Vtx Correctly Identified?", 2, -0.5, 1.5);


        return true;
    }


    bool SampleSelIIAAna::setBGWTimes() {

        if (_filetype == kINPUT_FILE_TYPE_MAX) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("DID NOT SET INPUT FILE TYPE!"));
            return false;
        }
        else if ( _filetype == kOnBeam ) {
            BGW_mintime = 3.3;
            BGW_maxtime = 4.9;
        }
        else if ( _filetype == kOffBeam ) {
            BGW_mintime = 3.65;
            BGW_maxtime = 5.25;
        }
        else if ( _filetype == kCorsikaInTime ) {
            BGW_mintime = 3.2;
            BGW_maxtime = 4.8;
        }
        else if ( _filetype == kBNBOnly ) {
            BGW_mintime = 3.55;
            BGW_maxtime = 5.15;
        }
        else if ( _filetype == kBNBCosmic ) {
            BGW_mintime = 3.55;
            BGW_maxtime = 5.15;
        }
        return true;
    }


    bool SampleSelIIAAna::analyze(storage_manager* storage) {

        total_events++;

        auto ev_vtx = storage->get_data<event_vertex>("pmtrack");
        if (!ev_vtx) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, vertex!"));
            return false;
        }
        if (!ev_vtx->size()) {
            //print(larlite::msg::kERROR, __FUNCTION__, Form("Zero reconstructed vertices in this event!"));
            return false;
        }

        auto ev_track = storage->get_data<event_track>("pandoraNuPMA");
        if (!ev_track) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, track!"));
            return false;
        }
        if (!ev_track->size()) {
            //print(larlite::msg::kERROR, __FUNCTION__, Form("Zero reconstructed tracks in this event!"));
            return false;
        }
        auto ev_opflash = storage->get_data<event_opflash>("opflashSat");
        if (!ev_opflash) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, opflash!"));
            return false;
        }
        if (!ev_opflash->size()) {
            print(larlite::msg::kERROR, __FUNCTION__, Form("opflash size is zero!"));
            return false;
        }
        event_calorimetry* ev_calo = nullptr;
        auto const& ass_calo_v = storage->find_one_ass(ev_track->id(),
                                 ev_calo,
                                 Form("%scalo", ev_track->name().c_str()));

        if (!ev_calo) {
            std::cerr << "no event_calorimetry!" << std::endl;
            return false;
        }

        if (ev_calo->empty()) {
            std::cout << "ev_calo empty" << std::endl;
            return false;
        }


        // Try to find a neutrino vertex in this event... return a reco vertex,
        // and a std::vector of tracks that are associated with that vertex
        // std::pair<larlite::vertex, std::vector<larlite::track> > reco_neutrino;
        // CCNuItxn_t is defined in CCInclConstants
        CCNuItxn_t reco_neutrino;
        try {
            reco_neutrino = _nu_finder.findNeutrino(ev_track, ev_calo, ass_calo_v, ev_vtx, ev_opflash);
        }
        catch (...) {
            return false;
        }

        // std::cout << "Number of tracks associated with this reconstructed neutrino is "
        //           << (int)reco_neutrino.second.size() << std::endl;

        larlite::mcnu mcnu;
        // If we found a vertex and we are running over MC, let's check if it is accurate
        if (!_running_on_data) {

            auto ev_mctruth = storage->get_data<event_mctruth>("generator");
            if (!ev_mctruth) {
                print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, mctruth!"));
                return false;
            }
            if (ev_mctruth->size() != 1) {
                print(larlite::msg::kERROR, __FUNCTION__, Form("MCTruth size doesn't equal one!"));
                return false;
            }
            auto ev_mcflux = storage->get_data<event_mcflux>("generator");
            if (!ev_mcflux) {
                print(larlite::msg::kERROR, __FUNCTION__, Form("Did not find specified data product, mcflux!"));
                return false;
            }
            // Require exactly one neutrino interaction
            if (ev_mcflux->size() != 1) {
                print(larlite::msg::kINFO, __FUNCTION__, Form("ev_mcflux size is not 1!"));
                return false;
            }

            ::geoalgo::Sphere thevertexsphere(reco_neutrino.first.X(),
                                              reco_neutrino.first.Y(),
                                              reco_neutrino.first.Z(),
                                              5.0);
            bool _correct_ID = thevertexsphere.Contain(ev_mctruth->at(0).GetNeutrino().Nu().Trajectory().front().Position());
            _hcorrect_ID->Fill(_correct_ID);
        }

        // Here is how you loop over associated tracks:
        for (auto const& asstd_trk_pair : reco_neutrino.second) {
            auto const &asstd_trk = asstd_trk_pair.second;
            // std::cout << "This associated track has length = " << asstd_trk.Length() << "!" << std::endl;
        }

        passed_events++;

        return true;
    }


    bool SampleSelIIAAna::finalize() {

        std::cout << "SampleSelIIAAna finalize! "
                  << total_events << " analyzed in total, "
                  << passed_events << " reconstructed neutrinos." << std::endl;

        if (_fout) {
            _fout->cd();
            _hcorrect_ID->Write();
        }

        return true;
    }

}

#endif
