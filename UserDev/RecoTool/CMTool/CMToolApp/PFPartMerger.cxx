#ifndef RECOTOOL_PFPARTMERGER_CXX
#define RECOTOOL_PFPARTMERGER_CXX

#include "PFPartMerger.h"
#include <algorithm>


namespace larlite {

PFPartMerger::PFPartMerger()
{
  _name = "PFPartMerger";
  _fout = 0;
  _input_producer = "pandoraNu";
  _input_shower_producer = "showerreco";
  _output_producer = "";
  _write_output = false;
  _doMergeByHierarchy = false;
  _doMergeByPCAndVertex = false;
}

bool PFPartMerger::initialize() {

  return true;
}

bool PFPartMerger::analyze(storage_manager* storage) {

  // if (!_write_output) return true;

  // Get the input PFParticles
  auto ev_pfpart = storage->get_data<event_pfpart>(_input_producer);

  // Initialize the output cluster data product
  if (_output_producer.empty()) _output_producer = Form("%sMerged", _input_producer.c_str());
  auto pfpart_v = storage->get_data<event_pfpart>(_output_producer);
  auto cluster_v = storage->get_data<event_cluster>(_output_producer);
  auto pfpart_cluster_ass = storage->get_data<event_ass>(pfpart_v->name());
  larlite::event_ass * pfpart_sps_ass = 0;
  larlite::event_ass * pfpart_vertex_ass = 0;
  larlite::event_ass * cluster_hit_ass = 0;

  // set event ID through storage manager
  storage->set_id(ev_pfpart->run(), ev_pfpart->subrun(), ev_pfpart->event_id());
  std::cout << "run: " << ev_pfpart->run() << ", subrun: " << ev_pfpart->subrun() 
            << ", event: " << ev_pfpart->event_id() << std::endl;

  if (!pfpart_v) {
    print(msg::kERROR, __FUNCTION__,
          Form("Failed initializing the output pfparticle data product with producer name %s!", _output_producer.c_str()));
    return false;
  }
  // without the above (!out_pfpart_v) check, code crashes here
  pfpart_v->clear();

  // Proceed to write an output data product if any cluster data product exists in the input
  if (!ev_pfpart or (ev_pfpart->size() ==0) ) {
    print(msg::kWARNING, __FUNCTION__,
          Form("No input pfparticle! Saving empty event to output ttree..."));
     return false;
  }

  // retrieve showers associated with this pfpart
  // Showers are retrieved for saving the associations at the end.
  event_shower *ev_shower = nullptr;
  // association vector
  AssSet_t ass_shower_v;
  ass_shower_v = storage->find_one_ass(ev_pfpart->id(), ev_shower, _input_shower_producer );

  // if associated shower not found -> nothing happens
  if ( !ev_shower || ev_shower->size() == 0 ) {
    print(msg::kWARNING, __FUNCTION__,
          Form("No showers found associated to PFPart w/ producer %s",
               _input_producer.c_str()));
    return false;
  }

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
               _input_producer.c_str()));
    return false;
  }
  else {
    pfpart_cluster_ass = storage->get_data<event_ass>(pfpart_v->name());
  }

  // retrieve spacepoints associated with this pfpart
  // Spacepoints are retrieved for saving the associations at the end.
  event_spacepoint *ev_sps = nullptr;
  // association vector
  AssSet_t ass_sps_v;
  ass_sps_v = storage->find_one_ass(ev_pfpart->id(), ev_sps, ev_pfpart->name());

  // if associated clusters not found -> quit and exit
  if ( !ev_sps or (ev_sps->size() == 0) ) {
    print(msg::kERROR, __FUNCTION__,
          Form("No spacepoints found associated to PFPart w/ producer %s",
               _input_producer.c_str()));
    return false;
  }
  else {
    pfpart_sps_ass = storage->get_data<event_ass>(pfpart_v->name());
  }

  // retrieve vertexes associated with this pfpart
  // Vertexes are retrieved for saving the associations at the end.
  event_vertex *ev_vertex = nullptr;
  // association vector
  AssSet_t ass_vertex_v;
  ass_vertex_v = storage->find_one_ass(ev_pfpart->id(), ev_vertex, ev_pfpart->name());

  // if associated clusters not found -> quit and exit
  if ( !ev_vertex or (ev_vertex->size() == 0) ) {
    print(msg::kERROR, __FUNCTION__,
          Form("No vertexes found associated to PFPart w/ producer %s",
               _input_producer.c_str()));
    return false;
  }
  else {
    pfpart_vertex_ass  = storage->get_data<event_ass>(pfpart_v->name());
  }

  event_hit* ev_hits = nullptr;
  auto const& original_hit_ass = storage->find_one_ass(ev_cluster->id(), ev_hits, ev_cluster->name());
  // if associated hits not found -> quit and exit
  if ( !ev_hits or (ev_hits->size() == 0) ) {
    print(msg::kERROR, __FUNCTION__,
          Form("No hits found associated to clusters w/ producer %s",
               _input_producer.c_str()));
    return false;
  }
  else {
    cluster_hit_ass = storage->get_data<event_ass>(cluster_v->name());
  }


  std::vector< std::vector < size_t > > pfpart_sets;
  if ( _doMergeByHierarchy ) ListByHierarchy( *ev_pfpart, pfpart_sets );
  else if ( _doMergeByPCAndVertex ) {
    ListByPCAAndVertex( *ev_pfpart, *ev_shower, ass_shower_v, *ev_vertex, ass_vertex_v, pfpart_sets );
  }

/*
  for ( size_t i = 0; i < pfpart_sets.size(); ++i ) {
    std::cout << "Merged particle " << i << ":" << std::endl;
    for ( size_t j = 0; j < pfpart_sets[i].size(); ++j ) {
       std::cout << pfpart_sets[i][j] << ", ";
    }
    std::cout << std::endl;
  }
*/

  // Merged association
  std::vector< std::vector< unsigned int > > pfpart_sps_ass_v;
  std::vector< std::vector< unsigned int > > pfpart_vertex_ass_v;
  std::vector< std::vector< unsigned int > > pfpart_cluster_ass_v;
  std::vector< std::vector< unsigned int > > cluster_hit_ass_v;

  // Merging the PFParticles
  for ( size_t ipfparts = 0; ipfparts < pfpart_sets.size(); ++ipfparts ) {
    auto pfparts = pfpart_sets[ipfparts];
    int pdgCode = ev_pfpart->at(pfparts[0]).PdgCode();
    std::map< larlite::geo::View_t, std::vector< unsigned int > > hits;
    std::vector< unsigned int > sps;
    std::vector< unsigned int > vertex;

    for ( size_t ipfp = 0; ipfp < pfparts.size(); ++ipfp ) {
      // Determine the PDG code
      if ( ev_pfpart->at(pfparts[ipfp]).PdgCode() == 11 ) pdgCode = 11;

      // Fill the space points
      if ( ipfp < ass_sps_v.size() && !ass_sps_v[ipfp].empty() ) {
        for ( auto isps : ass_sps_v[ipfp] ) 
          sps.push_back( isps );
      }

      // Fill the vertices
      if ( ipfp < ass_vertex_v.size() && !ass_vertex_v[ipfp].empty() ) {
        for ( auto iver : ass_vertex_v[ipfp] )
          vertex.push_back( iver );
      }

      if ( ipfp < ass_cluster_v.size() && !ass_cluster_v[ipfp].empty() ) {
        for ( auto iclu : ass_cluster_v[ipfp] ) {
          if ( iclu < original_hit_ass.size() && !original_hit_ass[iclu].empty() ) {
            const auto ihits = original_hit_ass[iclu];
            larlite::geo::View_t iview = ev_cluster->at(iclu).View();
            for ( auto ihit : ihits )
              hits[iview].push_back( ihit );
          }
        } // for ( auto iclu : ass_cluster_v[ipfp] )
      } // if ( ipfp < ass_cluster_v.size() && !ass_cluster_v[ipfp].empty() )
    } // for ( size_t ipfp = 0; ipfp < pfpart.size(); ++ipfp )

    larlite::pfpart pfparticle( pdgCode, ipfparts, ev_pfpart->at(pfparts[0]).Parent(), 
                                ev_pfpart->at(pfparts[0]).Daughters() );
    pfpart_v->push_back( pfparticle );
    pfpart_sps_ass_v.push_back( sps );
    pfpart_vertex_ass_v.push_back( vertex );

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

    pfpart_cluster_ass_v.push_back( iclusters );

  } // for ( size_t ipfpart = 0; ipfpart < pfpart_sets.size(); ++ipfpart )

  std::cout << "Number of merged pfparticles: " << pfpart_v->size() << std::endl;
  std::cout << "Number of merged clusters: " << cluster_v->size() << std::endl;

  cluster_hit_ass->set_association( cluster_v->id(), 
                   product_id( data::kHit, ev_hits->name() ), cluster_hit_ass_v );
  pfpart_cluster_ass->set_association( pfpart_v->id(),
                      product_id( data::kCluster, cluster_v->name() ), pfpart_cluster_ass_v );
  pfpart_sps_ass->set_association( pfpart_v->id(), 
                  product_id( data::kSpacePoint, ev_sps->name() ), pfpart_sps_ass_v );
  pfpart_vertex_ass->set_association( pfpart_v->id(),
                     product_id( data::kVertex, ev_vertex->name() ), pfpart_vertex_ass_v );

  return true;
}

bool PFPartMerger::finalize() {

  return true;
}

void PFPartMerger::ListByHierarchy( const larlite::event_pfpart& ev_pfpart,
  std::vector< std::vector < size_t > >& pfpart_sets ) {
  for ( auto & part : ev_pfpart ) {
    if ( !part.IsPrimary() ) continue;
    if ( part.PdgCode() == 12 || part.PdgCode() == 14 ) {
      for ( auto ipart : part.Daughters() ) {
        std::vector< size_t > pfparts;
        CreateHierarchyList( ev_pfpart, ipart, pfparts );
        pfpart_sets.push_back( pfparts );
      }
    } else {
      std::vector< size_t > pfparts;
      CreateHierarchyList( ev_pfpart, part.Self(), pfparts );
      pfpart_sets.push_back( pfparts );
    }
  }
  return;
}

void PFPartMerger::CreateHierarchyList( const larlite::event_pfpart& ev_pfpart, 
     size_t ipart, std::vector< size_t >& pfpList ) {
  auto const & pfpart = ev_pfpart.at( ipart );
  pfpList.push_back( pfpart.Self() );
  if ( pfpart.NumDaughters() == 0 ) return;
  else {
    for ( size_t i = 0; i < pfpart.Daughters().size(); i++ ) {
      CreateHierarchyList( ev_pfpart, pfpart.Daughters().at( i ), pfpList );
    }
  }
  return;
}

void PFPartMerger::ListByPCAAndVertex( const larlite::event_pfpart& ev_pfpart, 
     const larlite::event_shower& ev_shower, const larlite::AssSet_t& ass_shower_v, 
     const larlite::event_vertex& ev_vertex, const larlite::AssSet_t& ass_vertex_v, 
     std::vector< std::vector < size_t > >& pfpart_sets ) {

  // list of indices of PFParticles we are interested in: no neutrinos
  std::vector<size_t> interesting_particles;
  for ( size_t ipfp = 0; ipfp < ev_pfpart.size(); ipfp++ ) {
    auto const& pfp = ev_pfpart.at( ipfp );
    if ( pfp.PdgCode() == 12 || pfp.PdgCode() == 14 ) continue;
    interesting_particles.push_back(ipfp);
  } 

  // std::cout << "Number of interesting particles: " << interesting_particles.size() << std::endl;
  // returns whether particle at iA should be before iB in the sorted list
  // (that is, if particle A has larger energy than B);
  // If no shower is associated to a particle, that particle is pushed
  // at the end of the list
  auto SortByShowerEnergy = [&ev_shower,&ass_shower_v](size_t iA, size_t iB){
    if (iA >= ass_shower_v.size()) return false; // A after B, or both with no shower
    if (iB >= ass_shower_v.size()) return true; // B after A
    
    auto const& showersA = ass_shower_v[iA];
    if (showersA.empty()) return false; // A after B, or both with no shower
    auto const& showersB = ass_shower_v[iB];
    if (showersB.empty()) return true; // B after A
    auto const EA = ev_shower.at( showersA[0] ).Energy();
    auto const EB = ev_shower.at( showersB[0] ).Energy();
    return EA > EB;
  };
  
  std::sort(interesting_particles.begin(), interesting_particles.end(),
    SortByShowerEnergy);

  // std::cout << "Sorted... " << std::endl; 
/*
  for (size_t index: interesting_particles) {
    std::cout << "Particle #" << index;
    if ((index >= ass_shower_v.size()) || ass_shower_v[index].empty())
      std::cout << " (no shower)";
    else {
      std::cout << " shower E="
        << ev_shower.at( ass_shower_v[index][0] ).Energy();
    }
    std::cout << " ID=" << ev_pfpart[index].PdgCode() << std::endl;
  }
*/

  const size_t AlreadyUsed = ev_pfpart.size();
  for ( size_t ip = 0; ip < interesting_particles.size(); ip++ ) {
    if ( interesting_particles[ip] >= AlreadyUsed ) continue;
    
    bool iAssociatedVertex = true, iAssociatedShower = true;
    size_t iip = interesting_particles[ip];

    std::vector<size_t> merging;
    merging.push_back(ip);
    interesting_particles[ip] = AlreadyUsed;
    if ( iip > ass_vertex_v.size() || ass_vertex_v[iip].empty() ) iAssociatedVertex = false;
    if ( !iAssociatedVertex ) continue;
    const auto & iver = ev_vertex[ass_vertex_v[iip].at(0)];
    const TVector3 ivertex( iver.X(), iver.Y(), iver.Z() );

    if ( iip > ass_shower_v.size() || ass_shower_v[iip].empty() ) iAssociatedShower = false;

    for ( size_t jp = ip + 1; jp < interesting_particles.size(); ++jp) {
      if ( interesting_particles[jp] >= AlreadyUsed ) continue;

      bool jAssociatedVertex = true, jAssociatedShower = true;
      size_t jjp = interesting_particles[jp];
      if ( jjp > ass_vertex_v.size() || ass_vertex_v[jjp].empty() ) jAssociatedVertex = false;
      if ( !jAssociatedVertex ) continue;
      const auto & jver = ev_vertex[ass_vertex_v[jjp].at(0)];
      const TVector3 jvertex( jver.X(), jver.Y(), jver.Z() );
      if ( !iAssociatedShower || jjp > ass_shower_v.size() || ass_shower_v[jjp].empty() ) jAssociatedShower = false;

      // if ( ShareTheSameVertex( ivertex, jvertex ) ) continue;

      if ( !iAssociatedShower ) continue;

      bool doMerge = false;
      const auto & ishr = ev_shower[ass_shower_v[iip].at(0)];
      if ( !doMerge && IsVertexAligned( ivertex, ishr, jvertex ) ) doMerge = true;

      if ( !jAssociatedShower ) {
        const auto & jshr = ev_shower[ass_shower_v[jjp].at(0)];
        if ( !doMerge && IsPCAAligned( ivertex, ishr, jvertex, jshr ) ) doMerge = true;
      }

      if ( doMerge ) {
        merging.push_back(jp);
        interesting_particles[jp] = AlreadyUsed;
      }
    } // for particles with less energy
    pfpart_sets.push_back( merging );
  }

  return;
}

bool PFPartMerger::ShareTheSameVertex( const TVector3& ivertex, const TVector3& jvertex ) {
  TVector3 dist = jvertex - ivertex;
  // std::cout << "Distance between vertices: " << dist.Mag() << std::endl;
  if ( dist.Mag() > 3. ) return false;

  return true;
}

bool PFPartMerger::IsVertexAligned( const TVector3& ivertex, const larlite::shower& ishr,
                                    const TVector3& jvertex ) {

  TVector3 dist = jvertex - ivertex;
  // std::cout << "Angle between PCA and vertex: " << ishr.Direction().Angle( dist ) 
  //           << " (" << ishr.OpeningAngle() << ")" << std::endl;
  if ( ishr.Direction().Angle( dist ) < ishr.OpeningAngle() ) return true;
  
  return false;
}

bool PFPartMerger::IsPCAAligned( const TVector3& ivertex, const larlite::shower& ishr,
                                 const TVector3& jvertex, const larlite::shower& jshr ) {

  TVector3 dist = jvertex - ivertex;
  // std::cout << "Angle between PCAs: " << ishr.Direction().Angle( jshr.Direction() ) 
  //           << " (" << ishr.OpeningAngle() << ")" << std::endl;
  if ( ishr.Direction().Angle( jshr.Direction() ) < ishr.OpeningAngle() ) return true;
  else if ( ishr.Direction().Angle( jshr.Direction() ) < jshr.OpeningAngle() ) return true;
  else if ( ishr.Direction().Angle( dist ) < jshr.OpeningAngle() ) return true;

  return false;
}

}
#endif
