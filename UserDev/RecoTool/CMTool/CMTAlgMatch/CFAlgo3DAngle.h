/**
 * \file CFAlgo3DAngle.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CFAlgo3DAngle
 *
 * @author ariana hackenburg
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CFALGO3DANGLE_H
#define RECOTOOL_CFALGO3DANGLE_H

#include "CMTool/CMToolBase/CFloatAlgoBase.h"

namespace cmtool {
  /**
     \class CFAlgo3DAngle
     User implementation for CFloatAlgoBase class
     doxygen documentation!
  */
  class CFAlgo3DAngle : public CFloatAlgoBase {
    
  public:
    
    /// Default constructor
    CFAlgo3DAngle();
   
    /// Default destructor
    virtual ~CFAlgo3DAngle(){};

    /**
      Calculate 3d angles from all permutations of the 3 planes.  Weight them according to
	  charge profile. Choose the 2 best weights and compare those 2 theta and phi 
    */
    virtual float Float(const std::vector<const cluster::ClusterParamsAlg*> &clusters);

	/**
		If phi or theta are <360 (or greater than), adjust them to be in a more easily
		comparable range.
	*/
	virtual void FixPhiTheta(double &phi, double &theta) ;

	//Order the theta, phi, hits per plane to make cuts convenient
	virtual void SetMaxMiddleMin(const double first, const double second, const double third, double &most, double &middle, double &least) ;

	void SetDebug(bool debug) { _debug = debug ; }
	
	void SetThetaCut(float theta_cut ) { _theta_cut = theta_cut ; }

	void SetPhiCut(float phi_cut ) { _phi_cut = phi_cut ; }

	void SetRatio(float ratio ) { _ratio_cut = ratio ; }

    virtual void Report();
    virtual void Reset();

	protected:
	
	float _theta_cut ;
	float _phi_cut	 ; 
	float _ratio_cut ;
	float _debug 	 ;

	//Histogram for charge profile
//	TH1D *cProfile ;

  };
}
#endif
/** @} */ // end of doxygen group 

