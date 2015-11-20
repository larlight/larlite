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
	double length = 0;
	if(mct.dEdx().size()){
	  for(unsigned int i = 0; i < mct.dEdx().size();++i)
	    {
	      fdEdxTrue.push_back(mct.dEdx(i));
	      fStepEnergyTrue.push_back(mct[i+1].E());
	      length += mct.dX(i);
	    }      
	  fRangeTrue.push_back(length);
	  fHitsNumbTrue.push_back(mct.dEdx().size());
	}
<<<<<<< HEAD
	fTracksNumbTrue.push_back(ev_mct->size());
      }
    

    auto const ev_track = storage->get_data<event_track>("trackkalmanhit");
    fTracksNumbReco.push_back(ev_track->size());
	
    for(size_t track_index=0; track_index<ev_track->size();++track_index){
=======
	fTruePdg.push_back(mct.PdgCode());
      } // end of loop on mctracks 
    fTracksNumbTrue.push_back(ev_mct->size());    

    auto const ev_track = storage->get_data<event_track>("trackkalmanhit");
    fTracksNumbReco.push_back(ev_track->size());
    
    size_t track_index=0;
    for(auto const& t : *ev_track){
      
      //    for( track_index<ev_track->size();){ 
>>>>>>> 6f9994df970a4e5b3f39ead90c9c60cb1a091108
      
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
    
     


      fPIDA_0.push_back(ev_pid->at(ass_pid_v[track_index][0]).PIDA());
      fPIDA_1.push_back(ev_pid->at(ass_pid_v[track_index][1]).PIDA());
      fPIDA_2.push_back(ev_pid->at(ass_pid_v[track_index][2]).PIDA());
      
      for(unsigned int i = 0; i < ev_calo->at(ass_calo_v[track_index][0]).dEdx().size();++i)
	{
	  fdEdxReco_0.push_back(ev_calo->at(ass_calo_v[track_index][0]).dEdx()[i]);
	}
      for(unsigned int i = 0; i < ev_calo->at(ass_calo_v[track_index][1]).dEdx().size();++i)
	{
	  fdEdxReco_1.push_back(ev_calo->at(ass_calo_v[track_index][1]).dEdx()[i]);
	}
      for(unsigned int i = 0; i < ev_calo->at(ass_calo_v[track_index][2]).dEdx().size();++i)
	{
	  fdEdxReco_2.push_back(ev_calo->at(ass_calo_v[track_index][2]).dEdx()[i]);
	}

<<<<<<< HEAD
      //      fStepEnergyReco.push_back();

=======

      
      fRecoPdg_0.push_back(ev_pid->at(ass_pid_v[track_index][0]).Pdg());
      fRecoPdg_1.push_back(ev_pid->at(ass_pid_v[track_index][1]).Pdg());
      fRecoPdg_2.push_back(ev_pid->at(ass_pid_v[track_index][2]).Pdg());

      fHitsNumbReco_0.push_back(ev_calo->at(ass_calo_v[track_index][0]).dEdx().size());
      fHitsNumbReco_1.push_back(ev_calo->at(ass_calo_v[track_index][1]).dEdx().size());
      fHitsNumbReco_2.push_back(ev_calo->at(ass_calo_v[track_index][2]).dEdx().size());

      fRangeReco.push_back(t.Length());

      ++track_index;
>>>>>>> 6f9994df970a4e5b3f39ead90c9c60cb1a091108
    } // End on tracks loop


    
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
      
<<<<<<< HEAD
=======
      fRecoPdg_0.clear();
      fRecoPdg_1.clear();
      fRecoPdg_2.clear();
>>>>>>> 6f9994df970a4e5b3f39ead90c9c60cb1a091108
      fTruePdg.clear();
      
      fdEdxReco_0.clear();
      fdEdxReco_1.clear();
      fdEdxReco_2.clear();
      
      fdEdxTrue.clear();
      
      fdEdxDelta_0.clear();
      fdEdxDelta_1.clear();
      fdEdxDelta_2.clear();
      
      fRangeReco.clear();
      fRangeTrue.clear();
      
      fResRangeReco.clear();
      fResRangeTrue.clear();
      
      fKInitReco.clear();
      fKInitTrue.clear();
      
<<<<<<< HEAD
      fHitsNumbReco.clear();
=======
      fHitsNumbReco_0.clear();
      fHitsNumbReco_1.clear();
      fHitsNumbReco_2.clear();
>>>>>>> 6f9994df970a4e5b3f39ead90c9c60cb1a091108
      fHitsNumbTrue.clear();
      
      fTracksNumbReco.clear();
      fTracksNumbTrue.clear();      

  }
  
  void LookPIDA::InitializeAnaTree()
  {
    if(fPIDATree) delete fPIDATree;
    fPIDATree = new TTree("fPIDATree","");
<<<<<<< HEAD
    fPIDATree->Branch("fPIDA_0        ","vector<double> ", &fPIDA_0        );
    fPIDATree->Branch("fPIDA_1        ","vector<double> ", &fPIDA_1        );
    fPIDATree->Branch("fPIDA_2        ","vector<double> ", &fPIDA_2        );
	 	   	      			   
    fPIDATree->Branch("fTruePdg       ","vector<double> ", &fTruePdg       );
	                 
    fPIDATree->Branch("fdEdxReco_0    ","vector<double> ", &fdEdxReco_0    );
    fPIDATree->Branch("fdEdxReco_1    ","vector<double> ", &fdEdxReco_1    );
    fPIDATree->Branch("fdEdxReco_2    ","vector<double> ", &fdEdxReco_2    );
	                 
    fPIDATree->Branch("fdEdxTrue      ","vector<double> ", &fdEdxTrue      );
	                 
    fPIDATree->Branch("fdEdxDelta_0   ","vector<double> ", &fdEdxDelta_0   );
    fPIDATree->Branch("fdEdxDelta_1   ","vector<double> ", &fdEdxDelta_1   );
    fPIDATree->Branch("fdEdxDelta_2   ","vector<double> ", &fdEdxDelta_2   );
=======
    fPIDATree->Branch("fPIDA_0"        ,"vector<double> ", &fPIDA_0        );
    fPIDATree->Branch("fPIDA_1"        ,"vector<double> ", &fPIDA_1        );
    fPIDATree->Branch("fPIDA_2"        ,"vector<double> ", &fPIDA_2        );

    fPIDATree->Branch("fRecoPdg_0"     ,"vector<double> ", &fRecoPdg_0     );
    fPIDATree->Branch("fRecoPdg_1"     ,"vector<double> ", &fRecoPdg_1     );
    fPIDATree->Branch("fRecoPdg_2"     ,"vector<double> ", &fRecoPdg_2     );	 	   	      			   
    fPIDATree->Branch("fTruePdg"       ,"vector<double> ", &fTruePdg       );
	                 
    fPIDATree->Branch("fdEdxReco_0"    ,"vector<double> ", &fdEdxReco_0    );
    fPIDATree->Branch("fdEdxReco_1"    ,"vector<double> ", &fdEdxReco_1    );
    fPIDATree->Branch("fdEdxReco_2"    ,"vector<double> ", &fdEdxReco_2    );
	                 
    fPIDATree->Branch("fdEdxTrue"      ,"vector<double> ", &fdEdxTrue      );
	                 	       
    fPIDATree->Branch("fdEdxDelta_0"   ,"vector<double> ", &fdEdxDelta_0   );
    fPIDATree->Branch("fdEdxDelta_1"   ,"vector<double> ", &fdEdxDelta_1   );
    fPIDATree->Branch("fdEdxDelta_2"   ,"vector<double> ", &fdEdxDelta_2   );
>>>>>>> 6f9994df970a4e5b3f39ead90c9c60cb1a091108

    fPIDATree->Branch("fStepEnergyReco","vector<double> ", &fStepEnergyReco);
    fPIDATree->Branch("fStepEnergyTrue","vector<double> ", &fStepEnergyTrue);
	                 
<<<<<<< HEAD
    fPIDATree->Branch("fRangeReco     ","vector<double> ", &fRangeReco     );
    fPIDATree->Branch("fRangeTrue     ","vector<double> ", &fRangeTrue     );
	                 		
    fPIDATree->Branch("fResRangeReco  ","vector<double> ", &fResRangeReco  );
    fPIDATree->Branch("fResRangeTrue  ","vector<double> ", &fResRangeTrue  );
	                 
    fPIDATree->Branch("fKInitReco     ","vector<double> ", &fKInitReco     );
    fPIDATree->Branch("fKInitTrue     ","vector<double> ", &fKInitTrue     );
	                 		
    fPIDATree->Branch("fHitsNumbReco  ","vector<double> ", &fHitsNumbReco  );
    fPIDATree->Branch("fHitsNumbTrue  ","vector<double> ", &fHitsNumbTrue  );
=======
    fPIDATree->Branch("fRangeReco"     ,"vector<double> ", &fRangeReco     );
    fPIDATree->Branch("fRangeTrue"     ,"vector<double> ", &fRangeTrue     );
	                 		
    fPIDATree->Branch("fResRangeReco"  ,"vector<double> ", &fResRangeReco  );
    fPIDATree->Branch("fResRangeTrue"  ,"vector<double> ", &fResRangeTrue  );
	                 	       
    fPIDATree->Branch("fKInitReco"     ,"vector<double> ", &fKInitReco     );
    fPIDATree->Branch("fKInitTrue"     ,"vector<double> ", &fKInitTrue     );
	                 	       	
    fPIDATree->Branch("fHitsNumbReco_0","vector<double> ", &fHitsNumbReco_0);
    fPIDATree->Branch("fHitsNumbReco_1","vector<double> ", &fHitsNumbReco_1);
    fPIDATree->Branch("fHitsNumbReco_2","vector<double> ", &fHitsNumbReco_2);
    fPIDATree->Branch("fHitsNumbTrue"  ,"vector<double> ", &fHitsNumbTrue  );
>>>>>>> 6f9994df970a4e5b3f39ead90c9c60cb1a091108
	                 
    fPIDATree->Branch("fTracksNumbReco","vector<double> ", &fTracksNumbReco);
    fPIDATree->Branch("fTracksNumbTrue","vector<double> ", &fTracksNumbTrue);

  }

}
#endif

/*
<<<<<<< HEAD
      fTruePdg.clear();
            
      fdEdxTrue.clear();
      
      fdEdxDelta_0.clear();
      fdEdxDelta_1.clear();
      fdEdxDelta_2.clear();
      
      fRangeReco.clear();

      
=======
      fdEdxDelta_0.clear();
      fdEdxDelta_1.clear();
      fdEdxDelta_2.clear();
          
>>>>>>> 6f9994df970a4e5b3f39ead90c9c60cb1a091108
      fResRangeReco.clear();
      fResRangeTrue.clear();
      
      fKInitReco.clear();
      fKInitTrue.clear();
      
<<<<<<< HEAD
      fHitsNumbReco.clear();
=======
      fStepEnergyReco.push_back();
>>>>>>> 6f9994df970a4e5b3f39ead90c9c60cb1a091108
*/
