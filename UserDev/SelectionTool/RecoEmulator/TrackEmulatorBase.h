/**
 * \file TrackEmulatorBase.h
 *
 * \ingroup RecoEmulator
 * 
 * \brief Class def header for a class TrackEmulatorBase
 *
 * @author kazuhiro
 */

/** \addtogroup RecoEmulator

    @{*/
#ifndef TRACKEMULATORBASE_H
#define TRACKEMULATORBASE_H

#include <iostream>
#include "RecoEmulatorTypes.h"
#include "RecoEmulatorBase.h"
namespace recoemu {
  /**
     \class TrackEmulatorBase
     \brief Base class for track reco emulation algorithm
  */
  class TrackEmulatorBase : public RecoEmulatorBase{
    
  public:
    
    /// Default constructor
    TrackEmulatorBase(const std::string name="noname") : RecoEmulatorBase(name) {}
    
    /// Default destructor
    virtual ~TrackEmulatorBase(){}

    /// Emulator method to be implemented
    virtual recoemu::Track_t Emulate(const recoemu::Track_t&) = 0;
    
  };
}

#endif
/** @} */ // end of doxygen group 

