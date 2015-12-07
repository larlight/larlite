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
    if (fdEdXVsResRange) delete fdEdXVsResRange;
    fdEdXVsResRange = new TH2D("fdEdXVsResRange",
			       "dEdX vs Residual Range;  Residual Range [cm]; dEdX [MeV/cm]",
			       351, -0.05, 35.05, 501, -0.05, 50.05);

    return true;
  }
  
  bool LookPIDA::analyze(storage_manager* storage) {
    
    //    std::cout<<" New Event "<<std::endl<<std::endl;
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
	double averageDEdX = 0;
	if(mct.dEdx().size()){
	  for(unsigned int i = 0; i < mct.dEdx().size();++i)
	    {
	      averageDEdX+=mct.dEdx(i);
	      fdEdxTrue.push_back(mct.dEdx(i));
	      fStepEnergyTrue.push_back(mct[i+1].E());
	      length += mct.dX(i);
	    }      
	  fRangeTrue.push_back(length);
	  fHitsNumbTrue.push_back(mct.dEdx().size());
	  faverageDEdXTrue.push_back((averageDEdX/(double)mct.dEdx().size()));
	  
	}
	
	fStartXTrue.push_back(mct.Start().X());
	fStartYTrue.push_back(mct.Start().Y());
	fStartZTrue.push_back(mct.Start().Z());
	
	fEndXTrue.push_back(mct.End().X());
	fEndYTrue.push_back(mct.End().Y());
	fEndZTrue.push_back(mct.End().Z());
	
	fTruePdg.push_back(mct.PdgCode());
      } // end of loop on mctracks 
    fTracksNumbTrue.push_back(ev_mct->size());    

    auto const ev_track = storage->get_data<event_track>("trackkalmanhit");
    fTracksNumbReco.push_back(ev_track->size());
    
    size_t track_index=0;

    for(auto const& t : *ev_track){
      
      //    for( track_index<ev_track->size();){ 
      
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
    
      auto const pidObj0 = ev_pid->at(ass_pid_v[track_index][0]);
      auto const pidObj1 = ev_pid->at(ass_pid_v[track_index][1]);
      auto const pidObj2 = ev_pid->at(ass_pid_v[track_index][2]);

      auto const calObj0 = ev_calo->at(ass_calo_v[track_index][0]);
      auto const calObj1 = ev_calo->at(ass_calo_v[track_index][1]);
      auto const calObj2 = ev_calo->at(ass_calo_v[track_index][2]);


      fPIDA_0.push_back(pidObj0.PIDA());
      fPIDA_1.push_back(pidObj1.PIDA());
      fPIDA_2.push_back(pidObj2.PIDA());
      double averageDEdX = 0;

      for(unsigned int i = 0; i < calObj0.dEdx().size();++i)
	{
	  fdEdxReco_0.push_back(calObj0.dEdx()[i]);
	}
      for(unsigned int i = 0; i < calObj1.dEdx().size();++i)
	{
	  fdEdxReco_1.push_back(calObj1.dEdx()[i]);
	}
      for(unsigned int i = 0; i < calObj2.dEdx().size();++i)
	{
	  fdEdxReco_2.push_back(calObj2.dEdx()[i]);
	  averageDEdX+=calObj2.dEdx().size();
	}


      
      fRecoPdg_0.push_back(pidObj0.Pdg());
      fRecoPdg_1.push_back(pidObj1.Pdg());
      fRecoPdg_2.push_back(pidObj2.Pdg());

      fHitsNumbReco_0.push_back(calObj0.dEdx().size());
      fHitsNumbReco_1.push_back(calObj1.dEdx().size());
      fHitsNumbReco_2.push_back(calObj2.dEdx().size());

      fRangeReco.push_back(t.Length());
      
      fStartXReco.push_back(t.Vertex().X());
      fStartYReco.push_back(t.Vertex().Y());
      fStartZReco.push_back(t.Vertex().Z());

      fEndXReco.push_back(t.End().X());
      fEndYReco.push_back(t.End().Y());
      fEndZReco.push_back(t.End().Z());

      faverageDEdXReco.push_back(averageDEdX/ (double)calObj2.dEdx().size());
      ++track_index;
      /*
      //############################################################################# 
      //########## Some cout to understand the PID and Calorimetry objects ########## 
      //############################################################################# 
      std::cout<<std::endl<<std::endl;
      std::cout<<std::endl;
      std::cout<<"####################################################"<<std::endl;
      std::cout<<"####################################################"<<std::endl;
      std::cout<<"####################################################"<<std::endl;
      std::cout<<"################# Track Points  ####################"<<std::endl;
      std::cout<<"track size               : "<<t.NumberTrajectoryPoints()         <<std::endl;
      std::cout<<"track length             : "<<t.Length()                         <<std::endl;
      std::cout<<"track Projected length 0 : "<<t.ProjectedLength(geo::kU)         <<std::endl;
      std::cout<<"track Projected length 1 : "<<t.ProjectedLength(geo::kV)         <<std::endl;
      std::cout<<"track Projected length 2 : "<<t.ProjectedLength(geo::kZ)         <<std::endl;
 
      std::cout<<std::endl<<"################# checks  ####################"<<std::endl;
      std::cout<<"size 0             : "<<(t.NumberTrajectoryPoints()-calObj0.ResidualRange().size())<<std::endl;
      std::cout<<"size 1             : "<<(t.NumberTrajectoryPoints()-calObj1.ResidualRange().size())<<std::endl;
      std::cout<<"size 2             : "<<(t.NumberTrajectoryPoints()-calObj2.ResidualRange().size())<<std::endl;
      std::cout<<"length 0           : "<<(t.Length()-calObj0.Range())                               <<std::endl;
      std::cout<<"length 1           : "<<(t.Length()-calObj1.Range())                               <<std::endl;
      std::cout<<"length 2           : "<<(t.Length()-calObj2.Range())                               <<std::endl;


      std::cout<<std::endl<<"#################### Plane 0 ####################"<<std::endl;
      std::cout<<"calObj0.dEdx()          size : "<<calObj0.dEdx().size()         <<std::endl;
      std::cout<<"calObj0.dQdx()          size : "<<calObj0.dQdx().size()         <<std::endl;
      std::cout<<"calObj0.ResidualRange() size : "<<calObj0.ResidualRange().size()<<std::endl;
      std::cout<<"calObj0.DeadWireResRC() size : "<<calObj0.DeadWireResRC().size()<<std::endl;
      std::cout<<"calObj0.KineticEnergy()      : "<<calObj0.KineticEnergy()       <<std::endl;
      std::cout<<"calObj0.Range()              : "<<calObj0.Range()               <<std::endl;        
      std::cout<<"calObj0.TrkPitchVec()   size : "<<calObj0.TrkPitchVec().size()  <<std::endl;
      std::cout<<"calObj0.TrkPitchC()          : "<<calObj0.TrkPitchC()           <<std::endl;    
      std::cout<<"calObj0.PlaneID().TPC        : "<<calObj0.PlaneID().TPC         <<std::endl;
      std::cout<<"calObj0.PlaneID().Cryostat   : "<<calObj0.PlaneID().Cryostat    <<std::endl;
      std::cout<<"calObj0.PlaneID().Plane      : "<<calObj0.PlaneID().Plane       <<std::endl;
      std::cout<<"calObj0.XYZ()           size : "<<calObj0.XYZ().size()          <<std::endl;

      std::cout<<std::endl<<"#################### Plane 1 ####################"<<std::endl;
      std::cout<<"calObj1.dEdx()          size : "<<calObj1.dEdx().size()         <<std::endl;
      std::cout<<"calObj1.dQdx()          size : "<<calObj1.dQdx().size()         <<std::endl;
      std::cout<<"calObj1.ResidualRange() size : "<<calObj1.ResidualRange().size()<<std::endl;
      std::cout<<"calObj1.DeadWireResRC() size : "<<calObj1.DeadWireResRC().size()<<std::endl;
      std::cout<<"calObj1.KineticEnergy()      : "<<calObj1.KineticEnergy()       <<std::endl;
      std::cout<<"calObj1.Range()              : "<<calObj1.Range()               <<std::endl;
      std::cout<<"calObj1.TrkPitchVec()   size : "<<calObj1.TrkPitchVec().size()  <<std::endl;        
      std::cout<<"calObj1.TrkPitchC()          : "<<calObj1.TrkPitchC()           <<std::endl;    
      std::cout<<"calObj1.PlaneID().TPC        : "<<calObj1.PlaneID().TPC         <<std::endl;
      std::cout<<"calObj1.PlaneID().Cryostat   : "<<calObj1.PlaneID().Cryostat    <<std::endl;
      std::cout<<"calObj1.PlaneID().Plane      : "<<calObj1.PlaneID().Plane       <<std::endl;
      std::cout<<"calObj1.XYZ()           size : "<<calObj1.XYZ().size()          <<std::endl;

      std::cout<<std::endl<<"#################### Plane 2 ####################"<<std::endl;
      std::cout<<"calObj2.dEdx()          size : "<<calObj2.dEdx().size()         <<std::endl;
      std::cout<<"calObj2.dQdx()          size : "<<calObj2.dQdx().size()         <<std::endl;
      std::cout<<"calObj2.ResidualRange() size : "<<calObj2.ResidualRange().size()<<std::endl;
      std::cout<<"calObj2.DeadWireResRC() size : "<<calObj2.DeadWireResRC().size()<<std::endl;
      std::cout<<"calObj2.KineticEnergy()      : "<<calObj2.KineticEnergy()       <<std::endl;
      std::cout<<"calObj2.Range()              : "<<calObj2.Range()               <<std::endl;
      std::cout<<"calObj2.TrkPitchVec()   size : "<<calObj2.TrkPitchVec().size()  <<std::endl;        
      std::cout<<"calObj2.TrkPitchC()          : "<<calObj2.TrkPitchC()           <<std::endl;    
      std::cout<<"calObj2.PlaneID().TPC        : "<<calObj2.PlaneID().TPC         <<std::endl;
      std::cout<<"calObj2.PlaneID().Cryostat   : "<<calObj2.PlaneID().Cryostat    <<std::endl;
      std::cout<<"calObj2.PlaneID().Plane      : "<<calObj2.PlaneID().Plane       <<std::endl;
      std::cout<<"calObj2.XYZ()           size : "<<calObj2.XYZ().size()          <<std::endl;

      
      std::cout<<"##################################################################"<<std::endl;
      std::cout<<"#################### Let's try to understand  ####################"<<std::endl;
      std::cout<<"#################### the relationship between ####################"<<std::endl;
      std::cout<<"#################### ResidualRange and Range  ####################"<<std::endl;
      std::cout<<"##################################################################"<<std::endl;
      */
	if(calObj0.ResidualRange().size()){
	  int lastIndex = calObj0.ResidualRange().size()-1;
	  /*
	  std::cout<<"Range()              : "<<calObj0.Range()               <<std::endl;  
	  std::cout<<"ResidualRange 0      : "<<calObj0.ResidualRange()[0]        <<" "<<calObj0.dEdx()[0]        <<std::endl;  
	  std::cout<<"ResidualRange 1      : "<<calObj0.ResidualRange()[1]        <<" "<<calObj0.dEdx()[1]        <<std::endl;  
	  std::cout<<"ResidualRange size   : "<<calObj0.ResidualRange()[lastIndex]<<" "<<calObj0.dEdx()[lastIndex]<<std::endl<<std::endl;  
	  */
	  int dedexInt = 0;
	  for(auto const& resRang : calObj1.ResidualRange()){
	    //	std::cout<<"ResidualRanges              : "<<resRang<<std::endl;
	    fdEdXVsResRange->Fill(resRang,calObj0.dEdx()[dedexInt]);        
	    dedexInt++;
	  }
	}
      
    } // End on tracks loop

    
    
    fPIDATree->Fill();
    return true;
  }

  bool LookPIDA::finalize() {
    
    if(_fout) {
      if (fPIDATree)       fPIDATree->Write();
      if (fdEdXVsResRange) fdEdXVsResRange->Write();
    }
    return true;
  }

  void LookPIDA::ClearTreeVar(){
    
    fPIDA_0.clear();        
    fPIDA_1.clear();        
    fPIDA_2.clear();        
              
    fRecoPdg_0.clear();     
    fRecoPdg_1.clear();     
    fRecoPdg_2.clear();     
    fTruePdg.clear();       
    
    fdEdxReco_0.clear();    
    fdEdxReco_1.clear();    
    fdEdxReco_2.clear();    
    
    fdEdxTrue.clear();      

    fStartXTrue.clear();      
    fStartYTrue.clear();      
    fStartZTrue.clear();      
    
    fStartXReco.clear();      
    fStartYReco.clear();      
    fStartZReco.clear();      

    fEndXTrue.clear();      
    fEndYTrue.clear();      
    fEndZTrue.clear();      
    
    fEndXReco.clear();      
    fEndYReco.clear();      
    fEndZReco.clear();      
    

    /*          
    fdEdxDelta_0.clear();   
    fdEdxDelta_1.clear();   
    fdEdxDelta_2.clear();   
    */
        
    fStepEnergyReco.clear();
    fStepEnergyTrue.clear();
              
    fRangeReco.clear();     
    fRangeTrue.clear();     
    /*        
    fResRangeReco.clear();  
    fResRangeTrue.clear();  
              
    fKInitReco.clear();     
    fKInitTrue.clear();     
    */          
    fHitsNumbReco_0.clear();
    fHitsNumbReco_1.clear();
    fHitsNumbReco_2.clear();
    fHitsNumbTrue.clear();  
              
    fTracksNumbReco.clear();
    fTracksNumbTrue.clear();
    
    faverageDEdXTrue.clear();
    faverageDEdXReco.clear();

  }
  
  void LookPIDA::InitializeAnaTree()
  {
    if(fPIDATree) delete fPIDATree;
    fPIDATree = new TTree("fPIDATree","");
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

    fPIDATree->Branch("fStartXTrue"    ,"vector<double> ", &fStartXTrue    );
    fPIDATree->Branch("fStartYTrue"    ,"vector<double> ", &fStartYTrue    );
    fPIDATree->Branch("fStartZTrue"    ,"vector<double> ", &fStartZTrue    );
		       	       	       		       	       
    fPIDATree->Branch("fStartXReco"    ,"vector<double> ", &fStartXReco    );
    fPIDATree->Branch("fStartYReco"    ,"vector<double> ", &fStartYReco    );
    fPIDATree->Branch("fStartZReco"    ,"vector<double> ", &fStartZReco    );

    fPIDATree->Branch("fEndXTrue"      ,"vector<double> ", &fEndXTrue    );
    fPIDATree->Branch("fEndYTrue"      ,"vector<double> ", &fEndYTrue    );
    fPIDATree->Branch("fEndZTrue"      ,"vector<double> ", &fEndZTrue    );
		       	       	       		       	       
    fPIDATree->Branch("fEndXReco"      ,"vector<double> ", &fEndXReco    );
    fPIDATree->Branch("fEndYReco"      ,"vector<double> ", &fEndYReco    );
    fPIDATree->Branch("fEndZReco"      ,"vector<double> ", &fEndZReco    );


    /*	                 	       			                   
    fPIDATree->Branch("fdEdxDelta_0"   ,"vector<double> ", &fdEdxDelta_0   );
    fPIDATree->Branch("fdEdxDelta_1"   ,"vector<double> ", &fdEdxDelta_1   );
    fPIDATree->Branch("fdEdxDelta_2"   ,"vector<double> ", &fdEdxDelta_2   );
    */							                   
    fPIDATree->Branch("fStepEnergyReco","vector<double> ", &fStepEnergyReco);
    fPIDATree->Branch("fStepEnergyTrue","vector<double> ", &fStepEnergyTrue);
	                 				                   
    fPIDATree->Branch("fRangeReco"     ,"vector<double> ", &fRangeReco     );
    fPIDATree->Branch("fRangeTrue"     ,"vector<double> ", &fRangeTrue     );
    /*				                   
    fPIDATree->Branch("fResRangeReco"  ,"vector<double> ", &fResRangeReco  );
    fPIDATree->Branch("fResRangeTrue"  ,"vector<double> ", &fResRangeTrue  );
	                 	       			                   
    fPIDATree->Branch("fKInitReco"     ,"vector<double> ", &fKInitReco     );
    fPIDATree->Branch("fKInitTrue"     ,"vector<double> ", &fKInitTrue     );
    */	                 	       			                   
    fPIDATree->Branch("fHitsNumbReco_0","vector<double> ", &fHitsNumbReco_0);
    fPIDATree->Branch("fHitsNumbReco_1","vector<double> ", &fHitsNumbReco_1);
    fPIDATree->Branch("fHitsNumbReco_2","vector<double> ", &fHitsNumbReco_2);
    fPIDATree->Branch("fHitsNumbTrue"  ,"vector<double> ", &fHitsNumbTrue  );
	                 				                   
    fPIDATree->Branch("fTracksNumbReco","vector<double> ", &fTracksNumbReco);
    fPIDATree->Branch("fTracksNumbTrue","vector<double> ", &fTracksNumbTrue);

    fPIDATree->Branch("faverageDEdXTrue","vector<double> ", &faverageDEdXTrue);
    fPIDATree->Branch("faverageDEdXReco","vector<double> ", &faverageDEdXReco);


  }

}
#endif
