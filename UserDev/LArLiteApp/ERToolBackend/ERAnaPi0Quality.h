/**
 * \file ERAnaPi0Quality.h
 *
 * \ingroup LArLite_ERTool
 * 
 * \brief Class def header for a class ERAnaPi0Quality
 *
 * @author david
 */

/** \addtogroup LArLite_ERTool

    @{*/

#ifndef LARLITE_ERANAPI0QUALITY_H
#define LARLITE_ERANAPI0QUALITY_H

#include "Analysis/ana_base.h"
#include "ERTool/Base/Manager.h"
#include "ERToolAnaBase.h"
namespace larlite {
  /**
     \class ERAnaPi0Quality
     User custom analysis class made by david
   */
  class ERAnaPi0Quality : public ERToolAnaBase {
  
  public:

    /// Default constructor
    ERAnaPi0Quality();

    /// Default destructor
    virtual ~ERAnaPi0Quality(){};

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

    /// Selection manager class instance
    ::ertool::Manager _mgr;
    
    /// Tree info
    TTree* _pi0_tree;
    double _mc_E, _reco_E;
    double _mc_x, _mc_y, _mc_z, _reco_x, _reco_y, _reco_z;
    double _start_res;

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
