#ifndef ERTOOL_ERALGOFLASHMATCH_CXX
#define ERTOOL_ERALGOFLASHMATCH_CXX

#include "ERAlgoFlashMatch.h"
#include <set>
#include <sstream>
// #include "OpT0Helper.h"
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
  {
    _match_tree = 0;
    // This should come from config file... these are just guesses for right now.
    _beam_dt_min = -10.;
    _beam_dt_max = 100.;
    _ignore_showers = true;
    _ignore_cosmics = false;
    _match_primary_only = false;
  }

  void ERAlgoFlashMatch::Reset()
  {}

  void ERAlgoFlashMatch::AcceptPSet(const ::fcllite::PSet& cfg)
  {

    auto p = cfg.get_pset(Name());

    std::string ertool_cfg_file = p.get<std::string>("ConfigFile");

    ertool_cfg_file = std::string(getenv("LARLITE_BASEDIR")) + "/" + ertool_cfg_file;

    std::stringstream ss;

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
    // Flash Match algorithm (required)
    //
    std::string match_alg = p.get<std::string>( "Match" );
    if      ( match_alg == "QLLMatch"     ) _mgr.SetAlgo( new ::flashana::QLLMatch     () );
    else if ( match_alg == "CommonAmps"   ) _mgr.SetAlgo( new ::flashana::CommonAmps   () );
    else if ( match_alg == "QWeightPoint" ) _mgr.SetAlgo( new ::flashana::QWeightPoint () );
    else {
      ss << "Unknown Match algorithm: " << match_alg.c_str();
      throw ERException(ss.str());
    }

    //
    // Custom algo (configured from fcl file but not executed)
    //
    std::string LP_alg =  p.get<std::string>( "Custom" );
    if (LP_alg == "LightPath") _mgr.AddCustomAlgo( new ::flashana::LightPath() );

    _mgr.Configure(ertool_cfg_file);

    /// Get the photons-per-MEV light yield from configured LightPath instance.
    // This is used for hacky flash matching for SHOWER particles done in this code
    // rather than inside of LightPath
    _light_yield = LP.GetLightYield();

    //
    // Set if we should use primary particle only
    //
    _match_primary_only = p.get("MatchPrimaryOnly",false);
    
  }

  void ERAlgoFlashMatch::ProcessBegin()
  {
    if (!_match_tree) {
      _match_tree = new TTree("match_tree", "match_tree");
      _match_tree->Branch("_mct", &_mct, "mct/D");
      _match_tree->Branch("_mct_x", &_mct_x, "mct_x/D");
      _match_tree->Branch("_ft", &_ft, "ft/D");
      _match_tree->Branch("_e", &_e, "e/D");
    }
  }

  void ERAlgoFlashMatch::ProcessEnd(TFile* fout)
  {
    if (fout) {
      fout->cd();
      // _match_tree->Write();
    }
  }

  bool ERAlgoFlashMatch::Reconstruct(const EventData &data, ParticleGraph& graph)
  {
    // OpT0Helper helper;

    _mgr.Reset();
    // _mgr.PrintConfig();

    std::multimap<double, std::pair<NodeID_t, FlashID_t> > score_m;

    std::vector<NodeID_t> used_base_node_id_v;

    int i = 0;

    std::vector<NodeID_t> base_node_id_v;

    if(_match_primary_only) base_node_id_v = graph.GetPrimaryNodes();
    else base_node_id_v = graph.GetBaseNodes();
    
    for (auto const& base_node_id : base_node_id_v) {

      auto const& base_part = graph.GetParticle(base_node_id);

      if (_ignore_showers && base_part.RecoType() == kShower) continue;
      if (_ignore_cosmics &&
          ( base_part.ProcessType() == kCosmic ||
            graph.GetParticle(base_part.Ancestor()).ProcessType() == kCosmic ) )

        continue;

      auto const& children_v = graph.GetAllDescendantNodes(base_part.ID());

      std::vector<NodeID_t> shower_v, track_v;
      shower_v.reserve(children_v.size());
      track_v.reserve(children_v.size());

      for (auto const& id : children_v) {
        auto const& child_part = graph.GetParticle(id);
        switch (child_part.RecoType()) {
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
      if (base_part.RecoType() == kTrack) track_v.push_back(base_part.RecoID());
      else if (base_part.RecoType() == kShower) shower_v.push_back(base_part.RecoID());

      //auto cluster = helper.GetQCluster(data, shower_v, track_v);

      ::flashana::QCluster_t clusters;
      clusters.clear();
      // Implement LightPath for track particles
      // std::cout<<"size of track_v is "<<track_v.size()<<std::endl;
      for (auto const& id : track_v) {

        auto const& track = data.Track(id);

        // if ( track._time < -2050000 || track._time > 2750000 ) continue;

        if (track.size() < 2) continue;

        if (!track.IsLonger(0.1)) continue;

        clusters += LP.FlashHypothesis(track);
      }

      // Implement LightPath for shower particles
      for (auto const& id : shower_v) {
        auto const& shower = data.Shower(id);

        ::flashana::QPoint_t q_pt;
        // Shower Qcluster is a single point in the "center" of the shower
        auto mypoint = shower.Start() + shower.Dir() * shower.Length() / 2;
        q_pt.x = mypoint[0];
        q_pt.y = mypoint[1];
        q_pt.z = mypoint[2];
        //energy * lightyield = charge
        q_pt.q = shower._energy * _light_yield;
        clusters.emplace_back(q_pt);
      }

      if (!clusters.size()) continue;

      clusters.idx = i; //base_node_id;
      //std::cout<<clusters.size()<<" ";
      used_base_node_id_v.push_back(base_node_id);
      _mgr.Emplace(std::move(clusters));
      i++;

    }
    //std::cout<<std::endl;
    std::vector<FlashID_t> flash_id_v;
    flash_id_v.reserve(data.Flash().size());

    for (size_t i = 0; i < data.Flash().size(); i++) {

      auto const& erflash = data.Flash().at(i);

      ::flashana::Flash_t f;
      f.x = erflash._x;
      f.y = erflash._y;
      f.z = erflash._z;
      f.pe_v.reserve(erflash._npe_v.size());
      for (auto const& v : erflash._npe_v)
        f.pe_v.push_back(v);
      f.time = erflash._t;
      f.idx  = i;
      flash_id_v.push_back(erflash.FlashID());
      _mgr.Emplace(std::move(f));
    }
    //_mgr.SetVerbosity(::flashana::msg::kDEBUG);
    auto const res = _mgr.Match();

    std::set<NodeID_t> nu_candidates;
    // std::cout << "ERAlgoFlashMatch: " << res.size() << " match found..." << std::endl;
    for (auto const& match : res ) {
      //std::cout<<"TPC: "<<match.tpc_id<<"/"<<used_base_node_id_v.size()<<std::endl;
      //std::cout<<"Flash: "<<match.flash_id<<"/"<<flash_id_v.size()<<std::endl;
      auto const& node_id  = used_base_node_id_v[match.tpc_id];
      auto const& flash_id = flash_id_v[match.flash_id];

      // The base particle's node is associated with the flash.
      // It is also possible to loop through all children and
      // associate each with the same flash... but for now, I'll
      // make the user do something like this if they want to
      // find the flash associated with an arbitrary particle:
      // particlegraph.GetParticle( particle.Ancestor() ).FlashID()
      // std::cout << "Setting the flashID for node " << node_id << " to " << flash_id << std::endl;
      graph.SetFlashID(node_id, flash_id);

      auto const& flash = data.Flash(flash_id);
      auto& part = graph.GetParticle(node_id);
      //std::cout << "Setting the flashID for the particle itself!"<<std::endl;
      //part.SetFlashID(flash_id);

      if (part.RecoType() == kTrack) {
        auto const& data_t = data.Track(part.RecoID());
        _mct = data_t._time / 1000 ;
        _mct_x = data_t.at(0).at(0);
        _ft  = flash._t ;
        _e   = data_t._energy ;
      }
      else if (part.RecoType() == kShower) {
        auto const& data_t = data.Shower(part.RecoID());
        _mct = data_t._time / 1000 ;
        _mct_x = data_t.Start().at(0);
        _ft  = flash._t ;
        _e   = data_t._energy ;
      }

      if ( _beam_dt_min < flash._t && flash._t < _beam_dt_max ) {
        nu_candidates.insert(node_id);
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

      // _match_tree->Fill();


    }

    //all base particles that were not matched to a flash are set as kCosmic
    for (auto const& node_id : graph.GetBaseNodes()) {
      // std::cout << "Checking if node " << node_id << " is in potential nu candidates..." << std::endl;

      auto& part = graph.GetParticle(node_id);

      if (part.ProcessType() == kCosmic) continue;

      if (nu_candidates.find(node_id) == nu_candidates.end())
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
