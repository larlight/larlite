/**
 * \file SPAlgoPE.h
 *
 * \ingroup SPAlgo
 * 
 * \brief Class def header for a class SPAlgoPE
 *
 * @author kazuhiro
 */

/** \addtogroup SPAlgo

    @{*/

#ifndef SELECTIONTOOL_SPALGOPE_H
#define SELECTIONTOOL_SPALGOPE_H

#include "SPAlgo/SPAlgoEMPart.h"
#include "SPTBase/SPAlgoBase.h"

namespace sptool {

  /**
     \class SPAlgoPE
     User custom SPAFilter class made by kazuhiro
   */
  class SPAlgoPE : public SPAlgoBase {
  
  public:

    /// Default constructor
    SPAlgoPE();

    /// Default destructor
    virtual ~SPAlgoPE(){};

    /// Reset function
    virtual void Reset();

    /// What to do before event-loop begins
    virtual void ProcessBegin();

    /// Override the sptool::SPTBase::LoadParams function
    virtual void LoadParams(std::string fname="",size_t version=kINVALID_SIZE);

    /// Function to evaluate input showers and determine a score
    virtual SPArticleSet Reconstruct(const SPAData &data);

    
  protected:

    SPAlgoEMPart _alg_emp;
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
