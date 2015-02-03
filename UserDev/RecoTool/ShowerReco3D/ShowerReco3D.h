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
#include "ClusterRecoUtil/CRUHelper.h"
#include "ShowerRecoManager.h"

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
    virtual ~ShowerReco3D(){}

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
    void SetShowerAlgo(::showerreco::ShowerRecoAlgBase *alg) { fManager.Algo(alg); }

    /// Function to set an input cluster/pfparticle producer name to work with
    void SetInputProducer(std::string name, bool use_pfpart=false) 
    { fInputProducer = name; fUsePFParticle = use_pfpart; }

    /// Function to set an output shower producer label
    void SetOutputProducer(std::string name) { fOutputProducer = name; }

    /// Getter for MatchManager instance, to attach algorithms
    ::cmtool::CMatchManager& GetManager() { return fManager.MatchManager(); }

  protected:

    /// Input producer name
    std::string fInputProducer;

    /// Output producer name
    std::string fOutputProducer;

    /// Boolean flag to use PFParticle as an input or not
    bool fUsePFParticle;

    /// Shower reco core class instance
    ::showerreco::ShowerRecoManager fManager;

    /// CRUHelper converts framework dependent data product to PxUtil
    ::cluster::CRUHelper fCRUHelper;

  };
}
#endif

/** @} */ // end of doxygen group 
