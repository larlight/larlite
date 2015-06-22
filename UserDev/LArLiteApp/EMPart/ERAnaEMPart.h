/**
 * \file ERAnaEMPart.h
 *
 * \ingroup CCSingleE
 * 
 * \brief Class def header for a class ERAnaEMPart
 *
 * @author david
 */

/** \addtogroup CCSingleE

    @{*/

#ifndef ERTOOL_ERANAEMPART_H
#define ERTOOL_ERANAEMPART_H

#include "ERTool/Base/AnaBase.h"
#include "ERTool/Base/EventData.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2F.h"
#include "GeoAlgo/GeoAlgo.h"
#include "ERTool/Algo/AlgoFindRelationship.h"
#include "DataFormat/mctruth.h"

namespace ertool {

  /**
     \class ERAnaEMPart
     User custom Analysis class made by david caratelli
   */
  class ERAnaEMPart : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaEMPart(const std::string& name="ERAnaEMPart");

    /// Default destructor
    virtual ~ERAnaEMPart(){};

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

  private:

    // debug flag
    bool _debug;

    // Result tree comparison for reconstructed events
    TTree* _result_tree;
    double _dist_part;  ///< Distance between Reco & MC Part Vtx()
    double _dist_shrs;  ///< Distance between Reco & MC Shr Start()
    double _dedx;       ///< dEdx of reco shower
    double _e_reco;     ///< Energy of reco shower
    double _e_mc;       ///< energy of MC shower
    double _dot;        ///< dot-prod. of reco & mc part directions
    int    _pdg_reco;   ///< PDG of reconstructed particle
    int    _pdg_mc;     ///< PDG of MC particle

    /// counters to cout to screen after running
    int _numEvts;

    ::geoalgo::GeoAlgo _geoAlgo;
    ::geoalgo::AABox fTPC;
   
  };
}
#endif

/** @} */ // end of doxygen group 
