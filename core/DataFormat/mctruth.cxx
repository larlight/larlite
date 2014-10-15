#ifndef MCTRUTH_CXX
#define MCTRUTH_CXX

#include "mctruth.h"

namespace larlite {

  //***************************************************
  void mctruth::clear_data()
  //***************************************************
  {
    data_base::clear_data();
    fPartList.clear();
    fMCNeutrino.clear_data();
    fNeutrinoSet=false;
    fOrigin=mc::kUnknown;
  }

  //*************************************
  void mctruth::SetNeutrino(Int_t CCNC,
			    Int_t mode,
			    Int_t interactionType,
			    Int_t target,
			    Int_t nucleon,
			    Int_t quark,
			    Double_t w,
			    Double_t x,
			    Double_t y,
			    Double_t qsqr)
  //*************************************
  {

    if( !fNeutrinoSet ){
      fNeutrinoSet = true;
      // loop over the mcpart list and get the outgoing lepton
      // assume this is a neutral current interaction to begin with
      // which means the outgoing lepton is the incoming neutrino
      mcpart nu  = fPartList[0];
      mcpart lep = fPartList[0];

      // start at i = 1 because i = 0 is the incoming neutrino
      for(unsigned int i = 1; i < fPartList.size(); ++i){
        if(fPartList[i].Mother() == nu.TrackId() &&
           (fPartList[i].PdgCode()  == nu.PdgCode() ||
            abs(fPartList[i].PdgCode()) == abs(nu.PdgCode())-1) ){
          lep = fPartList[i];
          break;
	}
      }//done looping over particles


      fMCNeutrino = mcnu(nu, lep,
			 CCNC, mode, interactionType,
			 target, nucleon, quark,
			 w, x, y, qsqr);

    } // end if mcnu is not already set

    else

      std::cerr<<"\033[93m"<<"Attempt to set neutrino when already set"<<"\033[00m"<<std::endl;

    return;

  }

}
#endif
  
