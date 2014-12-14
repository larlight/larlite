/**
 * \file SPAlgoPi0.h
 *
 * \ingroup ShowerPdfTool
 * 
 * \brief Class def header for a class SPAlgoPi0
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerPdfTool

    @{*/

#ifndef SELECTIONTOOL_SPALGOPI0_H
#define SELECTIONTOOL_SPALGOPI0_H

#include "SPTBase/SPAlgoBase.h"

namespace sptool {

  /**
     \class SPAlgoPi0
     User custom SPAFilter class made by kazuhiro
   */
  class SPAlgoPi0 : public SPAlgoBase {
  
  public:

    /// Default constructor
    SPAlgoPi0();

    /// Default destructor
    virtual ~SPAlgoPi0(){};

    /// Function to evaluate input showers and determine a score
    virtual SPArticleSet Select(const SPAData &data);

    /// Function to fill data sample
    virtual void Fill(const SPAData &data);

  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
