#ifndef SELECTIONTOOL_SPALGOSINGLEE_CXX
#define SELECTIONTOOL_SPALGOSINGLEE_CXX

#include "SPAlgoSingleE.h"

namespace sptool {

  size_t single_e_shower_events = 0;
  
  SPAlgoSingleE::SPAlgoSingleE() : SPAlgoBase()
  {
    _name     = "SPAlgoSingleE";
  }

  void SPAlgoSingleE::Reset()
  {
    std::cout<<"Number of (independent) single electron shower events found by SPAlgoSingleE = "<<single_e_shower_events<<std::endl;
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
    
    std::vector<sptool::SPAShower> e_showers;

    //Loop over all showers in the event, store ones that are electron-like
    for(auto const& shower : data._showers) 
      if(IsShowerElectron(shower)) e_showers.push_back(shower);
    
    //Get a list of the indicies of the independent electron showers
    std::vector<size_t> ind_shower_idxs = ListIndependentShowersIdxs(e_showers);


    ///Here is where to check that each shower is not correlated with
    ///anything *else* in the event (tracks pointing towards it, etc)



    
    if(ind_shower_idxs.size() == 1)
      single_e_shower_events++;

    /// Make an electron SPArticle for each independent shower and add it to the set
    for(size_t i = 0; i < ind_shower_idxs.size(); ++i){
      
      SPArticle p_e;
      p_e.pdg_code(11);
      p_e.pos(e_showers.at(i).Start());
      p_e.energy(e_showers.at(i)._energy);
      	
      res.push_back(p_e);

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

  bool SPAlgoSingleE::AreTwoShowersCorrelated(const sptool::SPAShower &s1, const sptool::SPAShower &s2){

    //Is the start point of s1 close to start point of s2?
    double dist = s1.Start().Dist(s2.Start());
    //Hard cut on 1cm for now. This value chosen after viewing some std::couts.
    //Later: use a PDF to determine the likelihood "dist" is small enough
    return  (dist < 1.) ? true : false;

  }

  std::vector<size_t> SPAlgoSingleE::ListIndependentShowersIdxs(const std::vector<sptool::SPAShower> &e_showers){

    //Loop over electron showers, make sure they aren't
    //correlated w/ other electron showers
    //Make a list of electron shower indices that are *not* correlated with any others
    std::vector<size_t> good_shower_indices;

    //Save some time if the list of e_showers is length 0
    //(return empty vector)
    if(e_showers.size() == 0) return good_shower_indices;

    //Save some time if the list of e_showers is length 1
    //(return a vector with just the index "0" in it)
    if(e_showers.size() == 1){
      good_shower_indices.push_back(0);
      return good_shower_indices;
    }
    

    //For more electron showers, need to do some combinatorics

    //This is a list of showers that *are* correlated with other showers
    std::vector<size_t> bad_shower_indices;
    for(size_t i = 0; i < e_showers.size()-1; ++i){
      for(size_t j = i+1; j < e_showers.size(); ++j){
	
	if(AreTwoShowersCorrelated(e_showers.at(i),e_showers.at(j))){
	  //if list of showers that aren't independent doesn't already contain "i"
	  //add "i" to the list of showers
	  if( !(std::find(bad_shower_indices.begin(),bad_shower_indices.end(),i) != bad_shower_indices.end()) )
	    bad_shower_indices.push_back(i);

	  //if list of showers that aren't independent doesn't already contain "j"
	  //add "j" to the list of showers
	  if( !(std::find(bad_shower_indices.begin(),bad_shower_indices.end(),j) != bad_shower_indices.end()) )
	    bad_shower_indices.push_back(j);
	}
      }
    }


    //Now that we have a list of all shower indices, and a list of bad shower indices
    //make a list of *good* shower indices
    for(size_t idx = 0; idx < e_showers.size(); idx++)
      //if shower index is not in "bad showers", it is a good shower
      if( !(std::find(bad_shower_indices.begin(),bad_shower_indices.end(),idx) != bad_shower_indices.end()) )
	good_shower_indices.push_back(idx);
    
    return good_shower_indices;

  }

}

#endif
