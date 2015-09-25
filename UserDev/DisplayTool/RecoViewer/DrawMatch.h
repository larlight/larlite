/**
 * \file DrawMatch.h
 *
 * \ingroup RecoViewer
 * 
 * \brief Class def header for a class DrawMatch
 *
 * @author cadams
 */

/** \addtogroup RecoViewer

    @{*/

#ifndef LARLITE_DRAWMATCH_H
#define LARLITE_DRAWMATCH_H

#include "Analysis/ana_base.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"
#include "LArUtil/DetectorProperties.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"

#include "DrawCluster.h"

#include "ClusterRecoUtil/Alg/DefaultParamsAlg.h"
#include "ClusterRecoUtil/Base/CRUHelper.h"


namespace evd {
  /**
     \class DrawMatch
     User custom analysis class made by SHELL_USER_NAME
   */


  class DrawMatch : public larlite::ana_base, public RecoBase<Cluster2d>{
  
  public:

    /// Default constructor
    DrawMatch();

    /// Default destructor
    virtual ~DrawMatch();

    /** IMPLEMENT in DrawMatch.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in DrawMatch.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(larlite::storage_manager* storage);

    /** IMPLEMENT in DrawMatch.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:

    ::cluster::CRUHelper _cru_helper;

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
