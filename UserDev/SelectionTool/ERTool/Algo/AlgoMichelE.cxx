#ifndef ERTOOL_ALGOMICHELE_CXX
#define ERTOOL_ALGOMICHELE_CXX

#include "AlgoMichelE.h"

namespace ertool {

  size_t n_michel_e = 0;

  AlgoMichelE::AlgoMichelE() : AlgoBase()
  {
    _name     = "AlgoMichelE";
    michel_energy = 0;
    shower_st_to_track_end = 0;
  }

  void AlgoMichelE::ProcessEnd(TFile* fout){
 
    if(fout){
      fout->cd();
      if(michel_energy)
	michel_energy->Write();
      if(shower_st_to_track_end)
	shower_st_to_track_end->Write();

    }

  }

  void AlgoMichelE::Finalize()
  {
    std::cout<<"Number of michel E's found is "<<n_michel_e<<std::endl;
  }

  void AlgoMichelE::ProcessBegin()
  {
    _alg_emp.LoadParams();
    _alg_emp.ProcessBegin();
    InitializeHistos();

    return;
  }

  void AlgoMichelE::InitializeHistos(){
    
    if(!michel_energy)
      michel_energy = new TH1F("michel_energy","michel_energy",100,0,100);
    if(!shower_st_to_track_end)
      shower_st_to_track_end = new TH1F("shower_st_to_track_end","shower_st_to_track_end",100,0,100);
  }
  

  void AlgoMichelE::LoadParams(std::string fname, size_t version){
    
    // Load singleE params
    _alg_emp.LoadParams(fname,version);
    
    return;
  }

  ParticleSet AlgoMichelE::Reconstruct(const EventData &data)
  { 
    ParticleSet res;    

    //Loop over all showers in the event    
    for(size_t is = 0; is < data.Shower().size(); is++){

      Shower shower(data.Shower(is));

      //Ask EMP if shower is electron, using only de/dx
      if( _alg_emp.LL(true, shower._dedx, -1) > _alg_emp.LL(false, shower._dedx, -1)){

	//Loop over all tracks 
	for(size_t it = 0; it < data.Track().size(); it++){

	  Track track(data.Track(it));

	  shower_st_to_track_end->Fill(shower.Start().Dist(track.back()));

	  //Check if this shower is at the end (w/in 3cm) of this tracks,
	  //Making sure the track is at least 10cm long
	  if(shower.Start().Dist(track.back()) < 3. && track.Length() > 10.){
	    n_michel_e++;
	    
	    if(michel_energy)
	      michel_energy->Fill(shower._energy);
	    break;

	  }//End if michel was found

	}//End loop over tracks
      }//End if shower is likely an electron
    }//End loop over showers

    return res;
    
  }
  
}

#endif
