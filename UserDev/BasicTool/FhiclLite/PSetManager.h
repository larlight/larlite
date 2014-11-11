/**
 * \file PSetManager.h
 *
 * \ingroup FhiclLite
 * 
 * \brief Class def header for a class PSet
 *
 * @author David Caratelli
 */

/** \addtogroup FhiclLite

    @{*/
#ifndef PSETMANAGER_H
#define PSETMANAGER_H

#include <iostream>
#include <string>
#include <map>
#include <TString.h>
#include "FhiclLiteException.h"
#include "PSet.h"

namespace fcl {
  /**
     \class PSetManager
     User defined class PSetManager ... these comments are used to generate
     Intended to be used to manage the Parameter Setting on a single file
     doxygen documentation!
  */
  class PSetManager {
    
  public:
    
    /// Default constructor
    PSetManager(const std::string name="noname") : _Name(name)
    { _currentSet = 0; _topSet = 0; }

    /// Default destructor
    virtual ~PSetManager(){};

    /// clear method
    void Reset() 
    { _set.clear(); }

    /// Add a set for this file
    void addSet(PSet ps) { _set.push_back(ps); }

    // Get Pointer to last element of _set
    PSet* getBackSet() { return &(_set.back()); }

    /// Returns a Pointer to the Current set
    /// the current set is the one to which
    /// Params should be added
    PSet* getCurrentSet() { return _currentSet; }

    /// Returns Top-level set
    PSet* getTopSet() { return _topSet; }

    /// Clear Top set
    void clearTopSet() { _topSet = 0; }

    /// Clear Current Set
    void clearCurrentSet() { _currentSet = 0; }

    /// Set Current Set
    void setCurrentSet(PSet *ps) { _currentSet = ps; }
    
    /// Set Top Set
    void setTopSet(PSet *ps) { _topSet = ps; }

    /// Dump
    void dump();

  protected:

    std::vector<PSet> _set;

    PSet *_currentSet;
    PSet *_topSet;

    std::string _Name;

  };
}

#endif
/** @} */ // end of doxygen group 

