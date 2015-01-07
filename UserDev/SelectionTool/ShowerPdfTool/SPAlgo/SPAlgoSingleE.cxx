#ifndef SELECTIONTOOL_SPALGOSINGLEE_CXX
#define SELECTIONTOOL_SPALGOSINGLEE_CXX

#include "SPAlgoSingleE.h"

namespace sptool {

  size_t counter = 0; 

  SPAlgoSingleE::SPAlgoSingleE() : SPAlgoBase()
  {
    _name     = "SPAlgoSingleE";
  }

  void SPAlgoSingleE::Reset()
  {
    std::cout<<"counter = "<<counter<<std::endl;
  }
  
  void SPAlgoSingleE::ProcessBegin()
  {
    _alg_emp.ProcessBegin();

    return;
  }

  void SPAlgoSingleE::LoadParams(std::string fname, size_t version){
    
    // Load EMPart params
    _alg_emp.LoadParams(fname,version);
    
    return;
  }

  SPArticleSet SPAlgoSingleE::Reconstruct(const SPAData &data){
    
    SPArticleSet res;
    
    //Loop over showers in the event
    for(auto const& shower : data._showers) {
      
      if(!IsShowerElectron(shower)) continue;
      	
      //Save a neutrino particle, with daughters being the proton and electron
      SPArticle p_nu;
      //nue
      p_nu.pdg_code(12);
      //position = shower start position
      p_nu.pos(shower.Start());
      //energy = shower energy
      p_nu.energy(shower._energy);
      
      //daughters
      SPArticle p_e;
      p_e.pdg_code(11);
      p_e.pos(shower.Start());
      p_e.energy(shower._energy);
      	
      std::vector<sptool::SPArticle> daughters;
      daughters.push_back(p_e);
      p_nu.daughters( daughters );
      
      //save the neutrino
      res.push_back(p_nu);
      counter++;
      
    }
    
    return res;
  }
  

  bool SPAlgoSingleE::IsShowerElectron(const sptool::SPAShower &shower){
    
    //Make sure the shower is likely an electron
    //by using LL function from an SPAlgoEMPart instance
    //(LL uses only dEdX data if "dist" is plugged in negative)
    //If likelihood its gamma is more than electron, skip
    
    bool e_like = (
      _alg_emp.LL(true, shower._dedx, -1.) >
      _alg_emp.LL(false,shower._dedx, -1.)) ?
      true : false;
    
    return e_like;
    
  }

  /*
  bool SPAlgoSingleE::DoesProtonMatchElectron(const sptool::SPATrack &track, 
					 const sptool::SPAShower &shower){

    //Is the start point of proton close to start point of shower?
    double P_E_dist = shower.Start().Dist(track.at(0));
    //Hard cut on 1cm for now. This value chosen after viewing some std::couts.
    //Later: use a PDF to determine the likelihood "dist" is small enough
    return  (P_E_dist < 1.) ? true : false;
    
  }
  */
}

#endif
