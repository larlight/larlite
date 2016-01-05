#ifndef MCQCLUSTER_CXX
#define MCQCLUSTER_CXX

#include "MCQCluster.h"
#include "LArUtil/LArProperties.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/mcshower.h"
#include "OpT0Finder/Base/OpT0FinderTypes.h"
#include "GeoAlgo/GeoVector.h"

namespace flashana {

  MCQCluster::MCQCluster(const std::string name)
    : BaseAlgorithm( kCustomAlgo, name )
    , _light_yield ( 29000 )
    , _step_size   ( 0.5   )
    , _use_xshift  ( true  )
  {}

  void MCQCluster::Configure(const ::fcllite::PSet &pset)
  {
    _light_yield = pset.get<double>("LightYield");
    _step_size   = pset.get<double>("StepSize");
  }
  
  const std::vector<flashana::QCluster_t>& MCQCluster::QClusters() const
  { return _qcluster_v; }
  
  MCSource_t MCQCluster::Identify( const unsigned int ancestor_track_id,
				   const ::larlite::event_mctrack& ev_mct,
				   const ::larlite::event_mcshower& ev_mcs) const
  {
    MCSource_t res;

    for (size_t mct_index = 0; mct_index < ev_mct.size(); ++mct_index){
      
      auto const& mct = ev_mct[mct_index];
      
      if (mct.TrackID() == ancestor_track_id) {
	res.index_id = (int)mct_index;
	res.g4_time  = mct.Start().T();

	res.source_type = kMCTrackAncestor;
	return res;
      }
      
    }
    
    for (size_t mcs_index = 0; mcs_index < ev_mcs.size(); ++mcs_index) {
      
      auto const& mcs = ev_mcs[mcs_index];
      
      if (mcs.TrackID() == ancestor_track_id) {
	res.index_id = (int)mcs_index;
	res.g4_time  = mcs.Start().T();
	res.source_type = kMCShowerAncestor;
	return res;
      }
    }

//	std::cout<<"2) g4 time is: "<<ancestor_track_id<<", "<<res.g4_time<<", "<<res.index_id<<std::endl ;
    
    return res;
  }
  
  void MCQCluster::Construct( const ::larlite::event_mctrack& ev_mct,
			      const ::larlite::event_mcshower& ev_mcs )
  {

    //std::cout<<"Number of tracks: "<<ev_mct.size()<<std::endl ;
    //
    // 0) Initialization
    //

    // Look at tracks on 'per interaction' basis
    std::map<unsigned int, size_t> usedIDs;
    
    // Clear attributes to be filled
    _mctrack_2_qcluster.clear();
    _mcshower_2_qcluster.clear();
    _qcluster_v.clear();

//    std::cout<<"All track ancestors: "<<std::endl;
//    for (size_t mct_index = 0; mct_index < ev_mct.size(); mct_index++) 
//	std::cout<<ev_mct[mct_index].AncestorTrackID()<<", ";
//
//	std::cout<<"\n\nAll track ids: "<<std::endl;
//    for (size_t mct_index = 0; mct_index < ev_mct.size(); mct_index++) {
//        if (ev_mct[mct_index].size() )
//	 std::cout<<", ("<<ev_mct[mct_index].TrackID()<<", "<<ev_mct[mct_index].AncestorTrackID()<<") ";
//    }

    //
    // 1) Loop over mctrack
    //
    // Reserve vector size for MCTrack
    _mctrack_2_qcluster.reserve(ev_mct.size());
    _qcluster_v.reserve(ev_mct.size());
    for (size_t mct_index = 0; mct_index < ev_mct.size(); mct_index++) {
      //  std::cout<<"Ancestors: "<<ev_mct.at(mct_index).AncestorTrackID()<<std::endl;
      
      auto const& trk = ev_mct[mct_index];
      if (trk.size() <= 2) continue;
      
      auto usedIDs_iter = usedIDs.find(trk.AncestorTrackID());
      
      size_t qcluster_index = 0;
      if ( usedIDs_iter == usedIDs.end() ) {
	qcluster_index = _qcluster_v.size();
	usedIDs.emplace(trk.AncestorTrackID(), qcluster_index);
	// Search ancestor identity
	_qcluster_2_mcobject.emplace_back(Identify(trk.AncestorTrackID(),ev_mct,ev_mcs));
	QCluster_t qcluster;	
	qcluster.idx = _qcluster_2_mcobject.back().index_id;
	_qcluster_v.emplace_back(qcluster);
      }
      else
	qcluster_index = usedIDs_iter->second;
      
      _mctrack_2_qcluster.push_back(qcluster_index);

//      std::cout<<" what's our idx: "<<_qcluster_2_mcobject.back().index_id<<", and size of thing: "<<_qcluster_2_mcobject.size()<<", "<<qcluster_index<<std::endl ;
      
      auto& tpc_obj = _qcluster_v[qcluster_index];
      auto& tpc_src = _qcluster_2_mcobject[qcluster_index];

      if (trk.size() <= 2) continue;

      // per track calculate the shift in x-direction
      // so that the x-position is what would be seen
      // in the TPC, not the truth x-position
      // Some constants needed
      double det_drift_velocity = ::larutil::LArProperties::GetME()->DriftVelocity(); ///< cm/us
      double event_time = trk[0].T(); // ns
      double shift_x = event_time * det_drift_velocity * pow(10,-3); //cm
      tpc_obj.reserve(tpc_obj.size() + trk.size());

      // Now loop over all mctracks that share an ancestor and treat
      // them as one interaction
      ::geoalgo::Point_t step_dir(0, 0, 0);
      
      for (size_t step_index = 0; (step_index + 1) < trk.size(); ++step_index) {
	
	auto const& pt1 = trk[step_index];
	auto const& pt2 = trk[step_index + 1];

	double dx = pt2.X() - pt1.X();
	double dy = pt2.Y() - pt1.Y();
	double dz = pt2.Z() - pt1.Z();
	double e_diff = pt1.E() - pt2.E();
	tpc_src.energy_deposit += e_diff;

	//std::cout<<"\nOld X, Y, Z, Q: "<<pt1.X() + dx/2. +shift_x<<", "<<pt1.Y()+dy/2.<<", "<<pt1.Z()+dz/2.<<", "<<(pt1.E() - pt2.E())<<std::endl ;
	
	double distance = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));
	double dedx = e_diff / distance;
	//dedx = 2.3;
	step_dir[0] = dx;
	step_dir[1] = dy;
	step_dir[2] = dz;
	step_dir.Normalize();
	
	// Create individual points
	size_t n_segments = size_t( distance / _step_size ) + 1;
	_n = _n+n_segments;
	tpc_obj.reserve(n_segments + tpc_obj.size());
	for (size_t segment_index = 1; segment_index <= n_segments; ++segment_index) {
	  
	  QPoint_t pt;
	  
	  if (segment_index != n_segments) {
	    pt.q = _light_yield * dedx * _step_size;

	    if( _use_xshift)pt.x = pt1.X() + step_dir[0] * _step_size * (segment_index - 0.5) + shift_x;
	    
	    if(!_use_xshift)pt.x = pt1.X() + step_dir[0] * _step_size * (segment_index - 0.5);
	    pt.y = pt1.Y() + step_dir[1] * _step_size * (segment_index - 0.5);
	    pt.z = pt1.Z() + step_dir[2] * _step_size * (segment_index - 0.5);
//	    std::cout<<"pt.x in MCQCluster: "<<pt.x<<std::endl ;
	    tpc_obj.emplace_back(pt);
	  } else if(segment_index == n_segments && (distance - _step_size*(n_segments-1))!=0) {
	    double offset = _step_size * (segment_index - 1);
	    double remain = distance - offset;

	    pt.q = _light_yield * dedx * remain;
	    if( _use_xshift)pt.x = pt1.X() + step_dir[0] * (offset + remain / 2.) + shift_x;
	    if(!_use_xshift)pt.x = pt1.X() + step_dir[0] * (offset + remain / 2.);
	    pt.y = pt1.Y() + step_dir[1] * (offset + remain / 2.);
	    pt.z = pt1.Z() + step_dir[2] * (offset + remain / 2.);
//	    std::cout<<"pt.x in MCQCluster: "<<pt.x<<", "<<distance << std::endl ;
	    tpc_obj.emplace_back(pt);
	    break;
	  }
//	    std::cout<<"pt.x in MCQCluster: "<<pt.x<<", "<<segment_index<<std::endl ;
	  

	}// Finish looping over segments
      }// Finish looping over mctrack trajectory points
    } // Finish looping over mctracks
    std::cout<<"size in MCQCluster output is "<<_n<<std::endl;
    _n=0;
    //
    // MCShower treatment to be added
    //
  }// EOF

  void MCQCluster::Swap(std::vector<flashana::QCluster_t>&& qcluster_v,
			std::vector<flashana::MCSource_t>&& source_v)
  {
    std::swap(_qcluster_v,qcluster_v);
    std::swap(_qcluster_2_mcobject,source_v);
  }
  
  const flashana::QCluster_t& MCQCluster::QCluster(size_t id) const
  {
    if (id < _qcluster_v.size())
      
      return _qcluster_v[id];
    
    std::cerr << "Invalid QCluster index: " << id << std::endl;
    throw std::exception();
  }
  
  size_t MCQCluster::MCTrack2QCluster(size_t id) const
  {
    if (id < _mctrack_2_qcluster.size()) return _mctrack_2_qcluster[id];
    
    std::cerr << "Invalid MCTrack index: " << id << std::endl;
    throw std::exception();
  }
  
  size_t MCQCluster::MCShower2QCluster(size_t id) const
  {
    if (id < _mcshower_2_qcluster.size()) return _mcshower_2_qcluster[id];
    
    std::cerr << "Invalid MCShower index: " << id << std::endl;
    throw std::exception();
  }
  
  const MCSource_t& MCQCluster::MCObjectID(size_t id) const
  {
    if (id < _qcluster_2_mcobject.size() ) return _qcluster_2_mcobject[id];
    
    std::cerr << "Invalid QCluster index: " << id << std::endl;
    throw std::exception();
  }
  
}

#endif
