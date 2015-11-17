#ifndef OPT0FINDER_FLASHMATCHMANAGER_CXX
#define OPT0FINDER_FLASHMATCHMANAGER_CXX

#include <sstream>
#include <map>
#include <set>
#include "FlashMatchManager.h"
#include "OpT0FinderException.h"
#include "FhiclLite/ConfigManager.h"
namespace flashana {
  
  FlashMatchManager::FlashMatchManager()
    : _alg_flash_filter(nullptr)
    , _alg_tpc_filter(nullptr)
    , _alg_match_prohibit(nullptr)
    , _alg_flash_match(nullptr)
    , _alg_flash_hypothesis(nullptr)
    , _configured(false)
    , _config_file("FlashMatch.fcl")
  {
    _allow_reuse_flash = true;
  }

  void FlashMatchManager::SetAlgo(BaseAlgorithm* alg)
  {
    _configured = false;
    // Figure out the type of a provided algorithm
    switch(alg->AlgorithmType()) {

      // TPC filter
    case kTPCFilter: 
      _alg_tpc_filter   = (BaseTPCFilter*)alg; break;

      // Flash filter
    case kFlashFilter:
      _alg_flash_filter = (BaseFlashFilter*)alg; break;

      // Match prohibit algo
    case kMatchProhibit:
      _alg_match_prohibit = (BaseProhibitAlgo*)alg; break;

      // Flash matching
    case kFlashMatch:
      _alg_flash_match  = (BaseFlashMatch*)alg; break;

      // Flash hypothesis
    case kFlashHypothesis:
      _alg_flash_hypothesis = (BaseFlashHypothesis*)alg; break;

      // Fuck it
    default:
      std::stringstream ss;
      ss << "Unsupported algorithm type: " << alg->AlgorithmType();
      throw OpT0FinderException(ss.str());
    }
  }

  void FlashMatchManager::Configure(const std::string cfg_file)
  {
    if(!cfg_file.empty()) {
      if(!_config_file.empty())
	Print(msg::kWARNING,__FUNCTION__,"Over-riding config file...");
      _config_file = cfg_file;
    }

    ::fcllite::ConfigManager cfg_mgr("FlashMatchManager");

    cfg_mgr.AddCfgFile(_config_file);
    
    auto const& main_cfg = cfg_mgr.Config();

    auto const& pmt_pos_cfg = main_cfg.get_pset("PMT_POSITION");

    auto const pmt_x_pos = pmt_pos_cfg.get<std::vector<double> >("X");
    auto const pmt_y_pos = pmt_pos_cfg.get<std::vector<double> >("Y");
    auto const pmt_z_pos = pmt_pos_cfg.get<std::vector<double> >("Z");

    if(pmt_x_pos.size() != pmt_y_pos.size() ||
       pmt_x_pos.size() != pmt_z_pos.size() )
      throw OpT0FinderException("PMT position array length has a mismatch among x vs. y or x vs. z");
    
    if(_alg_flash_filter) {
      _alg_flash_filter->Configure(main_cfg.get_pset(_alg_flash_filter->AlgorithmName()));
      _alg_flash_filter->SetOpDetPositions(pmt_x_pos, pmt_y_pos, pmt_z_pos);
    }
    if(_alg_tpc_filter) {
      _alg_tpc_filter->Configure(main_cfg.get_pset(_alg_tpc_filter->AlgorithmName()));
      _alg_tpc_filter->SetOpDetPositions(pmt_x_pos, pmt_y_pos, pmt_z_pos);
    }
    if(_alg_match_prohibit) {
      _alg_match_prohibit->Configure(main_cfg.get_pset(_alg_match_prohibit->AlgorithmName()));
      _alg_match_prohibit->SetOpDetPositions(pmt_x_pos, pmt_y_pos, pmt_z_pos);
    }
    if(_alg_flash_match) {
      _alg_flash_match->Configure(main_cfg.get_pset(_alg_flash_match->AlgorithmName()));
      _alg_flash_match->SetOpDetPositions(pmt_x_pos, pmt_y_pos, pmt_z_pos);
    }
    if(_alg_flash_hypothesis) {
      _alg_flash_hypothesis->Configure(main_cfg.get_pset(_alg_flash_hypothesis->AlgorithmName()));
      _alg_flash_hypothesis->SetOpDetPositions(pmt_x_pos, pmt_y_pos, pmt_z_pos);
    }    
    _configured = true;
  }

  BaseAlgorithm* FlashMatchManager::GetAlgo(flashana::Algorithm_t type)
  {
    if(!_configured)
      Print(msg::kWARNING,__FUNCTION__,"Algorithm may be not configured yet!");
    
    // Figure out the type of a provided algorithm
    switch(type) {

      // TPC filter
    case kTPCFilter: 
      return _alg_tpc_filter;

      // Flash filter
    case kFlashFilter:
      return _alg_flash_filter;

      // Match prohibit algo
    case kMatchProhibit:
      return _alg_match_prohibit;

      // Flash matching
    case kFlashMatch:
      return _alg_flash_match;

      // Flash hypothesis
    case kFlashHypothesis:
      return _alg_flash_hypothesis;

      // Fuck it
    default:
      std::stringstream ss;
      ss << "Unsupported algorithm type: " << type;
      throw OpT0FinderException(ss.str());
    }
    return nullptr;
  }

  void FlashMatchManager::Add(flashana::QCluster_t& obj)
  { _tpc_object_v.push_back(obj); }

  void FlashMatchManager::Emplace(flashana::QCluster_t&& obj)
  { _tpc_object_v.emplace_back(obj); }

  void FlashMatchManager::Add(flashana::Flash_t& obj)
  { _flash_v.push_back(obj); }

  void FlashMatchManager::Emplace(flashana::Flash_t&& obj)
  { _flash_v.emplace_back(obj); }

  // CORE FUNCTION
  std::vector<FlashMatch_t> FlashMatchManager::Match()
  {
    // check if required algorithms are provided or not
    if(!_alg_flash_match) 
      throw OpT0FinderException("Flash matching algorithm is reuqired! (not attached)");
    if(!_alg_flash_hypothesis)
      throw OpT0FinderException("Flash hypothesis algorithm is required! (not attached)");

    if(!_configured)
      Configure(_config_file);
    
    //
    // Filter stage: for both TPC and Flash
    //
    
    // IDArray_t to store candidate list of tpc/flash to be used for matching
    IDArray_t tpc_index_v;
    IDArray_t flash_index_v;

    // Figure out which tpc object to use: if algorithm provided, ask it. Else use all.
    if(_alg_tpc_filter)
      tpc_index_v = _alg_tpc_filter->Filter(_tpc_object_v);
    else{
      tpc_index_v.reserve(_tpc_object_v.size());
      for(size_t i=0; i<_tpc_object_v.size(); ++i) tpc_index_v.push_back(i);
    }
    if(_verbosity<=msg::kINFO) {
      std::stringstream ss;
      ss << "TPC Filter: " << _tpc_object_v.size() << " => " << tpc_index_v.size();
      Print(msg::kINFO,__FUNCTION__,ss.str());
    }
    
    // Figure out which flash to use: if algorithm provided, ask it. Else use all
    if(_alg_flash_filter)
      flash_index_v = _alg_flash_filter->Filter(_flash_v);
    else{
      flash_index_v.reserve(_flash_v.size());
      for(size_t i=0; i<_flash_v.size(); ++i) flash_index_v.push_back(i);
    }
    if(_verbosity<=msg::kINFO) {
      std::stringstream ss;
      ss << "Flash Filter: " << _flash_v.size() << " => " << flash_index_v.size();
      Print(msg::kINFO,__FUNCTION__,ss.str());
    }

    //
    // Flash matching stage
    //

    // use multi-map for possible equally-scored matches
    std::multimap<double,FlashMatch_t> score_map;

    // Double loop over a list of tpc object & flash
    // Call matching function to inspect the compatibility.
    for(size_t tpc_index=0; tpc_index < tpc_index_v.size(); ++tpc_index) {

      // Loop over flash list
      for(auto const& flash_index : flash_index_v) {

	auto const& tpc   = _tpc_object_v[tpc_index]; // Retrieve TPC object
	auto const& flash = _flash_v[flash_index];    // Retrieve flash

	// run the match-prohibit algo first
	if (_alg_match_prohibit){
	  bool compat = _alg_match_prohibit->MatchCompatible( tpc, flash);
	  if (compat == false)
	    continue;
	}
	
	auto res = _alg_flash_match->Match( tpc, flash ); // Run matching

	// ignore this match if the score is <= 0
	if(res.score<=0) continue; 

	// Else we store this match. Assign TPC & flash index info
	res.tpc_id = tpc.idx;//_index;
	res.flash_id = flash.idx;//_index;
	// For ordering purpose, take an inverse of the score for sorting
	score_map.emplace( 1./res.score, res);

	if(_verbosity <= msg::kDEBUG) {
	  std::stringstream ss;
	  ss << "Candidate Match: " << " TPC=" << tpc_index << " Flash=" << flash_index
	     << " Score=" << res.score;
	  Print(msg::kINFO,__FUNCTION__,ss.str());
	}

      }
    }

    // We have a score-ordered list of match information at this point.
    // Prepare return match information by respecting a score of each possible match.
    // Note _allow_reuse_flash becomes relevant here as well.

    // Create a std::set of tpc/flash IDs to keep track of already-matched tpc/flash input.
    std::set<ID_t> tpc_used, flash_used;
    // Create also a result container
    std::vector<FlashMatch_t> result;
    result.reserve(tpc_index_v.size());
    // Loop over score map created with matching algorithm
    for(auto& score_info : score_map) {

      auto&       match_info  = score_info.second;   // match information
      auto const& tpc_index   = match_info.tpc_id;   // matched tpc original id
      auto const& flash_index = match_info.flash_id; // matched flash original id

      // If this tpc object is already assigned (=better match found), ignore
      if(tpc_used.find(tpc_index) != tpc_used.end()) continue;

      // If this flash object is already assigned + re-use is not allowed, ignore
      if(!_allow_reuse_flash && flash_used.find(flash_index) != flash_used.end()) continue;

      // Reaching this point means a new match. Yay!
      if(_verbosity <= msg::kINFO) {
	std::stringstream ss;
	ss << "Concrete Match: " << " TPC=" << tpc_index << " Flash=" << flash_index
	   << " Score=" << match_info.score;
	Print(msg::kINFO,__FUNCTION__,ss.str());
      }

      // Register to a list of a "used" flash and tpc info
      tpc_used.insert(tpc_index);
      flash_used.insert(flash_index);

      // std::move matched info from the map to result vector
      result.emplace_back( match_info );
      
    }
    // Return result
    return result;
    
  }
}

#endif
