/**
 * \file Record.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class Record
 *
 * @author kazuhiro
 */

/** \addtogroup ERTool

    @{*/
#ifndef ERTOOL_RECORD_H
#define ERTOOL_RECORD_H

#include <iostream>
#include <TChain.h>
#include <TFile.h>
#include <TError.h>
#include "ERException.h"
#include "ERToolConstants.h"
#include "ERToolTypes.h"
#include "Params.h"

namespace ertool {
  /**
     \class Record
     @brief A base class for ertool::Filter and ertool::Algo, which implements parameter storage feature.
     The mere purpose of this class is to implement a feature to store multiple variables (name & value).
     In order to achieve this, two attribute variables (and relevant functions) are introduced:

     0) string "name" of a class ... _name protected attribute             \n
     1) dynamic parameter value container ... _params protected attribute  \n

     where 1) uses ertool::Params class and hence can store bool/double/int & vector of those with
     specified variable name (see ertool::Params for details). Since user_info is a data product that inherits 
     from TObject, it can be directly stored in TTree.
     
     The function ertool::Params::StoreParams which takes a storage TFile name as an input argument implements such 
     storage feature.  If no argument is given, default filename for ERTool package (ertool::kDataFileName) 
     will be used. In order to identify a unique TTree for a specific class, _name is used to name the storage 
     TTree. If one calls ertool::Params::StoreParams multiple times, each call stores a distinct entry in a TTree.
     Then the entry number in TTree is used as a version number of stored parameter sets. 

     A stored set of parameters (TTree) can be read-in via function ertool::Params::LoadParams. The first input argument 
     to this function should be the TFile name. The second argument can specify the parameter version numbers. If 
     the opened TFile does not contain a relevant TTree or stored TTree does not contain enough entries for specified 
     version number, it throws an exception (ertool::ERException).

  */
  class Record {
    
  public:
    
    /// Default constructor
    Record(){ _name = "record"; }
    
    /// Default destructor
    virtual ~Record(){}

    /// Name
    const std::string& Name() { return _name; }
    
    /// Parameter (Record) getter
    const ::ertool::Params& Params() { return _params; }

    /// Parameter (larlite::user_info) getter
    ::ertool::Params& WriteableParams() { return _params; }

    /// Load parameter values from a TTree in a TFile. For official values, use default filename.
    virtual void LoadParams(std::string fname="",size_t version=kINVALID_SIZE);

    /// Store parameter values to a TFile. For official values, use default filename.
    virtual void StoreParams(std::string fname="");

  protected:

    /// Name of this algorithm, used for storing data
    std::string _name;

    /// Parameter set for an algorithm. Stored in a data file.
    ::ertool::Params _params;    

  };
}

#endif
/** @} */ // end of doxygen group 

