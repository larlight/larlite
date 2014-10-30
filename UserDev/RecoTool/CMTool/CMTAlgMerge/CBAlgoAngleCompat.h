/**
 * \file CBAlgoAngleCompat.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CBAlgoAngleCompat
 *
 * @author davidkaleko
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CBALGOANGLECOMPAT_H
#define RECOTOOL_CBALGOANGLECOMPAT_H

#include <iostream>
#include "CMTool/CMToolBase/CBoolAlgoBase.h"

namespace cmtool {
  /**
     \class CBAlgoAngleCompat
     User defined class CBAlgoAngleCompat ... these comments are used to generate
     doxygen documentation!
  */
  class CBAlgoAngleCompat : public CBoolAlgoBase{
    
  public:
    
    /// Default constructor
    CBAlgoAngleCompat();
    
    /// Default destructor
    virtual ~CBAlgoAngleCompat(){};
        
    /// Overloaded (from CBoolAlgoBase) Bool function
    virtual bool Bool(const ::cluster::ClusterParamsAlg &cluster1,
		      const ::cluster::ClusterParamsAlg &cluster2);

    /// Method to set debug mode
    void SetDebug(bool on) { _debug = on; }

    /// Method to set whether you allow angles to match with +/- 180 deg difference
    void SetAllow180Ambig(bool on) { _allow_180_ambig = on; }

    /// Method to set cut value in degrees for angle compatibility test
    void SetAngleCut(double angle) { _max_allowed_2D_angle_diff = angle; }

    /// Method to set angle cut value to be based on opening angle
    void SetUseOpeningAngle(bool on) { _use_opening_angle = on; }

    /// Set Minimum Number of Hits to consider Cluster
    void SetMinHits(size_t n) { _minHits = n; }

    //    TH1F* GetAngleDistHisto() const{ return angle_dist_histo; };

  protected:

    bool _debug;    /// bool to suppress lots of output if you want

    ///bool to allow "backwards" clusters (swapped start/end points)
    ///to still match in angle, even though they are 180 degrees apart
    ///only valid for _use_opening_angle==false
    bool _allow_180_ambig; 

    /// hard shower-axis angle cutoff (only valid for _use_opening_angle==false)
    double _max_allowed_2D_angle_diff; //in degrees

    /// bool set to true if you want to use opening angle as the cutoff
    /// angle instead of whatever you set with SetAngleCut
    bool _use_opening_angle;

    size_t _minHits;        /// Min Number of hits for cluster to be considered

    /// Histogram used for debugging/cut value settings
    TH1F *angle_dist_histo;


  };
  
} // end namespace cmtool

#endif
  /** @} */ // end of doxygen group 
  
