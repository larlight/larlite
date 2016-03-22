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

    /// Default destructor. Somehow not writing out my TTree. Meh, EC, 17-Mar-2017
    virtual ~PhotonLibHypothesis(){ 
      std::cout << "PhotonLibHypothesis():~PhotonLibHypothesis(): Where am I Write()'ing the *seen-by-track* pvlib _photlib_tree? Nowhere Eric can find it, it would seem." << std::endl;
      _photlib_tree->Write(); 
      if (_photlib_tree) delete _photlib_tree; 
    }

    void Configure(const ::fcllite::PSet &pset);

    void FillEstimate(const QCluster_t&, Flash_t&) const;
    void FillTree(const QCluster_t&);

    TTree* _photlib_tree;
    double _pvl_x;
    double _pvl_y;
    double _pvl_z;
    double _pvl_vis;
    int  _pvl_pmt;

			
			
  };			
}			
#endif

/** @} */ // end of doxygen group 
