#ifndef LARLITE_LOOKPIDA_CXX
#define LARLITE_LOOKPIDA_CXX

#include "LookPIDA.h"
#include "DataFormat/calorimetry.h"
#include "DataFormat/partid.h"
#include "DataFormat/track.h"
#include "DataFormat/mctrack.h"

namespace larlite {

  bool LookPIDA::initialize() {
    InitializeAnaTree();
    return true;
  }
  
  bool LookPIDA::analyze(storage_manager* storage) {

    //Let's start by cleaning up the analysis TTree
    ClearTreeVar();



    // Retrieve mctrack data product
    auto ev_mct = storage->get_data<event_mctrack>("mcreco");
    
    if(!ev_mct || !(ev_mct->size())) {
      print(msg::kERROR,__FUNCTION__,"MCTrack data product not found!");
      return false;
    }

    for(auto const& mct : *ev_mct)
      {
	if(mct.dEdx().size()) std::cout<<"Shower 0 dEdx: "<<mct.dEdx()[0]<<std::endl;
      }




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
    
      //std::cout<<"ass_pid_v  Size: "<< ass_pid_v[track_index].size()<<"\n";
      //std::cout<<"PIDA: "<< ev_pid->at(ass_pid_v[track_index][0]).PIDA()<<"\n"; // it's one per plane 
      //std::cout<<"PIDA: "<< ev_pid->at(ass_pid_v[track_index][1]).PIDA()<<"\n"; // there should be a plane id object
      //std::cout<<"PIDA: "<< ev_pid->at(ass_pid_v[track_index][2]).PIDA()<<"\n"; // we need to check into DataFormat
    
      //std::cout<<"ass_calo_v  Size: "<< ass_calo_v[track_index].size()<<"\n";
      //std::cout<<"Calo size: "<< ev_calo->at(ass_calo_v[track_index][0]).dEdx().size()<<"\n"; // it's one per plane
      //std::cout<<"Calo size: "<< ev_calo->at(ass_calo_v[track_index][1]).dEdx().size()<<"\n"; // and dEdx is a vector!
      //std::cout<<"Calo size: "<< ev_calo->at(ass_calo_v[track_index][2]).dEdx().size()<<"\n"; // once again, we need to look into DataFormat

      fPIDA_0.push_back(ev_pid->at(ass_pid_v[track_index][0]).PIDA());
      fPIDA_1.push_back(ev_pid->at(ass_pid_v[track_index][1]).PIDA());
      fPIDA_2.push_back(ev_pid->at(ass_pid_v[track_index][2]).PIDA());
      
      fdEdx_0.push_back(ev_calo->at(ass_calo_v[track_index][0]).dEdx()[0]);
      fdEdx_1.push_back(ev_calo->at(ass_calo_v[track_index][1]).dEdx()[0]);
      fdEdx_2.push_back(ev_calo->at(ass_calo_v[track_index][2]).dEdx()[0]);
    } // End on tracks loop


    //    for(auto const& pid : *ev_pid)
    //  {
    //	hPIDA->Fill(pid.PIDA());
	//std::cout<<"PIDA: "<<pid.PIDA()<<std::endl;
    //  }

    fPIDATree->Fill();
    return true;
  }

  bool LookPIDA::finalize() {
    
    if(_fout) {
      if (fPIDATree) fPIDATree->Write();
    }
    return true;
  }

  void LookPIDA::ClearTreeVar(){
    fPIDA_0.clear();
    fPIDA_1.clear();
    fPIDA_2.clear();
    
    fdEdx_0.clear();
    fdEdx_1.clear();
    fdEdx_2.clear();
  }
  
  void LookPIDA::InitializeAnaTree()
  {
    if(fPIDATree) delete fPIDATree;
    fPIDATree = new TTree("fPIDATree","");
    fPIDATree->Branch("fPIDA_0","vector<double>", &fPIDA_0);
    fPIDATree->Branch("fPIDA_1","vector<double>", &fPIDA_1);
    fPIDATree->Branch("fPIDA_2","vector<double>", &fPIDA_2);
		       	      			    	   
    fPIDATree->Branch("fdEdx_0","vector<double>", &fdEdx_0);
    fPIDATree->Branch("fdEdx_1","vector<double>", &fdEdx_1);
    fPIDATree->Branch("fdEdx_2","vector<double>", &fdEdx_2);
  }

}
#endif
