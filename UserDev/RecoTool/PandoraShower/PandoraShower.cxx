#ifndef RECOTOOL_PANDORASHOWER_CXX
#define RECOTOOL_PANDORASHOWER_CXX

#include "PandoraShower.h"
#include <map>

namespace larlite {

   PandoraShower::PandoraShower() : ana_base(), fShowerAlgo( nullptr ) {
      _name = "PandoraShower";
      fHitProducer = "pandoraCosmicKHitRemoval";
      fClusterProducer = "pandoraNu";
      fPFParticleProducer = "pandoraNu";

      /// Get the geometry of the detector, eps. the number of the wire planes
      auto geom = ::larutil::Geometry::GetME();
      fNPlanes = geom->Nplanes();
      // for ( auto iview : geom->Views() ) fViews.insert( iview );
      fViews   = geom->Views();
   }

   bool PandoraShower::initialize() {
      _mgr = 0;

      if( !fShowerAlgo ) {
         throw ::cluster::CRUException("Shower reco algorithm not attached... aborting.");
         return false;
      }

      return true;
   }

   bool PandoraShower::analyze( storage_manager* storage ) {
      _mgr = storage;

      // Re-initialize tools
      fShowerAlgo->Reset();

      // Retrieve hits, clusters and PF particles
      auto hits = ( event_hit* )( storage->get_data( data::kHit, fHitProducer ) );
      auto clusters = ( event_cluster* )( storage->get_data( data::kCluster, fClusterProducer ) );
      auto pfparts = ( event_pfpart* )( storage->get_data( data::kPFParticle, fPFParticleProducer ) );

      // Prepare output storage
      auto showers = storage->get_data<event_shower>("pandorashower");
      showers->clear();
      showers->reserve( pfparts->size() );
      showers->set_event_id( pfparts->event_id() );
      showers->set_run( pfparts->run() );
      showers->set_subrun( pfparts->subrun() );

      // Associate PF particles and clusters
      AssSet_t ass_clusters;
      ass_clusters.reserve( pfparts->size() );
      ass_clusters = pfparts->association( clusters->id() );

      for ( size_t ipart = 0; ipart < pfparts->size(); ipart++ ) {
         if ( std::abs( pfparts->at(ipart).PdgCode() ) != 11 ) break;

         std::map< larlite::geo::View_t, double > TotalClusterCharges;
         for ( auto const& iview : fViews ) TotalClusterCharges[iview] = 0.;

         for ( auto const& c_index : ass_clusters[ipart] ) {
            auto iview = clusters->at(c_index).View();
            TotalClusterCharges[iview] += clusters->at(c_index).Charge();
         }

      }

      // Calculate total hit charges
      std::map< larlite::geo::View_t, double > TotalHitCharges;
      for ( auto const& iview : fViews ) TotalHitCharges[iview] = 0.;
      CalculateTotalHitCharge( hits, TotalHitCharges );

   }

   bool PandoraShower::finalize() {
      return true;
   }

   void PandoraShower::CalculateTotalHitCharge( larlite::event_hit const* hits, std::map< larlite::geo::View_t, double >& charges ) {
      for ( auto const& hit : ( *hits ) ) {
         charges[hit.View()] += hit.Charge();
      }
      return;
   }

}
#endif
