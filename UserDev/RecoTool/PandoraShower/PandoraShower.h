/**
 * \file PandoraShower.h
 *
 * \ingroup PandoraShower
 * 
 * \brief Class def header for a class PandoraShower
 *
 * @author yuntse
 */

/** \addtogroup PandoraShower

    @{*/
#ifndef RECOTOOL_PANDORASHOWER_H
#define RECOTOOL_PANDORASHOWER_H

#include "Analysis/ana_base.h"
#include "DataFormat/DataFormat-TypeDef.h"
#include "ClusterRecoUtil/CRUHelper.h"
#include "CMTool/CMToolBase/CMatchManager.h"
#include "ShowerReco3D/ShowerRecoException.h"
#include "ShowerReco3D/ShowerRecoAlgBase.h"
#include <iostream>

/**
   \class PandoraShower
   This is a sort of copy of ShowerReco3D class
 */

namespace larlite {
   class PandoraShower : public ana_base {

   public:

      /// Default constructor
      PandoraShower();

      /// Default destructor
      virtual ~PandoraShower(){}

      /** IMPLEMENT in PandoraShower.cc!
          Initialization method to be called before the analysis event loop.
      */
      virtual bool initialize();

      /** IMPLEMENT in PandoraShower.cc!
          Analyze a data event-by-event
      */
      virtual bool analyze( storage_manager* storage );

      /** IMPLEMENT in PandoraShower.cc!
          Finalize method to be called after all events processed.
      */
      virtual bool finalize();

      /// Attach algo
      void SetShowerAlgo( ::showerreco::ShowerRecoAlgBase *alg ) { fShowerAlgo = alg; }

      /// hack! remove me later
      storage_manager* GetCurrentData() { return _mgr; };

      /// Function to set an input cluster type to work with
      void SetClusterProducer( std::string name ) { fClusterProducer = name; }

      /// Function to calculate total deposit charges from hits in each plane
      void CalculateTotalHitCharge( larlite::event_hit const* hits, std::map< larlite::geo::View_t, double >& charges );

   protected:

      /// Number of the wire planes
      UInt_t fNPlanes;

      /// All possible views
      std::set<larlite::geo::View_t> fViews;

      /// Cluster producer name
      std::string fClusterProducer;

      /// PF particle producer name
      std::string fPFParticleProducer;

      /// Hit producer name
      std::string fHitProducer;

      /// CRUHelper converts framework dependent data product to PxUtil
      ::cluster::CRUHelper fCRUHelper;

      /// Shower reconstruction algorithm
      ::showerreco::ShowerRecoAlgBase *fShowerAlgo;

      /// hack! remove me later
      storage_manager* _mgr;

   };
}
#endif
/** @} */ // end of doxygen group 

