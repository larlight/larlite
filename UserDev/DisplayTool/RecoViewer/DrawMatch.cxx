#ifndef LARLITE_DRAWMATCH_CXX
#define LARLITE_DRAWMATCH_CXX

#include "DrawMatch.h"
#include "CMTool/CMTAlgMatch/CFAlgoTimeOverlap.h"
#include "CMTool/CMTAlgPriority/CPAlgoNHits.h"

namespace evd {

DrawMatch::DrawMatch() {
  _name = "DrawMatch";
  _fout = 0;
  _time_ratio_cut  = 0.001;
  _start_time_cut  = 10 ;
  _require_3planes = true ;
  _match_mgr = nullptr;
}

bool DrawMatch::initialize() {


  // Resize data holder
  if (_dataByPlane.size() != geoService -> Nviews()) {
    _dataByPlane.resize(geoService -> Nviews());
  }


  return true;
}

bool DrawMatch::analyze(larlite::storage_manager* storage) {


  // Clear out the hit data but reserve some space for the hits
  for (unsigned int p = 0; p < geoService -> Nviews(); p ++) {
    _dataByPlane.at(p).clear();

    _wireRange.at(p).first  = 99999;
    _timeRange.at(p).first  = 99999;
    _timeRange.at(p).second = -1.0;
    _wireRange.at(p).second = -1.0;

  }

  if (_match_mgr == nullptr){
    std::cout << "ERROR: Match Manager not set, can not make matches.  Returning.\n";
    return false;
  }

  auto ev_clus = storage->get_data<larlite::event_cluster>(_producer);
  if (!ev_clus)
    return false;
  if (!ev_clus->size()) {
    print(larlite::msg::kWARNING, __FUNCTION__,
          Form("Skipping event %d since no cluster found...", ev_clus->event_id()));
    return false;
  }
  
  for (unsigned int p = 0; p < geoService -> Nviews(); p ++)
    _dataByPlane.at(p).reserve(ev_clus -> size());

  std::vector < Cluster2d> _pass;
  _pass.reserve(ev_clus -> size());

  ::larlite::event_hit* ev_hit = nullptr;
  auto const& hit_index_v = storage->find_one_ass(ev_clus->id(), ev_hit, _producer);

  // std::cout << "Hit _producer is " << ev_hit -> name() << std::endl;

  if (!ev_hit) {
    std::cout << "Did not find hit data product"
              << "!" << std::endl;
    return false;
  }


  if (!hit_index_v.size())
    return false;

  // Loop over the clusters and fill the necessary vectors.
  // I don't know how clusters are stored so I'm taking a conservative
  // approach to packaging them for drawing
  std::vector<int> cluster_index;
  cluster_index.resize(geoService -> Nviews());

  int view = ev_hit->at(hit_index_v.front()[0]).View();

  cluster::DefaultParamsAlg params_alg ;
  cluster::cluster_params params;
  params_alg.SetVerbose(false);
  params_alg.SetDebug(false);
  params_alg.SetMinHits(20);

  std::vector<::cluster::cluster_params> pass_clusters;

  for (auto const& hit_indices : hit_index_v) {
    view = ev_hit->at(hit_indices[0]).View();

    // Make a new cluster in the data:
    _pass.push_back(Cluster2d());
    _pass.back()._is_good = true;

    // Fill the cluster params alg
    _cru_helper.GenerateParams( hit_indices, ev_hit, params);
    params_alg.FillParams(params);

    // Set the params:
    _pass.back()._params = params;

    for (auto const& hit_index : hit_indices) {

      auto & hit = ev_hit->at(hit_index);
      // if (view == 0){
      //   std::cout << "Got a hit, seems to be view " << view
      //             << " and cluster " << cluster_index[view]
      //             << " at " << ev_hit->at(hit_index).WireID().Wire
      //             << ", " << ev_hit->at(hit_index).PeakTime()
      //             << std::endl;
      // }
      // Hit(float w, float t, float c, float r) :

      _pass.back().emplace_back(Hit(hit.WireID().Wire,
                                    hit.PeakTime(),
                                    hit.PeakAmplitude(),
                                    hit.RMS()));


      // Determine if this hit should change the view range:
      if (hit.WireID().Wire > _wireRange.at(view).second)
        _wireRange.at(view).second = hit.WireID().Wire;
      if (hit.WireID().Wire < _wireRange.at(view).first)
        _wireRange.at(view).first = hit.WireID().Wire;
      if (hit.PeakTime() > _timeRange.at(view).second)
        _timeRange.at(view).second = hit.PeakTime();
      if (hit.PeakTime() < _timeRange.at(view).first)
        _timeRange.at(view).first = hit.PeakTime();

    }

    cluster_index[view] ++;

  }


  for (auto & clust : _pass) {
    pass_clusters.push_back(clust.params());
  }


  // for (auto & c0 : _pass.at(0))
  //   pass_clusters.push_back(c0.params());
  // for (auto & c1 : _pass.at(1))
  //   pass_clusters.push_back(c1.params());
  // for (auto & c2 : _pass.at(2))
  //   pass_clusters.push_back(c2.params());

  // _pass.at(2).push_back(Cluster2d());
  // _pass.at(2).back()._is_good = false;

  //std::cout<<"Hits for fake cluster: "<<_pass.at(2).back().params().N_Hits<<std::endl ;


  _match_mgr -> Reset();
  _match_mgr->SetClusters(pass_clusters);
  _match_mgr -> Process();
  auto scores = _match_mgr->GetBookKeeper().GetResult();


//   std::vector<std::vector<int>> new_scores ;
//   new_scores.resize(scores.size()) ;

//   for (int i = 0; i < scores.size(); i++)
//     // new_scores.at(i).resize(3, -1);
//     new_scores.at(i).resize(geoService->Nview(), -1);

//   for (int i = 0; i < scores.size(); i++) {
//     for (int j = 0; j < scores.at(i).size(); j++) {

//       auto pl = pass_clusters.at(scores.at(i)[j]).plane_id.Plane ;
//       new_scores.at(i)[pl] = scores.at(i)[j] ;

//       //    std::cout<<"Plane is: "<<pl<<std::endl;

//       if ( new_scores.at(i)[pl] != -1 && pl == 1)
//         new_scores.at(i)[pl] -= _pass.at(0).size();

//       else if ( new_scores.at(i)[pl] != -1 && pl == 2)
//         new_scores.at(i)[pl] -= ( _pass.at(0).size() + _pass.at(1).size() );

//     }
// //  std::cout<< "New_scores are: "<<new_scores.at(i)[0]<<", "<<new_scores.at(i)[1]<<", "<<new_scores.at(i)[2]<<std::endl;
//   }

  // Put the matched clusters into _dataByPlane
  // Keep track of how many matches are made
  size_t matches = 0;
  for (auto & set : scores) {
    // The indexs in set are matched clusters, so copy the cluster2d objects
    // into _dataByPlane based on that indexing:
    size_t copied = 0;
    for (auto & index : set) {
      auto & plane = pass_clusters.at(index).plane_id.Plane;
      _dataByPlane.at(plane).push_back(_pass.at(index));
      copied ++;
    }
    matches ++;
    if (copied < geoService -> Nviews()) {
      for (auto & vec : _dataByPlane) {
        vec.resize(matches);
      }
    }
  }

//   for (int i = 0; i < new_scores.size(); i++) {
//     for (int j = 0; j < new_scores.at(i).size(); j++) {
//       if (new_scores.at(i)[j] == -1)
//         _dataByPlane.at(j).push_back(_pass.at(2).back()) ;//[score_array[i]]);
//       else
//         _dataByPlane.at(j).push_back(_pass.at(j)[new_scores.at(i)[j]]);

// //  std::cout<<"What are the score. Geez. "<<new_scores.at(i)[j]<< std::endl ;
//     }
//   }
//std::cout<<"Size of each dataBYPLane: "<<_dataByPlane.at(0).size()<<", "<<_dataByPlane.at(1).size()<<", "<<_dataByPlane.at(2).size()<<std::endl ;

  // Now that clusters are done filling, go through and pad out the rest of the data
  // Just in case they aren't the same length:
  // int max_val = cluster_index.front();
  // for (auto & val : cluster_index) {
  //   if (val > max_val ) {
  //     max_val = val;
  //   }
  // }

  // for (auto & planeOfClusters : _dataByPlane) {
  //   if (planeOfClusters.size() < max_val) {
  //     planeOfClusters.resize(max_val);
  //   }
  // }


  return true;
}

bool DrawMatch::finalize() {

  // This function is called at the end of event loop.
  // Do all variable finalization you wish to do here.
  // If you need, you can store your ROOT class instance in the outputmake
  // file. You have an access to the output file through "_fout" pointer.
  //
  // Say you made a histogram pointer h1 to store. You can do this:
  //
  // if(_fout) { _fout->cd(); h1->Write(); }
  //
  // else
  //   print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
  //
  //


  return true;
}

DrawMatch::~DrawMatch() {
  delete _match_mgr;
}

}
#endif
