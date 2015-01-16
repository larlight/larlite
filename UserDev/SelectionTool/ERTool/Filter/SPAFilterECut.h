/**
 * \file SPAFilterECut.h
 *
 * \ingroup SPAFilter
 * 
 * \brief Class def header for a class SPAFilterECut
 *
 * @author david
 */

/** \addtogroup SPAFilter

    @{*/

#ifndef SELECTIONTOOL_SPAFILTERECUT_H
#define SELECTIONTOOL_SPAFILTERECUT_H

#include "SPTBase/SPAFilterBase.h"

namespace sptool {

  /**
     \class SPAFilterECut
     User custom SPAFilter class made by david
   */
  class SPAFilterECut : public SPAFilterBase {
  
  public:

    /// Default constructor
    SPAFilterECut();

    /// Default destructor
    virtual ~SPAFilterECut(){};

    /// Select function
    virtual SPAOrder Select (const SPAData &data);

    /// Set Energy Cut for this Filter
    void SetECut(double ecut) { _ECut = ecut; }

  private:
    
    // Energy cut value
    double _ECut;

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
