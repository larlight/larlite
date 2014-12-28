/**
 * \file SPAFilterBase.h
 *
 * \ingroup ShowerPdfTool
 * 
 * \brief Class def header for a class SPAFilterBase
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerPdfTool

    @{*/
#ifndef SELECTIONTOOL_SPAFILTERBASE_H
#define SELECTIONTOOL_SPAFILTERBASE_H

#include "SPAUnitBase.h"

namespace sptool {
  /**
     \class SPAFilterBase
     @brief Base class for Shower Pdf Filter (SPAFilter)
     This is an abstract base for simple filter classes (SPAFilterX) to fit in the framework.   \n
     SPAFilterX children classes should implement SPAFilterBase::Select function which is used  \n
     by sptool::SPAManager to exclude some showers from candidates list to be fed into SPAlgoX. \n
     There are 2 points to have this filter:

     0) Computation time reduction ... As the sptool::SPAManager makes all possible combination \n
     of showers from framework input and loops over to run SPAlgoX, reducing the candidate list \n
     is useful to reduce computation time if possible.                                          \n

     1) Simple cut on individual shower ... Not all aspects of selection needs to be done via PDF \n
     but by simple cuts in this filter. This way, one can reduce some basic (and annoying) cuts   \n
     like "minimum energy 10 MeV" from SPAlgoX's implementation of sptool::SPAlgoBase.            \n

  */
  class SPAFilterBase : public SPAUnitBase {
    
  public:
    
    /// Default constructor
    SPAFilterBase() : SPAUnitBase() 
    { _name = "SPAUnitBase"; _training_mode = false; }
    
    /// Default destructor
    virtual ~SPAFilterBase(){}

    /// Select function
    virtual SPAOrder Select (const SPAData &data){ return SPAOrder(); }

  };
}

#endif
/** @} */ // end of doxygen group 

