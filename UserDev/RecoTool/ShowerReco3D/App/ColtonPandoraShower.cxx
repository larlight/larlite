#ifndef COLTONPANDORASHOWER_CXX
#define COLTONPANDORASHOWER_CXX

#include "ColtonPandoraShower.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"
#include "DataFormat/pfpart.h"
#include "ClusterRecoUtil/Alg/DefaultParamsAlg.h"

namespace showerreco {

bool ColtonPandoraShower::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D").
    //
    //
    if (! _fout ) {

    }

    return true;
}

bool ColtonPandoraShower::analyze(larlite::storage_manager* storage) {

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
    auto in_pfpart_v = storage->get_data<larlite::event_pfpart>(_input_producer);

    std::cout << "in_pfpart_v.size() is " << in_pfpart_v->size() << std::endl;

    // Get the clusters associated with the pfpart:
    ::larlite::event_cluster* ev_clust = nullptr;
    auto const& clust_index_v = storage->find_one_ass(in_pfpart_v->id(), ev_clust, _input_producer);

    if (! ev_clust ){
        std::cout << "No clusters found!\n";
        return false;
    }
    else{
        std::cout << "Clusters found!" << std::endl;
    }

    // And get the hits too:
    ::larlite::event_hit* ev_hit = nullptr;
    auto const& hit_index_v = storage->find_one_ass(ev_clust->id(), ev_hit, _input_producer);



    // Holder for new clusters:
    auto out_cluster_v = storage->get_data<larlite::event_cluster>(_output_producer);
    auto out_ass = storage->get_data<larlite::event_ass>(out_cluster_v->name());


    // set event ID through storage manager
    storage->set_id(storage->get_data<larlite::event_cluster>(_input_producer)->run(),
                    storage->get_data<larlite::event_cluster>(_input_producer)->subrun(),
                    storage->get_data<larlite::event_cluster>(_input_producer)->event_id());





    if (!ev_hit) {
        std::cout << "Did not find hit data product"
                  << "!" << std::endl;
        return false;
    }


    if (!hit_index_v.size())
        return false;


    // std::cout << "reset  id is " << out_cluster_v -> event_id() << std::endl;

    if (!ev_clust->size()) {

        print(larlite::msg::kWARNING, __FUNCTION__,
              Form("Skipping event %d since no cluster found...", ev_clust->event_id()));
        return false;
    }




    // Loop over the pfpart, and only save the ones that are interesting

    // Use this to save associations between clusters and hits:
    larlite::AssSet_t hit_ass;


    // Use an index to keep track
    size_t i = 0;
    for (auto & part : * in_pfpart_v) {

        // Use a bool to keep track of whether or not this particular pfpart passes
        bool pass = false;

        if (part.PdgCode() == 11) {
            pass = true;
        }

        if (pass) {
            // Then, save the clusters and hits in a new producer:
            out_cluster_v -> push_back(ev_clust->at(i));
            hit_ass.push_back(hit_index_v.at(i));
        }

        i ++;

    }

    out_ass->set_association(out_cluster_v->id(), ev_hit->id(), hit_ass);

    if (out_cluster_v->size() > 0)
        return true;
    else
        return false;
}

bool ColtonPandoraShower::finalize() {

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
