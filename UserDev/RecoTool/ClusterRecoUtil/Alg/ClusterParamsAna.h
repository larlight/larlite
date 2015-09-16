/**
 * \file ClusterParamsAna.h
 *
 * \ingroup Alg
 * 
 * \brief Class def header for a class ClusterParamsAna
 *
 * @author cadams
 */

/** \addtogroup Alg

    @{*/

#ifndef LARLITE_CLUSTERPARAMSANA_H
#define LARLITE_CLUSTERPARAMSANA_H

#include "Analysis/ana_base.h"
#include "TH1F.h"
#include "TTree.h"

namespace cluster {
  /**
     \class ClusterParamsAna
     User custom analysis class made by SHELL_USER_NAME
   */
  class ClusterParamsAna : public ::larlite::ana_base{
  
  public:

    /// Default constructor
    ClusterParamsAna();

    /// Default destructor
    virtual ~ClusterParamsAna(){}

    /** IMPLEMENT in ClusterParamsAna.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ClusterParamsAna.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(::larlite::storage_manager* storage);

    /** IMPLEMENT in ClusterParamsAna.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetVerbose(bool on) { _verbose = on; }

  protected:

    bool _verbose;
    
    std::vector<TH1F *> startPointW;
    std::vector<TH1F *> startPointT;
    std::vector<TH1F *> startSlope;
    std::vector<TH1F *> prinSlope;

    TTree* _param_tree;
    int _plane;
    double _E;
    double _w, _mc_w;
    double _t, _mc_t;
    double _angle, _start_angle, _mc_angle;
    double _containment;
    int _direction; //0 is forward, 1 is backwards

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
