/**
 * \file MergeAlignedClusters.h
 *
 * \ingroup PhotonTagging
 * 
 * \brief Class def header for a class MergeAlignedClusters
 *
 * @author david
 */

/** \addtogroup PhotonTagging

    @{*/

#ifndef LARLITE_MERGEALIGNEDCLUSTERS_H
#define LARLITE_MERGEALIGNEDCLUSTERS_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class MergeAlignedClusters
     User custom analysis class made by SHELL_USER_NAME
   */
  class MergeAlignedClusters : public ana_base{
  
  public:

    /// Default constructor
    MergeAlignedClusters();

    /// Default destructor
    virtual ~MergeAlignedClusters(){}

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

    void setVtxProducer(std::string s) { _vtx_producer = s; }
    void setPFPProducer(std::string s) { _pfp_producer = s; }

    void setOutPFPProducer(std::string s) { _out_pfp_producer = s; }
    void setOutClusterProducer(std::string s) { _out_cluster_producer = s; }

    void setAngleDiff(double a) { _anglediff = a; }

    void setDebug(bool on) { _debug = on; }

  protected:

    std::string _pfp_producer;
    std::string _vtx_producer;

    std::string _out_cluster_producer;
    std::string _out_pfp_producer;

    // map linking cluster index to pfparticle index
    std::map< size_t, size_t > _pfp_clus_map;
    std::map< size_t, std::pair< double, double > > _clus_pos;
    std::map< size_t, double > _clus_angle;

    double _w2cm, _t2cm;
    double _vtxW, _vtxT;

    double _anglediff;

    bool _debug;
    
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
