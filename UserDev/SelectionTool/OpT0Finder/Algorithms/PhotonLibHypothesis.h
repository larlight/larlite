/**
 * \file PhotonLibHypothesis.h
 *
 * \ingroup Algorithms
 * 
 * \brief Class def header for a class PhotonLibHypothesis
 *
 * @author yuntse
 */

/** \addtogroup Algorithms

    @{*/

#ifndef PHOTONLIBHYPOTHESIS_H
#define PHOTONLIBHYPOTHESIS_H

#include <iostream>
#include "OpT0Finder/Base/BaseFlashHypothesis.h"
#include <TTree.h>

namespace flashana {
  /**
     \class PhotonLibHypothesis
     User custom analysis class made by SHELL_USER_NAME
   */
  class PhotonLibHypothesis : public BaseFlashHypothesis {
  
  public:

    /// Default constructor
    PhotonLibHypothesis(const std::string name="PhotonLibHypothesis");

    /// Default destructor
    virtual ~PhotonLibHypothesis(){}

    void Configure(const ::fcllite::PSet &pset);

    void FillEstimate(const QCluster_t&, Flash_t&) const;

  };
}
#endif

/** @} */ // end of doxygen group 
