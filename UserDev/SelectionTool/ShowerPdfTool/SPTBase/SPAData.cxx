#ifndef SELECTIONTOOL_SPADATA_CXX
#define SELECTIONTOOL_SPADATA_CXX

#include "SPAData.h"

namespace sptool {

  SPAShower::SPAShower() 
  { Reset(); }

  void SPAShower::Reset() 
  {
    HalfLine::Start(::geoalgo::Point_t(3));
    HalfLine::Dir(::geoalgo::Point_t(3));

    _radius     = kINVALID_DOUBLE;
    _cone_angle = kINVALID_DOUBLE;
    _energy     = kINVALID_DOUBLE;
    _dedx       = kINVALID_DOUBLE;
    _cosmogenic = kINVALID_DOUBLE;
  }

  SPATrack::SPATrack()
  { Reset(); }

  void SPATrack::Reset()
  {
    Trajectory::clear();
    _energy     = kINVALID_DOUBLE;
    _cosmogenic = kINVALID_DOUBLE;
  }

  SPAData::SPAData()
    : _showers()
    , _tracks()
    , _vtxs()
  { Reset(); }

  void SPAData::Reset()
  {
    _showers.clear();
    _tracks.clear();
    _vtxs.clear();
  }

  void SPAData::ApplyOrder(const SPAOrder& order)
  {
    if( _showers.size() < order._rm_showers.size() ||
	_tracks.size()  < order._rm_tracks.size()  ||
	_vtxs.size()    < order._rm_vtxs.size() )
      throw SPAException("<<ApplyOrder>> SPAOrder contains shower/track/vtx not in SPAData!");

    if(order._rm_showers.size()) {
      std::vector<SPAShower> showers;
      showers.reserve(_showers.size() - order._rm_showers.size());
      for(size_t i=0; i<_showers.size(); ++i) {
	if(order._rm_showers.find(i) == order._rm_showers.end())
	  showers.push_back(_showers[i]);
      }
      _showers = showers;
    }

    if(order._rm_tracks.size()) {
      std::vector<SPATrack> tracks;
      tracks.reserve(_tracks.size() - order._rm_tracks.size());
      for(size_t i=0; i<_tracks.size(); ++i) {
	if(order._rm_tracks.find(i) == order._rm_tracks.end())
	  tracks.push_back(_tracks[i]);
      }
      _tracks = tracks;
    }

    if(order._rm_vtxs.size()) {
      std::vector< ::geoalgo::Point_t> vtxs;
      vtxs.reserve(_vtxs.size() - order._rm_vtxs.size());
      for(size_t i=0; i<_vtxs.size(); ++i) {
	if(order._rm_vtxs.find(i) == order._rm_vtxs.end())
	  vtxs.push_back(_vtxs[i]);
      }
      _vtxs = vtxs;
    }
  }

  std::vector<SPAComb_t> SPAData::Combination(const size_t n, 
					      const size_t m) const
  {

    if(n<m){
      std::ostringstream msg;
      msg << "<<" << __FUNCTION__ << ">>"
	  << " Cannot make a combination of " << m 
	  << " elements from the pool of " << n << std::endl;
      throw SPAException(msg.str());      
    }

    std::vector<SPAComb_t> result;

    std::vector<bool> comb_index_v(n);
    std::fill(comb_index_v.begin() + m, comb_index_v.end(), true);

    do {

      SPAComb_t index_v;
      index_v.reserve(m);

      for (int i = 0; i < comb_index_v.size(); ++i)

	if (!comb_index_v[i]) index_v.push_back(i);
      
      result.push_back(index_v);

    } while (std::next_permutation(comb_index_v.begin(), comb_index_v.end()));

    return result;
  }
  
}
#endif
