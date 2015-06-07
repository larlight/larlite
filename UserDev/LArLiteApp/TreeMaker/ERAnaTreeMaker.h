/**
 * \file ERAnaTreeMaker.h
 *
 * \ingroup TreeMaker
 * 
 * \brief Class def header for a class ERAnaTreeMaker
 *
 * @author david
 */

/** \addtogroup TreeMaker

    @{*/

#ifndef ERTOOL_ERANATREEMAKER_H
#define ERTOOL_ERANATREEMAKER_H

#include "ERTool/Base/AnaBase.h"
#include "TTree.h"
#include "DataFormat/mctruth.h"

namespace ertool {

  /**
     \class ERAnaTreeMaker
     User custom Analysis class made by kazuhiro
   */
  class ERAnaTreeMaker : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaTreeMaker();

    /// Default destructor
    virtual ~ERAnaTreeMaker(){};

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
