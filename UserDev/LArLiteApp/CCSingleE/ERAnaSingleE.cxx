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

    _result_tree->Branch("_n_singleReco",&_n_singleReco,"n_singleReco/I");
    _result_tree->Branch("_n_showers",&_n_showers,"n_showers/I");
    _result_tree->Branch("_n_showersReco",&_n_showersReco,"n_showersReco/I");
    _result_tree->Branch("_n_tracks",&_n_tracks,"n_tracks/I");
    _result_tree->Branch("_n_tracksReco",&_n_tracksReco,"n_tracksReco/I");
    _result_tree->Branch("_n_electrons",&_n_electrons,"n_electrons/I");
    _result_tree->Branch("_n_gammas",&_n_gammas,"n_gammas/I");
    _result_tree->Branch("_e_nu",&_e_nu,"e_nu/D");
    _result_tree->Branch("_n_protons",&_n_protons,"n_protons/I");
    _result_tree->Branch("_n_neutrons",&_n_neutrons,"n_neutrons/I");
    _result_tree->Branch("_n_piplus",&_n_piplus,"n_piplus/I");
    _result_tree->Branch("_n_pi0",&_n_pi0,"n_pi0/I");
    _result_tree->Branch("_pdg_nu",&_pdg_nu,"pdg_nu/I");
    _result_tree->Branch("_e_lep",&_e_lep,"e_lep/D");
    _result_tree->Branch("_pdg_lep",&_pdg_lep,"pdg_lep/I");
    _result_tree->Branch("_x_lep",&_x_lep,"x_lep/D");
    _result_tree->Branch("_y_lep",&_y_lep,"y_lep/D");
    _result_tree->Branch("_z_lep",&_z_lep,"z_lep/D");
    _result_tree->Branch("_px_lep",&_px_lep,"px_lep/D");
    _result_tree->Branch("_py_lep",&_py_lep,"py_lep/D");
    _result_tree->Branch("_pz_lep",&_pz_lep,"pz_lep/D");
    _result_tree->Branch("_theta_lep",&_theta_lep,"theta_lep/D");
    _result_tree->Branch("_phi_lep",&_phi_lep,"phi_lep/D");
    _result_tree->Branch("_e_lepReco",&_e_lepReco,"e_lepReco/D");
    _result_tree->Branch("_theta_lepReco",&_theta_lepReco,"theta_lepReco/D");
    _result_tree->Branch("_phi_lepReco",&_phi_lepReco,"phi_lepReco/D");
    _result_tree->Branch("_x_lepReco",&_x_lepReco,"x_lepReco/D");
    _result_tree->Branch("_y_lepReco",&_y_lepReco,"y_lepReco/D");
    _result_tree->Branch("_z_lepReco",&_z_lepReco,"z_lepReco/D");
    _result_tree->Branch("_px_lepReco",&_px_lepReco,"px_lepReco/D");
    _result_tree->Branch("_py_lepReco",&_py_lepReco,"py_lepReco/D");
    _result_tree->Branch("_pz_lepReco",&_pz_lepReco,"pz_lepReco/D");
    _result_tree->Branch("_lep_dot",&_lep_dot,"_lep_dot/D");
    _result_tree->Branch("_lep_vtxdist",&_lep_vtxdist,"_lep_vtxdist/D");
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
    ResetTreeVariables();


    _numEvts += 1;
    // count number of showers in event
    _n_electrons = 0;
    _n_gammas    = 0;

    // Get MC EventData (showers/tracks...)
    auto mc_data = MCEventData();
    // Count number of MC tracks and showers with E > 20 MeV
    _n_showers = 0;
    _n_tracks  = 0;
    for (auto &s : data.AllShower())
      if (s._energy > 20) { _n_showers += 1; }
    for (auto &t : data.AllTrack())
      if (t._energy > 20) { _n_tracks += 1; }

    // Get MC particle set
    auto mc_ps = MCParticleSet();
    for (auto &p : mc_ps){
      // Find the Lepton and store its energy
      if (abs(p.PdgCode()) == 12 || abs(p.PdgCode()) == 14){
	_e_nu = p.Energy();
	_pdg_nu = p.PdgCode();
	bool found_lepton_daughter = false;
	for (auto &nud : p.Daughters()){
	  if (abs(nud.PdgCode()) == 11 || abs(nud.PdgCode()) == 13){
	    if(found_lepton_daughter) 
	      std::cout<<"wtf already found daughter? overwriting variables..."<<std::endl;
	    found_lepton_daughter = true;
	    _e_lep = nud.Energy();
	    _x_lep = nud.Vertex()[0];
	    _y_lep = nud.Vertex()[1];
	    _z_lep = nud.Vertex()[2];
	    _px_lep = nud.Momentum()[0];
	    _py_lep = nud.Momentum()[1];
	    _pz_lep = nud.Momentum()[2];
	    _theta_lep = (180./3.14) * acos( _pz_lep / sqrt( _px_lep*_px_lep + _py_lep*_py_lep + _pz_lep*_pz_lep ) );
	    _phi_lep   = (180./3.14) * asin( _py_lep / sqrt( _px_lep*_px_lep + _py_lep*_py_lep ) );
					     
	    _pdg_lep = nud.PdgCode();
	  }
	}
      }
      // now keep track of all showers and tracks
      for (auto &d : p.AllDaughters()){
	if ( (d->PdgCode() == 22) && (d->Energy() > 20) )
	  _n_gammas += 1;
	else if ( (abs(d->PdgCode()) == 11) && (d->Energy() > 20) )
	  _n_electrons += 1;
	else if ( (d->PdgCode() == 111) && (d->Energy() > 20) )
	  _n_pi0 += 1;
	else if ( (d->PdgCode() == 211) && (d->Energy() > 20) )
	  _n_piplus += 1;
	else if ( (d->PdgCode() == 2112) && (d->Energy() > 20) )
	  _n_neutrons += 1;
	else if ( (d->PdgCode() == 2212) && (d->Energy() > 20) )
	  _n_protons += 1;
      }
    }
    
    // Count number of tracks and showers with E > 20 MeV
    _n_showersReco = 0;
    _n_tracksReco  = 0;
    for (auto &s : data.AllShower())
      if (s._energy > 20) { _n_showersReco += 1; }
    for (auto &t : data.AllTrack())
      if (t._energy > 20) { _n_tracksReco += 1; }


    // size of ParticleSet should be the number of single electrons found
    //ps is the reco particle set
    _n_singleReco = ps.size();
    if ( _n_singleReco == 1 ){
      Particle se = ps[0].Daughters()[0];
      _e_lepReco = se.Energy();
      _x_lepReco = se.Vertex()[0];
      _y_lepReco = se.Vertex()[1];
      _z_lepReco = se.Vertex()[2];
      _px_lepReco = se.Momentum()[0];
      _py_lepReco = se.Momentum()[1];
      _pz_lepReco = se.Momentum()[2];
      _theta_lepReco = (180./3.14) * acos( _pz_lepReco / sqrt( _px_lepReco*_px_lepReco + _py_lepReco*_py_lepReco + _pz_lepReco*_pz_lepReco ) );
      _phi_lepReco   = (180./3.14) * asin( _py_lepReco / sqrt( _px_lepReco*_px_lepReco + _py_lepReco*_py_lepReco ) );
      _lep_vtxdist = sqrt( (_x_lep-_x_lepReco)*(_x_lep-_x_lepReco) +
			   (_y_lep-_y_lepReco)*(_y_lep-_y_lepReco) + 
			   (_z_lep-_z_lepReco)*(_z_lep-_z_lepReco) );
      _lep_dot = ( ( _px_lep*_px_lepReco + _py_lep*_py_lepReco + _pz_lep*_pz_lepReco ) / 
		   ( sqrt( _px_lepReco*_px_lepReco + _py_lepReco*_py_lepReco + _pz_lepReco*_pz_lepReco ) * 
		     sqrt( _px_lep*_px_lep + _py_lep*_py_lep + _pz_lep*_pz_lep ) ) );
	
      _misID = 0;
      _singleE_ctr += 1;
    }

    _result_tree->Fill();

    if (_debug){
      std::cout << "Ana results:" << std::endl
		<< "Mis-ID                 : " << _misID << std::endl           
		<< "Lepton E  [Mc,Reco]    : [" << _e_lep << ", " << _e_lepReco << "]" <<  std::endl
		<< "Lepton Vtx dist Mc-Reco:  " << sqrt( (_x_lep-_x_lepReco)*(_x_lep-_x_lepReco) +
							 (_y_lep-_y_lepReco)*(_y_lep-_y_lepReco) +
							 (_z_lep-_z_lepReco)*(_z_lep-_z_lepReco) ) << std::endl
		<< "Lepton theta [Mc,Reco] : [" << _theta_lep << ", " << _theta_lepReco << "]" <<  std::endl
		<< "Lepton phi [Mc,Reco]   : [" << _phi_lep << ", " << _phi_lepReco << "]" <<  std::endl;
    }

    return true;
  }
  
  void ERAnaSingleE::ProcessEnd(TFile* fout)
  {

    std::cout << "RESULTS: " << std::endl
	      << "Tot Events    : " << _numEvts << std::endl
	      << "SingleE Events: " << _singleE_ctr << std::endl;

    MakeEffPlot("e_lep",10,0,2000);
    MakeEffPlot("e_nu",10,0,2000);
    MakeEffPlot("n_tracks",10,0,10);

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

  void ERAnaSingleE::ResetTreeVariables(){
    
    _misID = 1;

    _n_singleReco  = -1;
    _n_electrons   = -1;
    _n_gammas      = -1;
    _n_showers     = -1;
    _n_showersReco = -1;
    _n_tracks      = -1;
    _n_tracksReco  = -1;
    _n_protons     = -1;
    _n_neutrons    = -1;
    _n_piplus      = -1;
    _n_pi0         = -1;

    _e_nu          = -1000;
    _pdg_nu        = -1;
    _e_lep         = -1000;
    _x_lep         = -1000;
    _y_lep         = -1000;
    _z_lep         = -1000;
    _pdg_lep       = -1;
    _theta_lep     = -360;
    _phi_lep       = -360;
    _e_lepReco     = -1000;
    _x_lepReco     = -1000;
    _y_lepReco     = -1000;
    _z_lepReco     = -1000;
    _theta_lepReco = -360;
    _phi_lepReco   = -360;
    _lep_dot       = -2;
    _lep_vtxdist   = -1000;

    return;
  }

}

#endif
