/**
 * \file SPAlgo_Class_Name.h
 *
 * \ingroup Working_Package
 * 
 * \brief Class def header for a class SPAlgo_Class_Name
 *
 * @author kazuhiro
 */

/** \addtogroup Working_Package

    @{*/

#ifndef SELECTIONTOOL_SPALGO_CLASS_NAME_H
#define SELECTIONTOOL_SPALGO_CLASS_NAME_H

#include "SPAlgoBase.h"

namespace sptool {

  /**
     \class SPAlgo_Class_Name
     User custom SPAFilter class made by kazuhiro
   */
  class SPAlgo_Class_Name : public SPAlgoBase {
  
  public:

    /// Default constructor
    SPAlgo_Class_Name();

    /// Default destructor
    virtual ~SPAlgo_Class_Name(){};

    /// Function to evaluate input showers and determine a score
    virtual float Select(const std::vector<const larlite::shower*> &data);

    /// Fill true shower combination for sample fit
    virtual void Fill(const std::vector<const larlite::shower*> &data);

    /// Fill true shower combination for sample fit
    virtual void Fill(const std::vector<const larlite::mcshower*> &data);

    /// Evaluate parameters
    virtual void Fit();

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
