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
#include "DataFormat/pfpart.h"
#include "DataFormat/shower.h"
#include "DataFormat/vertex.h"
#include "DataFormat/spacepoint.h"
#include "DataFormat/cluster.h"

 
#include "ShowerReco3D/Base/ShowerRecoTypes.h"
#include "ShowerReco3D/Base/ProtoShowerHelper.h"

#include "ShowerReco3D/Base/ShowerRecoManager.h"
#include "ShowerReco3D/Base/ProtoShowerHelper.h"

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
    void AddShowerAlgo(::showerreco::ShowerRecoAlgBase *alg) { fManager.AddAlgo(alg); }

    /// Function to set an input cluster/pfparticle producer name to work with
    void SetInputProducer(std::string name) 
    { fInputProducer = name; }

    /// Function to set an output shower producer label
    void SetOutputProducer(std::string name) { fOutputProducer = name; }

  protected:

    /// Input producer name
    std::string fInputProducer;

    /// Output producer name
    std::string fOutputProducer;

    /// Shower reco core class instance
    ::showerreco::ShowerRecoManager fManager;

    // ProtoShowerHelper converts data products into protoshowers
    ::showerreco::ProtoShowerHelper _ps_helper;
  };
}
#endif

/** @} */ // end of doxygen group 
