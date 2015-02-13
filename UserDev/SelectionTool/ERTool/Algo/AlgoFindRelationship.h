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

#include "ERTool/Base/AlgoBase.h"
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


    double FindClosestApproach(const geoalgo::HalfLine_t& shr1,
			       const geoalgo::HalfLine_t& shr2,
			       geoalgo::Point_t& vtx) const;

    double FindClosestApproach(const geoalgo::HalfLine_t& shr,
			       const geoalgo::Trajectory_t& trk,
			       geoalgo::Point_t& vtx) const;
      
    double FindClosestApproach(const geoalgo::Trajectory_t& trk2,
			       const geoalgo::Trajectory_t& trk1,
			       geoalgo::Point_t& vtx) const;
      
    void setDebug(bool on) { _debug = on; }

    void setMinLength(double l) { _minLength = l; }


  private:

    bool _debug;

    double _minLength; // track length after which to stop and calculate direction

    geoalgo::GeoAlgo _geoAlgo;
    
  };
}

#endif
/** @} */ // end of doxygen group 

