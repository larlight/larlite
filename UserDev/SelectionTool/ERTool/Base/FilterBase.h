/**
 * \file FilterBase.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class FilterBase
 *
 * @author kazuhiro
 */

/** \addtogroup ERTool

    @{*/
#ifndef ERTOOL_FILTERBASE_H
#define ERTOOL_FILTERBASE_H

#include "UnitBase.h"
#include "EventData.h"
namespace ertool {
  /**
     \class FilterBase
     @brief Base class for Shower Pdf Filter (Filter)
     This is an abstract base for simple filter classes (FilterX) to fit in the framework.   \n
     FilterX children classes should implement FilterBase::Select function which is used  \n
     by ertool::Manager to exclude some showers from candidates list to be fed into AlgoX. \n
     There are 2 points to have this filter:

     0) Computation time reduction ... As the ertool::Manager makes all possible combination \n
     of showers from framework input and loops over to run AlgoX, reducing the candidate list \n
     is useful to reduce computation time if possible.                                          \n

     1) Simple cut on individual shower ... Not all aspects of selection needs to be done via PDF \n
     but by simple cuts in this filter. This way, one can reduce some basic (and annoying) cuts   \n
     like "minimum energy 10 MeV" from AlgoX's implementation of ertool::AlgoBase.            \n

  */
  class FilterBase : public UnitBase {
    
  public:
    
    /// Default constructor
    FilterBase() : UnitBase() 
    { _name = "UnitBase"; _training_mode = false; }
    
    /// Default destructor
    virtual ~FilterBase(){}

    /// Select function
    //virtual SPAOrder Select (const EventData &data){ return SPAOrder(); }
    virtual void Filter(EventData& data) { return; }

  };
}

#endif
/** @} */ // end of doxygen group 

