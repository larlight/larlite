/**
 * \file SPAnaPi0Quality.h
 *
 * \ingroup SPTApp
 * 
 * \brief Class def header for a class SPAnaPi0Quality
 *
 * @author david
 */

/** \addtogroup SPTApp

    @{*/

#ifndef LARLITE_SPANAPI0QUALITY_H
#define LARLITE_SPANAPI0QUALITY_H

#include "Analysis/ana_base.h"
#include "SPTBase/SPAManager.h"
#include "SPTAnaBase.h"
namespace larlite {
  /**
     \class SPAnaPi0Quality
     User custom analysis class made by david
   */
  class SPAnaPi0Quality : public SPTAnaBase {
  
  public:

    /// Default constructor
    SPAnaPi0Quality();

    /// Default destructor
    virtual ~SPAnaPi0Quality(){};

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

    /// Selection manager class instance
    ::sptool::SPAManager _mgr;
    
    /// Tree info
    TTree* _pi0_tree;
    double _mc_E, _reco_E;
    double _mc_x, _mc_y, _mc_z, _reco_x, _reco_y, _reco_z;

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
