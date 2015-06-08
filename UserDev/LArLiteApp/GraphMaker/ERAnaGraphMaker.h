/**
 * \file ERAnaGraphMaker.h
 *
 * \ingroup GraphMaker
 * 
 * \brief Class def header for a class ERAnaGraphMaker
 *
 * @author david
 */

/** \addtogroup GraphMaker

    @{*/

#ifndef ERTOOL_ERANAGRAPHMAKER_H
#define ERTOOL_ERANAGRAPHMAKER_H

#include "ERTool/Base/AnaBase.h"
#include "TTree.h"
#include "DataFormat/mctruth.h"

namespace ertool {

  /**
     \class ERAnaGraphMaker
     User custom Analysis class made by kazuhiro
   */
  class ERAnaGraphMaker : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaGraphMaker();

    /// Default destructor
    virtual ~ERAnaGraphMaker(){};

    /// Reset function
    virtual void Reset();

    /// Function to evaluate input showers and determine a score
    virtual bool Analyze(const EventData &data, const ParticleGraph &graph);

    void SetDebug(bool on) { _debug = on; }

    void ProcessEnd(TFile* fout);

  private:

    // debug flag
    bool _debug;

    // Result tree comparison for reconstructed events
    TTree* _result_tree;

  };
}
#endif

/** @} */ // end of doxygen group 
