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

  private:

    bool _debug;


    // Result tree comparison for reconstructed events
    TTree* _result_tree;

    //Number of reconstructed *particles* in the output PSet
    //(this is 1-to-1 with number of single electrons reconstructed)
    int _n_singleReco;
    //Total number of MC electron/positron particles in the event
    int _n_electrons;
    //Total number of MC gamma particles in the event
    int _n_gammas;
    int _n_showers;    ///Number of mc *showers*
    int _n_showersReco;/// Number of reconstructed *showers* in the event data (could be e or g, etc)
    int _n_tracks;     /// Number of mc *tracks*
    int _n_tracksReco; /// Number of reconstructed *tracks* in the event data
    int _n_protons;    /// number of protons ( E > 20 MeV)
    int _n_neutrons;   /// number of neutrons (E > 20 MeV)
    int _n_piplus;     /// number of pi+ (E > 20 MeV)
    int _n_pi0;        /// number of pi0 (E > 20 MeV)
    /// Whether this event was MID'd (if == 0: we found ==1 single electron)
    /// Note: this is 0 if _E is filled, otherwise this is 1 (redundant)
    int    _misID;
    double _e_nu;      /// Neutrino energy
    int _pdg_nu;       /// Neutrino PDG
    double _e_lep;     /// lepton from neutrino Energy
    int _pdg_lep;      /// lepton from neutrino Pdg
    double _theta_lep; /// lepton angle w.r.t. beam direction
    double _phi_lep;   /// lepton angle w.r.t. x on xy plane
    double _e_lepReco; /// lepton E from reconstruction (i.e. SingleE energy if found)
    double _theta_lepReco; /// lepton angle w.r.t. beam direction (from reco. i.e. SingleE if found)
    double _phi_lepReco;   /// lepton angle w.r.t. x on xy plane (from reco. i.e. SingleE if found)



    /// counters to cout to screen after running
    int _numEvts;
    int _singleE_ctr;

  };
}
#endif

/** @} */ // end of doxygen group 
