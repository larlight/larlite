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
  // std::cout << "run: " << ev_pfpart->run() << ", subrun: " << ev_pfpart->subrun() 
  //           << ", event: " << ev_pfpart->event_id() << std::endl;

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
  auto const& ass_hits_v = storage->find_one_ass(ev_cluster->id(), ev_hits, ev_cluster->name());
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

  std::vector< cluster3D::cluster3D_params > cluster3DParams;
  std::vector< TVector3 > vertices;
  cluster3DParams.resize( ev_pfpart->size() );
  vertices.resize( ev_pfpart->size() );

  for ( size_t ipfpart = 0; ipfpart < ev_pfpart->size(); ++ipfpart ) {
    auto & sps = ass_sps_v.at( ipfpart );
    if ( !sps.empty() ) {
      _cru3D_helper.GenerateParams3D( sps, ev_sps, cluster3DParams[ipfpart] );
      _params3D_alg.FillParams( cluster3DParams[ipfpart] );
    }
    auto & vertex_v = ass_vertex_v.at( ipfpart );
    if ( !vertex_v.empty() ) {
      auto index  = vertex_v[0];
      double xyz[3];
      ev_vertex->at(index).XYZ(xyz);
      TVector3 ivertex( xyz[0], xyz[1], xyz[2] );
      vertices[ipfpart] = ivertex;
    } else {
      TVector3 ivertex( -1000., -1000., -1000. );
      vertices[ipfpart] = ivertex;
    }
  }

  std::vector< std::vector < size_t > > pfpart_sets;
  if ( _doMergeByHierarchy ) ListByHierarchy( *ev_pfpart, pfpart_sets );
  else if ( _doMergeByPCAndVertex ) {
    std::vector< double > pfpart_charges;
    pfpart_charges.resize( ev_pfpart->size() );
    PFParticleCharges( ev_pfpart->size(), *ev_cluster, ass_cluster_v, pfpart_charges );
    ListByPCAAndVertex( *ev_pfpart, pfpart_charges, cluster3DParams, vertices, pfpart_sets );
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
          if ( iclu < ass_hits_v.size() && !ass_hits_v[iclu].empty() ) {
            const auto ihits = ass_hits_v[iclu];
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

void PFPartMerger::PFParticleCharges( const size_t npfp, const larlite::event_cluster& ev_cluster, 
     const AssSet_t ass_cluster_v, std::vector< double >& pfpart_charges ) {

  for ( size_t ipfp = 0; ipfp < npfp; ++ipfp ) {
    auto& iclusters = ass_cluster_v.at( ipfp );
    if ( iclusters.empty() ) pfpart_charges[ipfp] = 0.;
    else {
      double charge = 0.;
      for ( size_t jcluster = 0; jcluster < iclusters.size(); ++jcluster ) {
        if ( ev_cluster[jcluster].SummedADC() > charge ) charge = ev_cluster[jcluster].SummedADC();
      }
      pfpart_charges[ipfp] = charge;
    }
  }

  return;
}

void PFPartMerger::ListByPCAAndVertex( const larlite::event_pfpart& ev_pfpart, 
     std::vector< double >& pfpart_charge,
     std::vector< cluster3D::cluster3D_params >& cluster3DParams,
     std::vector< TVector3 >& vertices,
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
  auto SortByClusterCharge = [&cluster3DParams, &pfpart_charge](size_t iA, size_t iB){
    if (iA >= cluster3DParams.size()) return false; // A after B, or both with no 3d cluster params
    if (iB >= cluster3DParams.size()) return true; // B after A
    
    if ( pfpart_charge[iA] == 0. ) return false; // A after B, or both with no cluster charge
    if ( pfpart_charge[iB] == 0. ) return true; // B after A
    auto const ChargeA = pfpart_charge[iA];
    auto const ChargeB = pfpart_charge[iB];
    return ChargeA > ChargeB;
  };
  
  std::sort(interesting_particles.begin(), interesting_particles.end(),
    SortByClusterCharge);

  // std::cout << "Sorted... " << std::endl; 

  const size_t AlreadyUsed = ev_pfpart.size();
  for ( size_t ip = 0; ip < interesting_particles.size(); ip++ ) {
    if ( interesting_particles[ip] >= AlreadyUsed ) continue;
    
    bool iAssociatedVertex = true, iAssociatedCluster3D = true;
    size_t iip = interesting_particles[ip];

    std::vector<size_t> merging;
    merging.push_back(ip);
    interesting_particles[ip] = AlreadyUsed;
    if ( iip > vertices.size() || vertices[iip].X() < -999. ) iAssociatedVertex = false;
    if ( !iAssociatedVertex ) continue;
    const TVector3 ivertex = vertices[iip];

    if ( iip > cluster3DParams.size() || cluster3DParams[iip].N_points < 1. ) iAssociatedCluster3D = false;

    for ( size_t jp = ip + 1; jp < interesting_particles.size(); ++jp) {
      if ( interesting_particles[jp] >= AlreadyUsed ) continue;

      bool jAssociatedVertex = true, jAssociatedCluster3D = true;
      size_t jjp = interesting_particles[jp];
      if ( jjp > vertices.size() || vertices[jjp].X() < -999. ) jAssociatedVertex = false;
      if ( !jAssociatedVertex ) continue;
      const TVector3 jvertex = vertices[jjp];
      if ( !iAssociatedCluster3D || jjp > cluster3DParams.size() || cluster3DParams[jjp].N_points < 1. )
        jAssociatedCluster3D = false;

      // if ( ShareTheSameVertex( ivertex, jvertex ) ) continue;

      if ( !iAssociatedCluster3D ) continue;

      bool doMerge = false;
      if ( !doMerge && IsVertexAligned( ivertex, cluster3DParams[iip], jvertex ) ) doMerge = true;

      if ( jAssociatedCluster3D ) {
        if ( !doMerge && IsPCAAligned( ivertex, cluster3DParams[iip], jvertex, cluster3DParams[jjp] ) )
          doMerge = true;
      }

      if ( doMerge ) {
        merging.push_back(jjp);
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

bool PFPartMerger::IsVertexAligned( const TVector3& ivertex, const cluster3D::cluster3D_params& icluster3D,
                                    const TVector3& jvertex ) {

  TVector3 dist = jvertex - ivertex;
  double openAngle = 0.;
  if ( icluster3D.eigenvalue_principal != 0. && icluster3D.eigenvalue_secondary != 0. ) {
    double Theta = icluster3D.principal_dir.Angle( icluster3D.secondary_dir );
    double sinTheta = sin( Theta );
    openAngle = 2.* atan( sqrt( icluster3D.eigenvalue_secondary ) * sinTheta /
                          sqrt( icluster3D.eigenvalue_principal ) );

  }
  if ( icluster3D.principal_dir.Angle( dist ) < openAngle ) return true;
  
  return false;
}

bool PFPartMerger::IsPCAAligned( const TVector3& ivertex, const cluster3D::cluster3D_params& icluster3D,
                                 const TVector3& jvertex, const cluster3D::cluster3D_params& jcluster3D ) {

  TVector3 dist = jvertex - ivertex;
  double iopenAngle = 0., jopenAngle = 0.;
  if ( icluster3D.eigenvalue_principal != 0. && icluster3D.eigenvalue_secondary != 0. ) {
    double Theta = icluster3D.principal_dir.Angle( icluster3D.secondary_dir );
    double sinTheta = sin( Theta );
    iopenAngle = 2.* atan( sqrt( icluster3D.eigenvalue_secondary ) * sinTheta /
                           sqrt( icluster3D.eigenvalue_principal ) );

  }
  if ( icluster3D.principal_dir.Angle( jcluster3D.principal_dir ) < iopenAngle ) return true;
  if ( jcluster3D.eigenvalue_principal != 0. && jcluster3D.eigenvalue_secondary != 0. ) {
    double Theta = jcluster3D.principal_dir.Angle( jcluster3D.secondary_dir );
    double sinTheta = sin( Theta );
    jopenAngle = 2.* atan( sqrt( jcluster3D.eigenvalue_secondary ) * sinTheta /
                           sqrt( jcluster3D.eigenvalue_principal ) );

  }

  if ( icluster3D.principal_dir.Angle( jcluster3D.principal_dir ) < jopenAngle ) return true;
  else if ( icluster3D.principal_dir.Angle( dist ) < jopenAngle ) return true;

  return false;
}
}
#endif
