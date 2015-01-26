/**
 * \file AlgoCCSingleMu.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class AlgoCCSingleMu
 *
 * @author kazuhiro
 */

/** \addtogroup ERTool

    @{*/

#ifndef ERTOOL_ALGOCCSINGLEMU_H
#define ERTOOL_ALGOCCSINGLEMU_H

#include "Base/AlgoBase.h"

namespace ertool {

  /**
     \class AlgoCCSingleMu
     User custom Algorithm class made by kazuhiro
   */
  class AlgoCCSingleMu : public AlgoBase {
  
  public:

    /// Default constructor
    AlgoCCSingleMu();

    /// Default destructor
    virtual ~AlgoCCSingleMu(){};

    /// Reset function
    virtual void Reset();

    /// Function to evaluate input showers and determine a score
    virtual ParticleSet Reconstruct(const EventData &data);

  };
}
#endif

/** @} */ // end of doxygen group 
