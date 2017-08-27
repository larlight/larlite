/**
 * \file PhotonMerge.h
 *
 * \ingroup PhotonTagging
 * 
 * \brief Class def header for a class PhotonMerge
 *
 * @author david caratelli
 */

/** \addtogroup PhotonTagging

    @{*/

#ifndef LARLITE_PHOTONMERGE_H
#define LARLITE_PHOTONMERGE_H

#include "Analysis/ana_base.h"
#include "DataFormat/cluster.h"

#include "TwoDimTools/Poly2D.h"
#include "TwoDimTools/Linearity.h"

#include <map>

namespace larlite {
  /**
     \class PhotonMerge
     User custom analysis class made by SHELL_USER_NAME
   */
  class PhotonMerge : public ana_base{
  
  public:

    /// Default constructor
    PhotonMerge();

    /// Default destructor
    virtual ~PhotonMerge(){}

    /** IMPLEMENT in PhotonMerge.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in PhotonMerge.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in PhotonMerge.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();
    
    // producer setters
    void setVertexProducer    (std::string s) { _vtx_producer = s;         }
    void setPFPartProducer    (std::string s) { _pfpart_producer = s;      }
    void setPhotonProducer    (std::string s) { _photon_producer = s;      }
    void setOutPFPartProducer (std::string s) { _out_pfpart_producer = s;  }
    void setOutClusterProducer(std::string s) { _out_cluster_producer = s; }

    // set debug mode
    void setDebug(bool on) { _debug = on; }

    // set shower length [cm]
    void setShrLen(double d) { _shrLen = d; }
    // set shower width [degrees]
    void setShrWidth(double a) { _width = a; }
    // set tick offset
    void setTickOffset(double t) { _tick_offset = t; }
    // set max frac of shower (measured in number of hits) charge photon can add
    void setMaxQFracAdd(double f) { _frac_shr_q = f; }
    // set max photon angle
    void setMaxPhotonAngle(double a) { _max_slope_angle = a; }

    // get the polygon clusters
    twodimtools::Poly2D getShowerPolygon() { return _shr_polygon_v; }
    std::vector<twodimtools::Poly2D> getPhotonPolygon(int pl);
    
  protected:
    
    // function that projects reconstructed 3D shower
    // into 3 2D triangles, one per plane.
    void projectShower(const larlite::cluster& clus);
    
    // do the shower and cluster agree in slope?
    double slopeCompat(const twodimtools::Poly2D& shr,
		       const twodimtools::Linearity& photon);

    // does the photon cross the shower on both sides of the cone?
    bool photonCrossesShower(const twodimtools::Poly2D& shr,
			     const twodimtools::Poly2D& photon);
    
    // merge hits from shower and polygon
    void mergeHits(std::vector<unsigned int>& shr_hit_idx_v,
		   const std::vector<unsigned int>& photon_hit_idx_v);
    
    // store 2D triangles representing shower cones.
    twodimtools::Poly2D _shr_polygon_v;
    
    // store 2D polygons representing photon clusters
    // vector of vectors:
    // 1st vector is for the 3 planes
    // pairs hold index of cluster in original clus vector (.first)
    //            polygon object (.second)
    std::vector< std::vector< std::pair<size_t, twodimtools::Poly2D > > > _photon_poly_v;
    
    // map connecting cluster index to linearity object
    std::map< size_t, twodimtools::Linearity > _photon_lin_map;

    // producers
    std::string _pfpart_producer;
    std::string _photon_producer;
    std::string _out_pfpart_producer;
    std::string _out_cluster_producer;

    // debug
    bool _debug;

    // t2cm and wtcm constants
    double _t2cm, _w2cm;

    // how far to extend the shower? user-determined
    double _shrLen; // cm
    // how wide should the shower be? 3D angle [degrees]
    // if unspecified -> use opening width
    double _width;

    // max photon angle
    double _max_slope_angle;
    
    // maximum cluster linearity allowed
    double _minLin;

    // max frac of shower's hits that can be added
    float _frac_shr_q;

    // tick offset?
    double _tick_offset;

    // vertex coordinates
    double _vtxW, _vtxT;
    std::string _vtx_producer;
    
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
