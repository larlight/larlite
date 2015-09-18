/**
 * \file CFAlgoChargeDistrib.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CFAlgoChargeDistrib
 *
 * @author David Caratelli
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CFALGOCHARGEDISTRIB_H
#define RECOTOOL_CFALGOCHARGEDISTRIB_H

#include "CMTool/CMToolBase/CFloatAlgoBase.h"

namespace cmtool {
  /**
     \class CFAlgoChargeDistrib
     User implementation for CFloatAlgoBase class
     doxygen documentation!
  */
  class CFAlgoChargeDistrib : public CFloatAlgoBase {
    
  public:
    
    /// Default constructor
    CFAlgoChargeDistrib();
    
    /// Default destructor
    virtual ~CFAlgoChargeDistrib(){};

    //
    // Author should be aware of 3 functions at least: Float, Report, 
    // and Reset. More possibly-useful functions can be found in the later 
    // part but commented out. All of these functions are virtual and defined
    // in the base class.

    /**
       Core function: given a set of CPANs, return a float which indicates 
       the compatibility the cluster combination.
    */
    virtual float Float(const std::vector<const cluster::cluster_params*> &clusters);

    /**
       Optional function: called after each iterative approach if a manager class is
       run with verbosity level <= kPerIteration. Maybe useful for debugging.
    */
    virtual void Report();
    
    /// Function to reset the algorithm instance, called together with manager's Reset()
    virtual void Reset();

  private:

    float TProfConvol(std::vector<larutil::PxHit> hita ,std::vector<larutil::PxHit> hitb);	
    
    void SetVerbose(bool on) { _verbose = on; }

    void SetDebug(bool on) { _debug = on; }

    bool _verbose;
    bool _debug;


  };
}
#endif
/** @} */ // end of doxygen group 

