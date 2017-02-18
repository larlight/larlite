/**
 * \file ConeOverlapTag.h
 *
 * \ingroup PhotonTagging
 * 
 * \brief Class def header for a class ConeOverlapTag
 *
 * @author david caratelli
 */

/** \addtogroup PhotonTagging

    @{*/

#ifndef LARLITE_CONEOVERLAPTAG_H
#define LARLITE_CONEOVERLAPTAG_H

#include "Analysis/ana_base.h"
#include "DataFormat/shower.h"

#include "ClusterRecoUtil/Base/Polygon2D.h"

namespace larlite {
  /**
     \class ConeOverlapTag
     User custom analysis class made by SHELL_USER_NAME
   */
  class ConeOverlapTag : public ana_base{
  
  public:

    /// Default constructor
    ConeOverlapTag(){ _name="ConeOverlapTag"; _fout=0; _debug=false;}

    /// Default destructor
    virtual ~ConeOverlapTag(){}

    /** IMPLEMENT in ConeOverlapTag.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ConeOverlapTag.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ConeOverlapTag.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    // producer setters
    void setShowerProducer    (std::string s) { _shower_producer = s;      }
    void setPhotonProducer    (std::string s) { _photon_producer = s;      }
    void setOutPFPartProducer (std::string s) { _out_pfpart_producer = s;  }
    void setOutClusterProducer(std::string s) { _out_cluster_producer = s; }

    // set debug mode
    void setDebug(bool on) { _debug = on; }

  protected:

    // function that projects reconstructed 3D shower
    // into 3 2D triangles, one per plane.
    void projectShower(const larlite::shower& shr);

    // merge hits from shower and polygon
    void mergeHits(std::vector<unsigned int>& shr_hit_idx_v,
		   const std::vector<unsigned int>& photon_hit_idx_v);

    // store 2D triangles representing shower cones.
    std::vector<Polygon2D> _shr_polygon_v;

    // store 2D polygons representing photon clusters
    // vector of vectors:
    // 1st vector is for the 3 planes
    // pairs hold idex of cluster in original clus vector (.first)
    //            polygon object 
    std::vector< std::vector< std::pair<size_t, Polygon2D > > > _photon_poly_v;

    // producers
    std::string _shower_producer;
    std::string _photon_producer;
    std::string _out_pfpart_producer;
    std::string _out_cluster_producer;

    // debug
    bool _debug;

    // t2cm and wtcm constants
    double _t2cm, _w2cm;

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
