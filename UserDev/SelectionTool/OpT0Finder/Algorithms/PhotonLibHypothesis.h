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

    /// Singleton shared instance getter
    static PhotonLibHypothesis& GetME()
    {
      if(!_me) _me = new PhotonLibHypothesis;
      return *_me;
    }

    void SetOpDetPositions( const std::vector<double>&,
                            const std::vector<double>&,
                            const std::vector<double>&);

    const std::vector<double>& ChargeHypothesis(const QCluster_t&,const double);

  protected:
    std::vector<double> _pmt_x_v; ///< PMT X position
    std::vector<double> _pmt_y_v; ///< PMT Y position
    std::vector<double> _pmt_z_v; ///< PMT Z position

    std::vector<double> _qll_hypothesis_v; ///< Hypothesis PE distribution over PMTs
    std::vector<double> _flash_pe_v;       ///< Flash PE distribution over PMTs
  };
}
#endif

/** @} */ // end of doxygen group 
