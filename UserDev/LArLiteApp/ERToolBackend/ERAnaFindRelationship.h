/**
 * \file ERAnaFindRelationship.h
 *
 * \ingroup LArLite_ERTool
 * 
 * \brief Class def header for a class ERAnaFindRelationship
 *
 * @author david
 */

/** \addtogroup LArLite_ERTool

    @{*/

#ifndef LARLITE_ERANAFINDRELATIONSHIP_H
#define LARLITE_ERANAFINDRELATIONSHIP_H

#include "Analysis/ana_base.h"
#include "ERTool/Base/Manager.h"
#include "ERTool/Algo/AlgoFindRelationship.h"
#include "ERToolAnaBase.h"

namespace larlite {
  /**
     \class ERAnaFindRelationship
     User custom analysis class made by david
   */
  class ERAnaFindRelationship : public ERToolAnaBase {
  
  public:

    /// Default constructor
    ERAnaFindRelationship();

    /// Default destructor
    virtual ~ERAnaFindRelationship(){};

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

  private:
    
    /// Tree info
    TTree* _ana_tree;
    double _dot;
    double _thisToVtx, _thatToVtx;
    int _thisPDG, _thatPDG;
    int _thisID, _thatID;
    std::string _thisProcess;

    geoalgo::GeoAlgo _geoAlg;

    ertool::AlgoFindRelationship _findRel;

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
