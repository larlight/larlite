#include "DataFormat/storage_manager.h"
#include "DataFormat/event_ass.h"
#include <iostream>
#include <fstream>
int main(int argc, char** argv) {

  if(argc < 4) {
    std::cerr<< "Usage: " << argv[0] << " PRODUCER_NAME FILE_LIST1 FILE_LIST2 " << std::endl;
    return 1;
  }

  larlite::storage_manager out_mgr(larlite::storage_manager::kWRITE);
  out_mgr.set_out_filename("out.root");
  out_mgr.open();
  std::string producer(argv[1]);
  std::vector<larlite::storage_manager*> in_mgr_v;
  size_t min_entries=1e10;
  for(size_t i=0; i<(argc-2); ++i) {
    //std::cout<<"Listing files for IO " << i << std::endl;
    auto in_mgr = new larlite::storage_manager(larlite::storage_manager::kREAD);
    std::ifstream fin(argv[i+2]);
    std::string word;
    while(fin>>word) {
      if(!word.empty()) {
        std::cout<<"IO " << i << " input: " << word << std::endl;
  	    in_mgr->add_in_filename(word);
      }
    }
    fin.close();
    //std::cout<<"Done listing files for IO " << i << std::endl;
    in_mgr->open();

    if(in_mgr->get_entries() < min_entries)
      min_entries = in_mgr->get_entries();

    in_mgr_v.emplace_back(in_mgr);
  }

  size_t run, subrun, event;
  size_t entry_counter=0;

  while(entry_counter < min_entries) {

    auto out_ass = out_mgr.get_data<larlite::event_ass>(producer);
    for(size_t i=0; i<in_mgr_v.size(); ++i) {
      auto& io = in_mgr_v[i];
      io->next_event(false);

      if(!i) { run = io->run_id(); subrun = io->subrun_id(); event = io->event_id(); }
      else if(run != io->run_id() || subrun != io->subrun_id() || event != io->event_id()) {
          std::cerr << "Event mis-alignment!" << std::endl;
          throw std::exception();
      }
      auto in_ass = io->get_data<larlite::event_ass>(producer);
      //in_ass->list_association();
      if(!in_ass) {
        std::cerr << "Input data not found!" << std::endl;
        throw std::exception();
      }
      auto keys_v = in_ass->association_keys();
      for(auto const& key : keys_v) {
        auto assid = out_ass->assid(key.first,key.second);
        if(assid == larlite::data::kINVALID_UINT)
            out_ass->set_association(key.first,key.second,in_ass->association(key.first,key.second),true);
      }
    }
    out_mgr.set_id(run,subrun,event);
    out_mgr.next_event(true);
    ++entry_counter;
  }

  for(auto& io : in_mgr_v) io->close();
  out_mgr.close();

  return 0;
}
