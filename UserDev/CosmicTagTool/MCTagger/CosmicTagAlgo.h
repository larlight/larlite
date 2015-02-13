/**
 * \file CosmicTagAlgo.h
 *
 * \ingroup MCTagger
 * 
 * \brief Class def header for a class CosmicTagAlgo
 *
 * @author Ariana Hackenburg 
 */

/** \addtogroup MCTagger

    @{*/
#ifndef LARLITE_COSMICTAGALGO_H
#define LARLITE_COSMICTAGALGO_H

#include <iostream>
#include "BasicTool/GeoAlgo/GeoAlgo.h"
#include "BasicTool/GeoAlgo/GeoAlgoConstants.h"
#include "LArUtil/Geometry.h"

/**
   \class CosmicTagAlgo
	This class has 2 functions:
	1) Calcualtes the likelihood of Half Line (simplified shower) being a cosmic
	based on its location in Box (detector).  
	2) Calculates the likelihood of simplified shower beign a cosmic based on its
	proximity to nearest track
 */
class CosmicTagAlgo{

public:

  /// Default constructor
  CosmicTagAlgo(){};

  /// Default destructor
  virtual ~CosmicTagAlgo(){};

  //Compute score given simplified shower and box
  float ShowerBoxScore(const ::geoalgo::HalfLine&,const ::geoalgo::AABox&);

  //Compute score given simplified shower and cosmic-tagged track
  float ShowerTrackScore(std::vector<::geoalgo::Trajectory>&,::geoalgo::HalfLine_t & );

};

#endif
/** @} */ // end of doxygen group 

