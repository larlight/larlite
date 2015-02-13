/**
 * \file UnitBase.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class UnitBase
 *
 * @author kazuhiro
 */

/** \addtogroup ERTool

    @{*/
#ifndef ERTOOL_UNITBASE_H
#define ERTOOL_UNITBASE_H

#include <iostream>
#include "Record.h"

namespace ertool {

  /**
     \class UnitBase
     @brief Base class for ertool::AlgoBase and ertool::FilterBase
     This abstract base class introduces multiple common functions for children classes \n
     (ertool::AlgoBase and ertool::FilterBase) to fit in the ERTool fmwk.      \n

     As the class inherits from ertool::Record, it has a systemtaic procedure to store/load parameter \n
     sets (variable name & values) from larlite::user_info.
 */
  class UnitBase : public Record {

  public:
    
    /// Default constructor
    UnitBase() : Record() { _name = "UnitBase"; _training_mode = false; }
    
    /// Default destructor
    virtual ~UnitBase(){}
    
    /// Function to reset itself
    virtual void Reset(){}

    /// Called @ before processing the first event sample
    virtual void ProcessBegin(){}

    /// Called after processing the last event sample
    virtual void ProcessEnd(TFile* fout=nullptr) {}

    /// Boolean to set whether the use is in the training mode or not
    bool _training_mode;

  };
}

#endif
/** @} */ // end of doxygen group 

