/**
 * \file ShowerRecoAlgBase.h
 *
 * \ingroup ShowerReco3D
 * 
 * \brief Class def header for a class ShowerRecoAlgBase
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerReco3D

    @{*/
#ifndef RECOTOOL_SHOWERRECOALGBASE_H
#define RECOTOOL_SHOWERRECOALGBASE_H

#include <iostream>
#include "ShowerRecoException.h"
#include "ShowerRecoTypes.h"

namespace showerreco {

  /**
     \class ShowerRecoAlgBase
     User defined class ShowerRecoAlgBase ... these comments are used to generate
     doxygen documentation!
  */
  class ShowerRecoAlgBase{
    
  public:
    
    /// Default constructor
    ShowerRecoAlgBase();
    
    /// Default destructor
    virtual ~ShowerRecoAlgBase(){}

    /// Function to reset algorithm, to be called @ beginning of each event
    virtual void Reset() = 0;

    /// Function to reconstruct one shower
    virtual Shower_t RecoOneShower(const ShowerClusterSet_t& clusters) = 0;
    
    /// Verbosity switch
    void Verbose(bool on=true);

  protected:

    /// Verbosity flag
    bool _verbosity;

  };
}

#endif
/** @} */ // end of doxygen group 

