/**
 * \file WireGainAna.h
 *
 * \ingroup App
 * 
 * \brief Class def header for a class WireGainAna
 *
 * @author kazuhiro
 */

/** \addtogroup App

    @{*/

#ifndef LARLITE_WIREGAINANA_H
#define LARLITE_WIREGAINANA_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class WireGainAna
     User custom analysis class made by SHELL_USER_NAME
   */
  class WireGainAna : public ana_base{
  
  public:

    /// Default constructor
    WireGainAna(){ _name="WireGainAna"; _fout=0;}

    /// Default destructor
    virtual ~WireGainAna(){}

    /** IMPLEMENT in WireGainAna.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in WireGainAna.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in WireGainAna.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:

    TTree* _tree;
    std::vector<double> _tpc;
    std::vector<double> _pmt;
    double _sumpe_tpc;
    double _sumpe_pmt;
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
