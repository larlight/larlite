#ifndef SELECTIONTOOL_SPALGOPE_CXX
#define SELECTIONTOOL_SPALGOPE_CXX

#include "SPAlgoPE.h"

namespace sptool {

  size_t counter = 0; 

  SPAlgoPE::SPAlgoPE() : SPAlgoBase()
  {
    _name     = "SPAlgoPE";
  }

  void SPAlgoPE::Reset()
  {
    std::cout<<"counter = "<<counter<<std::endl;
  }
  
  void SPAlgoPE::ProcessBegin()
  {
    _alg_emp.ProcessBegin();

    return;
  }

  void SPAlgoPE::LoadParams(std::string fname, size_t version){
    
    // Load EMPart params
    _alg_emp.LoadParams(fname,version);

    return;
  }

  SPArticleSet SPAlgoPE::Reconstruct(const SPAData &data){
    
    SPArticleSet res;
    
    //Loop over showers in the event
    for(auto const& shower : data._showers) {
      
      //Make sure the shower is likely an electron
      //by using LL function from an SPAlgoEMPart instance
      //(LL uses only dEdX data if "dist" is plugged in negative)
      double e_like = _alg_emp.LL(true, shower._dedx, -1.);
      if(e_like < -0.1) continue;
      
      /// Loop over tracks in the event, looking for protons
      for(auto const& track : data._tracks) {
	
	//Skip if this track isn't a proton
	if(track._pid != SPATrack::kProton) continue;
	
	//Is the start point of proton close to start point of shower?
	double P_E_dist = shower.Start().Dist(track.at(0));
	//Hard cut on 1cm for now. This value chosen after viewing some couts.
	//Later: use a PDF to determine the likelihood "dist" is small enough
	if(P_E_dist > 1.) continue;
	
	//We have a potential match!
	//Since I want only 1 proton, let's make sure no other protons match
	bool quit_shower_loop = false;
	for(auto iter = data._tracks.rbegin();
	    (*iter) != track;
	    ++iter){
	  double check_dist = shower.Start().Dist((*iter).at(0));
	  if(check_dist < 1.){
	    //After finding a good pair of p+e, I found another p that fits it... 
	    //so, now I'm quitting and finding the next shower to loop over,
	    //and I'm not saving this p+e pair to res.
	    quit_shower_loop = true;
	    break;
	  }
	}
	if(quit_shower_loop) break;

	//Save a neutrino particle, with daughters being the proton and electron
	SPArticle p_nu;
	//nue
	p_nu.pdg_code(12);
	//position = shower start position
	p_nu.pos(shower.Start());
	//energy = shower energy + proton energy
	p_nu.energy(shower._energy + track._energy);
	
	//daughters
	SPArticle p_e;
	p_e.pdg_code(11);
	p_e.pos(shower.Start());
	p_e.energy(shower._energy);
	
	SPArticle p_p;
	p_p.pdg_code(2212);
	p_p.pos(track.at(0));
	p_p.energy(track._energy);
	
	std::vector<sptool::SPArticle> daughters;
	daughters.push_back(p_e);
	daughters.push_back(p_p);
	p_nu.daughters( daughters );
	
	//save the neutrino
	res.push_back(p_nu);
	counter++;
	
	//********
	//For now, skip the rest of the tracks
	//This is incorrect, but I'm just looking for 1 proton 
	//I should enforce that only 1 proton correlates with
	//the electron
      }

    }
    
    return res;
  }

}

#endif
