/**
 * \file ERAnaMichelE.h
 *
 * \ingroup CCSingleE
 * 
 * \brief Class def header for a class ERAnaMichelE
 *
 * @author david
 */

/** \addtogroup CCSingleE

    @{*/

#ifndef ERTOOL_ERANAMICHELE_H
#define ERTOOL_ERANAMICHELE_H

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
     \class ERAnaMichelE
     User custom Analysis class made by david caratelli
   */
  class ERAnaMichelE : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaMichelE(const std::string& name="ERAnaMichelE");

    /// Default destructor
    virtual ~ERAnaMichelE(){};

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
    double _dedx;       ///< dEdx of michel
    double _energy;     ///< Energy of michel
    int    _pdg;        ///< PDG of michel

    /// counters to cout to screen after running
    int _numEvts;

  };
}
#endif

/** @} */ // end of doxygen group 
