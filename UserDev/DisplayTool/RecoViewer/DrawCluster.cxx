#ifndef LARLITE_DRAWCLUSTER_CXX
#define LARLITE_DRAWCLUSTER_CXX

#include "DrawCluster.h"

namespace evd {

DrawCluster::DrawCluster() {
  _name = "DrawCluster";
  _fout = 0;

}

bool DrawCluster::initialize() {

  //
  // This function is called in the beginning of event loop
  // Do all variable initialization you wish to do here.
  // If you have a histogram to fill in the event loop, for example,
  // here is a good place to create one on the heap (i.e. "new TH1D").
  //

  // Resize data holder
  if (_dataByPlane.size() != geoService -> Nviews()) {
    _dataByPlane.resize(geoService -> Nviews());
  }

  return true;
}

bool DrawCluster::analyze(larlite::storage_manager* storage) {

  //
  // Do your event-by-event analysis here. This function is called for
  // each event in the loop. You have "storage" pointer which contains
  // event-wise data. To see what is available, check the "Manual.pdf":
  //
  // http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
  //
  // Or you can refer to Base/DataFormatConstants.hh for available data type
  // enum values. Here is one example of getting PMT waveform collection.
  //
  // event_fifo* my_pmtfifo_v = (event_fifo*)(storage->get_data(DATA::PMFIFO));
  //
  // if( event_fifo )
  //
  //   std::cout << "Event ID: " << my_pmtfifo_v->event_id() << std::endl;
  //

  // clear the spots that hold the data:
  // Obtain event-wise data object pointers
  //

  // Clear out the hit data but reserve some space for the hits
  for (unsigned int p = 0; p < geoService -> Nviews(); p ++) {
    _dataByPlane.at(p).clear();
    _wireRange.at(p).first  = 99999;
    _timeRange.at(p).first  = 99999;
    _timeRange.at(p).second = -1.0;
    _wireRange.at(p).second = -1.0;

  }

  auto ev_clus = storage->get_data<larlite::event_cluster>(_producer);
  if (!ev_clus)
    return false;
  if (!ev_clus->size()) {
    // print(larlite::msg::kWARNING, __FUNCTION__,
    // Form("Skipping event %d since no cluster found...", ev_clus->event_id()));
    return false;
  }

  for (unsigned int p = 0; p < geoService -> Nviews(); p ++)
    _dataByPlane.at(p).reserve(ev_clus -> size());


  ::larlite::event_hit* ev_hit = nullptr;
  auto const& hit_index_v = storage->find_one_ass(ev_clus->id(), ev_hit, _producer);


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
  params_alg.SetMinHits(10);

  for (auto const& hit_indices : hit_index_v) {
    view = ev_hit->at(hit_indices[0]).View();

    // Make a new cluster in the data:
    _dataByPlane.at(view).push_back(Cluster2d());
    _dataByPlane.at(view).back()._is_good = true;

    // Fill the cluster params alg
    _cru_helper.GenerateParams( hit_indices, ev_hit, params);
    params_alg.FillParams(params);

    // Set the params:
    _dataByPlane.at(view).back()._params = params;

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

      _dataByPlane.at(view).back().emplace_back(
        Hit(hit.WireID().Wire,
            hit.PeakTime(),
            hit.Integral(),
            hit.RMS(),
            hit.StartTick(),
            hit.PeakTime(),
            hit.EndTick(),
            hit.PeakAmplitude()
           ));

  
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


  return true;
}

bool DrawCluster::finalize() {

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

DrawCluster::~DrawCluster() {
}

}
#endif
