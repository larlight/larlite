/**
 * \file DrawCluster.h
 *
 * \ingroup RecoViewer
 *
 * \brief Class def header for a class DrawCluster
 *
 * @author cadams
 */

/** \addtogroup RecoViewer

    @{*/

#ifndef EVD_DRAWCLUSTER_H
#define EVD_DRAWCLUSTER_H

#include "Analysis/ana_base.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"
#include "LArUtil/DetectorProperties.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"

#include "DrawHit.h"

#include "ClusterRecoUtil/Alg/DefaultParamsAlg.h"
#include "ClusterRecoUtil/Base/CRUHelper.h"


namespace evd {
/**
   \class DrawCluster
   User custom analysis class made by SHELL_USER_NAME
 */

class Cluster2D : public std::vector<Hit2D> {

public:
  Cluster2D(){_is_good = false;}
  ::cluster::cluster_params _params;
  ::cluster::cluster_params params() {return _params;}
  bool _is_good;
  bool is_good() {return _is_good;}
  std::vector<float> _start;
};

class DrawCluster : public larlite::ana_base, public RecoBase<Cluster2D> {

public:

  /// Default constructor
  DrawCluster();

  /// Default destructor
  virtual ~DrawCluster();

  /** IMPLEMENT in DrawCluster.cc!
      Initialization method to be called before the analysis event loop.
  */
  virtual bool initialize();

  /** IMPLEMENT in DrawCluster.cc!
      Analyze a data event-by-event
  */
  virtual bool analyze(larlite::storage_manager* storage);

  /** IMPLEMENT in DrawCluster.cc!
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
