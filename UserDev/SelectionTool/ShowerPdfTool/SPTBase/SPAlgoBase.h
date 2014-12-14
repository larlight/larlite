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
#include "SPArticle.h"

namespace sptool {

  /**
     \class SPAlgoBase
     @brief Base class for Shower Pdf Algorithms (SPAlgo).
     This abstract base class introduces multiple common functions for children classes (SPAlgoX)
     to fit in the framework. 

     All SPAlgoX classes need to implement SPAlgoBase::Select function which reconstructs a     \n
     sptool::SPArticleSet object which is a set of sptool::SPArticle and is meant to represent  \n
     reconstructed particles. Given a muon track, one may simply populate one sptool::SPArticle \n
     of a muon type. Or, if you find 2 gamma showers that come from pi0, for instance, you may  \n
     fill SPArticleSet w/ a single pi0. Refer to sptool::SPArticle and sptool::SPArticleSet for \n
     details of how to store information.

     Naturally this algorithm should be able to compute likelihood parameters with suitable data    \n
     sample. Each chunk of such data sample can be provided by SPAlgoBase::Fill function call which \n
     takes a sptool::SPAData data holder. \n
     
     As the class inherits from sptool::SPTBase, it has a systemtaic procedure to store/load parameter \n
     sets (variable name & values) from larlite::user_info.
 */
  class SPAlgoBase : public SPTBase {

  public:
    
    /// Default constructor
    SPAlgoBase() { _name = "SPAlgoBase"; }
    
    /// Default destructor
    virtual ~SPAlgoBase(){}
    
    /// Function to evaluate input showers and determine a score
    virtual SPArticleSet Select(const SPAData &data) = 0;

    /// Function to fill data sample
    virtual void Fill(const SPAData &data) = 0;

    /// Function to reset itself
    virtual void Reset() = 0;

    /// Called @ before processing the first event sample
    virtual void ProcessBegin(){}

    /// Called @ beginning of each event
    virtual void EventBegin(){}

    /// Called @ end of each event
    virtual void EventEnd(){}

    /// Called after processing the last event sample
    virtual void ProcessEnd(TFile* fout=nullptr){}

  };
}

#endif
/** @} */ // end of doxygen group 

