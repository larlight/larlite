/**
 * \file ERAnaPi0Selection.h
 *
 * \ingroup Pi0Ana
 * 
 * \brief Class def header for a class ERAnaPi0Selection
 *
 * @author ariana
 */

/** \addtogroup Pi0Ana

    @{*/

#ifndef ERTOOL_ERANAPI0_SELECTION_H
#define ERTOOL_ERANAPI0_SELECTION_H

#include "ERTool/Base/AnaBase.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2F.h"
#include "GeoAlgo/GeoAlgo.h"
#include "ERTool/Algo/AlgoFindRelationship.h"
#include "DataFormat/mctruth.h"

namespace ertool {

  /**
     \class ERAnaPi0Selection
     User custom Analysis class made by kazuhiro
   */
  class ERAnaPi0Selection : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaPi0Selection();

    /// Default destructor
    virtual ~ERAnaPi0Selection(){};

    /// Reset function
    virtual void Reset();

    /// Function to evaluate input showers and determine a score
    virtual bool Analyze(const EventData &data, const ParticleSet &ps);

    void SetDebug(bool on) { _debug = on; }

    void ProcessEnd(TFile* fout);

    /// Function to re-set TTree variables
    void ResetTreeVariables();

    /// get reported efficiency
    double getEfficiency() { return _eff; }

    /// set the energy cut to be used when counting particles
    void SetECut(double c) { _eCut = c; }

  private:

    // debug flag
    bool _debug;

    // energy cut for counting particles
    float _eCut;

    // Result tree comparison for reconstructed events
    TTree* _pi0_tree;

    //Number of reconstructed *particles* in the output PSet
    //(this is 1-to-1 with number of single electrons reconstructed)
    int _n_psReco;
    int _n_gammas;     ///Total number of MC gamma particles in the event
    int _n_gammasReco;
    int _n_showers;    ///Number of mc *showers*
    int _n_showersReco;/// Number of reconstructed *showers* in the event data (could be e or g, etc)
    int _n_pi0;        /// number of pi0 (E > 20 MeV)
    int _misID;     /// Whether this event was MID'd (if == 0: we found == 1 single electron)

    double _e_nu_MC;     /// Neutrino energy
    double _x_nu_MC, _y_nu_MC, _z_nu_MC; /// Neutrino position
    double _px_nu_MC, _py_nu_MC, _pz_nu_MC; /// Neutrino momentum
    int _pdg_nu_MC;       /// Neutrino PDG
    double _e_gamma_MC;     /// gamma_MC from neutrino Energy
    double _x_gamma_MC, _y_gamma_MC, _z_gamma_MC; /// gamma_MC x,y,z start point
    double _px_gamma_MC, _py_gamma_MC, _pz_gamma_MC; /// gamma_MC x,y,z momentum point
    int _pdg_gamma_MC;      /// gamma_MC from neutrino Pdg
    double _theta_gamma_MC; /// gamma_MC angle w.r.t. beam direction
    double _phi_gamma_MC;   /// gamma_MC angle w.r.t. x on xy plane
    double _gamma_score_MC;

    double _e_nu_Reco;
    double _x_nu_Reco, _y_nu_Reco, _z_nu_Reco; /// Neutrino position
    double _px_nu_Reco, _py_nu_Reco, _pz_nu_Reco; /// Neutrino position
    double _e_gamma_Reco; /// lepton E from reconstruction (i.e. Pi0Selection energy if found)
    double _x_gamma_Reco, _y_gamma_Reco, _z_gamma_Reco; /// lepton x,y,z start point
    double _px_gamma_Reco, _py_gamma_Reco, _pz_gamma_Reco; /// lepton x,y,z momentum
    double _theta_gamma_Reco; /// lepton angle w.r.t. beam direction (from reco. i.e. Pi0Selection if found)
    double _phi_gamma_Reco;   /// lepton angle w.r.t. x on xy plane (from reco. i.e. Pi0Selection if found)
    double _gamma_score_Reco ;

    double _mom_dot;  /// Dot product between direction vector of MC Pi0Selection shower and Reco Pi0Selection shower
    double _vtx_dist;
    // value of efficiency measured at the end
    double _eff;



    /// counters to cout to screen after running
    int _numEvts;
    int _singlePi0_ctr;

    AlgoFindRelationship _findRel;
   
  };
}
#endif

/** @} */ // end of doxygen group 
