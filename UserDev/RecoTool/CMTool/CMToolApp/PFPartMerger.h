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
#include "DataFormat/cluster.h"
#include "DataFormat/spacepoint.h"
#include "DataFormat/vertex.h"
#include "DataFormat/hit.h"
// #include "CMergeHelper.h"
#include "Cluster3DRecoUtil/Alg/Default3DParamsAlg.h"
#include "Cluster3DRecoUtil/Base/CRU3DHelper.h"

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
    void SetOutputProducer(std::string name) {_output_producer = name;}

    void SetMergeByHierarchy( bool doit = false ) { _doMergeByHierarchy = doit; }
    void SetMergeByPCAndVertex( bool doit = false ) { _doMergeByPCAndVertex = doit; }

    void SaveOutputCluster(bool doit=true) { _write_output = doit; }

    void SetPrimaryConeFactor( double factor = 0.5 ) { _primary_cone_factor = factor; }
    void SetSecondaryConeFactor( double factor = 0.5 ) { _secondary_cone_factor = factor; }

    void ListByHierarchy( const larlite::event_pfpart& ev_pfpart, 
                          std::vector< std::vector < size_t > >& pfpart_sets );
 
    void CreateHierarchyList( const larlite::event_pfpart& ev_pfpart, size_t ipart, 
                              std::vector< size_t >& pfpList );

    void PFParticleCharges( const size_t npfp, const larlite::event_cluster& ev_cluster,
                            const AssSet_t ass_cluster_v, std::vector< double >& pfpart_charges );

    void ListByPCAAndVertex( const larlite::event_pfpart& ev_pfpart, 
                             std::vector< double >& pfpart_charge,
                             std::vector< cluster3D::cluster3D_params >& cluster3DParams,
                             std::vector< TVector3 >& vertices,
                             std::vector< std::vector < size_t > >& pfpart_sets );

    bool ShareTheSameVertex( const TVector3& ivertex, const TVector3& jvertex ); 

    bool IsVertexAligned( const TVector3& ivertex, const cluster3D::cluster3D_params& icluster3D,
                          const TVector3& jvertex ); 

    bool IsPCAAligned( const TVector3& ivertex, const cluster3D::cluster3D_params& icluster3D,
                       const TVector3& jvertex, const cluster3D::cluster3D_params& jcluster3D ); 

    double OpenAngle( const TVector3& principal_dir, const TVector3& secondary_dir,
                      const double principal_eigenvalue, const double secondary_eigenvalue );

  protected:

    bool _write_output;
    bool _doMergeByHierarchy;
    bool _doMergeByPCAndVertex;

    std::string _input_producer;
    std::string _output_producer;

    ::cluster3D::Default3DParamsAlg _params3D_alg;
    ::cluster3D::CRU3DHelper _cru3D_helper;

    double _primary_cone_factor;
    double _secondary_cone_factor;

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
