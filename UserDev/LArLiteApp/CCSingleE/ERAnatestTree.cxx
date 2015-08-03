#ifndef ERTOOL_ERANATESTTREE_CXX
#define ERTOOL_ERANATESTTREE_CXX

#include "ERAnatestTree.h"

namespace ertool {

  ERAnatestTree::ERAnatestTree(const std::string& name) : AnaBase(name),
  _tree(nullptr)
  {
    if (_tree) { delete _tree; }
    _tree = new TTree("_tree","Result Tree");
    _tree->Branch("_xMC",&_xMC,"xMC/D");
    _tree->Branch("_yMC",&_yMC,"yMC/D");
    _tree->Branch("_zMC",&_zMC,"zMC/D");
    _tree->Branch("_eMC",&_eMC,"eMC/D");
    _tree->Branch("_keMC",&_keMC,"keMC/D");
    _tree->Branch("_mMC",&_mMC,"mMC/D");
    _tree->Branch("_xReco",&_xReco,"xReco/D");
    _tree->Branch("_yReco",&_yReco,"yReco/D");
    _tree->Branch("_zReco",&_zReco,"zReco/D");
    _tree->Branch("_eReco",&_eReco,"eReco/D");
    _tree->Branch("_keReco",&_keReco,"keReco/D");
    _tree->Branch("_mReco",&_mReco,"mReco/D");
    counter = 0;
    event_counter = 0;
    neutrino = 0;
    _fv = 2.625;
    edgeEvents = 0;
    inside = true;
    all_event_counter = 0;
    positron_electron_events = 0;
    ep_check = 0;
    all_neutrino_events = 0;
  }

  void ERAnatestTree::Reset()
  {}

  void ERAnatestTree::PrepareTTree(){
    _xMC = -1000 ;
    _xReco = -1000 ;
    _yMC = -1000 ;
    _yReco = -1000 ;
    _zMC = -1000 ;
    _zReco = -1000 ;
    _mMC = -1000 ;
    _mReco = -1000 ;
    _eMC = -1000 ;
    _eReco = -1000 ;
    _keMC = -1000 ;
    _keReco = -1000 ;
    }

  void ERAnatestTree::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAnatestTree::ProcessBegin()
  {}

  bool ERAnatestTree::Analyze(const EventData &data, const ParticleGraph &ps)
  {
    all_event_counter++;
    for (auto &p : ps.GetParticleArray())
    {
        if (abs(p.PdgCode()) != 11) continue;
	else {
	  _xReco = p.Vertex()[0];
	  _yReco = p.Vertex()[1];
	  _zReco = p.Vertex()[2];
	}
    }
    ep_check = 0;
    for (auto &p : ps.GetParticleArray()){
      if (abs(p.PdgCode()) == 11) {ep_check++;}
    }
    for (auto &p : ps.GetParticleArray()){
      if (abs(p.PdgCode()) == 12 && ep_check < 2) {all_neutrino_events++;}
    }
   
    if( _xReco < _fv || _xReco > (45 - _fv) || _yReco < (-20 + _fv) || _yReco > (20 - _fv) || _zReco < _fv || _zReco > (90 - _fv) ) {
      edgeEvents++;
      std::cout << "edge event!\n";
      if (_xReco < _fv || _xReco > (45 - _fv)){
	std::cout << "problem in the x coordinate! x = " << _xReco << "\n\n";
      }
      if (_yReco < (-20 + _fv) || _yReco > (20 - _fv)){
	std::cout << "problem in the y coordinate! y = " << _yReco << "\n\n";
      }
      if (_zReco < _fv || _zReco > (90 - _fv)){
	std::cout << "problem in the z coordinate! z = " << _zReco << "\n\n";
      }
    }
    else {
      event_counter++;
      PrepareTTree() ;

      std::cout << "**************EVENT "<< event_counter << "******************\n";
      for (auto &p : ps.GetParticleArray())
      {
        if (abs(p.PdgCode()) != 11) continue;
	else {
	  _xReco = p.Vertex()[0];
	  std::cout << "Reco PDG is " << p.PdgCode() << "\n";
	  std::cout << "The Reco x coordinate is " << _xReco << "\n";
	  _yReco = p.Vertex()[1];
	  std::cout << "The Reco y coordinate is " << _yReco << "\n";
	  _zReco = p.Vertex()[2];
	  std::cout << "The Reco z coordinate is " << _zReco << "\n";
	  _tree->Fill();
	  std::cout << "\n ";
	}
	  //if( _xReco < _fv || _xReco > (47 - _fv) || _yReco < (-20 + _fv) || _yReco > (20 - _fv) || _zReco < _fv || _zReco > (90 - _fv) ){ 
	  // edgeEvents++;
	  //inside = false ;
	    //std::cout<<"BREAKING OUT! "<<std::endl ;	    
	    //inside = 0 ;  
	    // break ;
	    //continue;
          //}
	// else {
	  //  inside = true;
	  // }
	//}
      }

      neutrino = 0;
      for (auto &p : ps.GetParticleArray()){
	if (abs(p.PdgCode()) == 12 /*&& inside == true*/) {
	 neutrino++ ;
	}
      }


      if (neutrino == 1 /*&& inside == true*/) {
	counter++;
      }
      else  {
	std::cout << " Missed this event!" << "\n" ;
	std::cout << "Missed event is: " << event_counter <<"\n\n\n";
      }
 
      // if (neutrino == 1) {
      //all_event_counter++;
      //}

      positron_electron_events = 0;
      for (auto &p : ps.GetParticleArray()){
	if (abs(p.PdgCode()) == 11) {
	  positron_electron_events++ ;
	} 
      }

      if (positron_electron_events > 1) {
	event_counter = event_counter - 1;
	//all_event_counter = all_event_counter - 1;
      }

      return true; 
    }
    return true;
  }


  void ERAnatestTree::ProcessEnd(TFile* fout)
  {
    std::cout << "Total Events (BEFORE Any Cuts): " << all_event_counter << "\n";
    std::cout << "Total Events with Reconstructed Neutrinos: " << all_neutrino_events << "\n";
    //std::cout << "Events Selected: " << counter << "\n";
    std::cout << "Events Selected (WITH Fiducial Cut): " << counter << "\n";
    std::cout << "Edge Events Cut: " << edgeEvents << "\n";
    // std::cout << "Total Events: " << event_counter << "\n";
    std::cout << "Total Events (WITH Fiducial Cut): " << event_counter << "\n";
    std::cout << "Selection Efficiency: " << float (counter)/event_counter << "\n";
    std::cout << "Selection Efficiency with Fiducial Cut: " << float (counter)/(event_counter) << "\n";
    std::cout << "Overall Selection Efficiency: " << float (counter)/all_neutrino_events << "\n";
    if (fout){
      fout->cd();
      _tree->Write();
     }
  }

}

#endif
