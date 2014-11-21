/**
 * \file CCQEShowerSel.h
 *
 * \ingroup CCQESelection
 * 
 * \brief Class def header for a class CCQEShowerSel
 *
 * @author kazuhiro
 */

/** \addtogroup CCQESelection

    @{*/

#ifndef LARLITE_CCQESHOWERSEL_H
#define LARLITE_CCQESHOWERSEL_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class CCQEShowerSel
     User custom analysis class made by kazuhiro
   */
  class CCQEShowerSel : public ana_base{
  
  public:

    /// Default constructor
    CCQEShowerSel(){ _name="CCQEShowerSel"; _fout=0;};

    /// Default destructor
    virtual ~CCQEShowerSel(){};

    /** IMPLEMENT in CCQEShowerSel.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in CCQEShowerSel.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in CCQEShowerSel.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:
    
    std::string fTrackProducer;
    std::string fShowerProducer;

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
