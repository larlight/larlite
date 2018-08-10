/**
 * \file MichelElectronFilter.h
 *
 * \ingroup EventFilters
 * 
 * \brief Class def header for a class MichelElectronFilter
 *
 * @author davidkaleko
 */

/** \addtogroup EventFilters

    @{*/

#ifndef LARLITE_MICHELELECTRONFILTER_H
#define LARLITE_MICHELELECTRONFILTER_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class MichelElectronFilter
     User custom analysis class made by davidkaleko
   */
  class MichelElectronFilter : public ana_base{
  
  public:

    /// Default constructor
    MichelElectronFilter(){ _name="MichelElectronFilter"; _fout=0;};

    /// Default destructor
    virtual ~MichelElectronFilter(){};

    /** IMPLEMENT in MichelElectronFilter.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MichelElectronFilter.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in MichelElectronFilter.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    protected:

    size_t total_evts;
    size_t kept_evts;

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
