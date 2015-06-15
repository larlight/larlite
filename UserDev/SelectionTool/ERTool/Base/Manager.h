/**
 * \file Manager.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class Manager
 *
 * @author kazuhiro
 */

/** \addtogroup ERTool

    @{*/
#ifndef ERTOOL_MANAGER_H
#define ERTOOL_MANAGER_H

#include <utility>
#include <set>
#include "AlgoBase.h"
#include "AnaBase.h"
#include "EventData.h"
#include "ParticleGraph.h"
#include "FhiclLite/ConfigManager.h"
#include <TStopwatch.h>
namespace ertool {

  /**
     \class Manager
     @brief Management tool for running lgoX (ertool::AlgoBase children) and FilterX (ertool::FilterBase children)
     This class interfaces between the framework and ERTool framework. Given a vector of shower data products
     (either lalrite::shower or larlite::mcshower), it runs FilterX and AlgoX to identify combination
     of shower data products for a specific interaction type defined by AlgoX (e.g. Pi0 decay). This is
     done by Manager::Process function. A user must provide a relevant AlgoX and FilterX to be used.
     In technical details, it takes the following steps:

     0) If FilterX is provided, run over input shower array to pre-select relevant showers  \n
     1) From candidate shower list, make all possible combination of N-showers where N is     \n
        specified by AlgoX (see ertool::AlgoBase::_nshowers).                             \n
     2) Loop over all possible combinations and call AlgoBase::Select function to determine \n
        the score for each combination. If the score is negative value, it is excluded from   \n
	the result (i.e. not selected).                                                       \n
     3) Order all combinations by the score (higher to lower) and report back the result.     \n
        When creating a result, no combination is allowed to make a duplicate use of a shower.\n
	For instance consider the case of having 3 showers ("a","b","c","d"), N=2, and score  \n
	values from AlgoX shown below:                                                      \n
	("a","b") ... score = 1.0  \n
	("a","c") ... score = 0.5  \n
	("a","d") ... score = -1.0 \n
	("b","c") ... score = -1.0 \n
	("b","d") ... score = -1.0 \n
	("c","d") ... score = 0.3  \n

	Then the selected combinations would be ("a","b") and ("c","d"). 
	In particular ("a","c") is excluded because "a" is aleady used by the combination ("a","b") 
	which has a higher score.


  */
  class Manager{
    
  public:
    enum ManagerStatus_t {
      kIDLE,       ///< status after creation/reset before initialize
      kINIT,       ///< status after initialize before processing
      kPROCESSING, ///< status after 1st processing before finalize
      kFINISHED    ///< status after finalize before reset
    };

  public:
    
    /// Default constructor
    Manager();
    
    /// Default destructor
    virtual ~Manager(){};

    /// FhiclLite config file adder
    void AddCfgFile(const std::string cfg_fname);

    /// Reset FhiclLite config file to be used
    void ClearCfgFile();

    /// Algo setter
    void AddAlgo(AlgoBase* a);

    /// Ana setter
    void AddAna(AnaBase* a);

    /// Process input data
    bool Process();

    /// Function to be called before Process()
    void Initialize();

    /// Function to be called after Process()
    void Finalize(TFile* fout=nullptr);

    /// Function to store an "output configuration" in a text file (full path + name in the argument)
    void StorePSet(const std::string& fname=kDefaultConfigFileName) const;

    /// Function to reset things
    void Reset();

    /// Function to add input data product: Shower
    NodeID_t Add(const ertool::Shower& obj, const ertool::RecoInputID_t& input_id, const bool mc=false);
    /// Function to add input data product: Track
    NodeID_t Add(const ertool::Track&  obj, const ertool::RecoInputID_t& input_id, const bool mc=false);
#ifndef __CINT__
    /// Function to add input data product: Shower
    NodeID_t Emplace(const ertool::Shower&& obj, const ertool::RecoInputID_t&& input_id, const bool mc=false);
    /// Function to add input data product: Track
    NodeID_t Emplace(const ertool::Track&&  obj, const ertool::RecoInputID_t&& input_id, const bool mc=false);
#endif


    /// Function to clear data
    void ClearData();

    const ertool::EventData&   EventData     () const;
    const ertool::EventData&   MCEventData   () const;
    ertool::ParticleGraph& ParticleGraph   ();
    ertool::ParticleGraph& MCParticleGraph ();

    /// Status getter
    ManagerStatus_t Status() const { return _status; }

    /// Training mode
    bool _training_mode;

    /// Profile mode
    bool _profile_mode;

    /// Make MC info available to ana
    bool _mc_for_ana;

    /// struct for time profiling
    struct _tprof_t {
      /// ctor
      _tprof_t(){ Reset(); }
      /// resetter
      void Reset()
      { _time_start = _time_proc = _time_end = 0; }
      double _time_start; ///< Time to execute ertool::UnitBase::ProcessBegin
      double _time_proc;  ///< Time to execute ertool::AnaBase::Analyze or ertool::AlgoBase::Reconstruct
      double _time_end;   ///< Time to execute ertool::UnitBase::ProcessEnd
    };
    
  protected:

    ertool::EventData _data;
    ertool::EventData _mc_data;
    ertool::ParticleGraph _graph;
    ertool::ParticleGraph _mc_graph;

    ManagerStatus_t _status;

    std::vector< ertool::AlgoBase*> _algo_v;
    std::vector< ertool::AnaBase*> _ana_v;
    std::set<std::string> _name_v;

    TStopwatch fWatch;
    std::vector<_tprof_t> _time_algo_v;
    std::vector<_tprof_t> _time_ana_v;
    
    ::fcllite::ConfigManager _cfg_mgr;
    std::set<std::string> _cfg_file_v;
  };
}
#endif
/** @} */ // end of doxygen group 

