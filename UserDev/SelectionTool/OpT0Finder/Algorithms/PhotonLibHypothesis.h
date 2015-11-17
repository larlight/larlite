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

namespace flashana {
  /**
     \class PhotonLibHypothesis
     User custom analysis class made by SHELL_USER_NAME
   */
  class PhotonLibHypothesis : public BaseFlashHypothesis {
  
  public:

    /// Default constructor
    PhotonLibHypothesis(){ _name="PhotonLibHypothesis"; _fout=0;}

    /// Default destructor
    virtual ~PhotonLibHypothesis(){}

    void FillEstimate(const QCluster_t&, Flash_t&);

  };
}
#endif

/** @} */ // end of doxygen group 
