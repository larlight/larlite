#ifndef ERTOOL_ALGOMICHELE_CXX
#define ERTOOL_ALGOMICHELE_CXX

#include "AlgoMichelE.h"

namespace ertool {

  size_t n_michel_e = 0;

  AlgoMichelE::AlgoMichelE() : AlgoBase()
  {
    _name     = "AlgoMichelE";
    michel_energy = 0;
  }

  void AlgoMichelE::ProcessEnd(TFile* fout){
 
    if(fout){
      fout->cd();
      if(michel_energy)
	michel_energy->Write();

    }

  }

  void AlgoMichelE::Finalize()
  {
    std::cout<<"Number of michel E's found is "<<n_michel_e<<std::endl;
  }

  void AlgoMichelE::ProcessBegin()
  {
    //    _alg_emp.LoadParams();
    //    _alg_emp.ProcessBegin();
    InitializeHistos();

    return;
  }

  void AlgoMichelE::InitializeHistos(){
    
    if(!michel_energy)
      michel_energy = new TH1F("michel_energy","michel_energy",100,0,100);
    
  }
  

  void AlgoMichelE::LoadParams(std::string fname, size_t version){
    
    // Load singleE params
    //    _alg_emp.LoadParams(fname,version);
    
    return;
  }

  ParticleSet AlgoMichelE::Reconstruct(const EventData &data)
  { 
    ParticleSet res;    
/*
    std::cout<<"start of reconstruct"<<std::endl;
    
    
    //Loop over all showers in the event
    
    for(size_t is = 0; is < data.Shower().size(); is++){
      
      Shower shower(data.Shower(is));
      std::cout<<"start of shower loop"<<std::endl;
      //Ask EMP if shower is electron, using only de/dx

      if( _alg_emp.LL(true, shower._dedx, -1) > _alg_emp.LL(false, shower._dedx, -1)){
	//Loop over all tracks and ask if this shower is at the end
	//(w/in 1cm) of any tracks,
	//Making sure the track is at least 50cm long
	for(size_t it = 0; it < data.Track().size(); it++){
	  Track track(data.Track(it));

	  //Making sure track has at least 2 points (otherwise geoalgo
	  // throws exceptions)
	  std::cout<<"before"<<std::endl;
	  std::cout<<"size is "<<track.size()<<std::endl;
	  if(track.size() < 2) continue;
	  std::cout<<"after"<<std::endl;
	  std::cout<<shower.Start().Dist(track.back())<<std::endl;
	  std::cout<<track.Length()<<std::endl;
	  if(shower.Start().Dist(track.back()) < 1.
	     && track.Length() > 50.){
	    std::cout<<"here"<<std::endl;
	    n_michel_e++;
	    
	    if(michel_energy)
	      michel_energy->Fill(shower._energy);
	    
	    break;
	  }//End if michel was found
	  std::cout<<"end if michel was found"<<std::endl;
	}//End loop over tracks
	std::cout<<"end loop over tracks"<<std::endl;
      }//End if shower is likely an electron
      
      std::cout<<"end if shower is likely electron"<<std::endl;
      
    }//End loop over showers
      
    std::cout<<"end of routine"<<std::endl;
*/

    return res;
    
  }
  
}

#endif
