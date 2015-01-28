/**
 * \file AlgoAlgoFindRelationship.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class AlgoFindRelationship
 *
 * @author David C.
 */

/** \addtogroup ERTool
    
    @{*/
#ifndef ERTOOL_ALGOFINDRELATIONSHIP_H
#define ERTOOL_ALGOFINDRELATIONSHIP_H

#include "Base/AlgoBase.h"
#include "GeoAlgo/GeoAlgo.h"

namespace ertool {
  /**
     \class AlgoFindRelationship
     User defined class ertool::AlgoFindRelationship ... these comments are used to generate
     doxygen documentation!
  */
  class AlgoFindRelationship : public AlgoBase {
    
  public:
    
    /// Default constructor
    AlgoFindRelationship();
    
    /// Default destructor
    virtual ~AlgoFindRelationship(){ _debug=false; };
    
    virtual void Reset();
    
    bool isShowerFromTrack(const geoalgo::HalfLine_t& shr, 
			   const geoalgo::Trajectory_t& trk,
			   geoalgo::Point_t& vtx) const;

    bool isShowerFromShower(const geoalgo::HalfLine_t& sh1, 
			    const geoalgo::HalfLine_t& sh2,
			    geoalgo::Point_t& vtx) const;

    bool isTrackFromTrack(const geoalgo::Trajectory_t& trk1, 
			  const geoalgo::Trajectory_t& trk2,
			  geoalgo::Point_t& vtx) const;

    bool haveCommonOrigin(const geoalgo::HalfLine_t& shr1, 
			  const geoalgo::HalfLine_t& shr2,
			  geoalgo::Point_t& vtx) const;

    bool haveCommonOrigin(const geoalgo::Trajectory_t& trk1, 
			  const geoalgo::Trajectory_t& trk2,
			  geoalgo::Point_t& vtx) const;

    bool haveCommonOrigin(const geoalgo::Trajectory_t& trk, 
			  const geoalgo::HalfLine_t&   shr,
			  geoalgo::Point_t& vtx) const;

    void setDebug(bool on) { _debug = on; }


  private:

    bool _debug;

    geoalgo::GeoAlgo _geoAlgo;
    
  };
}

#endif
/** @} */ // end of doxygen group 

