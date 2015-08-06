/**
 * \file ERAnaNGamma.h
 *
 * \ingroup SingleGamma
 * 
 * \brief Class def header for a class ERAnaNGamma
 *
 * @author ber9
 */

/** \addtogroup SingleGamma

    @{*/

#ifndef ERTOOL_ERANANGAMMA_H
#define ERTOOL_ERANANGAMMA_H

#include "ERTool/Base/AnaBase.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2F.h"
#include "GeoAlgo/GeoAlgo.h"
#include "ERTool/Algo/AlgoFindRelationship.h"
#include "DataFormat/mctruth.h"

namespace ertool {

  /**
     \class ERAnaNGamma
     User custom Analysis class made by kazuhiro
   */
  class ERAnaNGamma : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaNGamma(const std::string& name="ERAnaNGamma");

    /// Default destructor
    virtual ~ERAnaNGamma(){}

    /// Reset function
    virtual void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    bool Analyze(const EventData &data, const ParticleGraph &ps);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);

    void SetDebug(bool on) {_debug = on;}

    /// Function to save efficiency vs. variable to histogram
    void MakeEffPlot(std::string varname,
		     int nbins,
		     double xmin,
		     double xmax);

    /// Get reported efficiency
    double getEfficiency() {return _eff;}

    /// Set the energy cut to be used when counting particles
    void SetECut(double c) {_eCut = c;}

  private:

    /// Debug flag
    bool _debug;

    /// Energy cut for counting particles
    float _eCut;

    /// Measured efficiency
    double _eff;
    
    /// Result tree comparison for reconstructed events
    TTree* _result_tree;

    /// Total energy deposited in detector by tracks/showers
    double _EDep;

    /// number of MC tracks
    int _n_tracks;

    /// number of MC showers
    int _n_showers;
    ///
    /// Counters to print to screen after running to completion
    int _numEvts;
    int _N_gamma_ctr;

    /// Neutrino energy
    double _e_nu;

    /// Gamma energy
    double _e_gamma;
    
    ::geoalgo::GeoAlgo _geoAlgo;
    ::geoalgo::AABox fTPC;
    AlgoFindRelationship _findRel;

    /// Prepare TTree with variables
    void PrepareTreeVariables();

    /// Re-set TTree variables
    void ResetTreeVariables();
  }; // <-- class ERAnaNGamma
} // <-- namespace
#endif

/** @} */ // end of doxygen group 
