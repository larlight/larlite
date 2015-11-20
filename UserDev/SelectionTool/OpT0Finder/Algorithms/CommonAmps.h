/**
 * \file CommonAmps.h
 *
 * \ingroup Algorithms
 * 
 * \brief Class def header for a class CommonAmps
 *
 * @author ariana Hackenburg 
 */

/** \addtogroup Algorithms

    @{*/
#ifndef OPT0FINDER_COMMONAMPS_H
#define OPT0FINDER_COMMONAMPS_H

#include "OpT0Finder/Base/BaseFlashMatch.h"
#include "TH1D.h"

namespace flashana {
  
  /**
     \class CommonAmps
     Implementation of flashana::BaseFlashHypothesis algorithm class. \n
     The goal of this algorithm is to compare the most prominent pieces of opflash spectra, 
     and spectra produced by photon library given QCluster points. 
     Tpc_point calculated as in QWeightPoint and stored based on the best match amplitudes.

     Only works with Photon Library currently
  */
  class CommonAmps : public BaseFlashMatch {
    
  public:
    
    /// Default constructor
    CommonAmps(const std::string name="CommonAmps");
    
    /// Default destructor
    ~CommonAmps(){}

    void Configure(const ::fcllite::PSet &pset);

    FlashMatch_t Match(const QCluster_t&, const Flash_t&);

  private:

    float _percent;
    float _score ;
    float _x_step_size;
    flashana::QCluster_t _tpc_qcluster;
    flashana::Flash_t    _vis_array;

  };
}
#endif
/** @} */ // end of doxygen group 

