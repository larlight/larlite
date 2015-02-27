/**
 * \file AlgoMichelE.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class AlgoMichelE
 *
 * @author davidkaleko
 */

/** \addtogroup ERTool

    @{*/

#ifndef ERTOOL_ALGOMICHELE_H
#define ERTOOL_ALGOMICHELE_H

#include "ERTool/Algo/AlgoEMPart.h"
#include "ERTool/Base/AlgoBase.h"

namespace ertool {

  /**
     \class AlgoMichelE
     User custom SPAFilter class made by davidkaleko
   */
  class AlgoMichelE : public AlgoBase {
  
  public:

    /// Default constructor
    AlgoMichelE();

    /// Default destructor
    virtual ~AlgoMichelE(){};

    /// Reset function
    virtual void Finalize();

    /// What to do before event-loop begins
    virtual void ProcessBegin();

    /// What to do once event-loop is over
    virtual void ProcessEnd(TFile* fout);

    void InitializeHistos();

    /// Override the ertool::SPTBase::LoadParams function
    virtual void LoadParams(std::string fname="",size_t version=kINVALID_SIZE);

    /// Function to evaluate input showers and determine a score
    virtual ParticleSet Reconstruct(const EventData &data);
    
    
  protected:
    
    AlgoEMPart _alg_emp;

    TH1F* michel_energy;

  };
}
#endif

/** @} */ // end of doxygen group 
