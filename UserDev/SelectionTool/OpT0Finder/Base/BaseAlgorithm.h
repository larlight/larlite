/**
 * \file BaseAlgorithm.h
 *
 * \ingroup Base
 * 
 * \brief Class def header for a class BaseAlgorithm
 *
 * @author kazuhiro
 */

/** \addtogroup Base

    @{*/
#ifndef OPT0FINDER_BASEALGORITHM_H
#define OPT0FINDER_BASEALGORITHM_H

#include "OpT0FinderTypes.h"
#include "ColorPrint.h"
#include "GeoAlgo/GeoAABox.h"
#include "FhiclLite/PSet.h"
#include <vector>
namespace flashana {

  class FlashMatchManager;
  
  /**
     \class BaseAlgorithm
  */
  class BaseAlgorithm : public ColorPrint {

    friend class FlashMatchManager;
    
  public:
    
    /// Default constructor
    BaseAlgorithm(const Algorithm_t type, const std::string name);
    
    /// Default destructor
    ~BaseAlgorithm(){}

    /// Function to accept configuration
    virtual void Configure(const ::fcllite::PSet &pset) = 0;

    /// Algorithm type
    Algorithm_t AlgorithmType() const;

    /// Algorithm name
    const std::string& AlgorithmName() const;

    double OpDetX(size_t i) const; ///< PMT X position getter
    double OpDetY(size_t i) const; ///< PMT Y position getter 
    double OpDetZ(size_t i) const; ///< PMT Z position getter

    const ::geoalgo::AABox& ActiveVolume() const; ///< Detector active volume getter
    
    double ActiveXMax() const; ///< Detector active volume's maximum x value
    double ActiveYMax() const; ///< Detector active volume's maximum x value
    double ActiveZMax() const; ///< Detector active volume's maximum x value
    double ActiveXMin() const; ///< Detector active volume's minimum x value
    double ActiveYMin() const; ///< Detector active volume's minimum x value
    double ActiveZMin() const; ///< Detector active volume's minimum x value    
    
    const std::vector<double>& OpDetXArray() const; ///< PMT X position array getter
    const std::vector<double>& OpDetYArray() const; ///< PMT Y position array getter
    const std::vector<double>& OpDetZArray() const; ///< PMT Z position array getter

    size_t NOpDets() const; ///< Getter for # of PMTs
    
  private:
    
    void SetOpDetPositions( const std::vector<double>& pos_x,
			    const std::vector<double>& pos_y,
			    const std::vector<double>& pos_z );

    void SetActiveVolume( const double xmin, const double xmax,
			  const double ymin, const double ymax,
			  const double zmin, const double zmax );
    
    Algorithm_t _type; ///< Algorithm type
    std::string _name; ///< Algorithm name
    std::vector<double> _opdet_x_v;  ///< OpticalDetector X position array
    std::vector<double> _opdet_y_v;  ///< OpticalDetector Y position array
    std::vector<double> _opdet_z_v;  ///< OpticalDetector Z position array
    ::geoalgo::AABox _active_volume; ///< Detector active volume
  };
}
#endif
/** @} */ // end of doxygen group 

