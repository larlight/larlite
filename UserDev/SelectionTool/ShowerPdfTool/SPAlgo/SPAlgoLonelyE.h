/**
 * \file SPAlgoLonelyE.h
 *
 * \ingroup SPAlgo
 * 
 * \brief Class def header for a class SPAlgoLonelyE
 *
 * @author david
 */

/** \addtogroup SPAlgo

    @{*/

#ifndef SELECTIONTOOL_SPALGOLONELYE_H
#define SELECTIONTOOL_SPALGOLONELYE_H

#include "SPTBase/SPAlgoBase.h"
#include "SPAlgo/SPAlgoSingleE.h"
// include GeoAlgo functions
#include "GeoAlgo/GeoAlgoConstants.h"
#include "GeoAlgo/GeoAlgo.h"

namespace sptool {
  
  /**
     \class SPAlgoLonelyE
     User custom SPAFilter class made by david
  */
  class SPAlgoLonelyE : public SPAlgoBase {
    
  public:
    
    /// Default constructor
    SPAlgoLonelyE();

    /// Default destructor
    virtual ~SPAlgoLonelyE(){};

    /// Reset function
    virtual void Reset();

    /// What to do before event-loop begins
    virtual void ProcessBegin();

    /// Override the sptool::SPTBase::LoadParams function
    virtual void LoadParams(std::string fname="",size_t version=kINVALID_SIZE);

    /// Function to evaluate input showers and determine a score
    virtual SPArticleSet Reconstruct(const SPAData &data);
    
    /// Likelyhood function where to figure out if shower is lonely
    double LL(const SPArticle shr, const SPAData &data);

  private:

    geoalgo::GeoAlgo _geoAlgo;

    // instance of SPAlgoSingleE
    SPAlgoSingleE _alg_singleE;
    
  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
