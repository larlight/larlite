
/*Code that reweighs MCC8 generated BNB MicroBooNE events according to 
a correction to the flux based on neutrino energy, type and parentage. 
Should only be used to reweigh neutrino running mode events.

This code is based on EventWeight/FluxHistWeightCalc found in uboonecode: 
https://github.com/larlight/LArLight/tree/lar1/UserDev/lar1Osc/

Code originially written by Robert Murrells,
then modified by J.Zennamo (based on Zarko's work)

robertmurrells@gmail.com
jzennamo@gmail.com

*/

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
  
  //Get CV flux file
  //  This lives here:
  //   /pnfs/uboone/persistent/uboonebeam/bnb_gsimple/bnb_gsimple_fluxes_02.28.2014_470_rwhist/gsimple_uboone_flux.root
  //
  // std::string const cv_file_name =
  //   "$LARLITE_BASEDIR/UserDev/LArLiteApp/fluxRW/gsimple_uboone_flux.root";

  std::string const cv_file_name = "/home/vgenty/bnb_oldflux_volAVTPC.root";
  
  //Get RW flux file
  //  This lives here:
  //    /pnfs/uboone/persistent/uboonebeam/bnb_gsimple/bnb_gsimple_fluxes_02.28.2014_470_rwhist/redecay_uboone_flux.root
  //
    std::string const rw_file_name = "/home/vgenty/bnb_newflux_volAVTPC.root";

  TFile * cv_file = TFile::Open(cv_file_name.c_str());
  TFile * rw_file = TFile::Open(rw_file_name.c_str());
  

  if(!cv_file) {

    std::cout << "fluxRW::fluxRW:\n"
	      << "\tCould not open file\n"
	      << "\tStopped process\n";
    exit(0);

  }

   
  /*Loop over 4 neutrino types (nue, nuebar, numu, numubar) and 4 parent types
    (muon, charged pion, K_L^0 and charged kaon) and record each neutrino-parent
    flux histogram in a vector.*/

  std::string ptype[] = {"pi", "k", "k0", "mu"};
  std::string ntype[] = {"numu", "numubar", "nue", "nuebar"};

  for (int iptyp=0;iptyp<4;iptyp++) {
    for (int intyp=0;intyp<4;intyp++) {
      for (int ibin=0;ibin<200;ibin++) {
	fCV[iptyp][intyp][ibin]=(dynamic_cast<TH1F*> (cv_file->Get(Form("h_%s_%s",ptype[iptyp].c_str(),ntype[intyp].c_str()))))->GetBinContent(ibin+1);
	fRW[iptyp][intyp][ibin]=(dynamic_cast<TH1F*> (rw_file->Get(Form("h_%s_%s",ptype[iptyp].c_str(),ntype[intyp].c_str()))))->GetBinContent(ibin+1);
      }
    }
  }

  cv_file->Close();
  rw_file->Close();

        
  //Return to working larlite root directory
  larlite_directory->cd();
  
}





/*Method to get flux weighting for an event using neutrino energy, type and 
  parent type*/
Double_t fluxRW::get_weight(Double_t energy, Int_t ntype, Int_t ptype) {

  Double_t weight = 1;

  if(ntype < 0 || ntype > 3) {

    std::cout << "fluxRW::get_weight:\n"
	      << "\tntype must be 0-3\n"
	      << "\tweight = 1\n";

  }

  else if(ptype < 0 || ptype > 3) {
    
    std::cout << "fluxRW::get_weight:\n"
	      << "\tptype must be 0-3\n"
	      << "\tweight = 1\n";
    
  }

  else {

    Int_t bin = Int_t(energy/bin_size);

    if(fRW[ptype][ntype][bin] && fCV[ptype][ntype][bin]){
      weight = 1-(1-(fRW[ptype][ntype][bin]/fCV[ptype][ntype][bin]));
    }
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
	Int_t ntype = -9999;	
	if(mcn.Nu().PdgCode() == 14) ntype = 0;	
	else if(mcn.Nu().PdgCode() == -14) ntype = 1;
	else if(mcn.Nu().PdgCode() == 12) ntype = 2;      
	else if(mcn.Nu().PdgCode() == -12) ntype = 3;	
	
	
	Int_t decay_type = storage->get_data<larlite::event_mcflux>("generator")->at(i).fptype;
	
	//Determine parent type (muon, charged pion, K_L^0 or charged kaon)
	Int_t ptype = -9999;
	if ( decay_type==211 || decay_type==-211 ) ptype = 0;
	else if ( decay_type==321 || decay_type==-321 ) ptype = 1;
	else if ( decay_type==130 ) ptype = 2;
	else if ( decay_type==13 || decay_type==-13 ) ptype = 3;
	
	weight *= get_weight(mcn.Nu().Trajectory().at(0).E(), ntype, ptype);
	
    }

    return weight;
    
}


//
//   I do not believe this step is needed
//
/*
  Rescaling of MiniBooNE flux histograms to normalize to gsimple (MicroBooNE)
  flux histograms: nu/m^2/1e7POT (1e6 POT * 10 redecays per event)
  Double_t scaling =
  
  1e7 * //(1e6 POT * 10 redecays per event)
  
  1e4 / //Convert from cm^2 to m^2 for area below
  
  (pow(610.6,2) * 3.14159); Divide by MiniBooNE detector cross-sectional
  area (radius of 610.6 cm)
*/


#endif

