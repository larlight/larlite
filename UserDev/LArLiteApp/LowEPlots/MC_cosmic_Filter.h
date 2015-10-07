/**
 * \file MC_cosmic_Filter.h
 *
 * \ingroup LowEPlots
 * 
 * \brief Class def header for a class MC_cosmic_Filter
 *
 * @author jzennamo
 */

/** \addtogroup LowEPlots

    @{*/

#ifndef LARLITE_MC_COSMIC_FILTER_H
#define LARLITE_MC_COSMIC_FILTER_H

#include "Analysis/ana_base.h"
#include "DataFormat/mctruth.h"

namespace larlite {
  /**
     \class MC_cosmic_Filter
     User custom analysis class made by SHELL_USER_NAME
   */
  class MC_cosmic_Filter : public ana_base{
  
  public:

    /// Default constructor
    MC_cosmic_Filter(){ _name="MC_cosmic_Filter"; _fout=0;}

    /// Default destructor
    virtual ~MC_cosmic_Filter(){}

    /** IMPLEMENT in MC_cosmic_Filter.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MC_cosmic_Filter.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in MC_cosmic_Filter.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void flip(bool on) { _flip = on; }
    
  protected:
    
    // boolean to flip logical operation of algorithm
    bool _flip;
    
    
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
