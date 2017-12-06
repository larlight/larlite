/**
 * \file GeometryHelper.h
 *
 * \ingroup LArUtil
 *
 * \brief Class def header for a class GeometryHelper
 *
 * @author cadams + david caratelli
 */

/** \addtogroup LArUtil

    @{*/
#ifndef GEOMETRYHELPER_H
#define GEOMETRYHELPER_H

#include <iostream>
#include "PxUtils.h"
#include "Geometry.h"
#include <climits>
#include <limits>

#include "LArUtil/PxUtils.h"

#include "DetectorProperties.h"
#include "LArProperties.h"

namespace larutil {


/// Utility: maximum value for double
const double kDOUBLE_MAX = std::numeric_limits<double>::max();

/// Utility: minimum value for double
const double kDOUBLE_MIN = std::numeric_limits<double>::min();

/// Utility: maximum value for int
const int    kINT_MAX    = std::numeric_limits<int>::max();

/// Utility: maximum value for unsigned int
const unsigned int kUINT_MAX    = std::numeric_limits<unsigned int>::max();

/// Utility: maximum value for size_t
const size_t kSIZE_MAX   = std::numeric_limits<size_t>::max();

typedef PxHit Hit2D;
typedef PxPoint Point2D;

/**
   \class GeometryHelper
   This class is meant as an aide to reconstruction by handling geometrical operations.
   It includes functionality to go between 3D and 2D for points, lines, cones.

   In this class, all units are in centimeters and radians unless explicitly declared to be otherwise.
   To aide in doing the conversions, you may use the functions TimeToCm() and WireToCm() to convert wire/time
   to cm coordinates as such:
     wire [cm] = wire [Number] * WireToCmConversion (GeometryHelper::WireToCm() )

 */


class GeometryHelper : public larlite::larlite_base {

private:

    /**
     * @brief Constructor
     * @details Constructor is private.  To access this class, get a pointer using the GeometryHelper::GetME() function
     */
    GeometryHelper() {}

    /// Default destructor
    ~GeometryHelper() {}

    // Pointer to sole instance of this object
    static GeometryHelper* _me;

public:

    /**
     * @brief Singleton Getter
     * @details Returns a pointer to the sole instance of this class
     * @return Pointer to GeometryHelper
     */
    static const GeometryHelper* GetME() {
        if (!_me) {
            _me = new GeometryHelper;
            _me -> Reconfigure();
        }
        return _me;
    }

    /**
     * @brief Reconfigures private parameters
     * @details Function to load private parameters necessary for conversions
     */
    void Reconfigure();

    /**
     * @brief Convert a 3D point in the detector to 2D point on a plane
     * @details Conversion is done by setting the time to the X coordinated, minus any corrections from trigger offset and plane offset.
     * The wire coordinate is a combination of Y, Z coordinates that depends on the angle of the wires WRT vertical
     *
     * @param xyz TVector3 of the point in 3D to project into 2D
     * @param plane The index of the plane to project into.  Ranges from 0 to Nplanes -1 (Nplanes available in Geometry::Nplanes() )
     *
     * @return Returns a Point2D of the 2D projection into the plane
     */
    Point2D Point_3Dto2D(const TVector3 & xyz, unsigned int plane) const;
    /**
     * @brief Convert a 3D point in the detector to 2D point on a plane
     * @details Conversion is done by setting the time to the X coordinated, minus any corrections from trigger offset and plane offset.
     * The wire coordinate is a combination of Y, Z coordinates that depends on the angle of the wires WRT vertical
     *
     * @param xyz std::vector<float> of the point in 3D to project into 2D
     * @param plane The index of the plane to project into.  Ranges from 0 to Nplanes -1 (Nplanes available in Geometry::Nplanes() )
     *
     * @return Returns a Point2D of the 2D projection into the plane
     */
    Point2D Point_3Dto2D(const std::vector<float> & xyz, unsigned int plane) const;

    /**
     * @brief Convert a 3D point in the detector to 2D point on a plane
     * @details Conversion is done by setting the time to the X coordinated, minus any corrections from trigger offset and plane offset.
     * The wire coordinate is a combination of Y, Z coordinates that depends on the angle of the wires WRT vertical
     *
     * @param xyz std::vector<float> of the point in 3D to project into 2D
     * @param plane The index of the plane to project into.  Ranges from 0 to Nplanes -1 (Nplanes available in Geometry::Nplanes() )
     *
     * @return Returns a Point2D of the 2D projection into the plane
     */
    Point2D Point_3Dto2D(const std::vector<double> & xyz, unsigned int plane) const;


    /**
     * @brief Convert a 3D point in the detector to 2D point on a plane
     * @details Conversion is done by setting the time to the X coordinated, minus any corrections from trigger offset and plane offset.
     * The wire coordinate is a combination of Y, Z coordinates that depends on the angle of the wires WRT vertical
     *
     * @param xyz Pointer to float of the point in 3D to project into 2D.  Caller is responsible for creating and deleting this array
     * @param plane The index of the plane to project into.  Ranges from 0 to Nplanes -1 (Nplanes available in Geometry::Nplanes() )
     *
     * @return Returns a Point2D of the 2D projection into the plane
     */
    Point2D Point_3Dto2D(float * xyz, unsigned int plane) const;

    /**
     * @brief Convert a 3D point in the detector to 2D point on a plane
     * @details Conversion is done by setting the time to the X coordinated, minus any corrections from trigger offset and plane offset.
     * The wire coordinate is a combination of Y, Z coordinates that depends on the angle of the wires WRT vertical
     *
     * @param xyz Pointer to double of the point in 3D to project into 2D.  Caller is responsible for creating and deleting this array
     * @param plane The index of the plane to project into.  Ranges from 0 to Nplanes -1 (Nplanes available in Geometry::Nplanes() )
     *
     * @return Returns a Point2D of the 2D projection into the plane
     */
    Point2D Point_3Dto2D(double * xyz, unsigned int plane) const;


    /**
     * @brief Convert a 3D point in the detector to 2D point on a plane
     * @details Conversion is done by setting the time to the X coordinated, minus any corrections from trigger offset and plane offset.
     * The wire coordinate is a combination of Y, Z coordinates that depends on the angle of the wires WRT vertical
     *
     * @param xyz Pointer to float of the point in 3D to project into 2D.  Caller is responsible for creating and deleting this array
     * @param plane The index of the plane to project into.  Ranges from 0 to Nplanes -1 (Nplanes available in Geometry::Nplanes() )
     *
     * @return Returns a Point2D of the 2D projection into the plane
     */
    Point2D Point_3Dto2D(float x, float y, float z, unsigned int plane) const;


    /**
     * @brief Take a line (start point and direction) and project that into 2D
     * @details This function will return the start point and direction of a line segment projected into 2D
     *  It works by using the Point_3Dto2D function to project the start point into 2D.  Then, it finds a second
     *  point along the line by using startPoint + direction3D.  If that point is not in the TPC, it halves the distance and
     *  tries that point.  Then, once it has two points in the TPC, it projects both into the plane, normalizes the direction,
     *  and returns them as PxPoints by reference
     *
     * @param startPoint3D TVector3 describing the 3D start point (in detector coordinates)
     * @param direction3D TVector3 describing the direction that the line emanates from the start point.  Need not be normalized
     * @param plane The plane in which the projection needs to be done.
     * @param startPoint2D Returned by reference: A Point2D representing the projection of the start point into the plane.
     * @param direction2D Returned by reference: A Point2D representing the projection of the direction into the plane, normalized.
     */
    void Line_3Dto2D( const TVector3 & startPoint3D, const TVector3 & direction3D, unsigned int plane,
                      Point2D & startPoint2D, Point2D & direction2D) const;

    float Slope_3Dto2D(const TVector3 & inputVector, unsigned int plane) const ;

    /**
     * @brief Given a 3D line get the 3D vector that is the projection of that line on a plane
     * @param inputVector -> input vector oriented in 3D space
     * @param planeNormal -> normal to the plane on which we want to project the line
     * @return TVector3   -> the projected line on the plane (not normalized to unit-length)
     */
    TVector3 Project_3DLine_OnPlane(const TVector3& inputVector, const TVector3& planeNormal) const ;

    /**
     * @brief Given a 3D line get the 3D vector that is the projection of that line on a plane
     * @param inputVector -> input vector oriented in 3D space
     * @param pl          -> uboone plane number (0,1,2) in (w,t)
     * @return TVector3   -> the projected line on the plane (not normalized to unit-length)
     */
    TVector3 Project_3DLine_OnPlane(const TVector3& inputVector, const int& pl) const ;

    /**
     * @brief Given a 3D line get the 3D vector that is the projection of that line on a plane
     * @param V -> input vector oriented in 3D space
     * @param N -> normal to the plane on which we want to project the line
     * @return std::vector<double>   -> the projected line on the plane (not normalized to unit-length)
     */
    std::vector<double> Project_3DLine_OnPlane(const std::vector<double>& V, const std::vector<double>& N) const ;

    /**
     * @brief Given a 3D line get the 3D vector that is the projection of that line on a plane
     * @param V -> input vector oriented in 3D space
     * @param pl          -> uboone plane number (0,1,2) in (w,t)
     * @return std::vector<double> -> the projected line on the plane (not normalized to unit-length)
     */
    std::vector<double> Project_3DLine_OnPlane(const std::vector<double>& V, const int& pl) const ;

    /**
     * @brief Calculate the shortest distance between a line and a point in 2D
     *
     * @param pointOnLine Point2D - Any point on the line of interest
     * @param directionOfLine Point2D - The direction of the line of interest
     * @param targetPoint Point2D - The point from which the closest distance is calculated
     * @return the distance between the point and the line in centimeters
     */
    float DistanceToLine2D( const Point2D & pointOnLine, const Point2D & directionOfLine,
                            const Point2D & targetPoint) const;

    /**
     * @brief Calculate the shortest distance between a line and a point in 2D
     *
     * @param pointOnLine Point2D - Any point on the line of interest
     * @param directionOfLine Point2D - The direction of the line of interest
     * @param targetPoint Point2D - The point from which the closest distance is calculated
     * @return the distance between the point and the line in centimeters
     */
    float DistanceToLine2D( const Point2D & pointOnLine, const Point2D & directionOfLine,
                            const Hit2D & targetPoint) const;

    /**
     * @brief Calculate the shortest distance between a line and a point in 2D
     *
     * @param pointOnLine  TVector2 - Any point on the line of interest
     * @param directionOfLine  TVector2 - The direction of the line of interest
     * @param targetPoint  TVector2 - The point from which the closest distance is calculated
     * @return the distance between the point and the line in centimeters
     */
    float DistanceToLine2D( const TVector2 & pointOnLine, const TVector2 & directionOfLine,
                            const TVector2 & targetPoint) const;

    /**
     * @brief Calculate the shortest distance between a line and a point in 3D
     *
     * @param pointOnLine TVector3 - Any point on the line of interest
     * @param directionOfLine TVector3 - The direction of the line of interest
     * @param targetPoint TVector3 - The point from which the closest distance is calculated
     * @return the distance between the point and the line in centimeters
     */
    float DistanceToLine3D( const TVector3 & pointOnLine, const TVector3 & directionOfLine,
                            const TVector3 & targetPoint) const;

    /**
     * @brief Calculate the distance along a line to the closest approach to a point of that line
     *
     * @param pointOnLine TVector3 - Starting point on the line of interest
     * @param directionOfLine TVector3 - The direction of the line of interest
     * @param targetPoint TVector3 - The point from which the closest distance is calculated
     * @return the distance between the starting point and the closest approach in centimeters
     */
    float DistanceAlongLine3D( const TVector3 & pointOnLine, const TVector3 & directionOfLine,
                               const TVector3 & targetPoint) const;



    /**
     * @brief Get 2D distance squared between two points
     * @param point1 : Point2D (w,t) in cm space
     * @param point2 : Point2D (w,t) in cm space
     * @return double -> distance between the two
     */
    float Get2DDistanceSqrd( const Point2D & point1, const Point2D & point2) const;

    /**
     * @brief Get 2D distance between two points
     * @param point1 : Point2D (w,t) in cm space
     * @param point2 : Point2D (w,t) in cm space
     * @return double -> distance between the two
     */
    float Get2DDistance( const Point2D & point1, const Point2D & point2) const;
    
    /**
     * @brief return the projected point on a line
     * @details given a slope and a point that define a line
     * this function returns the projected point on that
     * line of an input 2D point anywhere on the plane
     * @param slope -> slope in (cm,cm) space of the line
     * @param startpoint -> point that lies on the line
     * @param point1 -> point that we wish to find the projection for
     * @return pointout -> the point projected on the line
     */
    int GetPointOnLine(const double& slope,
                       const Point2D& startpoint,
                       const Point2D& point1,
                       Point2D& pointout) const;

    /**
     * @brief Get pitch given a certain 3D direction and plane
     * @details Provided a 3D direction (the shower dir) and a
     * plane, this algorithm returns the pitch for this direction
     * in that plane. The pitch is the effective distance between
     * seen between wires on this plane from the point of view of
     * the track. This is a quantity in cm and can only be >= 3 cm
     * @param direction -> TVector3 of the shower's 3D direction
     * @param plane     -> 0,1,2
     * @return the effective pitch in cm
     */
    double GetPitch(const TVector3& direction, const int& pl) const;

    /**
     * @brief get pitch on specific plane given phi and theta directions
     * This is the preferred pitch function to be called. The others are carried over.
     * @param pl -> unsigned integer : plane for which to calculate pitch
     * @param theta -> double : angle with beam direction (this is not unambiguous definition)
     * @param phi   -> double : angle with XY plane (this is not unambigous definition)
     * @return pitch in cm
     */
    double CalculatePitch(UInt_t pl, double phi, double theta) const;

    /**
     * @brief get pitch on specific plane given phi and theta directions
     * This is the preferred pitch function to be called. The others are carried over.
     * @param pl -> unsigned integer : plane for which to calculate pitch
     * @param theta -> double : angle with beam direction (this is not unambiguous definition)
     * @param phi   -> double : angle with XY plane (this is not unambigous definition)
     * @return pitch in cm
     */
    double PitchInView(UInt_t pl, double phi, double theta) const;


    /**
     * @brief given phi and theta, get the projection of this 3D vector on the coordinate axes
     * @param theta -> double : angle with beam direction (this is not unambiguous definition)
     * @param phi   -> double : angle with XY plane (this is not unambigous definition)
     * @param dirst -> double array : value to be edited and filled
     */
    void GetDirectionCosines(double phi, double theta, Double_t *dirs) const;

    /**
     * @brief Get cosine of angle between two lines defined by their slope
     * @param slope1 : slope of first line
     * @param slope2 : slope of second line
     * @return cosine angle between the two lines
     */
    double GetCosAngleBetweenLines(const double& s1, const double& s2) const;

    /**
     * @brief Get cosine of angle given three points
     * @param p1 : the point common to both segments
     * @param p2 : the end of one vector
     * @param p3 : the end of the other vector
     * @return cosine between vectors (p1->p2) and (p1->p3)
     */
    double GetCosAngleBetweenLines(const Point2D& p1, const Point2D& p2, const Point2D& p3) const;

    /**
     * @brief Get cosine of angle given two segments (p1->p2) and (p3->p4)
     * @param p1 : start of 1st segment
     * @param p2 : end of first segment
     * @param p3 : start of 2nd segment
     * @param p4 : end of second segment
     * @return cosine between vectors (p1->p2) and (p3->p4)
     */
    double GetCosAngleBetweenLines(const Point2D& p1, const Point2D& p2,
                                   const Point2D& p3, const Point2D& p4) const;

    /**
     * @brief Get tangent of angle between two lines defined by their slope
     * @param slope1 : slope of first line
     * @param slope2 : slope of second line
     * @return tangent of angle between the two lines
     */
    double GetTanAngleBetweenLines(const double& s1, const double& s2) const;

    /**
     * @brief Find the Polygon boundary given a list of hits
     * @param inputHits : list of PxHits that make up the cluster
     * @param edgeHits  : list of PxHits that define the boundary of the polygon
     * @param frac      : fraction of charge in list of hits that needs to be contained by the polygon
     */
    void SelectPolygonHitList(const std::vector<larutil::PxHit> &inputHits,
                              std::vector <const larutil::PxHit*> &edgeHits,
                              double frac = 0.95) const;

    /**
     * @brief Given a vector of PXHits return a sorted list of indices that represents the correct order for the polygon edges
     * @details ...
     * @param ordered_hits : vector of PxHits that represent the list of edges for the polygon
     * @param candidate_polygon : list of indices of the vector ordered_hits in the order in which they should be
     * connected to form a nice, non-overlapping polygon
     */
    std::vector<size_t>  OrderPolygonEdges( std::vector<const larutil::PxHit*> ordered_hits ,
                                            std::vector<size_t> candidate_polygon) const;

    /**
     * @brief Return if a triangle of points A,B,C is clockwire or not
     * @details given an ordered list of points A, B, and C, imagine
     * drawing a line connecting the points in that order. If to do so
     * you need to make a clockwise loop, the algorithm returns True
     * if not, it returns false.
     * @return boolean telling wether the three points are ordered clockwire or counter-clockwise
     */
    bool Clockwise(const double& Ax, const double& Ay,
                   const double& Bx, const double& By,
                   const double& Cx, const double& Cy) const;



    /**
     * @brief This function takes in a hit list and some parameters describing a box
     *        around that hit list,
     *        returns the hits inside the box
     * @details The list of input hits is looped over to find hits inside of the box.
     *          The box is defined to be of the length "distanceAlongLine" and the
     *          width of "distancePerpToLine."  The axis of the box starts at startingHit
     *          and continues for the length "distanceAlongLine" in the direction specified
     *          by "lineSlope"
     *
     * @param inputHits The list of hits to loop over
     * @param startingHit The hit that defines the start of the box
     * @param distanceAlongLine The maximum distance along the line to look
     * @param distancePerpToLine The maximum distance perpendicular to the line to look
     * @param lineSlope The direction of the line emanating from the starting Hit
     * @return A vector of the indexes of the hits to return
     */
    std::vector<unsigned int> SelectLocalPointList( const std::vector<Hit2D> & inputHits,
            const Hit2D & startingHit,
            const float & distanceAlongLine,
            const float & distancePerpToLine,
            const float & lineSlope,
            Hit2D & averagePoint ) const;

    /**
     * @brief calculate the peprendicular distance from a point to aline
     * @param pt = point for which to calculate the perpendicular distance
     * @param slope = slope of line
     * @param intercept = intercept of line with y-axis
     * @return 2D perpendicular distance in cm
     */
    double PerpendicularDistance(const Point2D& pt,
                                 const double& slope,
                                 const double& intercept) const;

    /**
     * @brief calcualte the perpendicular distance from a point to a line
     * @param pt = point for which to calculate the perpendicular distance
     * @param slope = slope of line
     * @param anchor = point on line to anchor the line
     * @return 2D perpendicular distance in cm
     */
    double PerpendicularDistance(const Point2D& pt,
                                 const double& slope,
                                 const Point2D& anchor) const;

    /**
     * @brief deterimine if a point is in the TPC
     * @details Checks this point against the geometry parameters
     *
     * @param pointIn3D The point to be evaluated, units in cm.
     * @return true if the point is in the TPC, false if otherwise
     */
    bool Point_isInTPC(const TVector3 & pointIn3D) const;

    /**
       @brief given 2 2D angles on 2 planes, get the corresponding 3D
       angle in 3 dimensions.
     */
    int Get3DAxisN(const int& iplane0, const int& iplane1,
                   const double& omega0, const double& omega1,
                   double& phi, double& theta) const;

    /**
     * @brief given 2 2D points on two planes, find the XYZ coordinate they correspond to
     * @input Point2D p0 (const)
     * @input Point2D p1 (const)
     * @input reference to dzy double[] array (output)
     */
    int GetXYZ(const Point2D *p0, const Point2D *p1, Double_t* xyz) const;

    /**
     * @brief given 2 2D points on two planes, find the YZ coordinate they correspond to
     * @input Point2D p0 (const)
     * @input Point2D p1 (const)
     * @input reference to dzy double[] array (output)
     */
    int GetYZ(const Point2D *p0, const Point2D *p1, Double_t* xy) const;

    /**
     * @brief return the conversion from time tick to centimeters
     * @return Floating point conversion in the units [cm / time tick]
     */
    float TimeToCm() const { return fTimeToCm; }

    /**
     * @brief return conversion from wires to centimeters
     * @return Floating point conversion in the units [ wire / cm ]
     */
    float WireToCm() const { return fWireToCm; }

    /**
     * @brief get number of wire-planes in the geometry
     * @return unsigned integer for the number of planes
     */
    UInt_t Nplanes() const { return fNPlanes; }

    /**
     * @brief return whether a point is contained in the TPC boundaries as defined by Geometry.h
     * only check Y and Z coordinates with this function
     * @return boolean: true -> contained
     */
    bool ContainedYZ(const double& y, const double& z) const;
    
    /**
     * @brief return whether a point is contained in the TPC boundaries as defined by Geometry.h
     * @return boolean: true -> contained
     */
    bool Contained(const double& x, const double& y, const double& z) const;

private:

    // services to be used
    larutil::Geometry* geom;
    larutil::DetectorProperties* detp;
    larutil::LArProperties* larp;

    float fTimeToCm;
    float fWireToCm;

    // number of planes in the geometry
    UInt_t fNPlanes;
    // angle w.r.t. veritcal per plane
    std::vector<double> vertangle;

};

}

#endif
/** @} */ // end of doxygen group

