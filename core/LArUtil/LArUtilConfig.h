/**
 * \file LArUtilConfig.h
 *
 * \ingroup LArUtil
 * 
 * \brief Class def header for a class LArUtilConfig
 *
 * @author kazuhiro
 */

/** \addtogroup LArUtil

    @{*/
#ifndef LARLITE_LARUTILCONFIG_H
#define LARLITE_LARUTILCONFIG_H

#include <iostream>
#include "Base/GeoConstants.h"
#include "Base/larlite_base.h"
#include "TString.h"
namespace larutil {
  /**
     \class LArUtilConfig
     User defined class LArUtilConfig ... these comments are used to generate
     doxygen documentation!
  */
  class LArUtilConfig : public larlite::larlite_base {
    
  private:
    
    /// Default constructor
    LArUtilConfig(){};
    
    /// Default destructor
    virtual ~LArUtilConfig(){};
    
    static larlite::geo::DetId_t _detector;
    
  public:
    
    static bool SetDetector(larlite::geo::DetId_t type);
    
    static larlite::geo::DetId_t Detector() { return _detector;}
    
  };
}

#endif
/** @} */ // end of doxygen group 

