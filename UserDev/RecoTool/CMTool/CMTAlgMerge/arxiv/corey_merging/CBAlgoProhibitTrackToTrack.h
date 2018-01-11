/**
 * \file CBAlgoProhibitTrackToTrack.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CBAlgoProhibitTrackToTrack
 *
 * @author Corey Adams
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CBALGOPROHIBITTRACKTOTRACK_H
#define RECOTOOL_CBALGOPROHIBITTRACKTOTRACK_H

#include <iostream>
#include "CMTool/CMToolBase/CBoolAlgoBase.h"
#include "LArUtil/GeometryUtilities.h"


namespace cmtool {
  /**
     \class CMalgoPolyContain
     Merge Polygons if the two overlap even partially
  */
  class CBAlgoProhibitTrackToTrack : public CBoolAlgoBase{
    
  public:
    
    enum mode
    {
      kBOTH,
      kEITHER      
    };

    /// Default constructor
    CBAlgoProhibitTrackToTrack();
    
    /// Default destructor
    virtual ~CBAlgoProhibitTrackToTrack(){};
 
    /**
       Core function: given the ClusterParamsAlg input, return whether a cluster should be
       merged or not.
    */
    virtual bool Bool(const ::cluster::ClusterParamsAlg &cluster1,
                      const ::cluster::ClusterParamsAlg &cluster2);

    bool isTrack(const ::cluster::ClusterParamsAlg &cluster1);

    /// Setter for track-like parameters
    void SetMinHits(size_t mh)          { _min_hits           = mh; }
    void SetMinModHitDens(Double_t mhd) { _min_mod_hit_dens   = mhd; }
    void SetMinMHitWiresFraction(Double_t mmhw) 
       { _min_multihit_wires_fraction = mmhw; }
    void SetMinPrincipal(Double_t mp)   { _min_principal      = mp; }
    void SetMinCharge(Double_t charge)  { _min_charge         = charge;}
    void SetMinLengthWidthRatio(Double_t length)  
        { _min_length_width_ratio = length;}
    void SetDebug(bool b){debug = b;}

    void SetMode(mode m);

  private:
    
    // Criteria for a cluster to be a "track"
    unsigned int   _min_hits;
    float _min_mod_hit_dens;
    float _min_multihit_wires_fraction;
    float _min_principal;
    float _min_charge;
    float _min_length_width_ratio;
    bool debug;

    mode _mode;
  };
}

#endif
/** @} */ // end of doxygen group 

