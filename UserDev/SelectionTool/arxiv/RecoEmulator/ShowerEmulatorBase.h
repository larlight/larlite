/**
 * \file ShowerEmulatorBase.h
 *
 * \ingroup RecoEmulator
 * 
 * \brief Class def header for a class ShowerEmulatorBase
 *
 * @author kazuhiro
 */

/** \addtogroup RecoEmulator

    @{*/
#ifndef SHOWEREMULATORBASE_H
#define SHOWEREMULATORBASE_H

#include <iostream>
#include "RecoEmulatorTypes.h"
#include "RecoEmulatorBase.h"

namespace recoemu {
  /**
     \class ShowerEmulatorBase
     \brief Base class for shower reco emulation algorithm
  */
  class ShowerEmulatorBase : public RecoEmulatorBase{
    
  public:
    
    /// Default constructor
    ShowerEmulatorBase(const std::string name="noname") : RecoEmulatorBase(name) {}
    
    /// Default destructor
    virtual ~ShowerEmulatorBase(){}

    /// Emulator method to be implemented
    virtual recoemu::Shower_t Emulate(const recoemu::Shower_t&) = 0;

  };

}

#endif
/** @} */ // end of doxygen group 

