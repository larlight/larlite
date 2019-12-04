#ifndef MCQCLUSTER_CXX
#define MCQCLUSTER_CXX

#include "MCQCluster.h"
#include "OpT0Finder/Base/OpT0FinderException.h"
#include "LArUtil/LArProperties.h"
#include "LArUtil/TimeService.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/mcshower.h"
#include "OpT0Finder/Base/OpT0FinderTypes.h"
#include "GeoAlgo/GeoVector.h"
#include "GeoAlgo/GeoAlgo.h"
namespace flashana {

  static MCQClusterFactory __global_MCQClusterFactory__;

  MCQCluster::MCQCluster(const std::string name)
    : BaseAlgorithm( kCustomAlgo, name )
    , _use_xshift  ( true  )
  {}

  void MCQCluster::_Configure_(const ::fcllite::PSet &pset)
  {
    _use_mc_dedx = pset.get<bool>("UseMCdEdX");
    _use_xshift  = pset.get<bool>("UseXShift");
    _op_RO_start = pset.get<float>("OpROStart");
    _op_RO_end   = pset.get<float>("OpROEnd");
    _extension   = pset.get<double>("Extension");
    _trigger_time = pset.get<double>("DefaultTriggerTime");
  }

  const std::vector<flashana::QCluster_t>& MCQCluster::QClusters() const
  { return _qcluster_v; }

  MCSource_t MCQCluster::Identify( const unsigned int ancestor_track_id,
                                   const ::larlite::event_mctrack& ev_mct,
                                   const ::larlite::event_mcshower& ev_mcs) const
  {
    MCSource_t res;

    for (size_t mct_index = 0; mct_index < ev_mct.size(); ++mct_index) {

      auto const& mct = ev_mct[mct_index];

      if (mct.TrackID() == ancestor_track_id) {
        res.index_id = (int)mct_index;
        res.g4_time  = mct.Start().T();

        //std::cout<<"1) g4 time is: "<<res.g4_time / 1000<<" size : "<<mct.size()<<std::endl ;
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

//  std::cout<<"2) g4 time is: "<<ancestor_track_id<<", "<<res.g4_time<<", "<<res.index_id<<std::endl ;

    return res;
  }

  void MCQCluster::Construct( const ::larlite::event_mctrack& ev_mct,
                              const ::larlite::event_mcshower& ev_mcs,
			      const ::flashana::LightPath& lightpath)
  {

    //
    // 0) Initialization
    //

    // Look at tracks on 'per interaction' basis
    std::map<unsigned int, size_t> usedIDs;

    // Clear attributes to be filled
    _mctrack_2_qcluster.clear();
    _mcshower_2_qcluster.clear();
    _qcluster_v.clear();

    //
    // 1) Loop over mctrack
    //
    // Reserve vector size for MCTrack
    _mctrack_2_qcluster.reserve(ev_mct.size());
    _qcluster_v.reserve(ev_mct.size());
    for (size_t mct_index = 0; mct_index < ev_mct.size(); mct_index++) {

      auto const& trk = ev_mct[mct_index];
      if (trk.size() <= 2 || trk[0].T() < _op_RO_start || trk[0].T() > _op_RO_end ) continue;

      auto usedIDs_iter = usedIDs.find(trk.AncestorTrackID());

      size_t qcluster_index = 0;
      if ( usedIDs_iter == usedIDs.end() ) {
        qcluster_index = _qcluster_v.size();
        usedIDs.emplace(trk.AncestorTrackID(), qcluster_index);
        // Search ancestor identity
        _qcluster_2_mcobject.emplace_back(Identify(trk.AncestorTrackID(), ev_mct, ev_mcs));
        QCluster_t qcluster;
        qcluster.idx = _qcluster_2_mcobject.back().index_id;
	qcluster.time = ::larutil::TimeService::GetME()->G4ToElecTime(trk[0].T()) - _trigger_time;
        _qcluster_v.emplace_back(qcluster);
      }
      else
        qcluster_index = usedIDs_iter->second;

      _mctrack_2_qcluster.push_back(qcluster_index);

      auto& tpc_obj = _qcluster_v[qcluster_index];
      auto& tpc_src = _qcluster_2_mcobject[qcluster_index];

      if (trk.size() <= 2) continue;

      tpc_obj.reserve(tpc_obj.size() + trk.size());

      ExpandQCluster(lightpath,trk,tpc_obj);
      
    } // Finish looping over mctracks

    //
    // MCShower treatment to be added
    //
  }// EOF

  void MCQCluster::ExpandQCluster(const flashana::LightPath& lightpath,
				  const larlite::mctrack& mct,
				  flashana::QCluster_t& tpc_obj) {

    if(mct.size() < 2) return;
    // per track calculate the shift in x-direction
    // so that the x-position is what would be seen
    // in the TPC, not the truth x-position
    // Some constants needed
    double det_drift_velocity = ::larutil::LArProperties::GetME()->DriftVelocity(); ///< cm/us
    double event_time = ::larutil::TimeService::GetME()->G4ToElecTime(mct[0].T()) - _trigger_time; // ns
    double shift_x = event_time * det_drift_velocity; //cm
    
    ::larlite::geoalgo::Vector pt0(0.,0.,0.); // variable to be used in this function
    ::larlite::geoalgo::Vector pt1(0.,0.,0.); // variable to be used in this function

    //
    // Add body
    //
    for(size_t step_idx=0; step_idx<(mct.size()-1); ++step_idx) {

      pt0[0] = mct[step_idx].X();
      pt0[1] = mct[step_idx].Y();
      pt0[2] = mct[step_idx].Z();

      pt1[0] = mct[step_idx+1].X();
      pt1[1] = mct[step_idx+1].Y();
      pt1[2] = mct[step_idx+1].Z();

      if(_use_xshift) {
	//std::cout<<"shifting " << pt0[0] << " => " << pt0[0] + shift_x << std::endl;
	pt0[0] += shift_x;
	pt1[0] += shift_x;
      }

      if(!_use_mc_dedx) lightpath.QCluster(pt0,pt1,tpc_obj);
      else {
	double dedx = (mct[step_idx].E() - mct[step_idx+1].E())/pt1.Dist(pt0);
	lightpath.QCluster(pt0,pt1,tpc_obj,dedx);
      }
    }

    //
    // Inspect "start edge"
    //
    ::larlite::geoalgo::GeoAlgo alg;
    auto const& tpc_vol = ActiveVolume();
    pt0[0] = mct[0].X(); pt0[1] = mct[0].Y(); pt0[2] = mct[0].Z();
    if(!tpc_vol.Contain(pt0)) {
      // start point outside TPC! Make sure to include enough distance
      // 0) find first contained point
      // 1) find crossing point
      // 2) calculate cumulative distance from 1st point to crossing point
      // 3) if needed, add an extra point to cover distance to be _extend_range

      // 0) find 1st contained point
      size_t idx=1;
      for(idx=1; idx<mct.size(); ++idx) {
	auto const& step = mct[idx];
	pt1[0] = step.X(); pt1[1] = step.Y(); pt1[2] = step.Z();
	if(tpc_vol.Contain(pt1)) break;
      }
      if(idx==mct.size()) throw OpT0FinderException("All points outside the active volume!");
      
      // 1) find crossing point
      pt0[0] = mct[idx-1].X(); pt0[1] = mct[idx-1].Y(); pt0[2] = mct[idx-1].Z();
      auto xs_pt_v = alg.Intersection(tpc_vol,::larlite::geoalgo::LineSegment(pt0,pt1));
      if(xs_pt_v.empty()) throw OpT0FinderException("Logic error: crossing point not found!");
      auto const& xs_pt = xs_pt_v[0];

      // 2) calculate cumulative distance
      double dist_sum=0;
      pt1[0] = mct[0].X(); pt1[1] = mct[0].Y(); pt1[2] = mct[0].Z();
      for(size_t i=0; i<(idx-1); ++i) {
	pt0[0] = mct[i  ].X(); pt0[1] = mct[i  ].Y(); pt0[2] = mct[i  ].Z();
	pt1[0] = mct[i+1].X(); pt1[1] = mct[i+1].Y(); pt1[2] = mct[i+1].Z();
	dist_sum += pt0.Dist(pt1);
      }
      dist_sum += pt1.Dist(xs_pt);

      // 3) see if we need to extend
      if(dist_sum < _extension) {
	//Extend in first 2 points' direction
	pt0[0] = mct[0].X(); pt0[1] = mct[0].Y(); pt0[2] = mct[0].Z();
	pt1[0] = mct[1].X(); pt1[1] = mct[1].Y(); pt1[2] = mct[1].Z();
	pt1 = pt0 - pt1; // now pt1 is a direction vector (not normalized)
	pt0 = pt0 + pt1.Dir() * (_extension - dist_sum); // pt0 is not extended point
	pt1[0] = mct[0].X(); pt1[1] = mct[0].Y(); pt1[2] = mct[0].Z(); // pt1 is now start point
	// shift x
	if(_use_xshift) { pt0[0] += shift_x; pt1[0] += shift_x; }
	if(!_use_mc_dedx) lightpath.QCluster(pt0,pt1,tpc_obj);
	else {
	  double step_dist = sqrt(pow(mct[0].X() - mct[1].X(),2) +
				  pow(mct[0].Y() - mct[1].Y(),2) +
				  pow(mct[0].Z() - mct[1].Z(),2));
	  double dedx = -1;
	  if(step_dist > 0.1)
	    dedx = (mct[0].E() - mct[1].E()) / step_dist;
	  lightpath.QCluster(pt0,pt1,tpc_obj,dedx);
	}
      }
    }

    //
    // Inspect "end edge"
    //
    const size_t end_idx=mct.size()-1;
    pt0[0] = mct[end_idx].X(); pt0[1] = mct[end_idx].Y(); pt0[2] = mct[end_idx].Z();
    if(!tpc_vol.Contain(pt0)) {
      // start point outside TPC! Make sure to include enough distance
      // 0) find first contained point
      // 1) find crossing point
      // 2) calculate cumulative distance from 1st point to crossing point
      // 3) if needed, add an extra point to cover distance to be _extend_range

      // 0) find 1st contained point
      size_t idx=mct.size()-2;
      for(idx=mct.size()-2; idx>=0; --idx) {
	auto const& step = mct[idx];
	pt1[0] = step.X(); pt1[1] = step.Y(); pt1[2] = step.Z();
	if(tpc_vol.Contain(pt1)) break;
	if(!idx) break;
      }
      if(idx==mct.size()) throw OpT0FinderException("All points outside the active volume!");
      
      // 1) find crossing point
      pt0[0] = mct[idx+1].X(); pt0[1] = mct[idx+1].Y(); pt0[2] = mct[idx+1].Z();
      auto xs_pt_v = alg.Intersection(tpc_vol,::larlite::geoalgo::LineSegment(pt0,pt1));
      if(xs_pt_v.empty()) throw OpT0FinderException("Logic error: crossing point not found!");
      auto const& xs_pt = xs_pt_v[0];

      // 2) calculate cumulative distance
      double dist_sum=0;
      pt1[0] = mct[end_idx].X(); pt1[1] = mct[end_idx].Y(); pt1[2] = mct[end_idx].Z();
      for(size_t i=end_idx; i>idx; --i) {
	pt0[0] = mct[i  ].X(); pt0[1] = mct[i  ].Y(); pt0[2] = mct[i  ].Z();
	pt1[0] = mct[i-1].X(); pt1[1] = mct[i-1].Y(); pt1[2] = mct[i-1].Z();
	dist_sum += pt0.Dist(pt1);
      }
      dist_sum += pt1.Dist(xs_pt);

      // 3) see if we need to extend
      if(dist_sum < _extension) {
	//Extend in first 2 points' direction
	pt0[0] = mct[end_idx  ].X(); pt0[1] = mct[end_idx  ].Y(); pt0[2] = mct[end_idx  ].Z();
	pt1[0] = mct[end_idx-1].X(); pt1[1] = mct[end_idx-1].Y(); pt1[2] = mct[end_idx-1].Z();
	pt1 = pt0 - pt1; // now pt1 is a direction vector (not normalized)
	pt0 = pt0 + pt1.Dir() * (_extension - dist_sum); // pt0 is not extended point
	pt1[0] = mct[end_idx].X(); pt1[1] = mct[end_idx].Y(); pt1[2] = mct[end_idx].Z(); // pt1 is now end point
	if(_use_xshift) { pt0[0] += shift_x; pt1[0] += shift_x; }
	if(!_use_mc_dedx) lightpath.QCluster(pt0,pt1,tpc_obj);
	else {
	  double step_dist = sqrt(pow(mct[end_idx].X() - mct[end_idx-1].X(),2) +
				  pow(mct[end_idx].Y() - mct[end_idx-1].Y(),2) +
				  pow(mct[end_idx].Z() - mct[end_idx-1].Z(),2));
	  double dedx = -1;
	  if(step_dist>0.1)
	    dedx = (mct[end_idx-1].E() - mct[end_idx].E()) / step_dist;
	  lightpath.QCluster(pt0,pt1,tpc_obj,dedx);
	}
      }
    }
    
  }

  void MCQCluster::Swap(std::vector<flashana::QCluster_t>&& qcluster_v,
                        std::vector<flashana::MCSource_t>&& source_v)
  {
    std::swap(_qcluster_v, qcluster_v);
    std::swap(_qcluster_2_mcobject, source_v);
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
