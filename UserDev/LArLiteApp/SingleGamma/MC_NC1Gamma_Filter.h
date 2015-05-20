/**
 * \file MC_NC1Gamma_Filter.h
 *
 * \ingroup SingleGamma
 * 
 * \brief Class def header for a class MC_NC1Gamma_Filter
 *
 * @author wforeman
 */

/** \addtogroup SingleGamma

    @{*/

#ifndef LARLITE_MC_NC1GAMMA_FILTER_H
#define LARLITE_MC_NC1GAMMA_FILTER_H

#include "Analysis/ana_base.h"
#include "DataFormat/mctruth.h"

namespace larlite {
  /**
     \class MC_NC1Gamma_Filter
     User custom analysis class made by SHELL_USER_NAME
   */
  class MC_NC1Gamma_Filter : public ana_base{
  
  public:

    int max_events;

    size_t looped_events;
		
    /// Default constructor
    MC_NC1Gamma_Filter(){ _name="MC_NC1Gamma_Filter"; _fout=0;}

    /// Default destructor
    virtual ~MC_NC1Gamma_Filter(){}

    /** IMPLEMENT in MC_NC1Gamma_Filter.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MC_NC1Gamma_Filter.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    void setMaxNEvents(int N) { max_events = N; }
    
    void flip(bool on) {_flip = on;}
		
    

    /** IMPLEMENT in MC_NC1Gamma_Filter.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:

    size_t total_events;
    size_t kept_events;
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
