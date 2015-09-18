/**
 * \file CFAlgoStartTimeCompat.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CFAlgoStartTimeCompat
 *
 * @author david
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CFALGOSTARTTIMECOMPAT_H
#define RECOTOOL_CFALGOSTARTTIMECOMPAT_H

#include "CMTool/CMToolBase/CFloatAlgoBase.h"
#include <math.h>

namespace cmtool {
  /**
     \class CFAlgoStartTimeCompat
     User implementation for CFloatAlgoBase class
     doxygen documentation!
  */
  class CFAlgoStartTimeCompat : public CFloatAlgoBase {
    
  public:
    
    /// Default constructor
    CFAlgoStartTimeCompat();
    
    /// Default destructor
    virtual ~CFAlgoStartTimeCompat(){};

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

    /// Function to set verbose output
    void SetVerbose( bool on ) { _verbose = on; }

    /// Function to set verbose output
    void SetTimeDist( double t ) { _timeDist = t; }

  private:

    double _w2cm, _t2cm;
    bool   _verbose;
    double _timeDist; // Max separation in time between start points [cm]
  };


}
#endif
/** @} */ // end of doxygen group 

