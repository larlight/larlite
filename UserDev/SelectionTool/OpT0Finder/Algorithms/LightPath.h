/**
 * \file LightPath.h
 *
 * \ingroup Algorithms
 * 
 * \brief Class def header for a class LightPath
 *
 * @author Rui
 */

/** \addtogroup Algorithms

    @{*/
#ifndef LIGHTPATH_H
#define LIGHTPATH_H

#include <iostream>
#include <numeric>
#include <functional>
#include <algorithm>
#include "OpT0Finder/Base/BaseAlgorithm.h"
#include "OpT0Finder/Base/CustomAlgoFactory.h"

namespace flashana{
/**
   \class LightPath
   User defined class LightPath ... these comments are used to generate
   doxygen documentation!
 */

  class LightPath : public flashana::BaseAlgorithm {
    
  public:
    
    /// Default constructor
    LightPath(const std::string name="LightPath");
    
    /// Default destructor
    ~LightPath(){}

    // Setter function
    double Set_Gap      ( double x) { _gap   =x;      return _gap;}
      
    // Flash Hypothesis for Trajectory (Track)
    flashana::QCluster_t FlashHypothesis(const larlite::geoalgo::Trajectory& trj) const;

    void QCluster(const larlite::geoalgo::Vector& pt_1,
                  const larlite::geoalgo::Vector& pt_2,
                  flashana::QCluster_t& Q_cluster,
		  double dedx=-1) const;

    // Getter for light yield configured paramater
    double GetLightYield() const { return _light_yield; }


  protected:

    void _Configure_(const Config_t &pset);
    
    double _gap;
    double _light_yield;
    double _dEdxMIP;

  };
  
  /**
     \class flashana::LightPathFactory
  */
  class LightPathFactory : public CustomAlgoFactoryBase {
  public:
    /// ctor
    LightPathFactory() { CustomAlgoFactory::get().add_factory("LightPath",this); }
    /// dtor
    ~LightPathFactory() {}
    /// creation method
    BaseAlgorithm* create(const std::string instance_name) { return new LightPath(instance_name); }
  };
} 

#endif
/** @} */ // end of doxygen group 

