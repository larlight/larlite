/**
 * \file PSet.h
 *
 * \ingroup FhiclLite
 * 
 * \brief Class def header for a class PSet
 *
 * @author kazuhiro
 */

/** \addtogroup FhiclLite

    @{*/
#ifndef BASICTOOL_PSET_H
#define BASICTOOL_PSET_H

#include <iostream>
#include <string>
#include <map>
#include <TString.h>
#include "FhiclLiteException.h"
namespace fcl {
  /**
     \class PSet
     User defined class PSet ... these comments are used to generate
     doxygen documentation!
  */
  class PSet {
    
  public:
    
    /// Default constructor
    PSet(const std::string name="noname") : fName(name)
    {}

    /// Default destructor
    virtual ~PSet(){};

    /// Copy ctor
    PSet(const PSet& orig) : fName     ( orig.fName     ),
			     fParams_m ( orig.fParams_m ),
			     fPSets_m  ( orig.fPSets_m  )
    {}

    /// clear method
    void clear() 
    { fParams_m.clear(); fPSets_m.clear(); }

    /// Insert method for a simple param
    void set(const std::string key,
	     const std::string value);

    /// Insert method for a parameter set
    void set(const std::string key,
	     const PSet ps);

    /// Get value 
    const std::string& get_param(std::string key) const;

    /// Get PSet
    const PSet& get_pset(std::string key) const;

    /// Get writeable PSet reference
    PSet& get_pset_writeable(std::string key);

    /// Dump into a text format
    std::string dump() const;

  protected:

    std::string fName;

    std::map<std::string,std::string> fParams_m;

    std::map<std::string,PSet> fPSets_m;
    
  };
}

#endif
/** @} */ // end of doxygen group 

