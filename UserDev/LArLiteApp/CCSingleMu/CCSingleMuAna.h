/**
 * \file CCSingleMuAna.h
 *
 * \ingroup MyAna
 * 
 * \brief Class def header for a class CCSingleMuAna
 *
 * @author bcarls
 */

/** \addtogroup MyAna

    @{*/

#ifndef LARLITE_MYANA_CCSingleMuAna_H
#define LARLITE_MYANA_CCSingleMuAna_H

#include "Analysis/ana_base.h"
#include <TH1D.h>
namespace larlite {
  /**
     \class CCSingleMuAna
     User custom analysis class made by bcarls
   */
  class CCSingleMuAna : public ana_base{
  
  public:

    /// Default constructor
    CCSingleMuAna() : ana_base() { _name="CCSingleMuAna"; }

    /// Default destructor
    virtual ~CCSingleMuAna(){};

    /** IMPLEMENT in CCSingleMuAna.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in CCSingleMuAna.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in CCSingleMuAna.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:

    std::string fTrackProducer;


    TH1D* h;
    TH1D* hTrackCosmicScore;
    TH1D* hLongestTrackCosmicScore;

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
