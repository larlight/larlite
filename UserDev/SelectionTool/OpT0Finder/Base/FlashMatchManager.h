/**
 * \file FlashMatchManager.h
 *
 * \ingroup Base
 * 
 * \brief Class def header for a class FlashMatchManager
 *
 * @author kazuhiro
 */

/** \addtogroup Base

    @{*/
#ifndef OPT0FINDER_FLASHMATCHMANAGER_H
#define OPT0FINDER_FLASHMATCHMANAGER_H

#include "LoggerFeature.h"
#include "OpT0FinderFMWKInterface.h"
#include "BaseAlgorithm.h"
#include "BaseTPCFilter.h"
#include "BaseFlashFilter.h"
#include "BaseProhibitAlgo.h"
#include "BaseFlashMatch.h"
#include "BaseFlashHypothesis.h"
namespace flashana {
  /**
     \class FlashMatchManager
  */
  class FlashMatchManager : public LoggerFeature {

  public:
    
    /// Default constructor
    FlashMatchManager(const std::string name="FlashMatchManager");
    
    /// Default destructor
    ~FlashMatchManager(){}

    /// Name getter
    const std::string& Name() const;

    /// Configuration
    void Configure(const Config_t& cfg);

    /// Algorithm getter
    flashana::BaseAlgorithm* GetAlgo(flashana::Algorithm_t type);

    /// Custom algorithm getter
    flashana::BaseAlgorithm* GetCustomAlgo(std::string name);
		 
#ifndef __CINT__
    /// Emplacer of a TPC object (hidden from ROOT5 CINT)
    void Emplace(flashana::QCluster_t&& obj);
    /// Emplacer of a TPC object (hidden from ROOT5 CINT)
    void Emplace(flashana::Flash_t&& obj);
#endif   
    /// Adder of a TPC object
    void Add(flashana::QCluster_t& obj);
    /// Adder of a TPC object
    void Add(flashana::Flash_t& obj);

    /**
       CORE FUNCTION: executes algorithms to find a match of TPC object and flash provided by users. \n
       The execution takes following steps:             \n
       0) TPC filter algorithm if provided (optional)   \n
       1) Flash filter algorithm if provided (optional) \n
       3) Flash matching algorithm (required)           \n
       4) Returns match information for created TPC object & flash pair which respects the outcome of 3)
     */
    std::vector<flashana::FlashMatch_t> Match();

    /// Clears locally kept TPC object (QClusterArray_t) and flash (FlashArray_t), both provided by a user
    void Reset()
    { _tpc_object_v.clear(); _flash_v.clear(); }

    /// Configuration option: true => allows an assignment of the same flash to multiple TPC objects
    void CanReuseFlash(bool ok=true)
    { _allow_reuse_flash = ok; }

    void PrintConfig();

    /// Access to an input: TPC objects in the form of QClusterArray_t
    const QClusterArray_t& QClusterArray() const { return _tpc_object_v; }

    /// Access to an input: PMT objects in the form of FlashArray_t
    const FlashArray_t& FlashArray() const { return _flash_v; }

    /// Access to a full results (if configured to store) for [tpc][flash] indexing
    const std::vector<std::vector<flashana::FlashMatch_t> > FullResultTPCFlash() const
    { return _res_tpc_flash_v; }

    /// Access to a full results (if configured to store) for [flash][tpc] indexing
    const std::vector<std::vector<flashana::FlashMatch_t> > FullResultFlashTPC() const
    { return _res_flash_tpc_v; }

  private:

    void AddCustomAlgo(BaseAlgorithm* alg);

    BaseFlashFilter*     _alg_flash_filter;     ///< Flash filter algorithm
    BaseTPCFilter*       _alg_tpc_filter;       ///< TPC filter algorithm
    BaseProhibitAlgo*    _alg_match_prohibit;   ///< Flash matchinig prohibit algorithm
    BaseFlashMatch*      _alg_flash_match;      ///< Flash matching algorithm
    BaseFlashHypothesis* _alg_flash_hypothesis; ///< Flash hypothesis algorithm

    /**
       A set of custom algorithms (not to be executed but to be configured)
    */
    std::map<std::string,flashana::BaseAlgorithm*> _custom_alg_m;

    /// TPC object information collection (provided by a user)
    QClusterArray_t _tpc_object_v;
    /// Flash object information collection (provided by a user)
    FlashArray_t _flash_v;
    /// Configuration option to allow re-use of a flash (i.e. 1 flash can be assigned to multiple TPC object)
    bool _allow_reuse_flash;
    /// Configuration readiness flag
    bool _configured;
    /// Configuration file
    std::string _config_file;
    /// Name
    std::string _name;
    /// Request boolean to store full matching result (per Match function call)
    bool _store_full;
    /// Full result container indexed by [tpc][flash]
    std::vector<std::vector<flashana::FlashMatch_t> > _res_tpc_flash_v;
    /// Full result container indexed by [flash][tpc]
    std::vector<std::vector<flashana::FlashMatch_t> > _res_flash_tpc_v;    
  };
}

#endif
/** @} */ // end of doxygen group 

