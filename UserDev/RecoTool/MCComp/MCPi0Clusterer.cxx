#ifndef LARLITE_MCPI0CLUSTERER_CXX
#define LARLITE_MCPI0CLUSTERER_CXX

#include "MCPi0Clusterer.h"
#include "DataFormat/hit.h"
#include "DataFormat/simch.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/mctruth.h"

namespace larlite {

  MCPi0Clusterer::MCPi0Clusterer() {
    _name="MCPi0Clusterer";
    _fout=0;
    _mc_energy_min = 10; // MeV
    _event = -1;

    //SetXOffset(0.0);
    _SCE = new larutil::SpaceChargeMicroBooNE();
    _time2cm = larutil::GeometryHelper::GetME()->TimeToCm();

  }

  bool MCPi0Clusterer::initialize() {

    return true;
  }
  
  bool MCPi0Clusterer::analyze(storage_manager* storage) {

    _event ++;

    std::cout<<"\n\n\nEVENT NUMBER! "<<_event <<std::endl ;

    auto ev_mcs     = storage->get_data<event_mcshower>("mcreco");
    auto ev_mct     = storage->get_data<event_mctrack>("mcreco");
    auto ev_hit       = storage->get_data<event_hit>("gaushit");
    auto ev_simch   = storage->get_data<event_simch>("largeant");

    if (!ev_mcs or ev_mcs->size() == 0){
      std::cout << "No mcshower data-product -> exit" << std::endl;
      return false;
    }

    if (!ev_mct or ev_mct->size() == 0){
      std::cout << "No mctrack data-product -> exit" << std::endl;
      return false;
    }

    if (!ev_hit or ev_hit->size() == 0){
      std::cout << "No hit data-product -> exit" << std::endl;
      return false;
    }

    if (!ev_simch or ev_simch->size() == 0){
      std::cout << "No simch data-product -> exit" << std::endl;
      return false;
    }

    auto ev_mctruth= storage->get_data<event_mctruth>("generator");
    if(!ev_mctruth || !ev_mctruth->size() ) {
      std::cout<<"Event has no mctruth info "<<std::endl;
      return false;
      }


    // Retrieve cluster data product (output)
    auto ev_mccluster = storage->get_data<event_cluster>("mccluster");
    auto cluster_ass_v = storage->get_data<event_ass>(ev_mccluster->name());

    // since we are creating a new data product,
    // reload the event information
    storage->set_id(ev_hit->run(),ev_hit->subrun(),ev_hit->event_id()); 
    
    // used track id vector
    std::vector<unsigned int> used_trk_id;

    // Create G4 track ID vector for MCShowers & MCTracks in the event
    std::vector<std::vector<unsigned int> > g4_trackid_v;
    std::vector<unsigned int> mc_index_v;
    // keep track of all the shower and track trackIDs
    g4_trackid_v.reserve(ev_mcs->size()+ev_mct->size());

    auto & truth = ev_mctruth->at(0);
    auto & nu  = truth.GetNeutrino();

    auto traj = nu.Nu().Trajectory();
    auto xvtx = traj.at(traj.size() - 1).X();
    auto yvtx = traj.at(traj.size() - 1).Y();
    auto zvtx = traj.at(traj.size() - 1).Z();

    // for each mcshower, find the geant trackIDs associated with that shower
    for (size_t mc_index = 0; mc_index < ev_mcs->size(); ++mc_index) {

      auto const& mcs = (*ev_mcs)[mc_index];
      double energy = mcs.DetProfile().E();

      auto st = mcs.Start();
      auto dist = sqrt( pow(st.X() - xvtx,2) + pow(st.Y() - yvtx,2) + pow(st.Z() - zvtx,2) );

      std::vector<unsigned int> id_v;
      id_v.reserve(mcs.DaughterTrackID().size());
      if ( dist < 0.01 && energy > _mc_energy_min && mcs.MotherPdgCode() == 111 ){
      //if ( energy > _mc_energy_min && mcs.MotherPdgCode() == 111) {
        for (auto const& id : mcs.DaughterTrackID()) {
          if (id == mcs.TrackID()) continue;
          used_trk_id.push_back(id);
          if (id == 1)
            std::cout << "ENTERING TrackID == 1!" << std::endl;
          id_v.push_back(id);
        }
        id_v.push_back(mcs.TrackID());
        //std::cout << "MCShower id : "  << mcs.TrackID()
        //	  << " w/ PDG code : " << mcs.PdgCode()
        //	  << " w/ energy : "   << energy << std::endl;
        used_trk_id.push_back(mcs.TrackID());
        g4_trackid_v.push_back(id_v);
        mc_index_v.push_back(mc_index);
      }// if this shower has enough energy
    }


    // for each mctrack, add it's track id info to the list of IDs to look at
    //for (size_t mc_index = 0; mc_index < ev_mct->size(); ++mc_index) {
    //  auto const& mct = (*ev_mct)[mc_index];
    //  auto mct_id = mct.TrackID();
    //  // if this track id has been used -> ignore
    //  if (std::find(used_trk_id.begin(),used_trk_id.end(),mct_id) != used_trk_id.end())
    //    continue;
    //  if (mct.size() < 2) continue;
    //  auto energy = mct[0].E() - mct[mct.size()-1].E();
    //  // the list of track IDs for an MCTrack consists only of
    //  // the track's trackID.
    //  std::vector<unsigned int> id_v = {mct_id};
    //  if ( _mc_energy_min < energy ) {
    //    //std::cout << "MCTrack id : "   << mct_id
    //    //	  << " w/ PDG code : " << mct.PdgCode()
    //    //	  << " w/ energy : "   << energy << std::endl;
    //    g4_trackid_v.push_back(id_v);
    //    mc_index_v.push_back(mc_index+ev_mcs->size());
    //  }
    //}

    std::cout << "Finished registering all Track IDs: " << g4_trackid_v.size()<<", and MC index: "<<mc_index_v.size()<<std::endl;
    if ( g4_trackid_v.size() == 0 ) return false; 

    // reset MCBTAlg
    _bt_algo.Reset(g4_trackid_v,*ev_simch);

    // create a vector of vectors in which to store the hit associations
    // for each cluster produced
    std::vector<std::vector<unsigned int> > cluster_hit_v;
    cluster_hit_v.resize( 3 * mc_index_v.size() );
    // vector to save the plane information for each cluster
    std::vector<larlite::geo::View_t> cluster_plane_v(cluster_hit_v.size(), larlite::geo::View_t::kUnknown);

    // loop through hits, use the back-tracker to find which MCX object
    // it should belong to, and add that hit to the cluster that is
    // indended for that MCX object
    // only use hits from association to rawclusters
    float _max_time = -9; 
    float max_pl = -9; 

    for (size_t i=0; i < ev_hit->size(); i ++){
	
	auto const& hit_idx = i;
	auto const& hit    = ev_hit->at(hit_idx);
	
	auto const& ch     = hit.Channel();

	auto const& tstart = hit.PeakTime() - 2*hit.RMS() ;//+ 7298; //2255;//3050;
	auto const& tend   = hit.PeakTime() + 2*hit.RMS() ;//+ 7298; //2255;//3050;
	auto const& pl     = hit.View();

        if ( hit.PeakTime() > _max_time ) { _max_time = hit.PeakTime() ; max_pl = hit.View(); }

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
    }// for all hits 

    // now create the clusters and save them as data-products

    // create a cluster for each entry in g4_trackid_v -> the vector
    // of MCShower / MCTrack trackIDs that we want to keep
    // also create a new vector for cluster associations
    // to be filled only if the cluster for the corresponding
    // hit list will be created after meeting the minimum
    // num of hit requirement
    std::vector<std::vector<unsigned int> > cluster_hit_ass_v;
    for (size_t idx=0; idx < cluster_hit_v.size(); idx++){

      // if the hits for this cluster is > some threshold
      if (cluster_hit_v[idx].size() < 5) //10)
	continue;

      // create a new cluster
      cluster clus;
      clus.set_n_hits(cluster_hit_v[idx].size());
      clus.set_view(cluster_plane_v[idx]);
      ev_mccluster->push_back(clus);
      cluster_hit_ass_v.push_back(cluster_hit_v[idx]);

    }// for all clusters created

    // now save the associations for the cluster
    cluster_ass_v->set_association(ev_mccluster->id(),product_id(data::kHit,ev_hit->name()),
				   cluster_hit_ass_v);
  
    return true;
  }

  bool MCPi0Clusterer::finalize() {
  
    return true;
  }

}
#endif
