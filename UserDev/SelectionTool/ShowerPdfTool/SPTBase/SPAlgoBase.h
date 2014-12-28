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

#include "SPAUnitBase.h"

namespace sptool {

  /**
     \class SPAlgoBase
     @brief Base class for Shower Pdf Algorithms (SPAlgo).
     This abstract base class introduces multiple common functions for children classes (SPAlgoX)
     to fit in the framework. 

     All SPAlgoX classes need to implement SPAlgoBase::Reconstruct function which reconstructs a\n
     sptool::SPArticleSet object which is a set of sptool::SPArticle and is meant to represent  \n
     reconstructed particles. Given a muon track, one may simply populate one sptool::SPArticle \n
     of a muon type. Or, if you find 2 gamma showers that come from pi0, for instance, you may  \n
     fill SPArticleSet w/ a single pi0. Refer to sptool::SPArticle and sptool::SPArticleSet for \n
     details of how to store information.

     As the class inherits from sptool::SPTBase, it has a systemtaic procedure to store/load parameter \n
     sets (variable name & values) from larlite::user_info.
 */
  class SPAlgoBase : public SPAUnitBase {

  public:
    
    /// Default constructor
    SPAlgoBase() : SPAUnitBase() { _name = "SPAlgoBase"; _training_mode = false; }
    
    /// Default destructor
    virtual ~SPAlgoBase(){}
    
    /// Function to evaluate input showers and determine a score
    virtual SPArticleSet Reconstruct(const SPAData &data) {return SPArticleSet(); }

  };
}

#endif
/** @} */ // end of doxygen group 

