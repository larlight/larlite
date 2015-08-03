/**
 * \file NP_Filter.h
 *
 * \ingroup CCQESelection
 * 
 * \brief Class def header for a class NP_Filter
 *
 * @author davidkaleko
 */

/** \addtogroup CCQESelection

    @{*/

#ifndef LARLITE_NP_FILTER_H
#define LARLITE_NP_FILTER_H

#include "Analysis/ana_base.h"
#include "DataFormat/mctruth.h"
namespace larlite {
  /**
     \class NP_Filter
     User custom analysis class made by davidkaleko
   */
  class NP_Filter : public ana_base{
  
  public:

    /// Default constructor
    NP_Filter(){ _name="NP_Filter"; _fout=0; };

    /// Default destructor
    virtual ~NP_Filter(){};

    /** IMPLEMENT in NP_Filter.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in NP_Filter.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in NP_Filter.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    protected:

    
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
