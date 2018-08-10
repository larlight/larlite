#ifndef ERTOOL_ERANANGAMMA_CXX
#define ERTOOL_ERANANGAMMA_CXX

#include <ERAnaNGamma.h>

namespace ertool {

  ERAnaNGamma::ERAnaNGamma(const std::string& name)
    : AnaBase(name)
    , _result_tree(nullptr)
    , fTPC(0.,-115.5,0.,254.8,117.5,1036.92)
  {
    PrepareTreeVariables();
    _numEvts = 0;
    _N_gamma_ctr = 0;
    _debug = false;
    _eCut = 0;
    _eff = 0.;
  }

  void ERAnaNGamma::Reset()
  {}

  void ERAnaNGamma::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAnaNGamma::ProcessBegin()
  {}

  bool ERAnaNGamma::Analyze(const EventData &data, const ParticleGraph &ps)
  {
    if(_debug)
      std::cout<<"***** Begin ERAnaNGamma Analysis *****"<<std::endl;
    /* if(_debug)
      {
      std::cout<<"Particle Graph Diagram: "<<std::endl
	       << graph.Diagram()<<std::endl;
      } */
    
    /// Get MC particle set
    auto const& mc_graph = MCParticleGraph();

    /*    if(_debug)
      {
	std::cout<<"MC Particle Diagram: "<<std::endl
		 <<mc_graph.Diagram()<<std::endl;
		 }*/
    /// Reset tree variables (assume we will mis-ID)
    ResetTreeVariables();
    
    _numEvts +=1;
    
    /// Keep track of all energy deposited in detector
    _EDep = 0;
    
    for(auto const s :data.Shower())
      {
	_EDep += s._energy;
	if(s._energy > _eCut) {_n_showers +=1;}
      }
    for(auto const &t : data.Track())
      {
	_EDep += t._energy;
	if(t._energy > _eCut) {_n_tracks +=1;}
      }
    if(_debug) {std::cout<<"Total energy deposited in detector: "<<_EDep<<std::endl;}

    int neutrinos = 0;
    int gammas = 0;
    /// Loop over all particles in MC particle set
    for(auto &p : mc_graph.GetParticleArray())
      {
	/// find the neutrino and store it's energy
	if(abs(p.PdgCode()) == 12 || abs(p.PdgCode()) == 14)
	  {
	    _e_nu = p.Energy();
	    /*_x_nu = p.Vertex()[0];
	    _y_nu = p.Vertex()[1];
	    _z_nu = p.Vertex()[2];
	    _px_nu = p.Momentum()[0];
	    _py_nu = p.Momentum()[1];
	    _pz_nu = p.Momentum()[2];
	    _pdg_nu = p.PdgCode();*/
	    neutrinos +=1;
	  }
	/// find the gamma and store its energy
	if(abs(p.PdgCode()) == 22)
	  {_e_gamma = p.Energy();
	    gammas +=1;
	  }
      } // <-- end p loop
    if(neutrinos ==1)
      {_N_gamma_ctr += 1;} /// number of Ngamma events
    return true;
  } // <-- end Analyze

  void ERAnaNGamma::ProcessEnd(TFile* fout)
  {
    _eff = 100*_N_gamma_ctr/float(_numEvts);

    std::cout << "RESULTS: "<<std::endl
	      << "Tot Events : " << _numEvts << std::endl
	      << "N gamma events: " << _N_gamma_ctr << std::endl
	      << "Efficiency : " << _eff << std::endl;
    if(fout)
      {
	fout->cd();
	_result_tree->Write();
      }
    return;
  }

  void ERAnaNGamma::PrepareTreeVariables()
  {
    if(_result_tree) {delete _result_tree;}
    _result_tree = new TTree("_result_tree","Result Tree");
    return;
  }
  void ERAnaNGamma::ResetTreeVariables()
  {
    return;
  }

}

#endif
