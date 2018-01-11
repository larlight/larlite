/**
 * \file HitCluster.h
 *
 * \ingroup CMTool
 *
 * \brief Class def header for a class HitCluster
 *
 * @author yuntse
 */

/** \addtogroup CMTool

    @{*/

#ifndef RECOTOOL_HITCLUSTER_H
#define RECOTOOL_HITCLUSTER_H

#include "Analysis/ana_base.h"
#include "DataFormat/pfpart.h"
#include "DataFormat/cluster.h"
#include "DataFormat/spacepoint.h"
#include "DataFormat/vertex.h"
#include "DataFormat/hit.h"

namespace larlite {
  /**
     \class HitCluster
     User custom analysis class made by kazuhiro
   */
  class HitCluster : public ana_base{

  public:

    /// Default constructor
    HitCluster();

    /// Default destructor
    virtual ~HitCluster(){}

    /** IMPLEMENT in HitCluster.cc!
        Initialization method to be called before the analysis event loop.
    */
    virtual bool initialize();

    /** IMPLEMENT in HitCluster.cc!
        Analyze a data event-by-event
    */
    virtual bool analyze( storage_manager* storage );

    /** IMPLEMENT in HitCluster.cc!
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetInputProducer( std::string name ) { _input_producer = name; }
    void SetInputSpacePointProducer( std::string name ) { _input_sps_producer = name; }
    void SetInputVertexProducer( std::string name ) { _input_vertex_producer = name; }
    void SetOutputProducer( std::string name ) { _output_producer = name; }

  protected:

    bool _write_output;

    std::string _input_producer;
    std::string _input_sps_producer;
    std::string _input_vertex_producer;
    std::string _output_producer;

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

