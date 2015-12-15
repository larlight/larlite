#ifndef ERTOOL_ERALGOFLASHMATCH_CXX
#define ERTOOL_ERALGOFLASHMATCH_CXX

#include "ERAlgoFlashMatch.h"
#include <set>
#include <sstream>
#include "OpT0Helper.h"
#include "OpT0Finder/Algorithms/LLMatch.h"
#include "OpT0Finder/Algorithms/QLLMatch.h"
#include "OpT0Finder/Algorithms/ChargeAnalytical.h"
#include "OpT0Finder/Algorithms/PhotonLibHypothesis.h"
#include "OpT0Finder/Algorithms/MaxNPEWindow.h"
#include "OpT0Finder/Algorithms/QWeightPoint.h"
#include "OpT0Finder/Algorithms/CommonAmps.h"
#include "OpT0Finder/Algorithms/NPtFilter.h"
#include "OpT0Finder/Algorithms/TimeCompatMatch.h"
//#include "OpT0Finder/Algorithms/FlashHypo.h"

namespace ertool {

  ERAlgoFlashMatch::ERAlgoFlashMatch(const std::string& name) : AlgoBase(name)
  {}

  void ERAlgoFlashMatch::Reset()
  {}

  void ERAlgoFlashMatch::AcceptPSet(const ::fcllite::PSet& cfg)
  {

    auto p = cfg.get_pset(Name());

    std::string ertool_cfg_file = p.get<std::string>("ConfigFile");

    ertool_cfg_file = std::string(getenv("LARLITE_BASEDIR")) + "/" + ertool_cfg_file;

    std::stringstream ss;
    //
    // Flash Match algorithm (required)
    //
    std::string match_alg = p.get<std::string>( "Match" );      
    if      ( match_alg == "QLLMatch"     ) _mgr.SetAlgo( new ::flashana::QLLMatch     () );
    else if ( match_alg == "LLMatch"      ) _mgr.SetAlgo( new ::flashana::LLMatch   () );
    else if ( match_alg == "CommonAmps"   ) _mgr.SetAlgo( new ::flashana::CommonAmps   () );
    else if ( match_alg == "QWeightPoint" ) _mgr.SetAlgo( new ::flashana::QWeightPoint () );
    else {
      ss << "Unknown Match algorithm: " << match_alg.c_str();
      throw ERException(ss.str());
    }

    //
    // Flash Hypothesis algorithm (required)
    //
    std::string hypothesis_alg = p.get<std::string>( "Hypothesis" );    
    if      ( hypothesis_alg == "PhotonLibHypothesis" ) _mgr.SetAlgo( new ::flashana::PhotonLibHypothesis () );
    else if ( hypothesis_alg == "ChargeAnalytical"    ) _mgr.SetAlgo( new ::flashana::ChargeAnalytical    () );
    else {
      ss << "Unknown Hypothesis algorithm: " << hypothesis_alg.c_str();
      throw ERException(ss.str());
    }

    //
    // TPC filter algorithm (optional)
    //
    std::string tpc_filter = p.get<std::string>( "TPCFilter", "" );
    if      ( tpc_filter == "NPtFilter" ) _mgr.SetAlgo( new ::flashana::NPtFilter () );
    else if ( ! tpc_filter.empty()      ) {
      ss << "Unknown TPCFilter algorithm: " << tpc_filter.c_str();
      throw ERException(ss.str());
    }

    //
    // Optical filter algorithm (optional)
    //
    std::string opt_filter = p.get<std::string>( "OpticalFilter", "" );
    if      ( opt_filter == "MaxNPEWindow" ) _mgr.SetAlgo( new ::flashana::MaxNPEWindow () );
    else if ( ! opt_filter.empty()      ) {
      ss << "Unknown OpticalFilter algorithm: " << opt_filter.c_str();
      throw ERException(ss.str());
    }

    //
    // Match Prohibit algorithm (optional)
    //
    std::string prohibit_alg   = p.get<std::string>( "Prohibit", "" );
    if      ( prohibit_alg == "TimeCompatMatch" ) _mgr.SetAlgo( new ::flashana::TimeCompatMatch () );
    else if ( ! prohibit_alg.empty()      ) {
      ss << "Unknown Prohibit algorithm: " << prohibit_alg.c_str();
      throw ERException(ss.str());
    }
    
    _mgr.Configure(ertool_cfg_file);
      
  }

  void ERAlgoFlashMatch::ProcessBegin()
  {}

  void ERAlgoFlashMatch::ProcessEnd(TFile* fout)
  {}

  bool ERAlgoFlashMatch::Reconstruct(const EventData &data, ParticleGraph& graph)
  {
    OpT0Helper helper;
    _mgr.Reset();
    
    std::multimap<double,std::pair<NodeID_t,FlashID_t> > score_m;

    std::vector<NodeID_t> primary_id_v;

    int i = 0;

    for(auto const& primary_node_id : graph.GetPrimaryNodes() ) {

      auto const& primary_part = graph.GetParticle(primary_node_id);

      auto const& children_v = graph.GetAllDescendantNodes(primary_part.ID());

      std::vector<NodeID_t> shower_v, track_v;
      shower_v.reserve(children_v.size());
      track_v.reserve(children_v.size());

      for(auto const& id : children_v) {
	auto const& child_part = graph.GetParticle(id);
	switch(child_part.RecoType()) {
	case kShower:
	  shower_v.push_back(child_part.RecoID());
	  break;
	case kTrack:
	  track_v.push_back(child_part.RecoID());
	  break;
	default:
	  break;
	}
      }
      if(primary_part.RecoType()==kTrack) track_v.push_back(primary_part.RecoID());
      else if(primary_part.RecoType()==kShower) shower_v.push_back(primary_part.RecoID());
      
      auto cluster = helper.GetQCluster(data,shower_v,track_v);
      cluster.idx = i; //primary_node_id;
      //std::cout<<cluster.size()<<" ";
      primary_id_v.push_back(primary_node_id);
      _mgr.Emplace(std::move(cluster));
      i++;
      
    }
    //std::cout<<std::endl;
    std::vector<FlashID_t> flash_id_v;
    flash_id_v.reserve(data.Flash().size());

    for (size_t i=0; i < data.Flash().size(); i++){

      auto const& erflash = data.Flash().at(i);

      ::flashana::Flash_t f;
      f.x = erflash._x;
      f.y = erflash._y;
      f.z = erflash._z;
      f.pe_v.reserve(erflash._npe_v.size());
      for(auto const& v : erflash._npe_v)
	f.pe_v.push_back(v);
      f.time = erflash._t;
      f.idx  = i;
      flash_id_v.push_back(erflash.FlashID());
      _mgr.Emplace(std::move(f));
    }
    //_mgr.SetVerbosity(::flashana::msg::kDEBUG);
    auto const res = _mgr.Match();
    
    std::set<NodeID_t> nu_candidates;
    //std::cout<<res.size()<<" match found..."<<std::endl;
    for(auto const& match : res ) {
      //std::cout<<"TPC: "<<match.tpc_id<<"/"<<primary_id_v.size()<<std::endl;
      //std::cout<<"Flash: "<<match.flash_id<<"/"<<flash_id_v.size()<<std::endl;
      auto const& nord_id  = primary_id_v[match.tpc_id];
      auto const& flash_id = flash_id_v[match.flash_id];
      graph.SetFlashID(nord_id,flash_id);

      auto const& flash = data.Flash(flash_id);
      auto& part = graph.GetParticle(nord_id);

      if( _beam_dt_min < flash._t && flash._t < _beam_dt_max ) {
	nu_candidates.insert(nord_id);
	//std::cout<<"Nu?! "<<flash._z<<" vs "<<match.tpc_point.z<<std::endl;
      }
      else {
	part.SetParticleInfo(part.PdgCode(),
			     part.Mass(),
			     part.Vertex(),
			     part.Momentum(),
			     part.RecoScore(),
			     kCosmic);
	//std::cout<<"Cosmic.. "<<flash._z<<" vs "<<match.tpc_point.z<<" @ "<<flash._t<<std::endl;
      }
    }

    for(auto const& node_id : graph.GetPrimaryNodes()) {

      auto& part = graph.GetParticle(node_id);

      if(part.ProcessType() == kCosmic) continue;

      if(nu_candidates.find(node_id) == nu_candidates.end())
	part.SetParticleInfo(part.PdgCode(),
			     part.Mass(),
			     part.Vertex(),
			     part.Momentum(),
			     part.RecoScore(),
			     kCosmic);
    }
    
    return true;
  }

}

#endif
