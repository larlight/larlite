/**
 * \file SPAFilterToy.h
 *
 * \ingroup SPAFilter
 * 
 * \brief Class def header for a class SPAFilterToy
 *
 * @author kazuhiro
 */

/** \addtogroup SPAFilter

    @{*/

#ifndef SELECTIONTOOL_SPAFILTERTOY_H
#define SELECTIONTOOL_SPAFILTERTOY_H

#include "SPTBase/SPAFilterBase.h"

namespace sptool {

  /**
     \class SPAFilterToy
     User custom SPAFilter class made by kazuhiro
   */
  class SPAFilterToy : public SPAFilterBase {
  
  public:

    /// Default constructor
    SPAFilterToy();

    /// Default destructor
    virtual ~SPAFilterToy(){};

    /// Select function
    virtual SPAOrder Select (const SPAData &data);

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
