/**
 * \file SPAUnitBase.h
 *
 * \ingroup ShowerPdfTool
 * 
 * \brief Class def header for a class SPAUnitBase
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerPdfTool

    @{*/
#ifndef LARLITE_SPAUNITBASE_H
#define LARLITE_SPAUNITBASE_H

#include <iostream>
#include "SPTBase.h"
#include "SPAData.h"
#include "SPArticle.h"

namespace sptool {

  /**
     \class SPAUnitBase
     @brief Base class for sptool::SPAlgoBase and sptool::SPAFilterBase
     This abstract base class introduces multiple common functions for children classes \n
     (sptool::SPAlgoBase and sptool::SPAFilterBase) to fit in the SPAManager fmwk.      \n

     As the class inherits from sptool::SPTBase, it has a systemtaic procedure to store/load parameter \n
     sets (variable name & values) from larlite::user_info.
 */
  class SPAUnitBase : public SPTBase {

  public:
    
    /// Default constructor
    SPAUnitBase() : SPTBase() { _name = "SPAUnitBase"; _training_mode = false; }
    
    /// Default destructor
    virtual ~SPAUnitBase(){}
    
    /// Function to reset itself
    virtual void Reset(){}

    /// Called @ before processing the first event sample
    virtual void ProcessBegin(){}

    /// Called @ beginning of each event
    virtual void EventBegin(){}

    /// Called @ end of each event
    virtual void EventEnd(){}

    /// Called after processing the last event sample
    virtual void ProcessEnd(TFile* fout=nullptr) {}

    /// Boolean to set whether the use is in the training mode or not
    bool _training_mode;

  };
}

#endif
/** @} */ // end of doxygen group 

