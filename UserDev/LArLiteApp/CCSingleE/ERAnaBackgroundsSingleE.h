/**
 * \file ERAnaBackgroundsSingleE.h
 *
 * \ingroup CCSingleE
 * 
 * \brief ERAna specificially used in making stacked background plots
 *
 * @author kaleko
 */

/** \addtogroup CCSingleE

    @{*/

#ifndef ERTOOL_ERANABACKGROUNDSSINGLEE_H
#define ERTOOL_ERANABACKGROUNDSSINGLEE_H

#include "ERTool/Base/AnaBase.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2F.h"
#include "GeoAlgo/GeoAlgo.h"
#include "ERTool/Algo/AlgoFindRelationship.h"
#include "DataFormat/mctruth.h"

namespace ertool {

  /**
     \class ERAnaBackgroundsSingleE
     User custom Analysis class made by david caratelli
   */
  class ERAnaBackgroundsSingleE : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaBackgroundsSingleE(const std::string& name="ERAnaBackgroundsSingleE");

    /// Default destructor
    virtual ~ERAnaBackgroundsSingleE(){};

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

    /// set the energy cut to be used when counting particles
    void SetECut(double c) { _eCut = c; }

    /// Function to compute the neutrino energy
    /// (can implement different definitions here)
    double ComputeNuEnergy(const Particle &neutrino, const ParticleGraph &graph);

  private:

    // debug flag
    bool _debug;

    // energy cut for counting particles
    float _eCut;

    // Result tree comparison for reconstructed events
    // This is filled once per reconstructed neutrino
    TTree* _result_tree;

    double _e_nuReco;     /// Neutrino energy
    double _e_lepReco;    /// lepton from neutrino Energy
    double _distToWall;   /// perp dist to TPC wall (fid vol. relevant)

    /// counters to cout to screen after running
    int _numEvts;
    int _singleE_ctr;

    ::geoalgo::GeoAlgo _geoAlgo;
    ::geoalgo::AABox fTPC;
   
  };
}
#endif

/** @} */ // end of doxygen group 
