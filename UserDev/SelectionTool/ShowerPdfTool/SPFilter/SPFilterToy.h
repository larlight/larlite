/**
 * \file SPFilterToy.h
 *
 * \ingroup SPFilter
 * 
 * \brief Class def header for a class SPFilterToy
 *
 * @author kazuhiro
 */

/** \addtogroup SPFilter

    @{*/

#ifndef SELECTIONTOOL_SPFILTERTOY_H
#define SELECTIONTOOL_SPFILTERTOY_H

#include "SPTBase/SPFilterBase.h"

namespace sptool {

  /**
     \class SPFilterToy
     User custom SPAFilter class made by kazuhiro
   */
  class SPFilterToy : public SPFilterBase {
  
  public:

    /// Default constructor
    SPFilterToy();

    /// Default destructor
    virtual ~SPFilterToy(){};

    /// Select function
    virtual SPAOrder Select (const SPAData &data);

    /// Fill function
    virtual void Fill(const SPAData &data) {}

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
