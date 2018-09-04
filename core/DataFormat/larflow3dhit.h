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
    
    int tick;        // row
    int srcwire;     // column in source image
    std::vector<int> targetwire;  // column in target image
    //int src_ctrid;   // contour index in source image
    //int tar_ctrid;   // contour index in target image
    int idxhit;      // index in eventhit vector
    MatchQuality_t matchquality; // quality of plane-correspondence match
    Consistency_t consistency3d; //flag for distance b/n y2u and y2v predicted spacepoints
    float center_y_dist;  // distance to center of y-image used for flow prediction
    float dy; //distance in y coord. between y2u and y2v predicted spacepoints
    float dz; //distance in z coord. between y2u and y2v predicted spacepoints
    
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
