/**
 * \file Filter_Class_Name.h
 *
 * \ingroup Working_Package
 * 
 * \brief Class def header for a class Filter_Class_Name
 *
 * @author kazuhiro
 */

/** \addtogroup Working_Package

    @{*/

#ifndef SELECTIONTOOL_FILTER_CLASS_NAME_H
#define SELECTIONTOOL_FILTER_CLASS_NAME_H

#include "SPTBase/SPFilterBase.h"

namespace sptool {

  /**
     \class Filter_Class_Name
     User custom SPAFilter class made by kazuhiro
   */
  class Filter_Class_Name : public SPFilterBase {
  
  public:

    /// Default constructor
    Filter_Class_Name();

    /// Default destructor
    virtual ~Filter_Class_Name(){};

    /// Select function
    virtual SPAOrder Select (const SPAData &data);

    /// Fill function
    virtual void Fill (const SPAData& data);

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
