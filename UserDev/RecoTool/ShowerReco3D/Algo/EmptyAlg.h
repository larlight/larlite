/**
 * \file EmptyAlg.h
 *
 * \ingroup ShowerReco3D
 * 
 * \brief Class def header for a class EmptyAlg
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerReco3D

    @{*/
#ifndef SHOWERRECO3D_ALGO_EMPTYALG_H
#define SHOWERRECO3D_ALGO_EMPTYALG_H

#include <iostream>
#include "ShowerReco3D/Base/ShowerRecoAlgBase.h"

namespace showerreco {

  /**
     \class EmptyAlg
     User defined class EmptyAlg ... these comments are used to generate
     doxygen documentation!
  */
  class EmptyAlg : public ShowerRecoAlgBase {
    
  public:
    
    /// Default constructor
    EmptyAlg();
    
    /// Default destructor
    ~EmptyAlg(){}

    /// Function to reset algorithm, to be called @ beginning of each event
    void Reset(){}

    /// Function to reconstruct one shower
    Shower_t RecoOneShower(const ShowerClusterSet_t& clusters);
    
  };
}

#endif
/** @} */ // end of doxygen group 

