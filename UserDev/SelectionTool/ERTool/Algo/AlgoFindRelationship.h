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

#include "TDatabasePDG.h"
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

    /*
    ParticleGraph FindTrackHierarchy(const std::vector<const ertool::Track> &tracks);

    ParticleGraph FindHierarchy(const std::vector<const ertool::Track> &tracks,
				const std::vector<const ertool::Shower> &showers);
    */

    // Get PDG Code given track type
    double GetPDG(const Track &trk);

  private:

    /// Compare shower with particles (and daughters of such particles)
    //bool CompareShrWithPartBranch(const Shower& shr,
    //				  const std::vector<const ertool::Track*>  &tracks,
    //				  Particle& part);

    bool _debug;

    double _minLength; // track length after which to stop and calculate direction

    geoalgo::GeoAlgo _geoAlgo;

    // Partile masses
    double _pi_mass, _pr_mass, _mu_mass, _ka_mass, _uk_mass;
    
  };
}

#endif
/** @} */ // end of doxygen group 

