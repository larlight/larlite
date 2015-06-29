#ifndef ERTOOL_ANANDKMODEZERO_CXX
#define ERTOOL_ANANDKMODEZERO_CXX

#include "AnaNdkModeZero.h"

namespace ertool {

  AnaNdkModeZero::AnaNdkModeZero(const std::string& name)
  : AnaBase(name)
  , _result_tree(nullptr)
  , fTPC(0.,-115.5,0.,254.8,117.5,1036.92)

  {

    if (_result_tree) { delete _result_tree; }
    _result_tree = new TTree("_result_tree","Result Tree");
    //_result_tree->Branch("_numEvts",&_numEvts,"numEvts/I");
    _result_tree->Branch("_n_triplets", &_n_triplets, "n_triplets/I");
    _result_tree->Branch("_triplet_vtx_radius", "vector<double>", &_triplet_vtx_radius);
    _result_tree->Branch("_triplet_energy", "vector<double>", &_triplet_energy);
    _result_tree->Branch("_triplet_px", "vector<double>", &_triplet_px);
    _result_tree->Branch("_triplet_py", "vector<double>", &_triplet_py);
    _result_tree->Branch("_triplet_pz", "vector<double>", &_triplet_pz);
    _result_tree->Branch("_shower1_e", "vector<double>", &_shower1_e);
    _result_tree->Branch("_shower1_px", "vector<double>", &_shower1_px);
    _result_tree->Branch("_shower1_py", "vector<double>", &_shower1_py);
    _result_tree->Branch("_shower1_pz", "vector<double>", &_shower1_pz);
    _result_tree->Branch("_shower1_m", "vector<double>", &_shower1_m);
    _result_tree->Branch("_shower1_x", "vector<double>", &_shower1_x);
    _result_tree->Branch("_shower1_y", "vector<double>", &_shower1_y);
    _result_tree->Branch("_shower1_z", "vector<double>", &_shower1_z);
    _result_tree->Branch("_shower1_pdg", "vector<double>", &_shower1_pdg);
    _result_tree->Branch("_shower2_e", "vector<double>", &_shower2_e);
    _result_tree->Branch("_shower2_px", "vector<double>", &_shower2_px);
    _result_tree->Branch("_shower2_py", "vector<double>", &_shower2_py);
    _result_tree->Branch("_shower2_pz", "vector<double>", &_shower2_pz);
    _result_tree->Branch("_shower2_m", "vector<double>", &_shower2_m);
    _result_tree->Branch("_shower2_x", "vector<double>", &_shower2_x);
    _result_tree->Branch("_shower2_y", "vector<double>", &_shower2_y);
    _result_tree->Branch("_shower2_z", "vector<double>", &_shower2_z);
    _result_tree->Branch("_shower2_pdg", "vector<double>", &_shower2_pdg);
    _result_tree->Branch("_shower3_e", "vector<double>", &_shower3_e);
    _result_tree->Branch("_shower3_px", "vector<double>", &_shower3_px);
    _result_tree->Branch("_shower3_py", "vector<double>", &_shower3_py);
    _result_tree->Branch("_shower3_pz", "vector<double>", &_shower3_pz);
    _result_tree->Branch("_shower3_m", "vector<double>", &_shower3_m);
    _result_tree->Branch("_shower3_x", "vector<double>", &_shower3_x);
    _result_tree->Branch("_shower3_y", "vector<double>", &_shower3_y);
    _result_tree->Branch("_shower3_z", "vector<double>", &_shower3_z);
    _result_tree->Branch("_shower3_pdg", "vector<double>", &_shower3_pdg);


    _result_tree->Branch("_n_pion", &_n_pion, "n_pion/I");
    _result_tree->Branch("_e_pion", "vector<double>", &_e_pion);
    _result_tree->Branch("_x_pion", "vector<double>", &_x_pion);
    _result_tree->Branch("_y_pion", "vector<double>", &_y_pion);
    _result_tree->Branch("_z_pion", "vector<double>", &_z_pion);
    _result_tree->Branch("_px_pion", "vector<double>", &_px_pion);
    _result_tree->Branch("_py_pion", "vector<double>", &_py_pion);
    _result_tree->Branch("_pz_pion", "vector<double>", &_pz_pion); 
    _result_tree->Branch("_n_eplus", &_n_eplus, "n_eplus/I");   
    _result_tree->Branch("_e_eplus", "vector<double>", &_e_eplus);
    _result_tree->Branch("_x_eplus", "vector<double>", &_x_eplus);
    _result_tree->Branch("_y_eplus", "vector<double>", &_y_eplus);
    _result_tree->Branch("_z_eplus", "vector<double>", &_z_eplus);
    _result_tree->Branch("_px_eplus", "vector<double>", &_px_eplus);
    _result_tree->Branch("_py_eplus", "vector<double>", &_py_eplus);
    _result_tree->Branch("_pz_eplus", "vector<double>", &_pz_eplus); 

    // keep track of number of events gone by
    _numEvts = 0;

    _debug = false;

    // set default energy cut (for counting) to 0
    _eCut = 0;

  }

  void AnaNdkModeZero::Reset()
  {}

  bool AnaNdkModeZero::Analyze(const EventData &data, const ParticleGraph &graph)
  {

    if (_debug)
      std::cout << "******  Begin AnaNdkModeZero Analysis  ******" << std::endl;

    if (_debug){
      std::cout << "ParticleGraph Diagram: " << std::endl
      << graph.Diagram() << std::endl;
    }

    // Get MC particle set
    auto const& mc_graph = MCParticleGraph();
    auto const& mc_graph_arr = mc_graph.GetParticleArray();

    if (_debug){
      std::cout << "MC Particle Diagram: " << std::endl
      << mc_graph.Diagram() << std::endl;
    }

    // Reset tree variables
    // Assume we will mis-ID
    ResetTreeVariables();

    _numEvts++;

    // Get MC EventData (showers/tracks...)
    auto const& mc_data = MCEventData();

    // FIRST WE PERFORM OUR PDG BASED SEARCH

    // loop over all particles in MC particle set to store TRUTH-LEVEL information about the decay
    for (auto &p : mc_graph_arr){
      // Find the pion and store its energy
      if ((p.PdgCode() == 111) && (p.Energy() > _eCut)){
        _n_pion++;
        _e_pion.push_back(p.Energy());
        _x_pion.push_back(p.Vertex()[0]);
        _y_pion.push_back(p.Vertex()[1]);
        _z_pion.push_back(p.Vertex()[2]);
        _px_pion.push_back(p.Momentum()[0]);
        _py_pion.push_back(p.Momentum()[1]);
        _pz_pion.push_back(p.Momentum()[2]);
      }

      // Find the e+ and store its energy
      if ((p.PdgCode() == -11) && (p.Energy() > _eCut)){
        _n_eplus++;
        _e_eplus.push_back(p.Energy());
        _x_eplus.push_back(p.Vertex()[0]);
        _y_eplus.push_back(p.Vertex()[1]);
        _z_eplus.push_back(p.Vertex()[2]);
        _px_eplus.push_back(p.Momentum()[0]);
        _py_eplus.push_back(p.Momentum()[1]);
        _pz_eplus.push_back(p.Momentum()[2]);
      }
    }

    // NOW LET'S DEAL WITH THE TOPOLOGY BASED SEARCH

    // vertices must be within one cm of each other
    double _vtx_cutoff = 20;

    // identify triplets of showers
    auto const& triplets = mc_graph.GetNodeCombinations(3, kShower, false);
    
    // loop through these triplets to find showers with similar origin
    for (auto &trip : triplets){
      auto const& shower1 = mc_graph.GetParticle(trip[0]);
      auto const& shower2 = mc_graph.GetParticle(trip[1]);
      auto const& shower3 = mc_graph.GetParticle(trip[2]);
      //_shower1_vec.push_back(shower1);
      // extract vertices
      auto const& vtx1 = shower1.Vertex();
      auto const& vtx2 = shower2.Vertex();
      auto const& vtx3 = shower3.Vertex();
      ::geoalgo::Sphere sphere(vtx1, vtx2, vtx3);
      // check to see if the vertices of the showers are nearby each other
      if(sphere.Radius() < _vtx_cutoff){

        // fill all triplet-related branches
        _n_triplets++;
        _triplet_vtx_radius.push_back(sphere.Radius());
        _triplet_energy.push_back(shower1.Energy()+shower2.Energy()+shower3.Energy());
        _triplet_px.push_back(shower1.Momentum()[0]+shower2.Momentum()[0]+shower3.Momentum()[0]);
        _triplet_py.push_back(shower1.Momentum()[1]+shower2.Momentum()[1]+shower3.Momentum()[1]);
        _triplet_pz.push_back(shower1.Momentum()[2]+shower2.Momentum()[2]+shower3.Momentum()[2]);
        _shower1_e.push_back(shower1.Energy());
        _shower1_px.push_back(shower1.Momentum()[0]);
        _shower1_py.push_back(shower1.Momentum()[1]);
        _shower1_pz.push_back(shower1.Momentum()[2]);
        _shower1_m.push_back(shower1.Mass());
        _shower1_x.push_back(shower1.Vertex()[0]);
        _shower1_y.push_back(shower1.Vertex()[1]);
        _shower1_z.push_back(shower1.Vertex()[2]);
        _shower1_pdg.push_back(shower1.PdgCode());
        _shower2_e.push_back(shower2.Energy());
        _shower2_px.push_back(shower2.Momentum()[0]);
        _shower2_py.push_back(shower2.Momentum()[1]);
        _shower2_pz.push_back(shower2.Momentum()[2]);
        _shower2_m.push_back(shower2.Mass());
        _shower2_x.push_back(shower2.Vertex()[0]);
        _shower2_y.push_back(shower2.Vertex()[1]);
        _shower2_z.push_back(shower2.Vertex()[2]);
        _shower2_pdg.push_back(shower2.PdgCode());
        _shower3_e.push_back(shower3.Energy());
        _shower3_px.push_back(shower3.Momentum()[0]);
        _shower3_py.push_back(shower3.Momentum()[1]);
        _shower3_pz.push_back(shower3.Momentum()[2]);
        _shower3_m.push_back(shower3.Mass());
        _shower3_x.push_back(shower3.Vertex()[0]);
        _shower3_y.push_back(shower3.Vertex()[1]);
        _shower3_z.push_back(shower3.Vertex()[2]);
        _shower3_pdg.push_back(shower3.PdgCode());

      }
    }

    // fill the tree
    _result_tree->Fill();

    return true;
  }
  
  void AnaNdkModeZero::ProcessEnd(TFile* fout)
  {
    
    // std::cout << "RESULTS: " << std::endl;
    // << "Tot Events    : " << _numEvts << std::endl
    // << "SingleE Events: " << _singleE_ctr << std::endl
    // << "Eff           : " << _eff << " " << std::endl;

    if (fout){
      fout->cd();
      _result_tree->Write();
    }

    return;
  }


  void AnaNdkModeZero::MakeEffPlot(std::string varname,
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

 void AnaNdkModeZero::ResetTreeVariables(){

  _n_triplets = 0;
  _triplet_vtx_radius.clear();
  _triplet_energy.clear();
  _triplet_px.clear();
  _triplet_py.clear();
  _triplet_pz.clear();
  _shower1_e.clear();
  _shower1_px.clear();
  _shower1_py.clear();
  _shower1_pz.clear();
  _shower1_m.clear();
  _shower1_x.clear();
  _shower1_y.clear();
  _shower1_z.clear();
  _shower1_pdg.clear();
  _shower2_e.clear();
  _shower2_px.clear();
  _shower2_py.clear();
  _shower2_pz.clear();
  _shower2_m.clear();
  _shower2_x.clear();
  _shower2_y.clear();
  _shower2_z.clear();
  _shower2_pdg.clear();
  _shower3_e.clear();
  _shower3_px.clear();
  _shower3_py.clear();
  _shower3_pz.clear();
  _shower3_m.clear();
  _shower3_x.clear();
  _shower3_y.clear();
  _shower3_z.clear();
  _shower3_pdg.clear();

  _n_pion = 0;
  _n_eplus = 0;
  _e_pion.clear();
  _x_pion.clear(); _y_pion.clear(); _z_pion.clear();
  _px_pion.clear(); _py_pion.clear(); _pz_pion.clear();
  _e_eplus.clear();
  _x_eplus.clear(); _y_eplus.clear(); _z_eplus.clear();
  _px_eplus.clear(); _py_eplus.clear(); _pz_eplus.clear();

  return;
}

}

#endif
