/**
 * \file CFAlgoIoU.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CFAlgoIoU
 *
 * @author david caratelli
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CFALGOIOU_H
#define RECOTOOL_CFALGOIOU_H

#include "CMTool/CMToolBase/CFloatAlgoBase.h"

namespace cmtool {
  /**
     \class CFAlgoIoU
     User implementation for CFloatAlgoBase class
     doxygen documentation!
  */
  class CFAlgoIoU : public CFloatAlgoBase {
    
  public:
    
    /// Default constructor
    CFAlgoIoU();
    
    /// Default destructor
    virtual ~CFAlgoIoU(){};

    /**This algorithm calculates the difference between start and end times for merged clusters,
		and compares across planes to form matches. 
    */
    virtual float Float(const std::vector<const cluster::cluster_params*> &clusters);

    void SetDebug(bool debug) { _debug = debug ; }
    
    void SetVerbose(bool verbose) { _verbose = verbose ; }

    void RequireThreePlanes(bool doit) { _require_3planes = doit; }

    virtual void Report();
    
    virtual void Reset();

  protected:

    void getMinMaxTime(const cluster::cluster_params* cluster, double& min, double& max);
    
    float _time_ratio_cut ;
    float _start_time_cut ;
    bool _debug ;
    bool _verbose ;
    bool _require_3planes;
  };
}
#endif
/** @} */ // end of doxygen group 

