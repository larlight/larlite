#ifndef LARLITE_LOOKPIDA_CXX
#define LARLITE_LOOKPIDA_CXX

#include "LookPIDA.h"
#include "DataFormat/calorimetry.h"
#include "DataFormat/partid.h"
#include "DataFormat/track.h"

namespace larlite {

  bool LookPIDA::initialize() {

    if(hPIDA) delete hPIDA;
    hPIDA = new TH1D("hPIDA",
		     "PIDA; hPIDA; Counts",
		     200,0.,20.);
    return true;
  }
  
  bool LookPIDA::analyze(storage_manager* storage) {

    //auto const ev_calo = storage->get_data<event_calorimetry>("trackkalmanhitcalo");
    //    auto const ev_pid = storage->get_data<event_partid>("trackkalmanhitpid");    
 
    
    auto const ev_track = storage->get_data<event_track>("trackkalmanhit");
    
    for(size_t track_index=0; track_index<ev_track->size();++track_index){
      
      event_partid* ev_pid = nullptr;
      auto const& ass_pid_v = storage->find_one_ass(ev_track->id(),
                                                    ev_pid,
                                                    Form("%spid",ev_track->name().c_str()));
      
      event_calorimetry* ev_calo = nullptr;
      auto const& ass_calo_v = storage->find_one_ass(ev_track->id(),
                                                     ev_calo,
                                                     Form("%scalo",ev_track->name().c_str()));



      
      if(!ev_pid) {
	std::cerr<<"no event_partid!"<<std::endl;
	return false;
      }
      

      if(ev_pid->empty()) {
      	std::cout<<"event_partid empty..."<<std::endl;
	return false;
      }
      
      
      if(!ev_calo) {
	std::cerr<<"no event_calorimetry!"<<std::endl;
	return false;
      }
      
      if(ev_calo->empty()) {
	std::cout<<"event_partid calorimetry..."<<std::endl;
	return false;
      }
    
      std::cout<<"ass_pid_v  Size: "<< ass_pid_v[track_index].size()<<"\n";
      std::cout<<"PIDA: "<< ev_pid->at(ass_pid_v[track_index][0]).PIDA()<<"\n"; // it's one per plane 
      std::cout<<"PIDA: "<< ev_pid->at(ass_pid_v[track_index][1]).PIDA()<<"\n"; // there should be a plane id object
      std::cout<<"PIDA: "<< ev_pid->at(ass_pid_v[track_index][2]).PIDA()<<"\n"; // we need to check into DataFormat
    
      std::cout<<"ass_calo_v  Size: "<< ass_calo_v[track_index].size()<<"\n";
      std::cout<<"Calo: "<< ev_calo->at(ass_calo_v[track_index][0]).dEdx()[0]<<"\n"; // it's one per plane
      std::cout<<"Calo: "<< ev_calo->at(ass_calo_v[track_index][1]).dEdx()[0]<<"\n"; // and dEdx is a vector!
      std::cout<<"Calo: "<< ev_calo->at(ass_calo_v[track_index][2]).dEdx()[0]<<"\n"; // once again, we need to look into DataFormat

    } // End on tracks loop

    //    for(auto const& pid : *ev_pid)
    //  {
    //	hPIDA->Fill(pid.PIDA());
	//std::cout<<"PIDA: "<<pid.PIDA()<<std::endl;
    //  }
    return true;
  }

  bool LookPIDA::finalize() {
    
    if(_fout) {
      if(hPIDA->GetEntries()) hPIDA->Write();
    }
    return true;
  }

}
#endif
