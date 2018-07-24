#ifndef SHOWERCLUSTERFINDER_CXX
#define SHOWERCLUSTERFINDER_CXX

#include "ShowerClusterFinder.h"
#include "DataFormat/cluster.h"
#include "ClusterRecoUtil/Alg/DefaultParamsAlg.h"

namespace showerreco {

bool ShowerClusterFinder::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D").
    //

    return true;
}

bool ShowerClusterFinder::analyze(larlite::storage_manager* storage) {

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

    // First, get all of the clusters desired as input

    std::cout << "You are running showerClusterFinder!  Great!\n"
              << "But, be aware that it's completely untuned and very little work has gone in"
              << "apart from the framework.\n";

    // Holder for new clusters:
    auto out_cluster_v = storage->get_data<larlite::event_cluster>(_output_producer);
    auto out_ass = storage->get_data<larlite::event_ass>(out_cluster_v->name());
    // std::cout << "Initial id is " << out_cluster_v -> event_id() << std::endl;


    // set event ID through storage manager
    storage->set_id(storage->get_data<larlite::event_cluster>(_input_producer)->run(),
                    storage->get_data<larlite::event_cluster>(_input_producer)->subrun(),
                    storage->get_data<larlite::event_cluster>(_input_producer)->event_id());

    // Get all of the clusters from this event:
    auto ev_clus = storage->get_data<larlite::event_cluster>(_input_producer);
    if (!ev_clus) {
        std::cout << "ev_clus is == 0, returning.\n";
        return false;
    }


    ::larlite::event_hit* ev_hit = nullptr;
    auto const& hit_index_v = storage->find_one_ass(ev_clus->id(), ev_hit, _input_producer);


    if (!ev_hit) {
        std::cout << "Did not find hit data product"
                  << "!" << std::endl;
        return false;
    }


    if (!hit_index_v.size())
        return false;


    // std::cout << "reset  id is " << out_cluster_v -> event_id() << std::endl;

    if (!ev_clus->size()) {

        print(larlite::msg::kWARNING, __FUNCTION__,
              Form("Skipping event %d since no cluster found...", ev_clus->event_id()));
        return false;
    }

    // Loop over the clusters, and save only the ones that have potential to be a shower


    cluster::DefaultParamsAlg params_alg ;
    cluster::cluster_params params;
    params_alg.SetVerbose(false);
    params_alg.SetDebug(false);
    params_alg.SetMinHits(50);


    size_t index = 0;

    larlite::AssSet_t hit_ass;

    for (auto const& hit_indices : hit_index_v) {

        if (hit_indices.size() < 50) {
            index ++;
            continue;
        }
        if ( ev_hit->at(hit_indices[0]).View() != larlite::geo::kCollection) {
            continue;
        }

        // Fill the cluster params alg
        _cru_helper.GenerateParams( hit_indices, ev_hit, params);

        params_alg.FillParams(params);


        // // params.Report();
        // std::cout << "Cluster in plane " << params.plane_id.Plane << " starting at "
        //           << "(" << params.start_point.w << ", " << params.start_point.t << "):\n";
        // params.PrintFANNVector();
        // // Also look at the dot product between the start direction and the pca axis:
        // std::cout << "   StartDir * PCA .......................... : "
        //           << params.start_dir[0] * params.principal_dir[0]
        //           +  params.start_dir[1] * params.principal_dir[1]
        //           << std::endl;
        // std::cout << "   Mean Charge ............................. : "
        //           << params.mean_charge << std::endl;

        // if (params.mean_charge < 5000 )
        // continue;

        // Only keep clusters that have potential to be showers, and only in the collection plane
        std::vector<float> dat;
        params.GetFANNVector(dat);
        // Check Opening - Closing Angle:
        if (dat[0] > 0.9) {
            continue;
        }
        // Check PCA:
        if (dat[1] > 0.9 || dat[1] < 0.5 ) {
            continue;
        }
        // Check length/width:
        if (dat[2] <  0.1 || dat[2] > 1.0) {
            continue;
        }
        // Check % Multihit wires:
        if (dat[3] < 0.1) {
            continue;
        }
        // Check Modified Hit Density:
        if (dat[4] > 0.9) {
            continue;
        }
        // Check Charge RMS /  Mean Charge:
        if (dat[5] > 0.9) {
            continue;
        }
        // Check sum charge / length:
        if (dat[6] > 0.9) {
            continue;
        }
        // Check start_dir (dot) PCA:
        if (params.start_dir[0] * params.principal_dir[0] +  params.start_dir[1] * params.principal_dir[1] < 0.98) {
            continue;
        }

        out_cluster_v->push_back(ev_clus->at(index));
        hit_ass.push_back(hit_indices);
        index ++;
    }

    out_ass->set_association(out_cluster_v->id(), ev_hit->id(), hit_ass);

    if (out_cluster_v->size() > 0)
        return true;
    else
        return false;
}

bool ShowerClusterFinder::finalize() {

    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
    //
    // Say you made a histogram pointer h1 to store. You can do this:
    //
    // if(_fout) { _fout->cd(); h1->Write(); }
    //
    // else
    //   print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    //

    return true;
}

}
#endif
