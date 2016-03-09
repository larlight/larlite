/**
 * \file PFPartMerger.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class PFPartMerger
 *
 * @author yuntse
 */

/** \addtogroup CMTool

    @{*/

#ifndef RECOTOOL_PFPARTMERGER_H
#define RECOTOOL_PFPARTMERGER_H

// ROOT
#include "TVector3.h"

#include "Analysis/ana_base.h"
#include "DataFormat/pfpart.h"
#include "DataFormat/shower.h"
#include "DataFormat/cluster.h"
#include "DataFormat/spacepoint.h"
#include "DataFormat/vertex.h"
#include "DataFormat/hit.h"
// #include "CMergeHelper.h"
// #include "ClusterRecoUtil/Base/CRUHelper.h"

namespace larlite {
  /**
     \class PFPartMerger
     User custom analysis class made by kazuhiro
   */
  class PFPartMerger : public ana_base{
  
  public:

    /// Default constructor
    PFPartMerger();

    /// Default destructor
    virtual ~PFPartMerger(){}

    /** IMPLEMENT in PFPartMerger.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in PFPartMerger.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in PFPartMerger.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetInputProducer(std::string name) {_input_producer = name;}
    void SetInputShowerProducer(std::string name) {_input_shower_producer = name;}

    void SetOutputProducer(std::string name) {_output_producer = name;}

    void SetMergeByHierarchy( bool doit = false ) { _doMergeByHierarchy = doit; }
    void SetMergeByPCAndVertex( bool doit = false ) { _doMergeByPCAndVertex = doit; }

    void SaveOutputCluster(bool doit=true) { _write_output = doit; }

    void ListByHierarchy( const larlite::event_pfpart& ev_pfpart, 
                          std::vector< std::vector < size_t > >& pfpart_sets );
 
    void CreateHierarchyList( const larlite::event_pfpart& ev_pfpart, size_t ipart, 
                              std::vector< size_t >& pfpList );

    void ListByPCAAndVertex( const larlite::event_pfpart& ev_pfpart, 
                             const larlite::event_shower& ev_shower, 
                             const larlite::AssSet_t& ass_shower_v, 
                             const larlite::event_vertex& ev_vertex, 
                             const larlite::AssSet_t& ass_vertex_v, 
                             std::vector< std::vector < size_t > >& pfpart_sets );

    bool ShareTheSameVertex( const TVector3& ivertex, const TVector3& jvertex ); 

    bool IsVertexAligned( const TVector3& ivertex, const larlite::shower& ishr,
                          const TVector3& jvertex ); 

    bool IsPCAAligned( const TVector3& ivertex, const larlite::shower& ishr,
                       const TVector3& jvertex, const larlite::shower& jshr ); 

  protected:

    bool _write_output;
    bool _doMergeByHierarchy;
    bool _doMergeByPCAndVertex;

    std::string _input_producer;
    std::string _input_shower_producer;

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
