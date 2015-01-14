/**
 * \file ShowerReco3D.h
 *
 * \ingroup ShowerReco3D
 * 
 * \brief Class def header for a class ShowerReco3D
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerReco3D

    @{*/

#ifndef RECOTOOL_SHOWERRECO3D_H
#define RECOTOOL_SHOWERRECO3D_H

#include "Analysis/ana_base.h"
#include "DataFormat/DataFormat-TypeDef.h"
#include "ClusterRecoUtil/CRUHelper.h"
#include "CMTool/CMToolBase/CMatchManager.h"
#include "ShowerRecoException.h"
#include "ShowerRecoAlgBase.h"

namespace larlite {
  /**
     \class ShowerReco3D
     User custom analysis class made by kazuhiro
   */
  class ShowerReco3D : public ana_base{
  
  public:

    /// Default constructor
    ShowerReco3D();

    /// Default destructor
    virtual ~ShowerReco3D(){ delete fMatchMgr; }

    /** IMPLEMENT in ShowerReco3D.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ShowerReco3D.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ShowerReco3D.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    /// Attach algo
    void SetShowerAlgo(::showerreco::ShowerRecoAlgBase *alg) { fShowerAlgo = alg;}

    /// hack! remove me later
    storage_manager* GetCurrentData() {return _mgr;};
    
    /// Function to set an input cluster/pfparticle producer name to work with
    void SetInputProducer(std::string name, bool use_pfpart=false) 
    { fInputProducer = name; fUsePFParticle = use_pfpart; }

    /// Function to set an output shower producer label
    void SetOutputProducer(std::string name) { fOutputProducer = name; }

    /// Getter for MatchManager instance, to attach algorithms
    ::cmtool::CMatchManager& GetManager() { return *fMatchMgr; }

  protected:

    /// Input producer name
    std::string fInputProducer;

    /// Output producer name
    std::string fOutputProducer;

    /// Boolean flag to use PFParticle as an input or not
    bool fUsePFParticle;

    /// CRUHelper converts framework dependent data product to PxUtil
    ::cluster::CRUHelper fCRUHelper;

    /// Shower reconstruction algorithm
    ::showerreco::ShowerRecoAlgBase *fShowerAlgo;

    /// Cluster matching code
    ::cmtool::CMatchManager *fMatchMgr;

    /// hack! remove me later
    storage_manager* _mgr;
  };
}
#endif

/** @} */ // end of doxygen group 
