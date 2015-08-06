
/*Code that reweighs MCC5 generated BNB MicroBooNE events according to 
MiniBooNE fluxes based on neutrino energy, type and parentage. Should only be 
used to reweigh neutrino running mode events.

This code is based on FluxRW found at: 
https://github.com/larlight/LArLight/tree/lar1/UserDev/lar1Osc/

Code written by Robert Murrells:
robertmurrells@gmail.com*/

#ifndef FLUXRW_CXX
#define FLUXRW_CXX

#include "fluxRW.h"
#include "TFile.h"
#include "TH1D.h"
#include "DataFormat/mctruth.h"
#include "DataFormat/mcflux.h"





fluxRW::fluxRW() {

  //Grab pointer to working larlite rood directory
  TDirectory * const larlite_directory = gDirectory;
  
  //Bin size of flux histograms
  bin_size = 0.05; //GeV


  
  //Get MiniBooNE flux file
  std::string const mb_file_name =
    "$LARLITE_BASEDIR/UserDev/LArLiteApp/fluxRW/april07_baseline_rgen610.6_flux.root";
  
  TFile * mb_file = TFile::Open(mb_file_name.c_str());

  if(!mb_file) {

    std::cout << "fluxRW::fluxRW:\n"
	      << "\tCould not open file\n"
	      << "\tStopped process\n";
    exit(0);

  }

  

  /*Rescaling of MiniBooNE flux histograms to normalize to gsimple (MicroBooNE)
    flux histograms: nu/m^2/1e7POT (1e6 POT * 10 redecays per event)*/
  Double_t scaling =
    
    1e7 * //(1e6 POT * 10 redecays per event)
    
    1e4 / //Convert from cm^2 to m^2 for area below
    
    (pow(610.6,2) * 3.14159); /*Divide by MiniBooNE detector cross-sectional
				area (radius of 610.6 cm)*/



  /*Loop over 4 neutrino types (nue, nuebar, numu, numubar) and 4 parent types
    (muon, charged pion, K_L^0 and charged kaon) and record each neutrino-parent
    flux histogram in a vector. This vector is stored in a 4x4 array with each 
    position corresponding to a specific neutrino-parent combination for
    MiniBooNE*/
  for(Int_t ntype = 1; ntype <= 4; ntype++) {
    
    for(Int_t ptype = 1; ptype <= 4; ptype++) {

      //Get specific MiniBooNE neutrino-parent flux histogram
      TH1F * hist_mb =
	dynamic_cast<TH1F*>(mb_file->Get(Form("h1%i%i",ptype,ntype)));

      hist_mb->Scale(scaling);

      std::vector<Double_t> temp;
      
      for(Int_t bin = 1; bin <= hist_mb->GetNbinsX(); bin++)

	temp.push_back(hist_mb->GetBinContent(bin));

      
      //Save vector in 4x4 array
      mb[ntype-1][ptype-1] = temp;
      
    }
    
  }
  
  mb_file->Close();


  
  /*gsimple fluxes are stored in 4 seperate files (one for nue, nuebar, numu and
    numubar)*/
  for(Int_t ntype = 1; ntype <= 4; ntype++) {

    //Get specified gsimple flux file
    std::string neutrino_type[4] = {"nue", "nuebar", "numu", "numubar"};
    
    std::string gs_file_name =
      Form("$LARLITE_BASEDIR/UserDev/LArLiteApp/fluxRW/%s_rawntp.root",
	   neutrino_type[ntype-1].c_str());
    
    TFile * gs_file = TFile::Open(gs_file_name.c_str());
    
    if(!gs_file) {

      std::cout << "fluxRW::fluxRW(): \n"
		<< "\tCould not open file\n"
		<< "\tStopped process\n";
      exit(0);
      
    }



    //Get and copy various gsimple flux histograms from this file
    TH1D * hist_muplus = dynamic_cast<TH1D*> (gs_file->Get("hMuPlus_Flux"));
    TH1D * hist_muminus = dynamic_cast<TH1D*> (gs_file->Get("hMuMinus_Flux"));
    TH1D * hist_piplus = dynamic_cast<TH1D*> (gs_file->Get("hPiPlus_Flux"));
    TH1D * hist_piminus = dynamic_cast<TH1D*> (gs_file->Get("hPiMinus_Flux"));
    TH1D * hist_k0 = dynamic_cast<TH1D*> (gs_file->Get("hKL_Flux"));
    TH1D * hist_kplus = dynamic_cast<TH1D*> (gs_file->Get("hKPlus_Flux"));
    TH1D * hist_kminus = dynamic_cast<TH1D*> (gs_file->Get("hKMinus_Flux"));
    
    std::vector<TH1D*> gs_hist_vector(4);

    gs_hist_vector.at(0) = dynamic_cast<TH1D*> (hist_muplus->Clone());
    gs_hist_vector.at(0)->Add(hist_muminus);

    gs_hist_vector.at(1) = dynamic_cast<TH1D*> (hist_piplus->Clone());
    gs_hist_vector.at(1)->Add(hist_piminus);

    gs_hist_vector.at(2) = dynamic_cast<TH1D*> (hist_k0->Clone());

    gs_hist_vector.at(3) = dynamic_cast<TH1D*> (hist_kplus->Clone());
    gs_hist_vector.at(3)->Add(hist_kminus);



    //Save neutrino-parent flux to 4x4 array for gsimple fluxes
    for(Int_t ptype = 1; ptype <= 4; ptype++) {

      std::vector<Double_t> temp;
      
      for(Int_t bin = 1; bin <= hist_muplus->GetNbinsX(); bin++) {

	temp.push_back(gs_hist_vector.at(ptype-1)->GetBinContent(bin));
		       
      }

      gs[ntype-1][ptype-1] = temp;
    
    }

    gs_file->Close();
    
  }


  
  //Return to working larlite root directory
  larlite_directory->cd();
  
}





/*Method to get flux weighting for an event using neutrino energy, type and 
  parent type*/
Double_t fluxRW::get_weight(Double_t energy, Int_t ntype, Int_t ptype) {

  Double_t weight = 1;

  if(ntype < 1 || ntype > 4) {

    std::cout << "fluxRW::get_weight:\n"
	      << "\tntype must be 1-4\n"
	      << "\tweight = 1\n";

  }

  else if(ptype < 1 || ptype > 4) {
    
    std::cout << "fluxRW::get_weight:\n"
	      << "\tptype must be 1-4\n"
	      << "\tweight = 1\n";
    
  }

  else {

    Int_t bin = Int_t(energy/bin_size) + 1;

    Double_t MB = mb[ntype-1][ptype-1][bin];

    Double_t GS = gs[ntype-1][ptype-1][bin];

    if(MB && GS) weight = MB / GS;

  }

  return weight;

}





/*

Method to get the event weight automatically using the decay type from mcflux
Use method only if beam events are being used (specifically if only one entry
is present in the event_mctruth vector and that entry contains beam events)

One also needs to be very careful to find the weight for every neutrino interaction
in a given event and then we will want to take the total weight (product of the 
individual weights) and apply that.

*/


Double_t fluxRW::event_weight(larlite::storage_manager * storage) {
  
    Double_t weight = 1; 

    auto const & mcn_v = storage->get_data<larlite::event_mctruth>("generator"); 
    
    for(int i = 0; i < mcn_v->size();  i++){
      
	auto &mcn= mcn_v->at(i).GetNeutrino();
	
	//Determine neutrino type (nue, nuebar, numu or numubar)
	Int_t ntype = 0;
	
	if(mcn.Nu().PdgCode() == 12) ntype = 1;
      
	else if(mcn.Nu().PdgCode() == -12) ntype = 2;
	
	else if(mcn.Nu().PdgCode() == 14) ntype = 3;
	
	else if(mcn.Nu().PdgCode() == -14) ntype = 4;
	
	Int_t decay_type = storage->get_data<larlite::event_mcflux>("generator")->at(i).fndecay;
	
	//Determine parent type (muon, charged pion, K_L^0 or charged kaon)
	Int_t ptype = 0;
	
	switch(decay_type) {
	  
	case 1:
	case 2:
	case 3:
	case 4:
	  ptype = 3;
	  break;
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	  ptype = 4;
	  break;
	case 11:
	case 12:
	  ptype = 1;
	  break;
	case 13:
	case 14:
	  ptype = 2;
	  
	}
	
	/*
	  Int_t parent_pdg = storage->get_data<larlite::event_mcflux>("generator")->
	  at(0).fptype;
	  
	  switch (abs(parent_pdg)) {
	  
	  case 13:
	  ptype = 1;
	  break;
	  case 321:
	  ptype = 4;
	  break;
	  case 130:
	  ptype = 3;
	  break;
	  case 211:
	  ptype = 2;
	  
	  }
	*/
	
	weight *= get_weight(mcn.Nu().Trajectory().at(0).E(), ntype, ptype);
	
      }
    return weight;
	
	}

#endif
