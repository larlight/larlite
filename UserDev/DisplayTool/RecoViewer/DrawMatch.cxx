#ifndef LARLITE_DRAWMATCH_CXX
#define LARLITE_DRAWMATCH_CXX

#include "DrawMatch.h"
#include "CMTool/CMTAlgMatch/CFAlgoTimeOverlap.h"
#include "CMTool/CMTAlgPriority/CPAlgoNHits.h"

namespace evd {

  DrawMatch::DrawMatch() {
    _name = "DrawMatch";
    _fout = 0;
  }

  bool DrawMatch::initialize() {


    // Resize data holder
    if (_dataByPlane.size() != geoService -> Nviews()) {
      _dataByPlane.resize(geoService -> Nviews());
    }


    return true;
  }

  void DrawMatch::set_proto_shower_alg(::protoshower::ProtoShowerAlgBase * alg) {
    _ps_helper.setProtoShowerAlg(alg);
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

  // Loop over the clusters and fill the necessary vectors.
    // grab pf particle
    auto ev_pfpart = storage->get_data<larlite::event_pfpart>(_producer);

    // if no pfparticle skip the event
    if (!ev_pfpart or !ev_pfpart->size()) {
      print(larlite::msg::kWARNING, __FUNCTION__, "Skipping event since no pfparticle found...");
      return false;
    }

    // grab clusters associated with the pfparticles
    larlite::event_cluster *ev_cluster = nullptr;

    auto const& cluster_ass_v = storage->find_one_ass(ev_pfpart->id(), ev_cluster, _producer);

    // if no pfparticle skip the event
    if (cluster_ass_v.size() == 0) {
      print(larlite::msg::kWARNING, __FUNCTION__, "Skipping event since no cluster found...");
      return false;
    }

    for (unsigned int p = 0; p < geoService -> Nviews(); p ++)
      _dataByPlane.at(p).reserve(ev_cluster -> size());



    ::larlite::event_hit* ev_hit = nullptr;
    auto const& hit_index_v = storage->find_one_ass(ev_cluster->id(), ev_hit, ev_cluster->name());

    if (!ev_hit or !ev_hit->size()) {
      print(larlite::msg::kWARNING, __FUNCTION__, "Skipping event since no hit found...");
      return false;
    }

    // Loop over the clusters and fill the necessary vectors.

    std::vector<::protoshower::ProtoShower> proto_showers;

    _ps_helper.GenerateProtoShowers( storage,
				     _producer,
				     proto_showers);

    // Now, build the evd clusters out of these events.
    int i_pfpart = 0;
    for (auto & pfpart_clust_set : cluster_ass_v) {

      // Make sure that the cluster set is evenly padded
      for (int p = 0; p < geoService->Nplanes(); p ++) {
	_dataByPlane.at(p).push_back(Cluster2d());
      }

      // Loop over the clusters and build the cluster2D:
      for (auto & i_clust : pfpart_clust_set) {

	// Need the hit indexes:
	auto const& hit_indices = hit_index_v.at(i_clust);

	int view = ev_hit->at(hit_indices[0]).View();


	// Loop over the hits in this cluster:
	_dataByPlane.at(view).back()._is_good = true;

	// Set the cluster params:
	if (proto_showers.at(i_pfpart).hasCluster2D()) {
	  // Figure out which params matches this plane:
	  for (auto const& params : proto_showers.at(i_pfpart).params()) {
	    if ( params.plane_id.Plane == view) {
	      _dataByPlane.at(view).back()._params = params;
	      break;
	    }
	  }
	}

	// Now set the hits of the cluster:

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


      }

      // for (auto const& hit_indices : hit_index_v) {
      //   int view = ev_hit->at(hit_indices[0]).View();

      //   // Make a new cluster in the data:
      //   _pass.push_back(Cluster2d());
      //   _pass.back()._is_good = true;

      //   // Fill the cluster params alg
      //   _cru_helper.GenerateParams( hit_indices, ev_hit, params);
      //   params_alg.FillParams(params);

      //   // Set the params:
      //   _pass.back()._params = params;


      //   cluster_index[view] ++;

      // }
    }




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
  }

}
#endif
