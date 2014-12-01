/**
 * \file SPFilterBase.h
 *
 * \ingroup ShowerPdfTool
 * 
 * \brief Class def header for a class SPFilterBase
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerPdfTool

    @{*/
#ifndef SELECTIONTOOL_SPFILTERBASE_H
#define SELECTIONTOOL_SPFILTERBASE_H

#include <iostream>
#include "DataFormat/shower.h"
#include "SPTBase.h"
#include "SPAData.h"

namespace sptool {
  /**
     \class SPFilterBase
     @brief Base class for Shower Pdf Filter (SPFilter)
     This is an abstract base for simple filter classes (SPFilterX) to fit in the framework.
     SPFilterX children classes should implement SPFilterBase::Select function which is used
     by sptool::SPAManager to exclude some showers from candidates list to be fed into SPAlgoX.
     There are 2 points to have this filter:

     0) Computation time reduction ... As the sptool::SPAManager makes all possible combination \n
     of showers from framework input and loops over to run SPAlgoX, reducing the candidate list \n
     is useful to reduce computation time if possible.

     1) Simple cut on individual shower ... Not all aspects of selection needs to be done via PDF \n
     but by simple cuts in this filter. This way, one can reduce some basic (and annoying) cuts \n
     like "minimum energy 10 MeV" from SPAlgoX's implementation of sptool::SPAlgoBase.

  */
  class SPFilterBase : public SPTBase {
    
  public:
    
    /// Default constructor
    SPFilterBase(){};
    
    /// Default destructor
    virtual ~SPFilterBase(){};

    /// Select function
    virtual bool Select (const SPAData &data) = 0;
    
  };
}

#endif
/** @} */ // end of doxygen group 

