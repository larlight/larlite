/**
 * \file ERAnaSingleE.h
 *
 * \ingroup CCSingleE
 * 
 * \brief Class def header for a class ERAnaSingleE
 *
 * @author david
 */

/** \addtogroup CCSingleE

    @{*/

#ifndef ERTOOL_ERANASINGLEE_H
#define ERTOOL_ERANASINGLEE_H

#include "ERTool/Base/AnaBase.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2F.h"
#include "GeoAlgo/GeoAlgo.h"
#include "GeoAlgo/GeoAABox.h"
#include "DataFormat/mctruth.h"

namespace ertool {

  /**
     \class ERAnaSingleE
     User custom Analysis class made by kazuhiro
   */
  class ERAnaSingleE : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaSingleE();

    /// Default destructor
    virtual ~ERAnaSingleE(){};

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
    TTree* _result_tree;

    //Number of reconstructed *particles* in the output PSet
    //(this is 1-to-1 with number of single electrons reconstructed)
    int _n_singleReco;
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
    double _e_nu, _e_nuReco;     /// Neutrino energy
    double _x_nu, _y_nu, _z_nu; /// Neutrino position
    double _x_nuReco, _y_nuReco, _z_nuReco; /// Neutrino position
    double _px_nu, _py_nu, _pz_nu; /// Neutrino momentum
    double _px_nuReco, _py_nuReco, _pz_nuReco; /// Neutrino position
    int _pdg_nu;       /// Neutrino PDG
    double _e_lep;     /// lepton from neutrino Energy
    double _x_lep, _y_lep, _z_lep; /// lepton x,y,z start point
    double _px_lep, _py_lep, _pz_lep; /// lepton x,y,z momentum point
    int _pdg_lep;      /// lepton from neutrino Pdg
    double _theta_lep; /// lepton angle w.r.t. beam direction
    double _phi_lep;   /// lepton angle w.r.t. x on xy plane
    double _e_lepReco; /// lepton E from reconstruction (i.e. SingleE energy if found)
    double _showerlength_lepReco; /// length of the reconstructed shower associated with the singleE electron (if found)
    double _x_lepReco, _y_lepReco, _z_lepReco; /// lepton x,y,z start point
    double _px_lepReco, _py_lepReco, _pz_lepReco; /// lepton x,y,z momentum
    double _px_lepNormReco, _py_lepNormReco, _pz_lepNormReco; /// lepton x,y,z momentum
    double _angle_Norm ;
    double _angle_PxPy ;
    double _theta_lepReco; /// lepton angle w.r.t. beam direction (from reco. i.e. SingleE if found)
    double _phi_lepReco;   /// lepton angle w.r.t. x on xy plane (from reco. i.e. SingleE if found)
    double _lep_dot;  /// Dot product between direction vector of MC SingleE shower and Reco SingleE shower
    double _lep_vtxdist; /// Distance [cm] between MC and Reco SingleE showers
    double _distToTopWall;
    double _distBackAlongTraj ;
    
    // value of efficiency measured at the end
    double _eff;


    /// counters to cout to screen after running
    int _numEvts;
    int _singleE_ctr;

::geoalgo::GeoAlgo _geoAlgo;
::geoalgo::AABox fTPC;

    TH2F* _h_e_nu_correlation;
  };
}
#endif

/** @} */ // end of doxygen group 
