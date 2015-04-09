/**
 * \file CCSingleMuFilter.h
 *
 * \ingroup CCSingleMu
 * 
 * \brief Class def header for a class CCSingleMuFilter
 *
 * @author kazuhiro
 */

/** \addtogroup CCSingleMu

    @{*/

#ifndef LARLITE_CCSINGLEMUFILTER_H
#define LARLITE_CCSINGLEMUFILTER_H

#include "Analysis/ana_base.h"
#include <TH1D.h>
namespace larlite {
  /**
     \class CCSingleMuFilter
     User custom analysis class made by kazuhiro
   */
  class CCSingleMuFilter : public ana_base{
  
  public:

    /// Default constructor
    CCSingleMuFilter();

    /// Default destructor
    virtual ~CCSingleMuFilter(){};

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

    void FlipSelection(bool flip) { _flip=flip; }
    
  protected:

    bool _flip;

    TH1D* hPrimaryMuEnergy;
    TH1D* hPrimaryMuCount;
    TH1D* hAllMuEnergy;
    TH1D* hAllMuCount;

    TH1D* hPrimaryPiEnergy;
    TH1D* hPrimaryPiCount;
    TH1D* hAllPiEnergy;
    TH1D* hAllPiCount;

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
