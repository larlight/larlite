/**
 * \file ERAnaLowEExcess.h
 *
 * \ingroup LowEPlots
 * 
 * \brief Class def header for a class ERAnaLowEExcess
 *
 * @author jzennamo
 */

/** \addtogroup LowEPlots

    @{*/

#ifndef ERTOOL_ERANALOWEEXCESS_H
#define ERTOOL_ERANALOWEEXCESS_H

#include "ERTool/Base/AnaBase.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2F.h"
#include <string>
#include "DataFormat/mctruth.h"
// #include "ERToolBackend/ParticleID.h"
#include "../../LArLiteApp/fluxRW/fluxRW.h"
#include "GeoAlgo/GeoAABox.h"
#include "LArUtil/Geometry.h"


namespace ertool {

  /**
     \class ERAnaLowEExcess
     User custom Analysis class made by kazuhiro
   */
  class ERAnaLowEExcess : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaLowEExcess(const std::string& name="ERAnaLowEExcess");

    /// Default destructor
    virtual ~ERAnaLowEExcess(){}

    /// Reset function
    virtual void Reset() {}

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg){}

    /// Called @ before processing the first event sample
    void ProcessBegin() {};

    /// Function to evaluate input showers and determine a score
    bool Analyze(const EventData &data, const ParticleGraph &ps);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout);

    /// setting result tree name for running the LowEExcess plotting code
    void SetTreeName(const std::string& name){ _treename = name; }

    /// set the energy cut to be used when counting particles
    void SetECut(double c) { _eCut = c; }

    geoalgo::AABox TPC;

    
  private:

    // Result tree comparison for reconstructed events
    TTree* _result_tree;
    std::string _treename;

    float _eCut;

    double _e_nuReco;     /// Neutrino energy
    double _e_dep;        /// Neutrino energy
    double _weight;
    int _numEvts;
    bool _is_fiducial;
    int _parentPDG; /// true PDG of parent of the electron (only for running on MC)
    int _mcPDG; /// true PDG of "single electron" (probably 11 or 22)
    double _longestTrackLen; /// longest track associated with the reconstructed neutrino
    double _x_vtx; /// Neutrino vertex point
    double _y_vtx;
    double _z_vtx;


    // prepare TTree with variables
    void PrepareTreeVariables();
    /// Function to re-set TTree variables
    void ResetTreeVariables();

    ::fluxRW _fluxRW;

    // ertool_helper::ParticleID singleE_particleID;
    ertool::Shower singleE_shower;
  };
}
#endif

/** @} */ // end of doxygen group 
