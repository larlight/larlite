/**
 * \file Pi0ProfileCut.h
 *
 * \ingroup ShowerStudy
 * 
 * \brief Class def header for a class Pi0ProfileCut
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerStudy

    @{*/
#ifndef PI0PROFILECUT_H
#define PI0PROFILECUT_H

#include <iostream>
#include <vector>
#include <cmath>
#include "BasicTool/GeoAlgo/GeoAlgo.h"
#include "ShowerAnaException.h"

namespace showerana {
  /**
     \class Pi0ProfileCut
     User defined class Pi0ProfileCut ... these comments are used to generate
     doxygen documentation!
  */
  class Pi0ProfileCut{
    
  public:
    
    /// Default constructor
    Pi0ProfileCut();

    /// Input data setter
    void SetDaughterInfo( std::vector<double> const& a_vtx,
			  std::vector<double> const& b_vtx,
			  std::vector<double> const& a_dir,
			  std::vector<double> const& b_dir );

    /// Resetter of data
    void Reset();
    
    /// Default destructor
    virtual ~Pi0ProfileCut(){};
    
  protected:

    std::vector<double> _vtx;   ///< Pi0 vtx point (x,y,z,error)
    
    std::vector<double> _a_vtx; ///< Daughter A's vtx
    std::vector<double> _b_vtx; ///< Daughter B's vtx
    
    std::vector<double> _a_dir; ///< Daughter A's dir 
    std::vector<double> _b_dir; ///< Daughter B's dir


    //Geometry algo instance
    geoalgo::GeoAlgo _myGeoAlgo;

  };
}

#endif
/** @} */ // end of doxygen group 

