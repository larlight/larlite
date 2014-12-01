/**
 * \file SPAManager.h
 *
 * \ingroup ShowerPdfTool
 * 
 * \brief Class def header for a class SPAManager
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerPdfTool

    @{*/
#ifndef LARLITE_SPAMANAGER_H
#define LARLITE_SPAMANAGER_H

#include <iostream>
#include <set>
#include <map>
#include <algorithm>
#include "DataFormat/shower.h"
#include "DataFormat/mcshower.h"
#include "SPAlgoBase.h"
#include "SPFilterBase.h"

namespace sptool {

  typedef std::vector<size_t> SPMatch_t;

  /**
     \class SPAManager
     @brief Management tool for running SPAlgoX (sptool::SPAlgoBase children) and SPFilterX (sptool::SPFilterBase children)
     This class interfaces between the framework and SPT framework. Given a vector of shower data products
     (either lalrite::shower or larlite::mcshower), it runs SPFilterX and SPAlgoX to identify combination
     of shower data products for a specific interaction type defined by SPAlgoX (e.g. Pi0 decay). This is
     done by SPAManager::Process function. A user must provide a relevant SPAlgoX and SPFilterX to be used.
     In technical details, it takes the following steps:

     0) If SPFilterX is provided, run over input shower array to pre-select relevant showers  \n
     1) From candidate shower list, make all possible combination of N-showers where N is     \n
        specified by SPAlgoX (see sptool::SPAlgoBase::_nshowers).                             \n
     2) Loop over all possible combinations and call SPAlgoBase::Select function to determine \n
        the score for each combination. If the score is negative value, it is excluded from   \n
	the result (i.e. not selected).                                                       \n
     3) Order all combinations by the score (higher to lower) and report back the result.     \n
        When creating a result, no combination is allowed to make a duplicate use of a shower.\n
	For instance consider the case of having 3 showers ("a","b","c","d"), N=2, and score  \n
	values from SPAlgoX shown below:                                                      \n
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
  class SPAManager{
    
  public:
    
    /// Default constructor
    SPAManager();
    
    /// Default destructor
    virtual ~SPAManager(){};

    /// SPAlgo setter
    void SetSPAlgo(SPAlgoBase* a) { _algo = a; }

    /// Filter setter
    void SetFilter(SPFilterBase* f) { _filter = f; }

    /// Process input data
    std::vector<std::pair<float,sptool::SPMatch_t> > Process(const SPAData& data);

  protected:

    SPAlgoBase* _algo;

    SPFilterBase* _filter;
    
  };
}
#endif
/** @} */ // end of doxygen group 

