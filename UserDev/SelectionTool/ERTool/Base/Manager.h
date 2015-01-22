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

#include <iostream>
#include <set>
#include <map>
#include <algorithm>
#include "AlgoBase.h"
#include "FilterBase.h"
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

    /// Algo setter
    void SetAlgo(AlgoBase* a);

    /// Filter setter
    void SetFilter(FilterBase* f);

    /// Process input data
    ParticleSet Process(EventData& data);

    /// Function to be called before Process()
    void Initialize();

    /// Function to be called after Process()
    void Finalize(TFile* fout=nullptr);

    /// Function to reset things
    void Reset();

    /// Status getter
    ManagerStatus_t Status() const { return _status; }

    /// Training mode
    bool _training_mode;

    /// Profile mode
    bool _profile_mode;

  protected:

    std::pair<double,double> _tprof_algo, _tprof_filter;
    double _time_algo_init, _time_filter_init;
    double _time_algo_finalize, _time_filter_finalize;

    TStopwatch fWatch;

    ManagerStatus_t _status;

    AlgoBase* _algo;

    FilterBase* _filter;

  };
}
#endif
/** @} */ // end of doxygen group 

