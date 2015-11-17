/**
 * \file ChargeAnalytical.h
 *
 * \ingroup Algorithms
 * 
 * \brief Class def header for a class ChargeAnalytical
 *
 * @author matthias
 */

/** \addtogroup Algorithms

    @{*/
#ifndef CHARGEANALYTICAL_H
#define CHARGEANALYTICAL_H

#include "OpT0Finder/Base/BaseFlashMatch.h"


#include <iostream>

namespace flashana {

  /**
     \class ChargeAnalytical
     User defined class ChargeAnalytical ... these comments are used to generate
     doxygen documentation!
  */
  class ChargeAnalytical : public BaseFlashHypothesis {
    
  public:
    
    /// Default constructor
    ChargeAnalytical(const std::string name);
    
    /// Default destructor
    ~ChargeAnalytical() { }

    void Configure(const ::fcllite::PSet &pset);
    
    void FillEstimate(const QCluster_t&, Flash_t&) const;
    
  };
}

#endif
/** @} */ // end of doxygen group 

