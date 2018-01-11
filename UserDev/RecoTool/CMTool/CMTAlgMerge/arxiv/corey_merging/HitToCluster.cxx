#ifndef LARLITE_HITTOCLUSTER_CXX
#define LARLITE_HITTOCLUSTER_CXX

#include <list>

#include "HitToCluster.h"
#include "DataFormat/hit.h"
#include "DataFormat/cluster.h"

namespace larlite {

  bool HitToCluster::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //

    return true;
  }
  
  bool HitToCluster::analyze(storage_manager* storage) {
  
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
  
    // Holder for new clusters:
    auto out_cluster_v = storage->get_data<event_cluster>(_output_producer);
    // std::cout << "Initial id is " << out_cluster_v -> event_id() << std::endl;

    // Get all of the clusters from this event:
    auto ev_clus = storage->get_data<event_cluster>(_input_producer);
    if(!ev_clus){
      std::cout << "ev_clus is == 0, returning.\n";
      return false;
    }

    // std::cout << "reset  id is " << out_cluster_v -> event_id() << std::endl;
    
    if(!ev_clus->size()) {

      print(msg::kWARNING,__FUNCTION__,
      Form("Skipping event %d since no cluster found...",ev_clus->event_id()));
      return false;
    }

    // Get the associations between the clusters and hits:
    event_hit* ev_hit = nullptr;
    auto const& ass_info = storage->find_one_ass( ev_clus->id(), ev_hit, ev_clus->id().second );
    if(!ev_hit || ev_hit->size() < 1)
      return false;

    // Copy an index of every hit so that it's possible to keep track of 
    // the ones associated to a cluster (and the ones NOT associated):
    std::vector<int> hit_index_vec;
    hit_index_vec.resize(ev_hit->size());

    // std::cout << "Number of starting hits: "  << ev_hit -> size() << std::endl;
    int found_hits = 0;
    for(auto const& hit_indices : ass_info) {
      for(auto const& hit_index : hit_indices){
        // std::cout << "Hit index is " << hit_index << std::endl;

        hit_index_vec[hit_index] = -1;  
        found_hits ++;
      }
    }

    // Figure out which hits are left:
    std::list<int> leftoverHitIndices;
    for (unsigned int index = 0; index < hit_index_vec.size(); index++){
      if (hit_index_vec[index] != -1){
        leftoverHitIndices.push_back(index);
      }
    }

    // std::cout << "Number of found hits: "  << found_hits << std::endl;
    // std::cout << "Number of leftover hits: "  << leftoverHitIndices.size() << std::endl;

    // Try to make new clusters:

    // Make new associations
    AssSet_t hit_ass;
    for (unsigned int i = 0; i < ass_info.size(); i ++){
      hit_ass.push_back(ass_info.at(i));
      out_cluster_v -> push_back(ev_clus -> at(i));
    }

    // Include the old clusters and associations into the new clusters
    // and associations:
    // std::cout << "original_hit_ass size: " << original_hit_ass.size() << std::endl;

    // hit_ass = original_hit_ass;
    // out_cluster_v = ev_clus;

    for (auto index : leftoverHitIndices){
      AssUnit_t new_association;
      // std::cout << "Getting the hit at index " << index << std::endl;
      auto hit = ev_hit->at(index);
      out_cluster_v -> push_back(larlite::cluster());
      out_cluster_v -> back().set_integral(hit.Integral(),0,0);
      out_cluster_v -> back().set_id(1);
      out_cluster_v -> back().set_view(hit.View());

      new_association.push_back(index);
      hit_ass.push_back(new_association);

    }
    // std::cout << "output id is: " << out_cluster_v -> event_id() << std::endl;
    auto out_ass = storage->get_data<event_ass>(out_cluster_v->name());
    out_ass->set_association(out_cluster_v->id(), ev_hit->id(), hit_ass);

    // // Print out all the hits in the new clusters on one plane:
    // for(auto const& hit_indices : hit_ass) {
    //   if (ev_hit -> at(hit_indices[0]).View() == 1) continue;
    //   for(auto const& hit_index : hit_indices){
    //       std::cout << "This hit is at " << ev_hit -> at(hit_index).Wire()
    //                 << ", " << ev_hit -> at(hit_index).PeakTime() << std::endl;
    //   }
    //   std::cout << std::endl;
    // }
    // std::cout << "ev_clus size: " << ev_clus -> size() << std::endl;
    // std::cout << "original_hit_ass size is " << original_hit_ass.size() << std::endl;
    // std::cout << "Cluster size is " << out_cluster_v -> size() << std::endl;
    // std::cout << "Assoc size is " << hit_ass.size() << std::endl;

    return true;
  }

  bool HitToCluster::finalize() {

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
  
    // std::cout << "Finalized!!\n\n\n\n";

    return true;
  }

}
#endif
