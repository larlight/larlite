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

#include "GeoAlgo/GeoAlgo.h"
#include "ERTool/Base/Shower.h"
#include "ERTool/Base/Track.h"
#include "ERTool/Base/ParticleGraph.h"
#include "ERTool/Base/ERToolTypes.h"
#include "ERTool/Base/UtilFunc.h"
#include "ERTool/Base/MessageUtil.h"
namespace ertool {
  /**
    \class AlgoFindRelationship
    User defined class ertool::AlgoFindRelationship ... these comments are used to generate
    doxygen documentation!
  */
  class AlgoFindRelationship : public MessageUtil {

  public:

    /// Default constructor
    AlgoFindRelationship();

    /// Default destructor
    virtual ~AlgoFindRelationship() { _debug = false; };

    void Reset();

    /// Versions that fill a vtx point
    double FindClosestApproach(const geoalgo::HalfLine_t& shr1,
                               const geoalgo::HalfLine_t& shr2,
                               geoalgo::Point_t& vtx) const;

    double FindClosestApproach(const geoalgo::HalfLine_t& shr,
                               const geoalgo::Trajectory_t& trk,
                               geoalgo::Point_t& vtx) const;

    double FindClosestApproach(const geoalgo::Trajectory_t& trk,
                               const geoalgo::HalfLine_t& shr,
                               geoalgo::Point_t& vtx) const;

    double FindClosestApproach(const geoalgo::Trajectory_t& trk2,
                               const geoalgo::Trajectory_t& trk1,
                               geoalgo::Point_t& vtx) const;

    /// Versions that don't fill a vtx point (useful when used inside of "const" functions)
    double FindClosestApproach(const geoalgo::HalfLine_t& shr1,
                               const geoalgo::HalfLine_t& shr2) const;

    double FindClosestApproach(const geoalgo::HalfLine_t& shr,
                               const geoalgo::Trajectory_t& trk) const;

    //Just Feed it a shower and track point, i.e. Track.front()
    double FindClosestApproach(const ::ertool::Shower& shr,
                               const geoalgo::Vector& trk_point) const;

    double FindClosestApproach(const geoalgo::Trajectory_t& trk,
                               const geoalgo::HalfLine_t& shr) const;

    double FindClosestApproach(const geoalgo::Trajectory_t& trk2,
                               const geoalgo::Trajectory_t& trk1) const;

    RelationType_t FindRelation(const geoalgo::HalfLine_t& s1,
                                const geoalgo::HalfLine_t& s2,
                                geoalgo::Point_t& vtx,
                                double& score) const;

    RelationType_t FindRelation(const geoalgo::HalfLine_t& s,
                                const geoalgo::Trajectory_t& t,
                                geoalgo::Point_t& vtx,
                                double& score) const;

    RelationType_t FindRelation(const geoalgo::Trajectory_t& t,
                                const geoalgo::HalfLine_t& s,
                                geoalgo::Point_t& vtx,
                                double& score) const;

    RelationType_t FindRelation(const geoalgo::Trajectory_t& t1,
                                const geoalgo::Trajectory_t& t2,
                                geoalgo::Point_t& vtx,
                                double& score) const;

    void setDebug(bool on) { _debug = on; }

    void setMinLength(double l) { _minLength = l; }

    void setMaxIP(double ip) { _maxIP = ip; }

    // Get PDG Code given track type
    double GetPDG(const Track &trk);

    // Get Mass given PDG
    double GetMass(const Track &trk);

    /// Function that based on primaries in event
    /// returns potential vertices.
    /// Takes start point of each object
    /// and groups together other start points
    /// within _IP of the start point
    /// Start points are this way grouped
    /// into vertices
    std::vector<::geoalgo::Point_t> GetVertices(const ParticleGraph& graph,
        const int minObjectsAtVertex) const;

  private:

    bool _debug;

    double _minLength; // track length after which to stop and calculate direction

    double _maxIP; ///< Maximum Impact Parameter allowed for success

    ::geoalgo::GeoAlgo _geoAlgo;

  };
}

#endif
/** @} */ // end of doxygen group

