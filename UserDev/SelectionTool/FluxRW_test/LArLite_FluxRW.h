/**
 * \file LArLite_FluxRW.h
 *
 * \ingroup FluxRW_test
 * 
 * \brief Class def header for a class LArLite_FluxRW
 *
 * @author Joseph Zennamo, jzennamo@uchicago.edu, Skype: joseph.zennamo.work
 */

/** \addtogroup FluxRW_test

    @{*/

#ifndef LARLITE_LARLITE_FLUXRW_H
#define LARLITE_LARLITE_FLUXRW_H

#include "Analysis/ana_base.h"
#include "DataFormat/mctruth.h"
#include "DataFormat/mcflux.h"
#include "../../LArLiteApp/fluxRW/fluxRW.h"

namespace larlite {
  /**
     \class LArLite_FluxRW
     User custom analysis class made by SHELL_USER_NAME
   */
  class LArLite_FluxRW : public ana_base{
  
  public:

    /// Default constructor
    LArLite_FluxRW(){ _name="LArLite_FluxRW"; _fout=0;}

    /// Default destructor
    virtual ~LArLite_FluxRW(){}

    /** IMPLEMENT in LArLite_FluxRW.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in LArLite_FluxRW.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in LArLite_FluxRW.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    ::fluxRW _fluxRW;

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
