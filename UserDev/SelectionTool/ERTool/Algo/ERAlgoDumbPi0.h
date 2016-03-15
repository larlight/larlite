/**
 * \file ERAlgoDumbPi0.h
 *
 * \ingroup Algo
 * 
 * \brief Class def header for a class ERAlgoDumbPi0
 *
 * @author ryan
 */

/** \addtogroup Algo

    @{*/

#ifndef ERTOOL_ERALGODUMBPI0_H
#define ERTOOL_ERALGODUMBPI0_H

#include "ERTool/Base/AlgoBase.h"

namespace ertool {

  /**
     \class ERAlgoDumbPi0
     User custom Algorithm class made by kazuhiro
   */
  class ERAlgoDumbPi0 : public AlgoBase {
  
  public:

    /// Default constructor
    ERAlgoDumbPi0(const std::string& name="ERAlgoDumbPi0");

    /// Default destructor
    virtual ~ERAlgoDumbPi0(){};

    /// Reset function
    void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    bool Reconstruct(const EventData &data, ParticleGraph& graph);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);

  };
}
#endif

/** @} */ // end of doxygen group 
