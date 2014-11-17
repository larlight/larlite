#ifndef RECOTOOL_MCSHOWERBTALG_CXX
#define RECOTOOL_MCSHOWERBTALG_CXX

#include "MCShowerBTAlg.h"

namespace larlite {

  ///////////////////////////////////////////////////
  //Make map of trackIDs for each shower           //
  ///////////////////////////////////////////////////
  void MCShowerBTAlg::BuildMap(const std::vector<mcshower>& mcshower_v,
			       const std::vector<simch>& simch_v)
  {
    fShowerPartMap.clear();
    fShowerIndex_v.clear();
    std::set<unsigned int> used_shower_id;
    for(size_t i=0; i<mcshower_v.size(); ++i) {
      
      auto const& mcs = mcshower_v[i];

      if( mcs.Start().Momentum()[3] > _max_energy ||
	  mcs.Start().Momentum()[3] < _min_energy ) continue;

      used_shower_id.insert(i);

      for(auto const &id : mcs.DaughterTrackID())

	fShowerPartMap.insert(std::make_pair((unsigned int)id,(unsigned int)i));
      
    }
    
    for(auto const& index : used_shower_id) fShowerIndex_v.push_back(index);

    fSimCh_v.clear();
    for(auto const& sch : simch_v) {

      if(fSimCh_v.size() <= sch.Channel()) fSimCh_v.resize(sch.Channel(),nullptr);
      fSimCh_v[sch.Channel()] = &sch;

    }
    
  }

  std::vector<double> MCShowerBTAlg::MCShowerQFrac(const WireRange_t &h) const
  {
    double q_tot = 0;

    auto q_abs_v = MCShowerQ(h);

    if(!q_abs_v.size()) return q_abs_v;

    for(auto const& q : q_abs_v) q_tot += q;
    
    for(auto& q : q_abs_v) q /= q_tot;
    
    return q_abs_v;
  }
  
  std::vector<double> MCShowerBTAlg::MCShowerQ(const WireRange_t &h) const
  {

    std::vector<double> num_electrons_v(fShowerIndex_v.size()+1,0);

    if(h.ch >= fSimCh_v.size() || !fSimCh_v[h.ch]) {

      std::cerr<<"Hit has no matched simchannel!!"<<std::endl;
      return std::vector<double>();

    }      
    
    auto *ts = ::larutil::TimeService::GetME();
      
    auto sch = fSimCh_v[h.ch];

    auto ide_v = sch->TrackIDsAndEnergies( ts->TPCTick2TDC(h.start), 
					   ts->TPCTick2TDC(h.end) );

    if(!(ide_v.size())) {

      std::cerr<<"Hit has no matched ide!!"<<std::endl;
      return std::vector<double>();

    }

    for(auto const& qdep : ide_v) {

      unsigned int track_id = abs(qdep.trackID);
      auto const part_iter = fShowerPartMap.find(track_id);
      if(part_iter == fShowerPartMap.end()) (*num_electrons_v.rbegin()) += qdep.numElectrons;

      else {

	size_t index=num_electrons_v.size()-1;
	for(size_t i=0; i<num_electrons_v.size(); ++i) {

	  if(fShowerIndex_v[i] == (*part_iter).second) {
	    index = i;
	    break;
	  }
	}
	
	num_electrons_v[index] += qdep.numElectrons;
      }
    }

    return num_electrons_v;
    
  }

  std::vector<double> MCShowerBTAlg::MCShowerQFrac(const std::vector<WireRange_t> &hit_v) const
  {
    double q_tot = 0;

    auto q_abs_v = MCShowerQ(hit_v);

    if(!q_abs_v.size()) return q_abs_v;

    for(auto const& q : q_abs_v) q_tot += q;

    for(auto& q : q_abs_v) q /= q_tot;

    return q_abs_v;
  }

  std::vector<double> MCShowerBTAlg::MCShowerQ(const std::vector<WireRange_t> &hit_v) const
  {

    std::vector<double> num_electrons_v(fShowerIndex_v.size()+1,0);

    bool filled = false;
    for(auto const& h : hit_v) {

      auto result_onehit = MCShowerQ(h);

      if(result_onehit.size()) {

	for(size_t i=0; i<num_electrons_v.size(); ++i) 

	  num_electrons_v[i] += result_onehit[i];

	filled = true;
      }

    }

    if(filled) return num_electrons_v;
    else return std::vector<double>();
  }
}

#endif  
