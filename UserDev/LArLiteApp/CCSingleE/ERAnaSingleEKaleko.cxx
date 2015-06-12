#ifndef ERTOOL_ERANASINGLEEKALEKO_CXX
#define ERTOOL_ERANASINGLEEKALEKO_CXX

#include "ERAnaSingleEKaleko.h"

namespace ertool {

  ERAnaSingleEKaleko::ERAnaSingleEKaleko() : AnaBase()
			       , _result_tree(nullptr)
			       , fTPC(0.,-115.5,0.,254.8,117.5,1036.92)

  {
    _name     = "ERAnaSingleEKaleko";

    if (_result_tree) { delete _result_tree; }
    _result_tree = new TTree("_result_tree","Result Tree");

    _result_tree->Branch("_n_singleReco",&_n_singleReco,"n_singleReco/I");
    _result_tree->Branch("_n_protonsInt",&_n_protonsInt,"n_protonsInt/I");
    _result_tree->Branch("_n_protonsIntReco",&_n_protonsIntReco,"n_protonsIntReco/I");
    _result_tree->Branch("_e_nu",&_e_nu,"e_nu/D");
    _result_tree->Branch("_e_nuReco",&_e_nuReco,"e_nuReco/D");
    _result_tree->Branch("_e_protonsInt",&_e_protonsInt,"e_protonsInt/D");
    _result_tree->Branch("_e_protonsIntReco",&_e_protonsIntReco,"e_protonsIntReco/D");
    _result_tree->Branch("_pdg_nu",&_pdg_nu,"pdg_nu/I");
    _result_tree->Branch("_e_lep",&_e_lep,"e_lep/D");
    _result_tree->Branch("_pdg_lep",&_pdg_lep,"pdg_lep/I");
    _result_tree->Branch("_e_lepReco",&_e_lepReco,"e_lepReco/D");
    _result_tree->Branch("_misID",&_misID,"misID/I");
    _result_tree->Branch("_inFidVol",&_inFidVol,"inFidVol/O");

    // set default energy cut (for counting) to 0
    _eCut = 0;

    // set initial efficiency to 0
    _eff = 0.;

    // Counters necessary to compute efficiency
    _numEvts = 0;
    _single_E_ctr = 0;


    _h_e_nu_correlation = new TH2F("h_eNu_eNuReco","True Neutrino Energy vs. Reconstructed Neutrino Energy;True Neutrino Energy [MEV];Reconstructed Neutrino Energy [MEV]",100,0,3000,100,0,3000);

  }

  void ERAnaSingleEKaleko::Reset()
  {}

  bool ERAnaSingleEKaleko::Analyze(const EventData &data, const ParticleSet &ps)
  {
   
    _numEvts += 1;

    //Right off the bat, if no single electron was reconstructed, skip this event
    if(ps.size() != 1)
      return false;

    // Reset tree variables
    // Assume we will mis-ID
    ResetTreeVariables();

    ////////////////////////////////////////////
    /// FIRST LOOK AT MC INFORMATION ///////////
    ////////////////////////////////////////////

    // Get MC particle set
    auto mc_ps = MCParticleSet();

    // Get MC EventData (showers/tracks...)
    auto mc_data = MCEventData();

    // loop over all particles in MC particle set (made by Helper)
    for (auto &p : mc_ps){
      // Find the Neutrino, and store its energy and position
      if (abs(p.PdgCode()) == 12 || abs(p.PdgCode()) == 14){
	_e_nu = p.Energy();
	_pdg_nu = p.PdgCode();
	_x_nu = p.Vertex()[0];
	_y_nu = p.Vertex()[1];
	_z_nu = p.Vertex()[2];

	bool found_lepton_daughter = false;

	//Loop through neutrino's daughters and store the lepton information
	for (auto &nud : p.Daughters()){
	  if (abs(nud.PdgCode()) == 11 || abs(nud.PdgCode()) == 13){
	    if(found_lepton_daughter) 
	      std::cout<<"wtf already found daughter? overwriting variables..."<<std::endl;
	    found_lepton_daughter = true;
	    _e_lep = nud.Energy();
	    _pdg_lep = nud.PdgCode();
	  }
	}
      }//end information about the neutrino


    }//end loop over MC particle set

    // True neutrino vertex
    geoalgo::Point_t nu_vtx(_x_nu,_y_nu,_z_nu);
    
    // Whether neutrino vertex is inside fiducial volume
    _inFidVol = sqrt(_geoAlgo.SqDist(nu_vtx,fTPC)) > 25 ? true : false;
    
    // Loop over the MC Particle set again now that I have neutrino interaction vertex info
    // find the number of proton tracks that start within 1 cm of the neutrino interaction
    // also require the protons have enough energy to be reconstructable
    _n_protonsInt = 0;

    for (auto &part : mc_ps){
      //The only mcparticle set pdg codes are 12, -12, and 1000180400
      //Find the protons that are daughters of the nucleus
      //(for some reason, protons are daughters of nucleus, not neutrino)
      if ( abs(part.PdgCode()) != 1000180400 ) continue;

      //Loop over the neutrino's daughters
      //The daughters right now are all pdg 11 or -11... why are there no proton daughters
      for (auto &nucd : part.Daughters()){

	if ( abs(nucd.PdgCode()) == 2212 && nucd.KineticEnergy() > _eCut){
	  if ( nu_vtx.Dist(nucd.Vertex()) < 1){
	    _n_protonsInt += 1;
	    _e_protonsInt += nucd.KineticEnergy();
	  }
	}
      }//End loop over neutrino daughters
    }//End loop over MC Particle set


    ////////////////////////////////////////////
    /// NOW LOOK AT RECO INFORMATION ///////////
    ////////////////////////////////////////////

    
    // size of ParticleSet should be the number of neutrinos found, each associated with a single electron
    _n_singleReco = ps.size();

    // if only 1 CCSingleE interaction was found -> misID = 0
    if ( _n_singleReco == 1){
      _misID = 0;
      _single_E_ctr += 1;
    }
    
    // Now we loop over reconstructed particle set, which has one element (the neutrino)
    // I don't need a for-loop because I skip this whole event if ps.size() != 1
    Particle neutrino = ps.at(0);

    if(abs(neutrino.PdgCode()) != 12  && abs(neutrino.PdgCode()) != 14)
      std::cout<<"wtf neutrino doesn't have a neutrino pdg code"<<std::endl;
    
    _n_protonsIntReco = 0;
    _e_protonsIntReco = 0;
    _e_nuReco = 0;
    
    //Loop over neutrino daughters
    for (auto const& daught : neutrino.Daughters()){
      
      // if it is a proton, add energy to interaction-tracks-energy
      // no need for energy cut here... the point of the energy cut is to simulate
      // which protons are reconstructable. now we're looking at reconstructed protons
      if (abs(daught.PdgCode()) == 2212){
	_n_protonsIntReco += 1;
	_e_protonsIntReco += daught.KineticEnergy();
      }
      
      // if it's an electron, add energy to lep energy
      if(abs(daught.PdgCode()) == 11)
	_e_lepReco = daught.Energy();
      
      
    }// for all daughters
    
    // reco neutrino energy is lepton energy plus sum of proton energies
    _e_nuReco = _e_protonsIntReco + _e_lepReco;

    _result_tree->Fill();

    //Fill neutrino energy correlation histo
    _h_e_nu_correlation->Fill(_e_nu,_e_nuReco);
    
    return true;
  }
  
  void ERAnaSingleEKaleko::ProcessEnd(TFile* fout)
  {
    
    _eff = 100*_single_E_ctr/float(_numEvts);
    
    std::cout << "RESULTS: " << std::endl
	      << "Tot Events    : " << _numEvts << std::endl
	      << "SingleE Events: " << _single_E_ctr << std::endl
	      << "Eff           : " << _eff << " %" << std::endl;

    MakeEffPlot("e_nu",10,0,3000);

    if (fout){
      fout->cd();
      _result_tree->Write();
      _h_e_nu_correlation->Write();
    }

    return;
  }


  void ERAnaSingleEKaleko::MakeEffPlot(std::string varname,
				 int nbins, double xmin, double xmax){

    // Efficiency vs. variable "varname"
    int Nall, Nok;
    TH1D *hist = new TH1D(Form("_hEffvs%s",varname.c_str()),Form("Efficiency vs %s",varname.c_str()),nbins,xmin,xmax);
    TH1D *All = new TH1D("All","All",nbins,xmin,xmax); // Histogram of all entries as a function of Elep
    TH1D *Ok = new TH1D("Ok","Ok",nbins,xmin,xmax);  // Histogram of all entries that are not mis-ID vs Elep
    Nall = _result_tree->Draw(Form("%s>>All",varname.c_str()),"");
    Nok = _result_tree->Draw(Form("%s>>Ok",varname.c_str()),"misID==0");
    for (int n=0; n < nbins; n++){
      if (All->GetBinContent(n+1) > 0){
	double eff = Ok->GetBinContent(n+1)/All->GetBinContent(n+1);
	hist->SetBinContent(n+1, eff);
	hist->SetBinError(n+1, sqrt( eff*(1-eff) / All->GetBinContent(n+1) ) );
      }
      else{
	hist->SetBinContent(n+1,0);
	hist->SetBinError(n+1,0.);
      }
    }
    delete All;
    delete Ok;
    hist->Write();

    return;
  }

  void ERAnaSingleEKaleko::ResetTreeVariables(){
    
    _misID = 1;

    _n_singleReco  = -1;
    _n_protonsInt   = -1;
    _n_protonsIntReco = -1;

    _e_nu          = -1000;
    _e_nuReco      = -1000;
    _pdg_nu        = -1;
    _e_lep         = -1000;
    _pdg_lep       = -1;
    _e_lepReco     = -1000;

    _e_protonsInt = -1;
    _e_protonsIntReco = -1;

    _inFidVol = false;

    _x_nu = -1000; _y_nu = -1000; _z_nu = -1000;

    return;
  }

  }

#endif
