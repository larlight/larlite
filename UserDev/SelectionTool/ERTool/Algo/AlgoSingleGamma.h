/**
 * \file AlgoSingleGamma.h
 *
 * \ingroup Algo
 * 
 * \brief Class def header for a class AlgoSingleGamma
 *
 * @author jzennamo
 */

/** \addtogroup Algo

    @{*/

#ifndef ERTOOL_ALGOSINGLEGAMMA_H
#define ERTOOL_ALGOSINGLEGAMMA_H

#include "ERTool/Base/AlgoBase.h"

namespace ertool {

  /**
     \class AlgoSingleGamma
     User custom Algorithm class made by kazuhiro
   */
  class AlgoSingleGamma : public AlgoBase {
  
  public:

    /// Default constructor
    AlgoSingleGamma();

    /// Default destructor
    virtual ~AlgoSingleGamma(){};

    /// Reset function
    virtual void Reset();

    /// Function to evaluate input showers and determine a score
    virtual ParticleSet Reconstruct(const EventData &data);

  };
}
#endif

/** @} */ // end of doxygen group 
