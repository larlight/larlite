/**
 * \file ERAnaPi0Selection.h
 *
 * \ingroup Pi0Ana
 * 
 * \brief Class def header for a class ERAnaPi0Selection
 *
 * @author david, ariana
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

    // Result tree comparison for reconstructed events
    TTree* _pi0_tree;

    // total energy deposited in detector by tracks & showers
    double _EDep;
    //Number of reconstructed *particles* in the output PSet
    //(this is 1-to-1 with number of single electrons reconstructed)
    int _n_psReco;
    int _n_electrons;  ///Total number of MC electron/positron particles in the event
    int _n_gammas;     ///Total number of MC gamma particles in the event
    int _n_showers;    ///Number of mc *showers*
    int _n_showersReco;/// Number of reconstructed *showers* in the event data (could be e or g, etc)
    int _n_tracks;     /// Number of mc *tracks*
    int _n_tracksReco; /// Number of reconstructed *tracks* in the event data
    int _n_tracksInt;     /// Number of mc *tracks* from interaction
    int _n_tracksIntReco; /// Number of reconstructed *tracks* from interaction in the return ParticleSet
    int _n_protons;    /// number of protons ( E > 20 MeV)
    int _n_neutrons;   /// number of neutrons (E > 20 MeV)
    int _n_piplus;     /// number of pi+ (E > 20 MeV)
    int _n_pi0;        /// number of pi0 (E > 20 MeV)
    int    _misID;     /// Whether this event was MID'd (if == 0: we found == 1 single electron)
    double _e_trkInt, _e_trkIntReco; /// Energy of tracks @ neutrino vertex
    double _e_neutrals; /// Energy of all neutral particles
    double _e_nucleus_diff; /// Energy of final nucleus minus initial Ar nucleus
    double _e_pi0MC, _e_pi0Reco;     /// Neutrino energy
    double _x_pi0MC, _y_pi0MC, _z_pi0MC; /// Neutrino position
    double _x_pi0Reco, _y_pi0Reco, _z_pi0Reco; /// Neutrino position
    double _px_pi0MC, _py_pi0MC, _pz_pi0MC; /// Neutrino momentum
    double _px_pi0Reco, _py_pi0Reco, _pz_pi0Reco; /// Neutrino position
    int _pdg_pi0MC;       /// Neutrino PDG
    double _e_pi0d_MC;     /// pi0d_MC from neutrino Energy
    double _x_pi0d_MC, _y_pi0d_MC, _z_pi0d_MC; /// pi0d_MC x,y,z start point
    double _px_pi0d_MC, _py_pi0d_MC, _pz_pi0d_MC; /// pi0d_MC x,y,z momentum point
    int _pdg_pi0d_MC;      /// pi0d_MC from neutrino Pdg
    double _theta_pi0d_MC; /// pi0d_MC angle w.r.t. beam direction
    double _phi_pi0d_MC;   /// pi0d_MC angle w.r.t. x on xy plane
    double _e_pi0dReco; /// lepton E from reconstruction (i.e. Pi0Selection energy if found)
    double _showerlength_pi0dReco; /// length of the reconstructed shower associated with the singleE electron (if found)
    double _x_pi0dReco, _y_pi0dReco, _z_pi0dReco; /// lepton x,y,z start point
    double _px_pi0dReco, _py_pi0dReco, _pz_pi0dReco; /// lepton x,y,z momentum
    double _px_lepNormReco, _py_lepNormReco, _pz_lepNormReco; /// lepton x,y,z momentum
    double _theta_pi0dReco; /// lepton angle w.r.t. beam direction (from reco. i.e. Pi0Selection if found)
    double _phi_pi0dReco;   /// lepton angle w.r.t. x on xy plane (from reco. i.e. Pi0Selection if found)
    double _lep_dot;  /// Dot product between direction vector of MC Pi0Selection shower and Reco Pi0Selection shower
    double _lep_vtxdist; /// Distance [cm] between MC and Reco Pi0Selection showers
    double _distToTopWall;
    double _distBackAlongTraj ;
    double _distToWall; 
    // value of efficiency measured at the end
    double _eff;


    /// counters to cout to screen after running
    int _numEvts;
    int _singlePi0_ctr;

    ::geoalgo::GeoAlgo _geoAlgo;
    ::geoalgo::AABox fTPC;
    AlgoFindRelationship _findRel;

    TH2F* _h_e_nu_correlation;
   
  };
}
#endif

/** @} */ // end of doxygen group 
