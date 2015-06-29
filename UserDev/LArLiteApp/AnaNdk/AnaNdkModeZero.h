#ifndef ERTOOL_ANANDKMODEZERO_H
#define ERTOOL_ANANDKMODEZERO_H

#include "ERTool/Base/AnaBase.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2F.h"
#include "GeoAlgo/GeoAlgo.h"
#include "ERTool/Algo/AlgoFindRelationship.h"
#include "DataFormat/mctruth.h"

namespace ertool {

  /**
     \class AnaNdkModeZero
     User custom Analysis class made by david caratelli
   */
  class AnaNdkModeZero : public AnaBase {
  
  public:

    /// Default constructor
    AnaNdkModeZero(const std::string& name="AnaNdkModeZero");

    /// Default destructor
    virtual ~AnaNdkModeZero(){};

    /// Accept Pset
    void AcceptPSet(const ::fcllite::PSet& cfg) {}

    /// Reset function
    virtual void Reset();

    /// Function to evaluate input showers and determine a score
    virtual bool Analyze(const EventData &data, const ParticleGraph &graph);

    void SetDebug(bool on) { _debug = on; }

    virtual void ProcessBegin() {}

    virtual void ProcessEnd(TFile* fout);

    /// Function to re-set TTree variables
    void ResetTreeVariables();

    /// Function to save efficiency vs. variable to histogram
    void MakeEffPlot(std::string varname,
		     int nbins, double xmin, double xmax);

    /// get reported efficiency
    double getEfficiency() { return _eff; }

    /// set the energy cut to be used when counting particles
    void SetECut(double c) { _eCut = c; }

  private:

    // debug flag
    bool _debug;

    // energy cut for counting particles
    float _eCut;

    // efficiency
    double _eff = 1;

    // Result tree comparison for reconstructed events
    TTree* _result_tree;

    int _n_triplets;
    std::vector<double> _triplet_vtx_radius, _triplet_energy, _triplet_px, _triplet_py, _triplet_pz;
    std::vector<double> _shower1_e, _shower1_px, _shower1_py, _shower1_pz, _shower1_m, _shower1_x, _shower1_y, _shower1_z, _shower1_pdg;
    std::vector<double> _shower2_e, _shower2_px, _shower2_py, _shower2_pz, _shower2_m, _shower2_x, _shower2_y, _shower2_z, _shower2_pdg;
    std::vector<double> _shower3_e, _shower3_px, _shower3_py, _shower3_pz, _shower3_m, _shower3_x, _shower3_y, _shower3_z, _shower3_pdg;

    int _n_pion;
    int _n_eplus;
    std::vector<double> _e_pion;
    std::vector<double> _x_pion, _y_pion, _z_pion;
    std::vector<double> _px_pion, _py_pion, _pz_pion;
    std::vector<double> _e_eplus;
    std::vector<double> _x_eplus, _y_eplus, _z_eplus;
    std::vector<double> _px_eplus, _py_eplus, _pz_eplus;

    /// counters to cout to screen after running
    int _numEvts;

    ::geoalgo::GeoAlgo _geoAlgo;
    ::geoalgo::AABox fTPC;
    AlgoFindRelationship _findRel;
   
  };
}
#endif

/** @} */ // end of doxygen group 
