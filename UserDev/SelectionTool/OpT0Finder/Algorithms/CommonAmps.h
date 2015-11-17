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
    CommonAmps(const double x_step_size=-1);
    
    CommonAmps( const std::vector<double>& pos_x,
		const std::vector<double>& pos_y,
		const std::vector<double>& pos_z,
		const double x_step_size=-1);
    
    /// Default destructor
    ~CommonAmps(){}

    FlashMatch_t Match(const QCluster_t&, const Flash_t&);

    void SetStepSize(const double x)  { _x_step_size = x; }

    void SetPercent(float amp) { _percent = amp ; }

    void SetScore(float score) { _score = score; }

    void UsePhotonLibrary(bool doit=true) {_use_library=doit;}

  private:

    std::vector<double> _pos_x;
    std::vector<double> _pos_y;
    std::vector<double> _pos_z;
    double _x_step_size;
    bool _use_library;

    float _percent;
    float _score ;

    std::vector<double> _vis_array;
    std::vector<double> _op_array;
    std::vector<std::vector<double>> _vis ;
    std::vector<std::vector<double>> _op ;

  };
}
#endif
/** @} */ // end of doxygen group 

