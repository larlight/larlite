/**
 * \file larflow3dhit.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for larflow3dhit data container
 *
 * @author R. Sharankova - Tufts - 08/2018
 */

/** \addtogroup DataFormat

    @{*/

#ifndef LARLITE_LARFLOW3DHIT_H
#define LARLITE_LARFLOW3DHIT_H

#include "data_base.h"
#include "Base/GeoConstants.h"
#include "Base/GeoTypes.h"
#include "Base/RawConstants.h"
#include <vector>
namespace larlite{
  /**
     \class larflow3dhit
     LArSoft Larflow3dhit class equivalent data container
  */
  class larflow3dhit : public std::vector<float>, public data_base {
    
  public:
    
    /// Default constructor
    larflow3dhit();
    
    /// Default destructor
    virtual ~larflow3dhit(){}

    void clear_data();


    typedef enum { kQandCmatch=0, kCmatch, kClosestC, kNoMatch } MatchQuality_t; // quality of match
    typedef enum { kIn5mm=0, kIn10mm, kIn50mm, kOut50mm, kNoValue } Consistency_t; // distance b/n y2u and y2v
    typedef enum { kY2U=0, kY2V, kNumFlowDir } FlowDirection_t; 
    typedef enum { kNoTruthMatch=0, kOnTrack, kOnSmear, kUnknown } TruthFlag_t; // should add kOnShower in the future?
    
    int tick;        // row
    int srcwire;     // column in source image
    std::vector<int> targetwire;  // column in target image
    //int src_ctrid;   // contour index in source image
    //int tar_ctrid;   // contour index in target image
    int idxhit;      // index in eventhit vector
    MatchQuality_t matchquality; // quality of plane-correspondence match
    Consistency_t consistency3d; // flag for distance b/n y2u and y2v predicted spacepoints
    FlowDirection_t flowdir;     // flow direction used
    TruthFlag_t truthflag; // do we have a matching mctruth?
    float center_y_dist;  // distance to center of y-image used for flow prediction
    float dy; //distance in y coord. between y2u and y2v predicted spacepoints
    float dz; //distance in z coord. between y2u and y2v predicted spacepoints
    float endpt_score;
    float track_score;
    float shower_score;
    float renormed_track_score;
    float renormed_shower_score;
    unsigned short src_infill; // source is on infill prediction. THIS SHOULD BE CONSIDERED A BOOL
    std::vector<unsigned short> tar_infill; // target is on infill prediction. THIS SHOULD BE CONSIDERED A BOOL
    std::vector<float> X_truth; // true position from mctrack
    int trackid; //mctrtack id
    float dWall; // mctruth min dist to wall
    
  private:
    
  };
  
  /**
     \class event_hit
     A collection storage class of multiple hits.
  */
  class event_larflow3dhit : public std::vector<larlite::larflow3dhit>, public event_base {
  public:
      
    /// Default constructor
    event_larflow3dhit(std::string name="noname") : event_base(data::kLArFlow3DHit,name) { clear_data(); }
    
    
    /// Default copy constructor
    event_larflow3dhit(const event_larflow3dhit& original) : std::vector<larlite::larflow3dhit>(original), event_base(original)
    {}
    
    /// Default destructor
    ~event_larflow3dhit(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}
    
  private:
    
  };
}
#endif

/** @} */ // end of doxygen group 
