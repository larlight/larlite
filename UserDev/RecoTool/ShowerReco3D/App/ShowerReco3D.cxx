#ifndef RECOTOOL_SHOWERRECO3D_CXX
#define RECOTOOL_SHOWERRECO3D_CXX

#include "ShowerReco3D.h"
#include "DataFormat/pfpart.h"
#include "DataFormat/shower.h"
#include "DataFormat/DataFormat-TypeDef.h"
namespace larlite {

  ShowerReco3D::ShowerReco3D() : ana_base()
  {
    _name="ShowerReco3D";
    fInputProducer  = "fuzzycluster";
    fOutputProducer = "showerreco";
    fUsePFParticle  = false;
  }
  
  bool ShowerReco3D::initialize() {

    return true;
  }
  
  bool ShowerReco3D::analyze(storage_manager* storage) {

    // Re-initialize tools
    fManager.Reset();

    // Retrieve clusters and fed into the algorithm
    std::vector<std::vector<larutil::PxHit> > local_clusters;
    fCRUHelper.GeneratePxHit(storage,fInputProducer,local_clusters);

    // Create output data product holder
    auto shower_v = storage->get_data<event_shower>(fOutputProducer);
    shower_v->clear();

    // set event ID through storage manager
    storage->set_id(storage->get_data<event_cluster>(fInputProducer)->run(),
                    storage->get_data<event_cluster>(fInputProducer)->subrun(),
                    storage->get_data<event_cluster>(fInputProducer)->event_id());

    // Result shower holder
    std::vector< ::showerreco::Shower_t> res_shower_v;
    
    // Run matching
    std::vector<std::vector<unsigned int> > matched_pairs;
    if(!fUsePFParticle) {
      matched_pairs = fManager.Reconstruct(local_clusters,res_shower_v);
    }else{

      auto pfpart_v = storage->get_data<event_pfpart>(fInputProducer);

      if(pfpart_v && pfpart_v->size()) {
      
        //auto const& ass_clusters = pfpart_v->association(data::kCluster, fInputProducer);
        event_cluster* ev_cluster = nullptr;
        auto const& ass_clusters = storage->find_one_ass(pfpart_v->id(),ev_cluster,pfpart_v->name());
        // Loop over pfparticles
        for(size_t pfpart_index=0; pfpart_index<pfpart_v->size(); ++pfpart_index) {
          
          auto const& pfp = (*pfpart_v)[pfpart_index];
          
          // If this is not pdg=11 pfparticle, ignore
          if(pfp.PdgCode()!=11) continue;
          
          // Otherwise we store the association information in matched_pair
          matched_pairs.push_back(std::vector<unsigned int>());
          
          auto& last_pair = (*matched_pairs.rbegin());
          
          last_pair.reserve(ass_clusters[pfpart_index].size());
          
          // Loop over associated index for *this* pfparticle
          for(auto const& ass_index : ass_clusters[pfpart_index]) 
            last_pair.push_back(ass_index);
        }
      }

      fManager.Reconstruct(local_clusters,
                           matched_pairs,
                           res_shower_v);
    }

    for(auto const& res_shower : res_shower_v) {

      shower s;
      s.set_id ( shower_v->size() );

      s.set_total_energy        ( res_shower.fTotalEnergy         );
      s.set_total_energy_err    ( res_shower.fSigmaTotalEnergy    );
      s.set_total_MIPenergy     ( res_shower.fTotalMIPEnergy      );
      s.set_total_MIPenergy_err ( res_shower.fSigmaTotalMIPEnergy );
      s.set_total_best_plane    ( res_shower.fBestPlane.Plane     );
      s.set_direction           ( res_shower.fDCosStart           );
      s.set_direction_err       ( res_shower.fSigmaDCosStart      );
      s.set_start_point         ( res_shower.fXYZStart            );
      s.set_start_point_err     ( res_shower.fSigmaXYZStart       );
      s.set_dedx                ( res_shower.fdEdx                );
      s.set_dedx_err            ( res_shower.fSigmadEdx           );
      s.set_length              ( res_shower.fLength              );
      s.set_opening_angle       ( res_shower.fOpeningAngle        );

      shower_v->emplace_back(s);
    }
        
    // Make sure result has the same size 
    if(shower_v->size() != matched_pairs.size())
      throw ::showerreco::ShowerRecoException("Mismatch in # of showers from algorithm's return!");

    for(size_t index = 0; index < shower_v->size(); ++index)

      (*shower_v)[index].set_id(index);

    // create association for showers
    auto shower_ass_v = storage->get_data<event_ass>(shower_v->name());    
    shower_ass_v->set_association(shower_v->id(),product_id(data::kCluster,fInputProducer),matched_pairs);
    //shower_v->set_association(data::kCluster,fInputProducer,matched_pairs);
    return true;
  }
  
  bool ShowerReco3D::finalize() {    
    
    fManager.Finalize(_fout);

    return true;
  }
  
}
#endif
