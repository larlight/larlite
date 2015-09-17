#ifndef LARLITE_CLUSTERPARAMSANA_CXX
#define LARLITE_CLUSTERPARAMSANA_CXX

#include "ClusterParamsAna.h"
#include "ClusterParams.h"
#include "LArUtil/GeometryHelper.h"
#include "LArUtil/Geometry.h"
#include "DefaultParamsAlg.h"
#include "CRUHelper.h"
#include "DataFormat/mcshower.h"

namespace cluster {

ClusterParamsAna::ClusterParamsAna()
    : _param_tree(nullptr)
{
    _fout = 0;
    _name = "ClusterParamsAna";
    _verbose = false;
}

bool ClusterParamsAna::initialize() {

    auto geo = larutil::Geometry::GetME();

    startPointW.resize(geo -> Nviews());
    startPointT.resize(geo -> Nviews());
    startSlope.resize(geo -> Nviews());
    prinSlope.resize(geo -> Nviews());

    int p = 0;
    for (auto & hist : startPointW) {
        hist = new TH1F(Form("startPointW_%i", p), "Start Point Wire Reco - True", 100, -25, 25);
        hist -> GetXaxis() -> SetTitle("Distance between Reco and True wire start point, cm");
        p ++;
    }
    p = 0;
    for (auto & hist : startPointT) {
        hist = new TH1F(Form("startPointT_%i", p), "Start Point Time Reco - True", 100, -25, 25);
        hist -> GetXaxis() -> SetTitle("Distance between Reco and True time start point, cm");
        p ++;
    }
    p = 0;
    for (auto & hist : startSlope) {
        hist = new TH1F(Form("startSlope_%i", p), "Start Angle Reco - True", 200, -M_PI, M_PI);
        hist -> GetXaxis() -> SetTitle("Difference in start-of-cluster angle and true angle, radians");
        p ++;
    }
    p = 0;
    for (auto & hist : prinSlope) {
        hist = new TH1F(Form("prinSlope_%i", p), "Prin Angle Reco - True", 200, -M_PI, M_PI);
        hist -> GetXaxis() -> SetTitle("Difference in principal axis angle and true angle, radians");
        p++;
    }

    if (_param_tree) delete _param_tree;
    _param_tree = new TTree("_param_tree", "Cluster Parameter TTree");
    _param_tree->Branch("_plane", &_plane, "plane/I");
    _param_tree->Branch("_E", &_E, "E/D");
    _param_tree->Branch("_containment", &_containment, "containment/D");
    _param_tree->Branch("_w", &_w, "w/D");
    _param_tree->Branch("_t", &_t, "t/D");
    _param_tree->Branch("_angle", &_angle, "angle/D");
    _param_tree->Branch("_start_angle", &_start_angle, "start_angle/D");
    _param_tree->Branch("_mc_w", &_mc_w, "mc_w/D");
    _param_tree->Branch("_mc_t", &_mc_t, "mc_t/D");
    _param_tree->Branch("_mc_angle", &_mc_angle, "mc_angle/D");
    _param_tree->Branch("_direction", &_direction, "_direction/D");


    _params_alg.SetVerbose(false);
    _params_alg.SetDebug(false);
    _params_alg.SetMinHits(25);

    return true;
}

bool ClusterParamsAna::analyze(::larlite::storage_manager* storage) {

    auto geoHelper = larutil::GeometryHelper::GetME();

    // Get the MCShowers:
    larlite::event_mcshower * ev_shower = storage -> get_data<larlite::event_mcshower>("mcreco");

    if (ev_shower -> size() != 1) {
        std::cerr << "Error, more than one mcshower!" << std::endl;
    }
    auto mcShower = ev_shower -> front();

    _E = mcShower.DetProfile().E();
    _containment = _E / mcShower.Start().E();

    std::vector<cluster_params> _params_vec;


    CRUHelper _cru_helper;
    _cru_helper.GenerateParams(storage, "mergeall", _params_vec);

    // Run ClusterParamsAlg on each cluster, compare to MC projection:
    for ( auto & clust : _params_vec) {
        _params_alg.FillParams(clust);
        _plane = clust.plane_id.Plane;
        // Get the mc info:
        Point2D mcStartPoint = geoHelper -> Point_3Dto2D(mcShower.Start().Position().Vect(), _plane);
        float mcSlope = geoHelper -> Slope_3Dto2D(mcShower.Start().Momentum().Vect(), _plane);
        if (clust.start_point.w == 0) {
            continue;
        }

        _mc_w = mcStartPoint.w;
        _mc_t = mcStartPoint.t;
        _mc_angle = atan(mcSlope);

        auto reco_start = clust.start_point;
        auto reco_end   = clust.end_point;

        _w = reco_start.w;
        _t = reco_start.t;
        _angle = clust.angle_2d;
        _start_angle = atan(clust.start_dir[1] / clust.start_dir[0]);

        if (_verbose)
            std::cout << "reco (" << reco_start.w << ", " << reco_start.t << ") vs "
                      << "(" << mcStartPoint.w << ", " << mcStartPoint.t << ").";

        // Figure out if this cluster is forwards, backwards, or just plain bad:
        float dist_start = sqrt(pow(reco_start.w - mcStartPoint.w, 2)
                                + pow(reco_start.t - mcStartPoint.t, 2));
        float dist_end   = sqrt(pow(reco_end.w - mcStartPoint.w, 2)
                                + pow(reco_end.t - mcStartPoint.t, 2));

        _direction = -1;
        if (dist_start < dist_end){
            _direction = 0;
        }
        else{
            _direction = 1;
        }

        if (_verbose) {
            if (dist_start < dist_end)
                std::cout << " Forwards." << dist_start << std::endl;
            else
                std::cout << " Backwards." << dist_end << std::endl;
        }

        // Fill the right histogram:
        startPointW.at(_plane) -> Fill(reco_start.w - mcStartPoint.w);
        startPointT.at(_plane) -> Fill(reco_start.t - mcStartPoint.t);

        startSlope.at(_plane) -> Fill(_start_angle - _mc_angle);
        prinSlope.at(_plane) -> Fill(_angle - _mc_angle);

        _param_tree->Fill();

    }// for all clusters


    return true;
}

bool ClusterParamsAna::finalize() {

    if (_fout) {
        _fout->cd();
        for (auto & hist : startPointW) {
            hist -> Write();
        }
        for (auto & hist : startPointT) {
            hist -> Write();
        }
        for (auto & hist : startSlope) {
            hist -> Write();
        }
        for (auto & hist : prinSlope) {
            hist -> Write();
        }
        if (_param_tree)
            _param_tree->Write();
    }

    return true;
}

}
#endif
