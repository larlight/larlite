/**
 * \file MuCSGainAna.h
 *
 * \ingroup App
 * 
 * \brief Class def header for a class MuCSGainAna
 *
 * @author kazuhiro
 */

/** \addtogroup App

    @{*/

#ifndef LARLITE_MUCSGAINANA_H
#define LARLITE_MUCSGAINANA_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class MuCSGainAna
     User custom analysis class made by SHELL_USER_NAME
   */
  class MuCSGainAna : public ana_base{
  
  public:

    /// Default constructor
    MuCSGainAna(){ _name="MuCSGainAna"; _fout=0;}

    /// Default destructor
    virtual ~MuCSGainAna(){}

    /** IMPLEMENT in MuCSGainAna.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MuCSGainAna.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in MuCSGainAna.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void OpHitProducer(const std::string name) { _ophit_producer = name; }

  protected:

    std::string _ophit_producer;
    TTree* _tree;
    double _area;
    double _amp;
    double _pe;
    int    _ch;
    double _twidth;
    double _tpeak;
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
