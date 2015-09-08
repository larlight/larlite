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

bool ClusterParamsAna::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D").
    //

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


    return true;
}

bool ClusterParamsAna::analyze(::larlite::storage_manager* storage) {

    //
    // Do your event-by-event analysis here. This function is called for
    // each event in the loop. You have "storage" pointer which contains
    // event-wise data. To see what is available, check the "Manual.pdf":
    //
    // http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
    //
    // Or you can refer to Base/DataFormatConstants.hh for available data type
    // enum values. Here is one example of getting PMT waveform collection.
    //
    // event_fifo* my_pmtfifo_v = (event_fifo*)(storage->get_data(DATA::PMFIFO));
    //
    // if( event_fifo )
    //
    //   std::cout << "Event ID: " << my_pmtfifo_v->event_id() << std::endl;
    //

    auto geoHelper = larutil::GeometryHelper::GetME();

    // Get the MCShowers:
    larlite::event_mcshower * ev_shower = storage -> get_data<larlite::event_mcshower>("mcreco");

    if (ev_shower -> size() != 1) {
        std::cerr << "Error, more than one mcshower!" << std::endl;
    }
    auto mcShower = ev_shower -> front();

    std::vector<cluster_params> _params_vec;

    DefaultParamsAlg params_alg ;
    params_alg.SetVerbose(false);
    params_alg.SetDebug(false);
    params_alg.SetMinHits(25);

    CRUHelper _cru_helper;
    _cru_helper.GenerateParams(storage, "mergeall", _params_vec);

    // Run ClusterParamsAlg on each cluster, compare to MC projection:
    for ( auto & clust : _params_vec) {
        params_alg.FillParams(clust);
        int plane = clust.plane_id.Plane;
        // Get the mc info:
        Point2D mcStartPoint = geoHelper -> Point_3Dto2D(mcShower.Start().Position().Vect(), plane);
        float mcSlope = geoHelper -> Slope_3Dto2D(mcShower.Start().Momentum().Vect(), plane);
        if (clust.start_point.w == 0) {
            continue;
        }
        std::cout << "reco (" << clust.start_point.w << ", " << clust.start_point.t << ") vs "
                  << "(" << mcStartPoint.w << ", " << mcStartPoint.t << ").";

        // Figure out if this cluster is forwards, backwards, or just plain bad:
        float dist_start = sqrt(pow(clust.start_point.w - mcStartPoint.w,2)
                              + pow(clust.start_point.t - mcStartPoint.t,2));
        float dist_end   = sqrt(pow(clust.end_point.w - mcStartPoint.w,2)
                              + pow(clust.end_point.t - mcStartPoint.t,2));

        if (dist_start < dist_end){
            std::cout << " Forwards." << dist_start << std::endl;
        }
        else {
            std::cout << " Backwards." << dist_end << std::endl;
        }

        // Fill the right histogram:
        startPointW.at(plane) -> Fill(clust.start_point.w - mcStartPoint.w);
        startPointT.at(plane) -> Fill(clust.start_point.t - mcStartPoint.t);
        float mcAngle, startAngle, prinAngle;
        mcAngle = atan(mcSlope);
        prinAngle  = clust.angle_2d;
        startAngle = atan(clust.start_dir[1] / clust.start_dir[0]);

        startSlope.at(plane) -> Fill(startAngle - mcAngle);
        prinSlope.at(plane) -> Fill(prinAngle - mcAngle);

    }


    return true;
}

bool ClusterParamsAna::finalize() {

    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
    //
    // Say you made a histogram pointer h1 to store. You can do this:
    //
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
    }
    //
    // else
    //   print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    //

    return true;
}

}
#endif
