#ifndef RECOTOOL_SHOWERRECO3D_CXX
#define RECOTOOL_SHOWERRECO3D_CXX

#include "ShowerReco3D.h"
#include "DataFormat/pfpart.h"
#include "DataFormat/shower.h"
#include "DataFormat/DataFormat-TypeDef.h"
namespace larlite {

ShowerReco3D::ShowerReco3D() : ana_base()
{
  _name = "ShowerReco3D";
  fInputProducer  = "fuzzycluster";
  fOutputProducer = "showerreco";
  fUsePFParticle  = false;
}

bool ShowerReco3D::initialize() {

  if (_fout)
    _fout->cd();

  fManager.Initialize();

  return true;
}

bool ShowerReco3D::analyze(storage_manager* storage) {

  // Re-initialize tools
  fManager.Reset();

  // Pass the storage manager to the reco manager to allow access to other data products.
  fManager.SetStorageManager(storage);

  // Retrieve clusters and fed into the algorithm
  std::vector<std::vector<Hit2D> > local_clusters;
  fCRUHelper.GenerateHit2D(storage, fInputProducer, local_clusters);

  // Create output data product holder
  auto shower_v = storage->get_data<event_shower>(fOutputProducer);
  shower_v->clear();
  // create association for showers
  auto shower_ass_v = storage->get_data<event_ass>(shower_v->name());

  // set event ID through storage manager
  storage->set_id(storage->get_data<event_cluster>(fInputProducer)->run(),
                  storage->get_data<event_cluster>(fInputProducer)->subrun(),
                  storage->get_data<event_cluster>(fInputProducer)->event_id());

  // Result shower holder
  std::vector< ::showerreco::Shower_t> res_shower_v;


  // Run matching
  std::vector<std::vector<unsigned int> > matched_pairs;
  if (!fUsePFParticle) {
    matched_pairs = fManager.Reconstruct(local_clusters, res_shower_v);
  }
  else {

    auto pfpart_v = storage->get_data<event_pfpart>(fInputProducer);

    if (pfpart_v && pfpart_v->size()) {

      //auto const& ass_clusters = pfpart_v->association(data::kCluster, fInputProducer);
      event_cluster* ev_cluster = nullptr;
      auto const& ass_clusters = storage->find_one_ass(pfpart_v->id(), ev_cluster, pfpart_v->name());
      // Loop over pfparticles
      for (size_t pfpart_index = 0; pfpart_index < pfpart_v->size(); ++pfpart_index) {

        auto const& pfp = (*pfpart_v)[pfpart_index];

        // If this is not pdg=11 pfparticle, ignore
        if (pfp.PdgCode() != 11) continue;

        // Otherwise we store the association information in matched_pair
        matched_pairs.push_back(std::vector<unsigned int>());

        auto& last_pair = (*matched_pairs.rbegin());

        last_pair.reserve(ass_clusters[pfpart_index].size());

        // Loop over associated index for *this* pfparticle
        for (auto const& ass_index : ass_clusters[pfpart_index])
          last_pair.push_back(ass_index);
      }
    }
    fManager.Reconstruct(local_clusters,
                         matched_pairs,
                         res_shower_v);
  }


  // re-size matched pair vector so that only the matched pairs
  // for showers that will actually be reconstructed is used to
  // for the association vector
  std::vector<std::vector<unsigned int> > recod_matched_pairs;

  // counter for reco'd showers
  int ctr = 0;

  for (size_t i = 0; i < res_shower_v.size(); i++) {

    auto const& res_shower = res_shower_v[i];


    // filter out showers with garbage values
    if (res_shower.fXYZStart.Mag2()  == 0)
      continue;
    if (res_shower.fDCosStart.Mag2() == 0)
      continue;


    shower s;
    s.set_id ( shower_v->size() );
    s.set_total_energy          ( res_shower.fTotalEnergy           );
    s.set_total_energy_err      ( res_shower.fSigmaTotalEnergy      );
    s.set_total_energy_v        ( res_shower.fTotalEnergy_v         );
    s.set_total_energy_err_v    ( res_shower.fSigmaTotalEnergy_v    );
    s.set_total_MIPenergy_v     ( res_shower.fTotalMIPEnergy_v      );
    s.set_total_MIPenergy_err_v ( res_shower.fSigmaTotalMIPEnergy_v );
    s.set_total_best_plane      ( res_shower.fBestPlane.Plane       );
    s.set_direction             ( res_shower.fDCosStart             );
    s.set_direction_err         ( res_shower.fSigmaDCosStart        );
    s.set_start_point           ( res_shower.fXYZStart              );
    s.set_start_point_err       ( res_shower.fSigmaXYZStart         );
    s.set_dedx                  ( res_shower.fdEdx                  );
    s.set_dedx_err              ( res_shower.fSigmadEdx             );
    s.set_dedx_v                ( res_shower.fdEdx_v                );
    s.set_dedx_err_v            ( res_shower.fSigmadEdx_v           );
    s.set_dqdx                  ( res_shower.fdQdx                  );
    s.set_dqdx_err              ( res_shower.fSigmadQdx             );
    s.set_dqdx_v                ( res_shower.fdQdx_v                );
    s.set_dqdx_err_v            ( res_shower.fSigmadQdx_v           );
    s.set_length                ( res_shower.fLength                );
    s.set_opening_angle         ( res_shower.fOpeningAngle          );

    shower_v->push_back(s);
    //(*shower_v)[i].set_id(ctr);
    ctr += 1;

    recod_matched_pairs.push_back(matched_pairs[i]);


  }// for all input cluster-paris

  if (shower_v->size() == 0)
    return true;

  shower_ass_v->set_association(shower_v->id(), product_id(data::kCluster, fInputProducer), recod_matched_pairs);
  return true;
}

bool ShowerReco3D::finalize() {

  fManager.Finalize(_fout);

  return true;
}

}
#endif
