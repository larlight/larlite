/**
 * \file RecoEmulatorBase.h
 *
 * \ingroup RecoEmulator
 * 
 * \brief Class def header for a class RecoEmulatorBase
 *
 * @author kazuhiro
 */

/** \addtogroup RecoEmulator

    @{*/
#ifndef __RECOEMULATORBASE_H__
#define __RECOEMULATORBASE_H__

#include <iostream>
#include "FhiclLite/PSet.h"

namespace recoemu {
  /**
     \class RecoEmulatorBase
     \brief Base class for reco emulation algorithms for track and shower
     Base class for reco emulation algorithms for track and shower. \n
     It uses FhiclLite package for instance configuration. \n
  */
  class RecoEmulatorBase{
    
  public:
    
    /// Default constructor: name is used to find a configuration parameter set
    RecoEmulatorBase(const std::string name="noname") : _name(name) {}
    
    /// Default destructor
    virtual ~RecoEmulatorBase(){}

    /// Name accessor
    const std::string& Name() const { return _name; }

    /// Configuration method
    virtual void Configure(const ::fcllite::PSet &pset) = 0;

  private:
    /// Name: also a keyword to locate a configuration parameter set
    std::string _name;
  
  };
}
#endif
/** @} */ // end of doxygen group 

