#ifndef RECOTOOL_HITCLUSTER_CXX
#define RECOTOOL_HITCLUSTER_CXX

#include "HitCluster.h"


namespace larlite {

HitCluster::HitCluster()
{
  _name = "HitCluster";
  _fout = 0;
  _input_producer = "gaushit";
  _input_vertex_producer = "";
  _input_sps_producer = "";
  _output_producer = "";
  _write_output = false;
}

bool HitCluster::initialize() {

  return true;
}

bool HitCluster::analyze(storage_manager* storage) {

  event_hit* ev_hits = storage->get_data<event_hit>( _input_producer );
  event_vertex* ev_vertex = storage->get_data<event_vertex>( _input_vertex_producer );

  // Check if the input producer is empty
  if ( !ev_hits or ( ev_hits->size() == 0 ) ) {
    print( msg::kWARNING, __FUNCTION__,
           Form("No input hit! Saving empty event to output ttree..."));
    // return false;
  }

  // Initialize the output cluster data product
  if ( _output_producer.empty() ) _output_producer = Form( "%sMerged", _input_producer.c_str() );
  auto pfpart_v = storage->get_data<event_pfpart>( _output_producer );
  auto cluster_v = storage->get_data<event_cluster>( _output_producer );
  auto vertex_v = storage->get_data<event_vertex>( _output_producer );
  auto sps_v = storage->get_data<event_spacepoint>( _output_producer );
  auto pfpart_cluster_ass = storage->get_data<event_ass>( pfpart_v->name() );
  larlite::event_ass * pfpart_sps_ass = 0;
  larlite::event_ass * pfpart_vertex_ass = 0;
  larlite::event_ass * cluster_hit_ass = storage->get_data<event_ass>( cluster_v->name() );

  // set event ID through storage manager
  storage->set_id( ev_hits->run(), ev_hits->subrun(), ev_hits->event_id() );
  // std::cout << "Run: " << ev_hits->run() << ", Subrun: " << ev_hits->subrun() 
  //           << ", Event: " << ev_hits->event_id() << std::endl;

  // retrieve spacepoints associated with this pfpart
  // Spacepoints are retrieved for saving the associations at the end.
  event_spacepoint *ev_sps = nullptr;
  // association vector
  AssSet_t ass_sps_v;
  if ( _input_sps_producer.empty() ) {
    _input_sps_producer = _input_producer;
    ass_sps_v = storage->find_one_ass( ev_hits->id(), ev_sps, _input_sps_producer );
  } else ev_sps = storage->get_data<event_spacepoint>( _input_sps_producer );

  // if associated clusters not found -> quit and exit
  if ( !ev_sps or ( ev_sps->size() == 0) ) {
    print( msg::kWARNING, __FUNCTION__,
           Form("No spacepoints found associated to hits w/ producer %s",
                _input_sps_producer.c_str()) );
    return false;
  } else {
    pfpart_sps_ass = storage->get_data<event_ass>( pfpart_v->name() );
  }

  if ( !ev_vertex or ( ev_vertex->size() == 0 ) ) {
    print( msg::kWARNING, __FUNCTION__, Form("No input vertex!"));
    return false;
  } else {
    pfpart_vertex_ass = storage->get_data<event_ass>( pfpart_v->name() );
  }

  // Merged association
  std::vector< std::vector< unsigned int > > pfpart_sps_ass_v;
  std::vector< std::vector< unsigned int > > pfpart_vertex_ass_v;
  std::vector< std::vector< unsigned int > > pfpart_cluster_ass_v;
  std::vector< std::vector< unsigned int > > cluster_hit_ass_v;

  pfpart_sps_ass_v.resize( 1 );
  pfpart_vertex_ass_v.resize( 1 );
  pfpart_cluster_ass_v.resize( 1 );

  // Loop over hits
  std::map< larlite::geo::View_t, std::vector< unsigned int > > hits;
  for ( size_t i = 0; i < ev_hits->size(); ++i ) {
    auto ihit = ev_hits->at(i);
    larlite::geo::View_t iview = ihit.View();
    hits[iview].push_back( i );
    // Fill the spacepoints in the case there is association between hits and spacepoints
    if ( !ass_sps_v.empty() ) {
      auto ispss = ass_sps_v[i];
      auto& pfpart_sps_ass_v_holder = pfpart_sps_ass_v[0];
      for ( auto isps : ispss ) {
        auto psps = std::find( pfpart_sps_ass_v_holder.begin(), pfpart_sps_ass_v_holder.end(), isps );
        if ( psps == pfpart_sps_ass_v_holder.end() ) {
          pfpart_sps_ass_v_holder.push_back( isps );
          sps_v->push_back( ev_sps->at(isps) );
        }
      }
    }
  }

  // Fill the spacepoints in the case there is no association between hits and spacepoints
  if ( ass_sps_v.empty() ) {
    auto& pfpart_sps_ass_v_holder = pfpart_sps_ass_v[0];
    for ( size_t i = 0; i < ev_sps->size(); ++i ) {
      pfpart_sps_ass_v_holder.push_back( i );
      sps_v->push_back( ev_sps->at(i) );
    }
  }

  // Fill the vertices
  if ( ev_vertex && ( !ev_vertex->empty() ) ) {
    vertex_v->resize( ev_vertex->size() );
    *vertex_v = *ev_vertex;
    for ( size_t i = 0; i < ev_vertex->size(); ++i ) {
      auto& pfpart_vertex_ass_v_holder = pfpart_vertex_ass_v[0];
      pfpart_vertex_ass_v_holder.push_back( i );
    }
  }

  // Fill hits into clusters, and set the association
  std::vector< unsigned int > iclusters;
  std::map< larlite::geo::View_t, std::vector< unsigned int > >::iterator it;
  for ( it = hits.begin(); it != hits.end(); ++it ) {
    larlite::cluster cluster;
    auto hits_per_plane = it->second;
    cluster.set_n_hits( hits_per_plane.size() );
    cluster.set_view( it->first );
    iclusters.push_back( cluster_v->size() );
    cluster_v->push_back( cluster );
    cluster_hit_ass_v.push_back( hits_per_plane );
  } // for ( auto it = hits.begin(); it != hits.end(); ++it )

  // Create a PFParticle, the parent and daughter information is fake
  std::vector< size_t > daughters;
  larlite::pfpart pfparticle( 11, 0, 0, daughters );
  pfpart_v->push_back( pfparticle );
  pfpart_cluster_ass_v[0] = iclusters;

  // std::cout << "There are " << cluster_v->size() << " clusters." << std::endl;
  // std::cout << "There are " << sps_v->size() << " spacepoints." << std::endl;
  // std::cout << "There are " << vertex_v->size() << " vertices." << std::endl;

  // Set association
  cluster_hit_ass->set_association( cluster_v->id(),
                   product_id( data::kHit, ev_hits->name() ), cluster_hit_ass_v );
  pfpart_cluster_ass->set_association( pfpart_v->id(),
                      product_id( data::kCluster, cluster_v->name() ), pfpart_cluster_ass_v );
  pfpart_sps_ass->set_association( pfpart_v->id(),
                  product_id( data::kSpacePoint, sps_v->name() ), pfpart_sps_ass_v );
  pfpart_vertex_ass->set_association( pfpart_v->id(),
                     product_id( data::kVertex, vertex_v->name() ), pfpart_vertex_ass_v );


  return true;
}

bool HitCluster::finalize() {

  return true;
}
}
#endif
