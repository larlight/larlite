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

  // retrieve pfparticle objects (if none found -> quit)
  auto ev_pfpart = storage->get_data<event_pfpart>(fInputProducer);

  // Create output data product holder
  auto shower_v = storage->get_data<event_shower>(fOutputProducer);
  shower_v->clear();
  // create association for showers -> pfparticle
  auto shower_pfpart_ass_v = storage->get_data<event_ass>(shower_v->name());
  // create association for showers -> cluster
  auto shower_cluster_ass_v = storage->get_data<event_ass>(shower_v->name());

  // set event ID through storage manager
  storage->set_id(storage->run_id(),
                  storage->subrun_id(),
                  storage->event_id());

  if (!ev_pfpart or (ev_pfpart->size() == 0) ){
    print(msg::kERROR,__FUNCTION__,Form("PFPart producer %s product not found!",fInputProducer.c_str()));
    return false;
  }

  // retrieve clusters associated with this pfpart
  event_cluster *ev_cluster = nullptr;
  // association vector
  AssSet_t ass_cluster_v;
  ass_cluster_v = storage->find_one_ass(ev_pfpart->id(),ev_cluster,ev_pfpart->name());

  // if associated clusters not found -> quit and exit
  if ( !ev_cluster or (ev_cluster->size() == 0) ){
    print(msg::kERROR,__FUNCTION__,Form("No clusters found associated to PFPart w/ producer %s",fInputProducer.c_str()));
    return false;
  }

  // Retrieve clusters and feed into the algorithm
  std::vector<::cluster::cluster_params > local_clusters;
  fCRUHelper.GenerateParams(storage, ev_cluster->name(), local_clusters);

  // set clusters for the manager
  fManager.SetClusters(local_clusters);

  for (auto & clust : local_clusters)
    _params_alg.FillParams(clust);


  // Result shower holder
  std::vector< ::showerreco::Shower_t> res_shower_v;

  // vector to hold matched pairs
  std::vector<std::vector<unsigned int> > matched_pairs;
  // vector to hold pfpart indices that are actually used to try and reconstruct showers
  // this is used then to create the correct shower -> pfpart association
  std::vector<unsigned int> pfpart_indices;
  
  // Loop over pfparticles
  for (size_t pfpart_index = 0; pfpart_index < ev_pfpart->size(); ++pfpart_index) {
    
    auto const& pfp = (*ev_pfpart)[pfpart_index];

    
    // If this is not pdg=11 pfparticle, ignore
    if (pfp.PdgCode() != 11) continue;
    
    // Otherwise we store the association information in matched_pair
    matched_pairs.push_back(std::vector<unsigned int>());
    
    auto& last_pair = (*matched_pairs.rbegin());
    
    last_pair.reserve(ass_cluster_v[pfpart_index].size());
    
    // Loop over associated index for *this* pfparticle
    for (auto const& ass_index : ass_cluster_v[pfpart_index])
      last_pair.push_back(ass_index);

    pfpart_indices.push_back(pfpart_index);
    
  }//for all matches in the event
  fManager.Reconstruct(matched_pairs, res_shower_v);

  // vector for shower -> cluster association
  std::vector<std::vector<unsigned int> > recod_matched_pairs;
  // vector for shower -> pfpart association
  std::vector<std::vector<unsigned int> > shower_pfpart_v;

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
    std::vector<unsigned int> pfpart_ass = { pfpart_indices[i] };
    shower_pfpart_v.push_back( pfpart_ass);


  }// for all input cluster-paris

  if (shower_v->size() == 0)
    return true;

  // set shower -> cluster and shower -> pfpart associations
  shower_cluster_ass_v->set_association(shower_v->id(), product_id(data::kCluster, ev_cluster->name()), recod_matched_pairs);
  shower_pfpart_ass_v->set_association(shower_v->id(), product_id(data::kPFParticle, ev_pfpart->name()), shower_pfpart_v);
  return true;
}

bool ShowerReco3D::finalize() {

  fManager.Finalize(_fout);

  return true;
}

}
#endif
