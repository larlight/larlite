#ifndef ERTOOL_ERANASINGLEE_CXX
#define ERTOOL_ERANASINGLEE_CXX

#include "ERAnaSingleE.h"

namespace ertool {

  ERAnaSingleE::ERAnaSingleE() : AnaBase()
			       , _result_tree(nullptr)
  {
    _name     = "ERAnaSingleE";

    if (_result_tree) { delete _result_tree; }
    _result_tree = new TTree("_result_tree","Result Tree");

    _result_tree->Branch("_singleRECO",&_singleRECO,"singleRECO/I");
    _result_tree->Branch("_RECOshowers",&_RECOshowers,"RECOshowers/I");
    _result_tree->Branch("_RECOtracks",&_RECOtracks,"RECOtracks/I");
    _result_tree->Branch("_MCelectrons",&_MCelectrons,"_MCelectrons/I");
    _result_tree->Branch("_MCgammas",&_MCgammas,"_MCgammas/I");
    _result_tree->Branch("_Enu",&_Enu,"Enu/D");
    _result_tree->Branch("_Pdgnu",&_Pdgnu,"Pdgnu/I");
    _result_tree->Branch("_Elep",&_Elep,"Elep/D");
    _result_tree->Branch("_Pdglep",&_Pdglep,"Pdglep/I");
    _result_tree->Branch("_E",&_E,"E/D");
    _result_tree->Branch("_nProtons",&_nProtons,"nProtons/I");
    _result_tree->Branch("_nNeutrons",&_nNeutrons,"nNeutrons/I");
    _result_tree->Branch("_nPiplus",&_nPiplus,"nPiplus/I");
    _result_tree->Branch("_nPi0",&_nPi0,"nPi0/I");
    _result_tree->Branch("_misID",&_misID,"misID/I");

    _numEvts = 0;
    _singleE_ctr = 0;

    _debug = false;
  }

  void ERAnaSingleE::Reset()
  {}

  bool ERAnaSingleE::Analyze(const EventData &data, const ParticleSet &ps)
  { 
    // Reset tree variables
    // Assume we will mis-ID
    _misID = 1;
    _E = 0;
    _Elep = -0;
    _Pdglep = 0;
    _Enu = 0;
    _Pdgnu = 0;
    _nProtons = 0;
    _nNeutrons = 0;
    _nPiplus = 0;
    _nPi0 = 0;

    _numEvts += 1;
    // count number of showers in event
    _MCelectrons = 0;
    _MCgammas    = 0;

    // Get MC particle set
    auto mc_ps = MCParticleSet();
    for (auto &p : mc_ps){
      // Find the Lepton and store its energy
      if (abs(p.PdgCode()) == 12 || abs(p.PdgCode()) == 14){
	_Enu = p.Energy();
	_Pdgnu = p.PdgCode();
	bool found_lepton_daughter = false;
	for (auto &nud : p.Daughters()){
	  if (abs(nud.PdgCode()) == 11 || abs(nud.PdgCode()) == 13){
	    if(found_lepton_daughter) 
	      std::cout<<"wtf already found daughter? overwriting variables..."<<std::endl;
	    found_lepton_daughter = true;
	    _Elep = nud.Energy();
	    _Pdglep = nud.PdgCode();
	  }
	}
      }
      // now keep track of all showers and tracks
      for (auto &d : p.AllDaughters()){
	if ( (d->PdgCode() == 22) && (d->Energy() > 20) )
	  _MCgammas += 1;
	else if ( (abs(d->PdgCode()) == 11) && (d->Energy() > 20) )
	  _MCelectrons += 1;
	else if ( (d->PdgCode() == 111) && (d->Energy() > 20) )
	  _nPi0 += 1;
	else if ( (d->PdgCode() == 211) && (d->Energy() > 20) )
	  _nPiplus += 1;
	else if ( (d->PdgCode() == 2112) && (d->Energy() > 20) )
	  _nNeutrons += 1;
	else if ( (d->PdgCode() == 2212) && (d->Energy() > 20) )
	  _nProtons += 1;
      }
    }
    
    // Count number of tracks and showers with E > 20 MeV
    _RECOshowers = 0;
    _RECOtracks  = 0;
    for (auto &s : data.AllShower())
      if (s._energy > 20) { _RECOshowers += 1; }
    for (auto &t : data.AllTrack())
      if (t._energy > 20) { _RECOtracks += 1; }


    // size of ParticleSet should be the number of single electrons found
    //ps is the reco particle set
    _singleRECO = ps.size();
    if ( _singleRECO == 1 ){
      _E = ps[0].Daughters()[0].Energy();
      _misID = 0;
      _singleE_ctr += 1;
    }

    _result_tree->Fill();

    if (_debug){
      std::cout << "Ana results:" << std::endl
		<< "Showers reconstructed: " << _RECOshowers << std::endl
		<< "Tracks reconstructed : " << _RECOtracks << std::endl
		<< "MCelectrons          : " << _MCelectrons << std::endl
		<< "MCgammas             : " << _MCgammas << std::endl
		<< "SingleE Reconstructed: " << _singleRECO << std::endl << std::endl;
    }

    return true;
  }
  
  void ERAnaSingleE::ProcessEnd(TFile* fout)
  {

    std::cout << "RESULTS: " << std::endl
	      << "Tot Events    : " << _numEvts << std::endl
	      << "SingleE Events: " << _singleE_ctr << std::endl;

    MakeEffPlot("Elep",10,0,2000);
    MakeEffPlot("Enu",10,0,2000);
    MakeEffPlot("RECOtracks",10,0,10);

    if (fout){
      fout->cd();
      _result_tree->Write();
    }

    return;
  }


  void ERAnaSingleE::MakeEffPlot(std::string varname,
				 int nbins, double xmin, double xmax){

    int Nall, Nok;
    // Efficiency vs. variable "varname"
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

}

#endif
