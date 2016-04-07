#ifndef LARLITE_MCCLUSTERER_CXX
#define LARLITE_MCCLUSTERER_CXX

#include "MCClusterer.h"
#include "DataFormat/hit.h"
#include "DataFormat/simch.h"
#include "DataFormat/cluster.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/mctrack.h"


namespace larlite {

  MCClusterer::MCClusterer() {
    _name="MCClusterer";
    _fout=0;
    _cluster_producer="rawcluster";
    _mc_energy_min = 10; // MeV
    _min_input_cluster_n_hits; // number of hits
  }

  bool MCClusterer::initialize() {

    return true;
  }
  
  bool MCClusterer::analyze(storage_manager* storage) {

    // Retrieve mcshower data product
    auto ev_mcs      = storage->get_data<event_mcshower>("mcreco");
    // Retrieve mctrack data product
    auto ev_mct      = storage->get_data<event_mctrack>("mcreco");
    // Retrieve mchit data product
    event_hit* ev_hit = nullptr;
    // Retrieve simch data product
    auto ev_simch   = storage->get_data<event_simch>("largeant");

    // Retrieve cluster data product (output)
    auto ev_rawcluster = storage->get_data<event_cluster>(_cluster_producer);

    // Retrieve cluster data product (output)
    auto ev_mccluster = storage->get_data<event_cluster>("mccluster");
    // association information to go here
    auto cluster_ass_v = storage->get_data<event_ass>(ev_mccluster->name());

    // grab hits from associations to rawcluster
    auto const& ass_hit_v = storage->find_one_ass(ev_rawcluster->id(), ev_hit, ev_rawcluster->name());

    // since we are creating a new data product,
    // reload the event information
    storage->set_id(ev_hit->run(),ev_hit->subrun(),ev_hit->event_id()); 

    if (!ev_simch or ev_simch->size() == 0){
      std::cout << "No simch data-product -> exit" << std::endl;
      return false;
    }

    if (!ev_rawcluster or ev_rawcluster->size() == 0){
      std::cout << "No rawcluster data-product -> exit" << std::endl;
      return false;
    }

    // used track id vector
    std::vector<unsigned int> used_trk_id;

    // Create G4 track ID vector for MCShowers & MCTracks in the event
    std::vector<std::vector<unsigned int> > g4_trackid_v;
    std::vector<unsigned int> mc_index_v;
    // keep track of all the shower and track trackIDs
    g4_trackid_v.reserve(ev_mcs->size()+ev_mct->size());
    // for each mcshower, find the geant trackIDs associated with that shower
    for (size_t mc_index = 0; mc_index < ev_mcs->size(); ++mc_index) {
      auto const& mcs = (*ev_mcs)[mc_index];
      double energy = mcs.DetProfile().E();
      std::vector<unsigned int> id_v;
      id_v.reserve(mcs.DaughterTrackID().size());
      if ( _mc_energy_min < energy ) {
	for (auto const& id : mcs.DaughterTrackID()) {
	  if (id == mcs.TrackID()) continue;
	  used_trk_id.push_back(id);
	  if (id == 1)
	    std::cout << "ENTERING TrackID == 1!" << std::endl;
	  id_v.push_back(id);
	}
	id_v.push_back(mcs.TrackID());
	std::cout << "MCShower id : "  << mcs.TrackID()
		  << " w/ PDG code : " << mcs.PdgCode()
		  << " w/ energy : "   << energy << std::endl;
	used_trk_id.push_back(mcs.TrackID());
	g4_trackid_v.push_back(id_v);
	mc_index_v.push_back(mc_index);
      }// if this shower has enough energy
    }

    // for each mctrack, add it's track id info to the list of IDs to look at
    for (size_t mc_index = 0; mc_index < ev_mct->size(); ++mc_index) {
      auto const& mct = (*ev_mct)[mc_index];
      auto mct_id = mct.TrackID();
      // if this track id has been used -> ignore
      if (std::find(used_trk_id.begin(),used_trk_id.end(),mct_id) != used_trk_id.end())
	continue;
      if (mct.size() < 2) continue;
      auto energy = mct[0].E() - mct[mct.size()-1].E();
      // the list of track IDs for an MCTrack consists only of
      // the track's trackID.
      std::vector<unsigned int> id_v = {mct_id};
      if ( _mc_energy_min < energy ) {
	std::cout << "MCTrack id : "   << mct_id
		  << " w/ PDG code : " << mct.PdgCode()
		  << " w/ energy : "   << energy << std::endl;
	g4_trackid_v.push_back(id_v);
	mc_index_v.push_back(mc_index+ev_mcs->size());
      }
    }

    std::cout << "Finished registering all Track IDs" << std::endl;

    // reset MCBTAlg
    _bt_algo.Reset(g4_trackid_v,*ev_simch);

    // create a vector of vectors in which to store the hit associations
    // for each cluster produced
    std::vector<std::vector<unsigned int> > cluster_hit_v;
    cluster_hit_v.resize( 3 * mc_index_v.size() );
    // vector to save the plane information for each cluster
    std::vector<larlite::geo::View_t> cluster_plane_v(cluster_hit_v.size(),
						      larlite::geo::View_t::kUnknown);

    // loop through hits, use the back-tracker to find which MCX object
    // it should belong to, and add that hit to the cluster that is
    // indended for that MCX object
    // only use hits from association to rawclusters
    for (size_t i=0; i < ass_hit_v.size(); i ++){
      // ignore cluster with < some number of hits
      std::cout << "associated hits : " << ass_hit_v[i].size() << std::endl;
      if (ass_hit_v[i].size() < _min_input_cluster_n_hits) continue;
      for (size_t k=0; k < ass_hit_v[i].size(); k++){
	
	auto const& hit_idx = ass_hit_v[i][k];
	auto const& hit    = ev_hit->at(hit_idx);
	
	auto const& ch     = hit.Channel();
	auto const& tstart = hit.PeakTime() - 3*hit.RMS() + 3050;
	auto const& tend   = hit.PeakTime() + 3*hit.RMS() + 3050;
	auto const& pl     = hit.View();
	// create a wire-range object with channel + (start,end) time info for the hit
	::btutil::WireRange_t wr(ch,tstart,tend);
	// use the back-tracker to return a vector of track IDs associated to this hit
	// contents of return vector are proportional to the fraction of the hit's
	// charge that is associated with the MCX id at that element's position
	// vector ordered such that
	// mcq_v [ index ] corresponds to index wihtin mc_index_v
	auto mcq_v = _bt_algo.MCQ(wr);
	// find entry with largest value -> this lets us know which MCX object
	// to assign this hit to and thus which cluster this hit should
	// be associated with
	size_t idx  = 0;
	double max_edep = 0;
	for (size_t j=0; j < mcq_v.size(); j++){
	  if (mcq_v[j] > max_edep){
	    max_edep = mcq_v[j];
	    idx  = j;
	  }
	}
	std::cout << "Edep for this hit : " << max_edep << std::endl;
	// if the maximum amount of charge is 0
	// ignore this hit
	if (max_edep == 0)
	  continue;
	// if the idx found is == to mcq_v.size() - 1
	// this means that most of the charge belongs to 
	// none of the MCX objects we are interested in
	// -> ignore this hit
	if (idx == mcq_v.size() -1 )
	  continue;
	// if not, associate this hit with the appropriate cluster
	// ( "i" is the hit index )
	cluster_hit_v[ pl * mc_index_v.size() + idx ].push_back( hit_idx );
	cluster_plane_v[ pl * mc_index_v.size() + idx ] = pl;
      }// for all hits in cluster
    }// for all rawclusters

    // now create the clusters and save them as data-products

    // create a cluster for each entry in g4_trackid_v -> the vector
    // of MCShower / MCTrack trackIDs that we want to keep
    // also create a new vector for cluster associations
    // to be filled only if the cluster for the corresponding
    // hit list will be created after meeting the minimum
    // num of hit requirement
    std::vector<std::vector<unsigned int> > cluster_hit_ass_v;
    for (size_t idx=0; idx < cluster_hit_v.size(); idx++){

      std::cout << "saving cluster w/ "  << cluster_hit_v[idx].size() << std::endl;
      
      // if the hits for this cluster is > some threshold
      if (cluster_hit_v[idx].size() < 10)
	continue;

      // create a new cluster
      cluster clus;
      clus.set_n_hits(cluster_hit_v[idx].size());
      clus.set_view(cluster_plane_v[idx]);
      ev_mccluster->push_back(clus);
      cluster_hit_ass_v.push_back(cluster_hit_v[idx]);

      std::cout << "saving cluster w/ "  << cluster_hit_v[idx].size() << std::endl;
      
    }// for all clusters created

    // now save the associations for the cluster
    cluster_ass_v->set_association(ev_mccluster->id(),product_id(data::kHit,ev_hit->name()),
				   cluster_hit_ass_v);
  
    return true;
  }

  bool MCClusterer::finalize() {

  
    return true;
  }

}
#endif
