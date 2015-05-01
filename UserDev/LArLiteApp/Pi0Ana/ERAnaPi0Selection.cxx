#ifndef ERTOOL_ERANAPI0SELECTION_CXX
#define ERTOOL_ERANAPI0SELECTION_CXX

#include "ERAnaPi0Selection.h"

namespace ertool {

  ERAnaPi0Selection::ERAnaPi0Selection() : AnaBase()
			       , _pi0_tree(nullptr)
			       , fTPC(0.,-115.5,0.,254.8,117.5,1036.92)

  {
    _name     = "ERAnaPi0Selection";

    if (_pi0_tree) { delete _pi0_tree; }
    _pi0_tree = new TTree("_pi0_tree","Pi0 Tree");

    _pi0_tree->Branch("_numEvts",&_numEvts,"numEvts/I");
 //   _pi0_tree->Branch("_EDep",&_EDep,"EDep/D");
//    _pi0_tree->Branch("_angle_Norm",&_angle_Norm,"_angle_Norm/D");
    _pi0_tree->Branch("_n_psReco",&_n_psReco,"n_psReco/I");
//    _pi0_tree->Branch("_distToTopWall",&_distToTopWall,"distToTopWall/D");
  //  _pi0_tree->Branch("_distToWall",&_distToWall,"distToWall/D");
   // _pi0_tree->Branch("_distBackAlongTraj",&_distBackAlongTraj,"distBackAlongTraj/D");
    _pi0_tree->Branch("_n_showers",&_n_showers,"n_showers/I");
//    _pi0_tree->Branch("_n_showersReco",&_n_showersReco,"n_showersReco/I");
//    _pi0_tree->Branch("_n_tracks",&_n_tracks,"n_tracks/I");
//    _pi0_tree->Branch("_n_tracksReco",&_n_tracksReco,"n_tracksReco/I");
//    _pi0_tree->Branch("_n_tracksInt",&_n_tracksInt,"n_tracksInt/I");
//    _pi0_tree->Branch("_n_tracksIntReco",&_n_tracksIntReco,"n_tracksIntReco/I");
    _pi0_tree->Branch("_n_electrons",&_n_electrons,"n_electrons/I");
    _pi0_tree->Branch("_n_gammas",&_n_gammas,"n_gammas/I");
    _pi0_tree->Branch("_e_pi0MC",&_e_pi0MC,"e_pi0MC/D");
    _pi0_tree->Branch("_x_pi0MC",&_x_pi0MC,"x_pi0MC/D");
    _pi0_tree->Branch("_y_pi0MC",&_y_pi0MC,"y_pi0MC/D");
    _pi0_tree->Branch("_z_pi0MC",&_z_pi0MC,"z_pi0MC/D");
    _pi0_tree->Branch("_px_pi0MC",&_px_pi0MC,"px_pi0MC/D");
    _pi0_tree->Branch("_py_pi0MC",&_py_pi0MC,"py_pi0MC/D");
    _pi0_tree->Branch("_pz_pi0MC",&_pz_pi0MC,"pz_pi0MC/D");
    _pi0_tree->Branch("_e_pi0Reco",&_e_pi0Reco,"e_pi0Reco/D");
    _pi0_tree->Branch("_x_pi0Reco",&_x_pi0Reco,"x_pi0Reco/D");
    _pi0_tree->Branch("_y_pi0Reco",&_y_pi0Reco,"y_pi0Reco/D");
    _pi0_tree->Branch("_z_pi0Reco",&_z_pi0Reco,"z_pi0Reco/D");
    _pi0_tree->Branch("_px_pi0Reco",&_px_pi0Reco,"px_pi0Reco/D");
    _pi0_tree->Branch("_py_pi0Reco",&_py_pi0Reco,"py_pi0Reco/D");
    _pi0_tree->Branch("_pz_pi0Reco",&_pz_pi0Reco,"pz_pi0Reco/D");
    _pi0_tree->Branch("_n_protons",&_n_protons,"n_protons/I");
    _pi0_tree->Branch("_n_neutrons",&_n_neutrons,"n_neutrons/I");
//    _pi0_tree->Branch("_e_trkInt",&_e_trkInt,"e_trkInt/D");
    _pi0_tree->Branch("_e_neutrals",&_e_neutrals,"e_neutrals/D");
//    _pi0_tree->Branch("_e_nucleus_diff",&_e_nucleus_diff,"e_nucleus_diff/D");
 //   _pi0_tree->Branch("_e_trkIntReco",&_e_trkIntReco,"e_trkIntReco/D");
    _pi0_tree->Branch("_n_piplus",&_n_piplus,"n_piplus/I");
    _pi0_tree->Branch("_n_pi0",&_n_pi0,"n_pi0/I");
    _pi0_tree->Branch("_pdg_pi0MC",&_pdg_pi0MC,"pdg_pi0MC/I");
    _pi0_tree->Branch("_e_pi0d_MC",&_e_pi0d_MC,"e_pi0d_MC/D");
    _pi0_tree->Branch("_pdg_pi0d_MC",&_pdg_pi0d_MC,"pdg_pi0d_MC/I");
    _pi0_tree->Branch("_x_pi0d_MC",&_x_pi0d_MC,"x_pi0d_MC/D");
    _pi0_tree->Branch("_y_pi0d_MC",&_y_pi0d_MC,"y_pi0d_MC/D");
    _pi0_tree->Branch("_z_pi0d_MC",&_z_pi0d_MC,"z_pi0d_MC/D");
    _pi0_tree->Branch("_px_pi0d_MC",&_px_pi0d_MC,"px_pi0d_MC/D");
    _pi0_tree->Branch("_py_pi0d_MC",&_py_pi0d_MC,"py_pi0d_MC/D");
    _pi0_tree->Branch("_pz_pi0d_MC",&_pz_pi0d_MC,"pz_pi0d_MC/D");
    _pi0_tree->Branch("_theta_pi0d_MC",&_theta_pi0d_MC,"theta_pi0d_MC/D");
    _pi0_tree->Branch("_phi_pi0d_MC",&_phi_pi0d_MC,"phi_pi0d_MC/D");
    _pi0_tree->Branch("_e_pi0dReco",&_e_pi0dReco,"e_pi0dReco/D");
//    _pi0_tree->Branch("_showerlength_pi0dReco",&_showerlength_pi0dReco,"showerlength_pi0dReco/D");
    _pi0_tree->Branch("_theta_pi0dReco",&_theta_pi0dReco,"theta_pi0dReco/D");
    _pi0_tree->Branch("_phi_pi0dReco",&_phi_pi0dReco,"phi_pi0dReco/D");
    _pi0_tree->Branch("_x_pi0dReco",&_x_pi0dReco,"x_pi0dReco/D");
    _pi0_tree->Branch("_y_pi0dReco",&_y_pi0dReco,"y_pi0dReco/D");
    _pi0_tree->Branch("_z_pi0dReco",&_z_pi0dReco,"z_pi0dReco/D");
    _pi0_tree->Branch("_px_pi0dReco",&_px_pi0dReco,"px_pi0dReco/D");
    _pi0_tree->Branch("_py_pi0dReco",&_py_pi0dReco,"py_pi0dReco/D");
    _pi0_tree->Branch("_pz_pi0dReco",&_pz_pi0dReco,"pz_pi0dReco/D");
 //   _pi0_tree->Branch("_px_lepNormReco",&_px_lepNormReco,"px_lepNormReco/D");
 //   _pi0_tree->Branch("_py_lepNormReco",&_py_lepNormReco,"py_lepNormReco/D");
 //   _pi0_tree->Branch("_pz_lepNormReco",&_pz_lepNormReco,"pz_lepNormReco/D");
 //   _pi0_tree->Branch("_lep_dot",&_lep_dot,"_lep_dot/D");
 //   _pi0_tree->Branch("_lep_vtxdist",&_lep_vtxdist,"_lep_vtxdist/D");
    _pi0_tree->Branch("_misID",&_misID,"misID/I");

    // keep track of number of events gone by
    _numEvts = 0;
    // keep track of singleEs found
    _singlePi0_ctr = 0;

    _debug = false;

    // set default energy cut (for counting) to 0
    _eCut = 0;

    // set initial efficiency to 0
    _eff = 0.;

    _h_e_nu_correlation = new TH2F("h_eNu_eNuReco","True Neutrino Energy vs. Reconstructed Neutrino Energy;True Neutrino Energy [MEV];Reconstructed Neutrino Energy [MEV]",100,0,3000,100,0,3000);

  }

  void ERAnaPi0Selection::Reset()
  {}

  bool ERAnaPi0Selection::Analyze(const EventData &data, const ParticleSet &ps)
{

  if (_debug)
    std::cout << "******  Begin ERAnaPi0Selection Analysis  ******" << std::endl;

    // Get MC particle set
    auto mc_ps = MCParticleSet();

    // Reset tree variables
    ResetTreeVariables();

    _numEvts += 1;
    // count number of showers in event
    _n_electrons = 0;
    _n_gammas    = 0;

    // Get MC EventData (showers/tracks...)
    auto mc_data = MCEventData();
    // Count number of MC tracks and showers with E > _eCut MeV
    _n_showers   = 0;
    _n_tracks    = 0;
    _n_tracksInt = 0;

    // Keep track of all energy deposited in detector
    _EDep = 0;

    for (auto &s : data.AllShower()){
      _EDep += s._energy;
      if (s._energy > _eCut) { _n_showers += 1; }
    }
    for (auto &t : data.AllTrack()){
      _EDep += t._energy;
      if (t._energy > _eCut) { _n_tracks += 1; }
    }
    if (_debug) { std::cout << "Total Energy deposited in detector: " << _EDep << std::endl; }

    // If debug -> print out MC particle set
    if (_debug){
      std::cout << "MC Particle Tree: " << std::endl;
      for (auto &p : mc_ps)
	std::cout << p.Diagram();
    }


    // loop over all particles in MC particle set (made by Helper)
    for (auto &p : mc_ps){
      // Find the pi0 and store its energy
      if (abs(p.PdgCode()) == 111){
	_e_pi0MC = p.Energy();
	_x_pi0MC = p.Vertex()[0];
	_y_pi0MC = p.Vertex()[1];
	_z_pi0MC = p.Vertex()[2];
	_px_pi0MC = p.Momentum()[0];
	_py_pi0MC = p.Momentum()[1];
	_pz_pi0MC = p.Momentum()[2];
	_pdg_pi0MC = p.PdgCode();
	
	bool found_daughter = false;

	for (auto &pi0d : p.Daughters()){
//	  if (abs(pi0d.PdgCode()) == 11 || abs(pi0d.PdgCode()) == 13){

//	    if(found_daughter) 
//	      std::cout<<"wtf already found daughter? overwriting variables..."<<std::endl;
	    found_daughter = true;

	    _e_pi0d_MC = pi0d.Energy();
	    _x_pi0d_MC = pi0d.Vertex()[0];
	    _y_pi0d_MC = pi0d.Vertex()[1];
	    _z_pi0d_MC = pi0d.Vertex()[2];
	    _px_pi0d_MC = pi0d.Momentum()[0];
	    _py_pi0d_MC = pi0d.Momentum()[1];
	    _pz_pi0d_MC = pi0d.Momentum()[2];
	    _theta_pi0d_MC = (180./3.14) * acos( _pz_pi0d_MC / sqrt( _px_pi0d_MC*_px_pi0d_MC + _py_pi0d_MC*_py_pi0d_MC + _pz_pi0d_MC*_pz_pi0d_MC ) );
	    _phi_pi0d_MC   = (180./3.14) * asin( _py_pi0d_MC / sqrt( _px_pi0d_MC*_px_pi0d_MC + _py_pi0d_MC*_py_pi0d_MC ) );
	    _pdg_pi0d_MC = pi0d.PdgCode();

	 // }
	}
      }

      // now keep track of all showers and tracks in the event
      _e_neutrals = 0;
      _e_nucleus_diff = 0;
      bool found_parent_nucleus = false;
      for (auto &d : p.AllDaughters()){
	if ( (d->PdgCode() == 22) && (d->Energy() > _eCut) )
	  _n_gammas += 1;
	else if ( (abs(d->PdgCode()) == 11) && (d->Energy() > _eCut) )
	  _n_electrons += 1;
	else if ( (d->PdgCode() == 111) && (d->Energy() > _eCut) )
	  _n_pi0 += 1;
	else if ( (d->PdgCode() == 211) && (d->Energy() > _eCut) )
	  _n_piplus += 1;
	else if ( (d->PdgCode() == 2112) && (d->Energy() > _eCut) ){
	  _n_neutrons += 1;
	  // Add up neutral particles' energies
	  _e_neutrals += d->KineticEnergy();
	}
	else if ( (d->PdgCode() == 2212) && (d->Energy() > _eCut) )
	  _n_protons += 1;
	else if (found_parent_nucleus &&  d->PdgCode() == 1000180400 )
	  _e_nucleus_diff = d->KineticEnergy();
	else if ( d->PdgCode() == 1000180400 )
	  found_parent_nucleus = true;
      }
    }
    
    // find the number of tracks that start within 1 cm of the neutrino interaction
    _n_tracksInt = 0;
    geoalgo::Point_t pi0_vtx(_x_pi0MC,_y_pi0MC,_z_pi0MC);
    for (auto &part : mc_ps){
      if (abs(part.PdgCode()) == 13 || abs(part.PdgCode()) == 211 || abs(part.PdgCode()) == 2212 ){
	if ( pi0_vtx.Dist(part.Vertex()) < 1){
	  _n_tracksInt += 1;
	  _e_trkInt += part.KineticEnergy();
	}
      }
      for (auto &nud : part.Daughters()){
	if (abs(nud.PdgCode()) == 13 || abs(nud.PdgCode()) == 211 || abs(nud.PdgCode()) == 2212 ){
	  if ( pi0_vtx.Dist(nud.Vertex()) < 1){
	    _n_tracksInt += 1;
	    _e_trkInt += nud.KineticEnergy();
	  }
	}
      }
    }
    
    // Count number of tracks and showers with E > _eCut MeV
    _n_showersReco = 0;
    _n_tracksReco  = 0;
    for (auto &s : data.Shower())
      if (s->_energy > _eCut) { _n_showersReco += 1; }
    _n_tracksReco = data.Track().size();


    // size of ParticleSet should be the number of neutrinos found, 
    _n_psReco = ps.size();

    // if only 1 Pi0 interaction was found -> misID = 0
    if ( _n_psReco == 3 || _n_psReco == 4 ){
      _misID = 0;
      _singlePi0_ctr += 1;
    }


    if (_debug){
      std::cout << "Reco Particle Tree: " << std::endl;
      for (auto &p : ps)
	std::cout << p.Diagram();
	}

    // If no single electrons reconstructed -> fill tree anyway with misID info
 //   if ( _n_psReco == 0 )
 //     _pi0_tree->Fill();
    
    for(int i = 0; i< ps.size(); i++){
      Particle pi0 = ps[i];
      
//      std::cout<<"\n\n*****PDG of ps particle: "<<pi0.PdgCode() <<std::endl;
      
      if(pi0.PdgCode() == 111){
        _x_pi0Reco = pi0.Vertex()[0];
        _y_pi0Reco = pi0.Vertex()[1];
        _z_pi0Reco = pi0.Vertex()[2];
        _px_pi0Reco = pi0.Momentum()[0];
        _py_pi0Reco = pi0.Momentum()[1];
        _pz_pi0Reco = pi0.Momentum()[2];
        _n_tracksIntReco = ps[i].Daughters().size()-1;
 //       std::cout<<"vertex of pi0: "<<_x_pi0Reco<<", "<<_y_pi0Reco<<", "<<_z_pi0Reco<<std::endl;
	
      double momMag = 0;
      
      _e_trkIntReco = 0;
      _e_pi0Reco = 0;
      //find the pi0 daughter that is an e+/e- or gamma
      for (auto const& daught : ps[i].Daughters()){
//	std::cout<<"daughter pdg code: "<<daught.PdgCode() <<std::endl;
	_e_pi0Reco += daught.KineticEnergy();
	
	// if not a electon/gamma, add energy to tracks
	if (abs(daught.PdgCode()) != 11 && daught.PdgCode()!=22 ){
	  _e_trkIntReco += daught.KineticEnergy();
	}
	
	if(abs(daught.PdgCode()) == 11 || daught.PdgCode() == 22){
	  _e_pi0dReco = daught.Energy();
	  
	  //length of shower (geoalgo cone) associated with the electron
	  //_showerlength_pi0dReco = data.AllShower().at(daught.RecoObjID()).Length(); //kaleko
	  _x_pi0dReco = daught.Vertex()[0];
	  _y_pi0dReco = daught.Vertex()[1];
	  _z_pi0dReco = daught.Vertex()[2];
	  _px_pi0dReco = daught.Momentum()[0];
	  _py_pi0dReco = daught.Momentum()[1];
	  _pz_pi0dReco = daught.Momentum()[2];
	  _theta_pi0dReco = (180./3.14) * acos( _pz_pi0dReco / sqrt( _px_pi0dReco*_px_pi0dReco + _py_pi0dReco*_py_pi0dReco + _pz_pi0dReco*_pz_pi0dReco ) );
	  _phi_pi0dReco   = (180./3.14) * asin( _py_pi0dReco / sqrt( _px_pi0dReco*_px_pi0dReco + _py_pi0dReco*_py_pi0dReco ) );
	  _lep_vtxdist = sqrt( (_x_pi0d_MC-_x_pi0dReco)*(_x_pi0d_MC-_x_pi0dReco) +
			       (_y_pi0d_MC-_y_pi0dReco)*(_y_pi0d_MC-_y_pi0dReco) + 
			       (_z_pi0d_MC-_z_pi0dReco)*(_z_pi0d_MC-_z_pi0dReco) );
	  _lep_dot = ( ( _px_pi0d_MC*_px_pi0dReco + _py_pi0d_MC*_py_pi0dReco + _pz_pi0d_MC*_pz_pi0dReco ) / 
		       ( sqrt( _px_pi0dReco*_px_pi0dReco + _py_pi0dReco*_py_pi0dReco + _pz_pi0dReco*_pz_pi0dReco ) * 
			 sqrt( _px_pi0d_MC*_px_pi0d_MC + _py_pi0d_MC*_py_pi0d_MC + _pz_pi0d_MC*_pz_pi0d_MC ) ) );
	  
	  ::geoalgo::HalfLine shr(daught.Vertex(),daught.Momentum());
	  
	//  double detHalfHeight = 116.5 ;
	//  _distToTopWall = (_y_pi0dReco - detHalfHeight)*daught.Momentum().Length()/_py_pi0dReco ;
	  
	  _distToWall = sqrt(_geoAlgo.SqDist(daught.Vertex(),fTPC));
	  if(_geoAlgo.Intersection(fTPC,shr,true).size() > 0)
	    _distBackAlongTraj = sqrt(daught.Vertex().SqDist(_geoAlgo.Intersection(fTPC,shr,true)[0])) ;
	  else
	    _distBackAlongTraj = -999; 
	  
	  momMag = sqrt(_px_pi0dReco*_px_pi0dReco + _py_pi0dReco *_py_pi0dReco + _pz_pi0dReco*_pz_pi0dReco);
	  _px_lepNormReco = _px_pi0dReco / momMag ;
	  _py_lepNormReco = _py_pi0dReco / momMag ;
	  _pz_lepNormReco = _pz_pi0dReco / momMag ;
	  
	}// if particle is lepton
      }// for all daughters
	}
    }// loop over all Pi0s found in event
      _pi0_tree->Fill();
    
    _h_e_nu_correlation->Fill(_e_pi0MC,_e_pi0Reco);
    
    if (_debug){
      std::cout << "Ana results:" << std::endl
		<< "Mis-ID                 : " << _misID << std::endl           
		<< "Pi0 E  [Mc,Reco]  : [" << _e_pi0MC << ", " << _e_pi0Reco << "]" <<  std::endl
		<< "Daughter E  [Mc,Reco]    : [" << _e_pi0d_MC << ", " << _e_pi0dReco << "]" <<  std::endl
		<< "Neutrals E  [Mc,Reco]  : [" << _e_neutrals << "]" <<  std::endl
		<< "Nucl dE  [Mc,Reco]     : [" << _e_nucleus_diff << "]" <<  std::endl
		<< "Lepton Vtx dist Mc-Reco:  " << sqrt( (_x_pi0d_MC-_x_pi0dReco)*(_x_pi0d_MC-_x_pi0dReco) +
							 (_y_pi0d_MC-_y_pi0dReco)*(_y_pi0d_MC-_y_pi0dReco) +
							 (_z_pi0d_MC-_z_pi0dReco)*(_z_pi0d_MC-_z_pi0dReco) ) << std::endl
		<< "Daughter theta [Mc,Reco] : [" << _theta_pi0d_MC << ", " << _theta_pi0dReco << "]" <<  std::endl
		<< "Daughter phi [Mc,Reco]   : [" << _phi_pi0d_MC << ", " << _phi_pi0dReco << "]" <<  std::endl << std::endl;
    }
    
    return true;
}
  
  void ERAnaPi0Selection::ProcessEnd(TFile* fout)
  {
    
    _eff = 100*_singlePi0_ctr/float(_numEvts);
    
    std::cout << "RESULTS: " << std::endl
	      << "Tot Events    : " << _numEvts << std::endl
	      << "Pi0 Events    : " << _singlePi0_ctr << std::endl
	      << "Eff           : " << _eff << " %" << std::endl;

    MakeEffPlot("e_pi0d_MC",10,0,2000);
    MakeEffPlot("e_pi0MC",10,0,2000);
//    MakeEffPlot("n_tracks",10,-0.5,9.5);
//    MakeEffPlot("n_tracksReco",10,-0.5,9.5);

    if (fout){
      fout->cd();
      _pi0_tree->Write();
      _h_e_nu_correlation->Write();
    }

    return;
  }


  void ERAnaPi0Selection::MakeEffPlot(std::string varname,
				 int nbins, double xmin, double xmax){

    int Nall, Nok;
    // Efficiency vs. variable "varname"
    TH1D *hist = new TH1D(Form("_hEffvs%s",varname.c_str()),Form("Efficiency vs %s",varname.c_str()),nbins,xmin,xmax);
    TH1D *All = new TH1D("All","All",nbins,xmin,xmax); // Histogram of all entries as a function of Elep
    TH1D *Ok = new TH1D("Ok","Ok",nbins,xmin,xmax);  // Histogram of all entries that are not mis-ID vs Elep
    Nall = _pi0_tree->Draw(Form("%s>>All",varname.c_str()),"");
    Nok = _pi0_tree->Draw(Form("%s>>Ok",varname.c_str()),"misID==0");
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

  void ERAnaPi0Selection::ResetTreeVariables(){
    
    _misID = 1;

    _n_psReco  = -1;
    _n_electrons   = -1;
    _n_gammas      = -1;
    _n_showers     = -1;
    _n_showersReco = -1;
    _n_tracks      = -1;
    _n_tracksReco  = -1;
    _n_tracksInt   = -1;
    _n_tracksIntReco = -1;
    _n_protons     = -1;
    _n_neutrons    = -1;
    _n_piplus      = -1;
    _n_pi0         = -1;

    _e_pi0MC          = -1000;
    _e_pi0Reco      = -1000;
    _x_pi0MC         = -1000;
    _y_pi0MC         = -1000;
    _z_pi0MC         = -1000;
    _x_pi0Reco     = -1000;
    _y_pi0Reco     = -1000;
    _z_pi0Reco     = -1000;
    _px_pi0MC         = -1000;
    _py_pi0MC         = -1000;
    _pz_pi0MC         = -1000;
    _px_pi0Reco     = -1000;
    _py_pi0Reco     = -1000;
    _pz_pi0Reco     = -1000;
    _pdg_pi0MC        = -1;
    _e_pi0d_MC         = -1000;
    _x_pi0d_MC         = -1000;
    _y_pi0d_MC         = -1000;
    _z_pi0d_MC         = -1000;
    _pdg_pi0d_MC       = -1;
    _theta_pi0d_MC     = -360;
    _phi_pi0d_MC       = -360;
    _e_pi0dReco     = -1000;
    _showerlength_pi0dReco     = -1000;
    _x_pi0dReco     = -1000;
    _y_pi0dReco     = -1000;
    _z_pi0dReco     = -1000;
    _theta_pi0dReco = -360;
    _phi_pi0dReco   = -360;
    _lep_dot       = -2;
    _lep_vtxdist   = -1000;
    _distToTopWall = -9999;

    _e_trkInt = -1;
    _e_trkIntReco = -1;

    return;
  }

  }

#endif
