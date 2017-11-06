#ifndef RECOTOOL_SHOWERRECO3D_CXX
#define RECOTOOL_SHOWERRECO3D_CXX

#include "ShowerReco3D.h"


#include "DataFormat/DataFormat-TypeDef.h"

namespace larlite {

ShowerReco3D::ShowerReco3D() : ana_base()
{
  _name = "ShowerReco3D";
  fInputProducer   = "fuzzycluster";
  fOutputProducer  = "showerreco";
  _require_shrlike = true;
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
  larlite::event_ass * shower_pfpart_ass_v  = storage->get_data<event_ass>(shower_v->name());
  // create association for showers -> cluster
  larlite::event_ass * shower_cluster_ass_v = 0;
  // create association for showers -> spacepoints
  larlite::event_ass * shower_sps_ass_v = 0;
  // create association for showers -> vertex
  larlite::event_ass * shower_vertex_ass_v = 0;


  // set event ID through storage manager
  storage->set_id(storage->run_id(),
                  storage->subrun_id(),
                  storage->event_id());

  if (!ev_pfpart or (ev_pfpart->size() == 0) ) {
    //print(msg::kERROR, __FUNCTION__,
    //    Form("PFPart producer %s product not found!",
    //         fInputProducer.c_str()));
    return false;
  }

  std::cout << "Run: " << storage->run_id() << ", Subrun: " << storage->subrun_id()
            << ", Event: " << storage->event_id() << std::endl;

  // This item holds the list of PFParticles tagged as showers (11)
  // We only run reco on particles tagged that way.

  std::vector<unsigned int> showerLikePFParts;

  unsigned int index = 0;
  for (auto & part : *ev_pfpart) {
    if ( (part.PdgCode() == 11) || (!_require_shrlike) ) {
      showerLikePFParts.push_back(index);
    }
    index ++;
  }

  std::cout << "This event has " << showerLikePFParts.size()
            << " particles of " << ev_pfpart->size()
            <<  "tagged as showers." << std::endl;


  // retrieve clusters associated with this pfpart
  // Cluster are retrieved for saving the associations at the end.
  event_cluster *ev_cluster = nullptr;
  // association vector
  AssSet_t ass_cluster_v;
  ass_cluster_v = storage->find_one_ass(ev_pfpart->id(), ev_cluster, ev_pfpart->name());

  // if associated clusters not found -> quit and exit
  if ( !ev_cluster or (ev_cluster->size() == 0) ) {
    print(msg::kERROR, __FUNCTION__,
          Form("No clusters found associated to PFPart w/ producer %s",
               fInputProducer.c_str()));
    // return false;
  }
  else {
    shower_cluster_ass_v = storage->get_data<event_ass>(shower_v->name());
  }


  // retrieve vertexes associated with this pfpart
  // Vertexes are retrieved for saving the associations at the end.
  event_vertex *ev_vertex = nullptr;
  // association vector
  AssSet_t ass_vertex_v;
  ass_vertex_v = storage->find_one_ass(ev_pfpart->id(), ev_vertex, ev_pfpart->name());

  // if associated clusters not found -> quit and exit
  if ( !ev_vertex or (ev_vertex->size() == 0) ) {
    print(msg::kDEBUG, __FUNCTION__,
          Form("No vertexes found associated to PFPart w/ producer %s",
               fInputProducer.c_str()));
    // return false;
  }
  else {
    shower_vertex_ass_v  = storage->get_data<event_ass>(shower_v->name());
  }

  // retrieve spacepoints associated with this pfpart
  // Spacepoints are retrieved for saving the associations at the end.
  event_spacepoint *ev_sps = nullptr;
  // association vector
  AssSet_t ass_sps_v;
  ass_sps_v = storage->find_one_ass(ev_pfpart->id(), ev_sps, ev_pfpart->name());

  // if associated clusters not found -> quit and exit
  if ( !ev_sps or (ev_sps->size() == 0) ) {
    print(msg::kDEBUG, __FUNCTION__,
          Form("No spacepoints found associated to PFPart w/ producer %s",
               fInputProducer.c_str()));
    // return false;
  }
  else {
    shower_sps_ass_v = storage->get_data<event_ass>(shower_v->name());
  }


  // Now use ProtoShowerHelper to generate the ProtoShowers:

  std::vector<protoshower::ProtoShower> proto_showers;


  _ps_helper.GenerateProtoShowers( storage,
                                   fInputProducer,
                                   proto_showers,
                                   showerLikePFParts);

  // Result shower holder
  std::vector< ::showerreco::Shower_t> res_shower_v;

  fManager.SetProtoShowers(proto_showers);


  fManager.Reconstruct(res_shower_v);

  // vector for shower -> cluster association
  std::vector<std::vector<unsigned int> > shower_cluster_v;

  // vector for shower -> pfpart association
  std::vector<std::vector<unsigned int> > shower_pfpart_v;

  // vector for shower -> vertex association
  std::vector<std::vector<unsigned int> > shower_vertex_v;

  // vector for shower -> sps association
  std::vector<std::vector<unsigned int> > shower_sps_v;

  for (size_t i = 0; i < res_shower_v.size(); i++) {

    // The res_shower_v vector is equal in length to the
    // pfparticle set.
    // So associations from Shower -> X can be copied from
    // the associations from pfpart -> X

    auto const& res_shower = res_shower_v[i];


    if (res_shower.fPassedReconstruction == false) {
      continue;
    }

    // filter out showers with garbage values
    if (res_shower.fXYZStart.Mag2()  == 0) {
      continue;
    }
    if (res_shower.fDCosStart.Mag2() == 0) {
      continue;
    }


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
    s.set_width                 ( res_shower.fWidth                 );
    s.set_opening_angle         ( res_shower.fOpeningAngle          );

    shower_v->push_back(s);

    // std::cout << "ass_cluster_v.size() " << ass_cluster_v.size()
    //           << " showerLikePFParts[i] " << showerLikePFParts[i]
    //           << std::endl;


    shower_cluster_v.push_back(ass_cluster_v[showerLikePFParts[i]]);
    
    if (shower_vertex_ass_v){
      shower_vertex_v.push_back(ass_vertex_v[showerLikePFParts[i]]);
    }
    if (shower_sps_ass_v){
      shower_sps_v.push_back(ass_sps_v[showerLikePFParts[i]]);
    }

    std::vector<unsigned int> pfpart_ass = { (unsigned int) showerLikePFParts[i] };
    shower_pfpart_v.push_back( pfpart_ass);

  }// for all input cluster-paris

  // std::cout << "shower_v -> size() is " << shower_v -> size() << std::endl;

  if (shower_v->size() == 0)
    return true;

  // std::cout << "Saving associations" << std::endl;

  // set shower -> cluster and shower -> pfpart associations

  shower_pfpart_ass_v->set_association(shower_v->id(),
                                       product_id(data::kPFParticle,
                                           ev_pfpart->name()),
                                       shower_pfpart_v);
  if ( shower_cluster_ass_v ) {
    shower_cluster_ass_v->set_association(shower_v->id(),
                                          product_id(data::kCluster,
                                              ev_cluster->name()),
                                          shower_cluster_v);
  }
  if ( shower_vertex_ass_v ) {
    shower_vertex_ass_v->set_association(shower_v->id(),
                                         product_id(data::kVertex,
                                             ev_pfpart->name()),
                                         shower_pfpart_v);
  }
  if ( shower_sps_ass_v ) {
    shower_sps_ass_v->set_association(shower_v->id(),
                                      product_id(data::kSpacePoint,
                                          ev_pfpart->name()),
                                      shower_pfpart_v);
  }

  // std::cout << "Finished the event!" << std::endl;
  return true;
}

bool ShowerReco3D::finalize() {

  fManager.Finalize(_fout);

  return true;
}

}
#endif
