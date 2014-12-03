/**
 * \file SPAlgoBase.h
 *
 * \ingroup ShowerPdfTool
 * 
 * \brief Class def header for a class SPAlgoBase
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerPdfTool

    @{*/
#ifndef LARLITE_SPALGOBASE_H
#define LARLITE_SPALGOBASE_H

#include <iostream>
#include "DataFormat/shower.h"
#include "DataFormat/mcshower.h"
#include "SPTBase.h"
#include "SPAData.h"

namespace sptool {
  /**
     \class SPAlgoBase
     @brief Base class for Shower Pdf Algorithms (SPAlgo).
     This abstract base class introduces multiple common functions for children classes (SPAlgoX)
     to fit in the framework. 

     All SPAlgoX classes need to implement SPAlgoBase::Select function which quantifies a likelihood 
     of a given shower combination to fit in a specific interaction patterns (e.g. 2 showers for Pi0). 
     The input argument to this function is an array of N showers where N is specified via public attribute 
     SPAlgoBase::_nshowers. 

     Naturally this algorithm should be able to compute likelihood parameters with suitable data sample. 
     Each chunk of such data sample can be provided by SPAlgoBase::Fill function call which takes an array
     of sptool::SPAData data holder. Finally SPAlgoBase::Fit function executes necessary procedure to extract 
     parameter values.
     
     As the class inherits from sptool::SPTBase, it has a systemtaic procedure to store/load parameter
     sets (variable name & values) from larlite::user_info.
 */
  class SPAlgoBase : public SPTBase {

  public:
    
    /// Default constructor
    SPAlgoBase(size_t nshowers=1) : _nshowers(nshowers) { _name = "SPAlgoBase"; }
    
    /// Default destructor
    virtual ~SPAlgoBase(){}

    /// Function to evaluate input showers and determine a score
    virtual float Select(const SPAData &data) = 0;

    /// Provide data sample for fitting
    virtual void Fill(const SPAData &data) = 0;

    /// Evaluate parameters
    virtual void Fit(TFile* fout=nullptr) = 0;

    /// # of showers to operate on
    size_t _nshowers;

  };
}

#endif
/** @} */ // end of doxygen group 

