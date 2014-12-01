/**
 * \file SPTBase.h
 *
 * \ingroup ShowerPdfTool
 * 
 * \brief Class def header for a class SPTBase
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerPdfTool

    @{*/
#ifndef SELECTIONTOOL_SPTBASE_H
#define SELECTIONTOOL_SPTBASE_H

#include <iostream>
#include <TChain.h>
#include <TFile.h>
#include <TError.h>
#include "DataFormat/user_info.h"
#include "SPAException.h"
#include "SPTEnv.h"
namespace sptool {
  /**
     \class SPTBase
     @brief A base class for sptool::SPFilter and sptool::SPAlgo, which implements parameter storage feature.
     The mere purpose of this class is to implement a feature to store multiple variables (name & value).
     In order to achieve this, two attribute variables (and relevant functions) are introduced:

     0) string "name" of a class ... _name protected attribute             \n
     1) dynamic parameter value container ... _params protected attribute  \n

     where 1) uses larlite::user_info class and hence can store bool/double/int & vector of those with
     specified variable name (see larlite::user_info for details). Since user_info is a data product that inherits 
     from TObject, it can be directly stored in TTree.
     
     The function SPTBase::StoreParams which takes a storage TFile name as an input argument implements such 
     storage feature.  If no argument is given, default filename for SPT package (sptool::kSPTDataFileName) 
     will be used. In order to identify a unique TTree for a specific class, _name is used to name the storage 
     TTree. If one calls SPTBase::StoreParams multiple times, each call stores a distinct entry in a TTree.
     Then the entry number in TTree is used as a version number of stored parameter sets. 

     A stored set of parameters (TTree) can be read-in via function SPTBase::LoadParams. The first input argument 
     to this function should be the TFile name. The second argument can specify the parameter version numbers. If 
     the opened TFile does not contain a relevant TTree or stored TTree does not contain enough entries for specified 
     version number, it throws an exception (sptool::SPTException).

  */
  class SPTBase{
    
  public:
    
    /// Default constructor
    SPTBase(){ _name = "sptbase"; }
    
    /// Default destructor
    virtual ~SPTBase(){}
    
    /// Parameter (larlite::user_info) getter
    const ::larlite::user_info& Params() { return _params; }

    /// Parameter (larlite::user_info) getter
    ::larlite::user_info& WriteableParams() { return _params; }

    /// Load parameter values from a TFile. For official values, use default filename.
    void LoadParams(std::string fname="",size_t version=kINVALID_SIZE);

    /// Store parameter values to a TFile. For official values, use default filename.
    void StoreParams(std::string fname="");

  protected:

    /// Name of this algorithm, used for storing data
    std::string _name;

    /// Parameter set for an algorithm. Stored in a data file.
    ::larlite::user_info _params;

  };
}

#endif
/** @} */ // end of doxygen group 

