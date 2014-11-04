/**
 * \file ShowerCalo.h
 *
 * \ingroup ShowerReco3D
 * 
 * \brief Class def header for a class ShowerCalo
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerReco3D

    @{*/
#ifndef RECOTOOL_SHOWERCALO_H
#define RECOTOOL_SHOWERCALO_H

#include <iostream>
#include <map>
#include "ShowerRecoException.h"
#include <TString.h>
namespace showerreco {

  namespace energy {

  enum RecoType_t {
    kDEFAULT
  };
    
    // Define constants to be used by ShowerCalo
    static const double DEFAULT_ECorr = 1.75133;
    static const double DEFAULT_ECorrError = 0.173707;
    
  }
}

namespace showerreco {

  /**
     \class ShowerCalo
     User defined class ShowerCalo ... these comments are used to generate
     doxygen documentation!
  */
  class ShowerCalo {
    
  public:
    
    /// Default constructor
    ShowerCalo(){};
    
    /// Default destructor
    virtual ~ShowerCalo(){};

    std::pair<double,double> ShowerEnergy(double energy, 
					  ::showerreco::energy::RecoType_t type=energy::kDEFAULT, 
					  bool mc=true);

  };
}


#endif
/** @} */ // end of doxygen group 

