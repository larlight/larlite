#ifndef LARLITE_DROPSINGLES_CXX
#define LARLITE_DROPSINGLES_CXX

#include <list>

#include "DropSingles.h"
#include "DataFormat/hit.h"
#include "DataFormat/cluster.h"

namespace larlite {

  bool DropSingles::initialize() {

    //
    // This function is called in the beginning of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //
    // _input_producer = "";
    return true;
  }
  
  bool DropSingles::analyze(storage_manager* storage) {
  
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

    event_hit* ev_hit = nullptr;
    auto const& ass_info = storage->find_one_ass( ev_clus->id(), ev_hit, ev_clus->id().second );
    if(!ev_hit || ev_hit->size() < 1)
      return false;

    // Copy an index of every hit so that it's possible to keep track of 
    // the ones associated to a cluster (and the ones NOT associated):
    // std::vector<int> hit_index_vec;
    // hit_index_vec.resize(ev_hit->size());

    // Loop Over the clusters and keep the ones with multiple hits
    // in the new set of clusters and associations
 
    AssSet_t hit_ass;
    unsigned int i = 0;
    for(auto const& hit_indices : ass_info) {
      if (hit_indices.size() > 1){
        hit_ass.push_back(ass_info.at(i));
        out_cluster_v -> push_back(ev_clus -> at(i));
      }

      i++;

    }
    auto ev_ass = storage->get_data<event_ass>(out_cluster_v->name());
    ev_ass->set_association(out_cluster_v->id(), ev_hit->id(), hit_ass);

    return true;
  }

  bool DropSingles::finalize() {

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
