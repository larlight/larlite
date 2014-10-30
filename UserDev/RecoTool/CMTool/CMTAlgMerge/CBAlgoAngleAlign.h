/**
 * \file CBAlgoAngleAlign.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CBAlgoAngleAlign
 *
 * @author davidkaleko
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef RECOTOOL_CBALGOANGLEALIGN_H
#define RECOTOOL_CBALGOANGLEALIGN_H

#include <iostream>
#include "CMTool/CMToolBase/CBoolAlgoBase.h"

namespace cmtool {
  /**
     \class CBAlgoAngleCompat
     User defined class CBAlgoAngleCompat ... these comments are used to generate
     doxygen documentation!
  */
  class CBAlgoAngleAlign : public CBoolAlgoBase{
    
  public:
    
    /// Default constructor
    CBAlgoAngleAlign();
    
    /// Default destructor
    virtual ~CBAlgoAngleAlign(){};
        
    /// Overloaded (from CBoolAlgoBase) Bool function
    virtual bool Bool(const ::cluster::ClusterParamsAlg &cluster1,
		      const ::cluster::ClusterParamsAlg &cluster2);

    /// Method to set debug mode
    void SetDebug(bool on) { _debug = on; }

    /// Method to set whether you allow angles to match with +/- 180 deg difference
    void SetAllow180Ambig(bool on) { _allow_180_ambig = on; }

    /// Method to set cut value in degrees for angle compatibility test
    void SetAngleCut(double angle) { _MaxAngleSep = angle; }

    void SetMinNHits(size_t n) { _MinNHits = n; }

  protected:

    bool _debug;
    size_t _MinNHits;    /// minimum number of hits for cluster to be considered
    
    ///bool to allow "backwards" clusters (swapped start/end points)
    ///to still match in angle, even though they are 180 degrees apart
    ///only valid for _use_opening_angle==false
    bool _allow_180_ambig; 

    /// hard shower-axis angle cutoff (only valid for _use_opening_angle==false)
    double _MaxAngleSep;


  };
  
} // end namespace cmtool

#endif
  /** @} */ // end of doxygen group 
  
