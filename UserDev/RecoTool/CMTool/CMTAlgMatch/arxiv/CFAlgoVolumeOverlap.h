/**
 * \file CFAlgoVolumeOverlap.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CFAlgoVolumeOverlap
 *
 * @author david
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CFALGOVOLUMEOVERLAP_H
#define RECOTOOL_CFALGOVOLUMEOVERLAP_H

#include "CMTool/CMToolBase/CFloatAlgoBase.h"
#include <math.h>
#include "ClusterRecoUtil/Base/Polygon2D.h"

namespace cmtool {
  /**
     \class CFAlgoVolumeOverlap
     User implementation for CFloatAlgoBase class
     doxygen documentation!
  */
  class CFAlgoVolumeOverlap : public CFloatAlgoBase {
    
  public:
    
    /// Default constructor
    CFAlgoVolumeOverlap();
    
    /// Default destructor
    virtual ~CFAlgoVolumeOverlap(){};

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

    /// Function to set debug output
    void SetDebug( bool on ) { _debug = on; }

    /// Function to set if _UseAllPlanes is on/off
    void SetUseAllPlanes( bool on ) { _UseAllPlanes = on; }

  private:

    double _w2cm, _t2cm;
    bool   _verbose;
    bool   _debug;
    bool   _UseAllPlanes; //If True matching only if clusters.size() == 3)
  };


}
#endif
/** @} */ // end of doxygen group 

