#ifndef GEOMETRYHELPER_CXX
#define GEOMETRYHELPER_CXX

#include "GeometryHelper.h"

namespace larutil {

GeometryHelper* GeometryHelper::_me = 0;


// Function to load any parameters for convenience :
void GeometryHelper::Reconfigure()
{

  geom = (larutil::Geometry*)(larutil::Geometry::GetME());
  detp = (larutil::DetectorProperties*)(larutil::DetectorProperties::GetME());
  larp = (larutil::LArProperties*)(larutil::LArProperties::GetME());

  fNPlanes = geom->Nplanes();
  vertangle.resize(fNPlanes);
  for (UInt_t ip = 0; ip < fNPlanes; ip++)
    vertangle[ip] = geom->WireAngleToVertical(geom->PlaneToView(ip)) - TMath::Pi() / 2; // wire angle


  fWireToCm = geom->WirePitch(0, 1, 0);
  fTimeToCm = detp->SamplingRate() / 1000.0 * larp->DriftVelocity(larp->Efield(), larp->Temperature());
}


// The next set of functions is the collection of functions to convert 3D Point to 2D point
// The first function is maintained, and the rest convert their arguments and call it
Point2D GeometryHelper::Point_3Dto2D(const TVector3 & _3D_position, unsigned int plane) const {

  // Make a check on the plane:
  if (plane > geom -> Nplanes()) {
    throw larutil::LArUtilException(Form("Can't project 3D point to unknown plane %u", plane));
  }

  // Verify that the point is in the TPC before trying to project:

  //initialize return value
  Point2D returnPoint;

  // The wire position can be gotten with Geometry::NearestWire()
  // Convert result to centimeters as part of the unit convention
  // Previously used nearest wire functions, but they are
  // slightly inaccurate
  // If you want the nearest wire, use the nearest wire function!
  returnPoint.w = geom->WireCoordinate(_3D_position, plane) * fWireToCm;
  // std::cout << "wire is " << returnPoint.w << " (cm)" << std::endl;

  // The time position is the X coordinate, corrected for
  // trigger offset and the offset of the plane
  // auto detp = DetectorProperties::GetME();
  returnPoint.t = _3D_position.X();
  // Add in the trigger offset:
  // (Trigger offset is time that the data is recorded
  // before the actual spill.
  // So, it moves the "0" farther away from the actual
  // time and is an addition)
  // returnPoint.t += detp -> TriggerOffset() * fTimeToCm;
  // std::cout << "trigger offset, plane " << plane
  //           << ": " << detp -> TriggerOffset() * fTimeToCm << std::endl;
  //
  //Get the origin point of this plane:
  // Double_t planeOrigin[3];
  // geom -> PlaneOriginVtx(plane, planeOrigin);
  // Correct for the origin of the planes
  // X = 0 is at the very first wire plane, so the values
  // of the coords of the planes are either 0 or negative
  // because the planes are negative, the extra distance
  // beyond 0 needs to make the time coordinate larger
  // Therefore, subtract the offest (which is already
  // in centimeters)
  // returnPoint.t -= planeOrigin[0];

// std::cout << "origin offset, plane " << plane
//             << ": " << planeOrigin[0] << std::endl;

  // Set the plane of the Point2D:
  returnPoint.plane = plane;

  return returnPoint;
}

Point2D GeometryHelper::Point_3Dto2D(double * xyz, unsigned int plane) const {
  TVector3 vec(xyz);
  return Point_3Dto2D(vec, plane);
}
Point2D GeometryHelper::Point_3Dto2D(float * xyz, unsigned int plane) const {
  TVector3 vec(xyz);
  return Point_3Dto2D(vec, plane);
}
Point2D GeometryHelper::Point_3Dto2D(float x, float y, float z, unsigned int plane) const {
  TVector3 vec(x, y, z);
  return Point_3Dto2D(vec, plane);
}
Point2D GeometryHelper::Point_3Dto2D(const std::vector<double> & xyz, unsigned int plane) const {
  TVector3 vec(&(xyz[0]));
  return Point_3Dto2D(vec, plane);
}
Point2D GeometryHelper::Point_3Dto2D(const std::vector<float> & xyz, unsigned int plane) const {
  TVector3 vec(&(xyz[0]));
  return Point_3Dto2D(vec, plane);
}

void GeometryHelper::Line_3Dto2D( const TVector3 & startPoint3D, const TVector3 & direction3D, unsigned int plane,
                                  Point2D & startPoint2D, Point2D & direction2D) const
{
  // First step, project the start point into 2D
  startPoint2D = Point_3Dto2D(startPoint3D, plane);
  if (! Point_isInTPC( startPoint3D ) ) {
    std::cerr << "ERROR - GeometryHelper::Line_3Dto2D: StartPoint3D must be in the TPC.\n";
    return;
  }
  // Next, get a second point in 3D:
  float alpha = 10;
  TVector3 secondPoint3D = startPoint3D + alpha * direction3D;
  while ( ! Point_isInTPC(secondPoint3D) ) {
    alpha *= -0.75;
    secondPoint3D = startPoint3D + alpha * direction3D;
  }

  // std::cout << "3D line is (" << startPoint3D.X() << ", " << startPoint3D.Y()
  //           << ", " << startPoint3D.Z() << ") to ( " << secondPoint3D.X()
  //           << ", " << secondPoint3D.Y() << ", " << secondPoint3D.Z() << ")\n";

  // Project the second point into 2D:
  Point2D secondPoint2D = Point_3Dto2D(secondPoint3D, plane);

  // std::cout << "2D line is (" << startPoint2D.w << ", " << startPoint2D.t
  //           << ") to (" << secondPoint2D.w << ", " << secondPoint2D.t << ")\n";

  // Now we have two points in 2D.  Get the direction by subtracting, and normalize
  TVector2 dir(secondPoint2D.w - startPoint2D.w, secondPoint2D.t - startPoint2D.t);
  if (dir.X() != 0.0 || dir.Y() != 0.0 )
    dir *= 1.0 / dir.Mod();
  direction2D.w = dir.X();
  direction2D.t = dir.Y();
  direction2D.plane = plane;

  return;
}

float GeometryHelper::Slope_3Dto2D(const TVector3 & inputVector, unsigned int plane) const {
  // Do this by projecting the line:
  // Generate a start point right in the middle of the detector:
  TVector3 startPoint3D(0, 0, 0);
  startPoint3D.SetZ(0.5 * geom -> DetLength());
  larutil::Point2D p1, slope;
  Line_3Dto2D(startPoint3D, inputVector, plane, p1, slope);
  return slope.t / slope.w;
}


TVector3 GeometryHelper::Project_3DLine_OnPlane(const TVector3& inputVector, const TVector3& planeNormal) const {

  // calculate the equivalent vector projected on the plane of interest

  // V is the original vector in 3D
  // N is the normal to the plane
  // the component on the plane then is
  // V - ( V dot N ) * N

  auto ret = inputVector - inputVector.Dot(planeNormal) * planeNormal;

  return ret;
}


TVector3 GeometryHelper::Project_3DLine_OnPlane(const TVector3& inputVector, const int& pl) const {

  // NOTE: use 0.866025404 for sqrt(3)/2 to avoid sqrt computation
  TVector3 plane(1., 1., 1.);

  // get the TVector3 corresponding to the normal in (w,t) space for the plane we are interested in
  if (pl == 0)
    plane = {0, 0.866025404, -0.5};
  else if (pl == 1)
    plane = {0, 0.866025404, 0.5};
  else if (pl == 2)
    plane = {0, 1., 0};
  else
    throw LArUtilException("Invalid plane! this plane number does not exist");

  return Project_3DLine_OnPlane(inputVector, plane);
}


std::vector<double> GeometryHelper::Project_3DLine_OnPlane(const std::vector<double>& V, const std::vector<double>& N) const {

  if ( (V.size() != 3) or (N.size() != 3) )
    throw LArUtilException("Project_3DLine_OnPlane failed due to unrecognized vector size");

  // calculate the equivalent vector projected on the plane of interest

  // V is the original vector in 3D
  // N is the normal to the plane
  // the component on the plane then is
  // V - ( V dot N ) * N

  double dot = V[0] * N[0] + V[1] * N[1] + V[2] * N[2];

  std::vector<double> ret = { V[0] - dot * N[0], V[1] - dot * N[1], V[2] - dot * N[2]};
  return ret;
}

std::vector<double> GeometryHelper::Project_3DLine_OnPlane(const std::vector<double>& V, const int& pl) const {

  // NOTE: use 0.866025404 for sqrt(3)/2 to avoid sqrt computation

  std::vector<double> plane = {1., 1., 1.};

  // get the vector corresponding to the normal in (w,t) space for the plane we are interested in
  if (pl == 0)
    plane = {0, 0.866025404, -0.5};
  else if (pl == 1)
    plane = {0, 0.866025404, 0.5};
  else if (pl == 2)
    plane = {0, 1., 0};
  else
    throw LArUtilException("Invalid plane! this plane number does not exist");

  return Project_3DLine_OnPlane(V, plane);
}


float GeometryHelper::DistanceToLine2D( const Point2D & pointOnLine, const Point2D & directionOfLine,
                                        const Point2D & targetPoint) const
{

  // Utilize the 3D function to make it easier to maintain
  TVector3 _pointOnLine(pointOnLine.w, pointOnLine.t, 0.0);
  TVector3 _directionOfLine(directionOfLine.w, directionOfLine.t, 0.0);
  TVector3 _targetPoint(targetPoint.w, targetPoint.t, 0.0);

  return DistanceToLine3D(_pointOnLine, _directionOfLine, _targetPoint);
}

float GeometryHelper::DistanceToLine2D( const Point2D & pointOnLine, const Point2D & directionOfLine,
                                        const Hit2D & targetPoint) const
{

  // Utilize the 3D function to make it easier to maintain
  TVector3 _pointOnLine(pointOnLine.w, pointOnLine.t, 0.0);
  TVector3 _directionOfLine(directionOfLine.w, directionOfLine.t, 0.0);
  TVector3 _targetPoint(targetPoint.w, targetPoint.t, 0.0);

  return DistanceToLine3D(_pointOnLine, _directionOfLine, _targetPoint);
}

float GeometryHelper::DistanceToLine2D( const TVector2 & pointOnLine, const TVector2 & directionOfLine,
                                        const TVector2 & targetPoint) const
{
  // Utilize the 3D function to make it easier to maintain
  TVector3 _pointOnLine(pointOnLine.X(), pointOnLine.Y(), 0.0);
  TVector3 _directionOfLine(directionOfLine.X(), directionOfLine.Y(), 0.0);
  TVector3 _targetPoint(targetPoint.X(), targetPoint.Y(), 0.0);

  return DistanceToLine3D(_pointOnLine, _directionOfLine, _targetPoint);
}

float GeometryHelper::DistanceToLine3D( const TVector3 & pointOnLine, const TVector3 & directionOfLine,
                                        const TVector3 & targetPoint) const
{
  // This algorithm finds the distance between a point and a line by finding the closest point on the line
  // Using minimization techniques from calculus.

  // Line is defined by the vectors pointOnLine and directionOfLine.
  // So, any point on the line can be parametrized as (pointOnLine + t * directionOfLine)
  // This distance between any point on the line and the target point is thus:
  // L = |(pointOnLine + t*directionOfLine) - targetPoint |
  //
  // Using this, minimize the distance with respect to t (actually, minimize the distance squared since it's easier):
  // d(L^2)/dt = 2 * ( (pointOnLine + t*directionOfLine) - targetPoint ) * directionOfLine
  //
  // Set equal to 0 and solve for t:
  // pointOnLine . directionOfLine + t * directionOfLine^2 - targetPoint . directionOfLine = 0;
  // Therefore:
  float t = ( targetPoint.Dot(directionOfLine) - pointOnLine.Dot(directionOfLine) ) / (directionOfLine.Dot(directionOfLine));

  // Now, construct the closest approach point, subtract the target point, and return the mag
  TVector3 closestApproach = pointOnLine + t * directionOfLine;

  closestApproach -= targetPoint;
  return closestApproach.Mag();

}

float GeometryHelper::DistanceAlongLine3D(
  const TVector3 & pointOnLine, const TVector3 & directionOfLine,
  const TVector3 & targetPoint) const
{
  // This algorithm finds the distance between a point and a line by finding the closest point on the line
  // Using minimization techniques from calculus.

  // Line is defined by the vectors pointOnLine and directionOfLine.
  // So, any point on the line can be parametrized as (pointOnLine + t * directionOfLine)
  // This distance between any point on the line and the target point is thus:
  // L = |(pointOnLine + t*directionOfLine) - targetPoint |
  //
  // Using this, minimize the distance with respect to t (actually, minimize the distance squared since it's easier):
  // d(L^2)/dt = 2 * ( (pointOnLine + t*directionOfLine) - targetPoint ) * directionOfLine
  //
  // Set equal to 0 and solve for t:
  // pointOnLine . directionOfLine + t * directionOfLine^2 - targetPoint . directionOfLine = 0;
  // Therefore:
  return ( targetPoint.Dot(directionOfLine) - pointOnLine.Dot(directionOfLine) ) / (directionOfLine.Dot(directionOfLine));



}

float GeometryHelper::Get2DDistanceSqrd(const Point2D& point1, const Point2D& point2) const
{
  return (point1.w - point2.w) * (point1.w - point2.w) + (point1.t - point2.t) * (point1.t - point2.t);
}


float GeometryHelper::Get2DDistance(const Point2D& point1, const Point2D& point2) const
{
  return TMath::Sqrt(Get2DDistanceSqrd(point1, point2));
}

int GeometryHelper::GetPointOnLine(const double& slope,
                                   const Point2D &startpoint,
                                   const Point2D &point1,
                                   Point2D& pointout) const
{

  double intercept = startpoint.t - slope * startpoint.w;

  double invslope = 0;

  if (slope)
    invslope = -1. / slope;

  double ort_intercept = point1.t - invslope * point1.w;

  if ((slope - invslope) != 0)
    pointout.w = (ort_intercept - intercept) / (slope - invslope);
  else
    pointout.w = point1.w;

  pointout.t = slope * pointout.w + intercept;

  return 0;
}

double GeometryHelper::GetPitch(const TVector3& direction, const int& pl) const
{

  // prepare a direction vector for the plane
  TVector3 wireDir = {0., 0., 0.};

  // the direction of the plane is the vector uniting two consecutive wires
  // such that this vector is perpendicular to both wires
  // basically this is the vector perpendicular to the wire length direction,
  // and still in the wire-plane direction
  if (pl == 0)
    wireDir = {0., -sqrt(3) / 2., 1 / 2.};
  else if (pl == 1)
    wireDir = {0.,  sqrt(3) / 2., 1 / 2.};
  else if (pl == 2)
    wireDir = {0., 0., 1.};
  else
    throw LArUtilException("Plane number out of bounds!");

  // cosine between shower direction and plane direction gives the factor
  // by which to divide 0.3, the minimum wire-spacing
  double minWireSpacing = 0.3;

  double cos = wireDir.Dot(direction);
  if (cos < 0) cos *= -1;
  cos /= (wireDir.Mag() * direction.Mag());

  // if cosine is 0 the direction is perpendicular and the wire-spacing is infinite
  if (cos == 0)
    return kDOUBLE_MAX;

  double pitch = minWireSpacing / cos;
  return pitch;
}


double GeometryHelper::CalculatePitch(UInt_t pl, double phi, double theta) const
{

  double pitch = -1.;

  if (geom->PlaneToView(pl) == larlite::geo::kUnknown ||
      geom->PlaneToView(pl) == larlite::geo::k3D) {
    print(larlite::msg::kERROR, __FUNCTION__, Form("Warning :  no Pitch foreseen for view %d", geom->PlaneToView(pl)));
    return pitch;
  }
  else if ( pl >= geom->Nplanes() ) {
    print(larlite::msg::kERROR, __FUNCTION__, "Plane number larger than max. number of planes");
    return pitch;
  }
  else {

    double pi = TMath::Pi();
    double fTheta = pi / 2 - theta;
    double fPhi = -(phi + pi / 2);
    double wirePitch = geom->WirePitch(0, 1, pl);
    double angleToVert = 0.5 * TMath::Pi() - geom->WireAngleToVertical(geom->PlaneToView(pl));
    double cosgamma = TMath::Abs(TMath::Sin(angleToVert) * TMath::Cos(fTheta)
                                 + TMath::Cos(angleToVert) * TMath::Sin(fTheta) * TMath::Sin(fPhi));

    if (cosgamma > 0) pitch = wirePitch / cosgamma;
  } // end if a reasonable view

  return pitch;
}

double GeometryHelper::PitchInView(UInt_t plane, double phi, double theta) const
{

  Double_t dirs[3] = {0.};
  GetDirectionCosines(phi, theta, dirs);

  /// \todo switch to using new Geometry::WireAngleToVertical(geo::View_t)
  /// \todo and Geometry::WirePitch(geo::View_t) methods
  Double_t wirePitch   = 0.;
  Double_t angleToVert = 0.;

  wirePitch = geom->WirePitch(0, 1, plane);
  angleToVert = geom->WireAngleToVertical(geom->PlaneToView(plane)) - 0.5 * TMath::Pi();

  //(sin(angleToVert),std::cos(angleToVert)) is the direction perpendicular to wire
  //fDir.front() is the direction of the track at the beginning of its trajectory
  Double_t cosgamma = TMath::Abs(TMath::Sin(angleToVert) * dirs[1] +
                                 TMath::Cos(angleToVert) * dirs[2]);

  if (cosgamma < 1.e-5)
    throw LArUtilException("cosgamma is basically 0, that can't be right");

  return wirePitch / cosgamma;
}

void GeometryHelper::GetDirectionCosines(double phi, double theta, Double_t *dirs) const

{
  theta *= (TMath::Pi() / 180);
  phi *= (TMath::Pi() / 180); // working on copies, it's ok.
  dirs[0] = TMath::Cos(theta) * TMath::Sin(phi);
  dirs[1] = TMath::Sin(theta);
  dirs[2] = TMath::Cos(theta) * TMath::Cos(phi);

  return;
}

double GeometryHelper::GetCosAngleBetweenLines(const double& s1, const double& s2) const
{

  double den = sqrt(1 + s1 * s1) + sqrt(1 + s2 * s2);
  return (s1 * s2 + 1) / den;
}

double GeometryHelper::GetCosAngleBetweenLines(const Point2D& p1, const Point2D& p2, const Point2D& p3) const
{

  if ( ( (p1.w == p2.w) and (p1.t == p2.t) ) or
       ( (p1.w == p3.w) and (p1.t == p3.t) ) )
    throw LArUtilException("Trying to calculate dot-product using a zero-length vector!");

  double den = sqrt( ( (p2.w - p1.w) * (p2.w - p1.w) + (p2.t - p1.t) * (p2.t - p1.t) ) *
                     ( (p3.w - p1.w) * (p3.w - p1.w) + (p3.t - p1.t) * (p3.t - p1.t) ) );

  return ( (p2.w - p1.w) * (p3.w - p1.w) + (p2.t - p1.t) * (p3.t - p1.t) ) / den;
}


double GeometryHelper::GetCosAngleBetweenLines(const Point2D& p1, const Point2D& p2,
    const Point2D& p3, const Point2D& p4) const
{

  if ( ( (p1.w == p2.w) and (p1.t == p2.t) ) or
       ( (p4.w == p3.w) and (p4.t == p3.t) ) )
    throw LArUtilException("Trying to calculate dot-product using a zero-length vector!");

  double den = sqrt( ( (p2.w - p1.w) * (p2.w - p1.w) + (p2.t - p1.t) * (p2.t - p1.t) ) *
                     ( (p4.w - p3.w) * (p4.w - p3.w) + (p4.t - p3.t) * (p4.t - p3.t) ) );

  return ( (p2.w - p1.w) * (p4.w - p3.w) + (p2.t - p1.t) * (p4.t - p3.t) ) / den;
}

double GeometryHelper::GetTanAngleBetweenLines(const double& s1, const double& s2) const
{

  // see this: http://planetmath.org/anglebetweentwolines

  // if the slopes are the same -> the angle is 0
  // if slope1 * slope2 == -1 -> perpendicular lines -> 90 degrees
  if (s1 * s2 == -1)
    return kDOUBLE_MAX;

  return (s1 - s2) / (1 + s1 * s2);
}


void GeometryHelper::SelectPolygonHitList(const std::vector<Hit2D> &inputHits,
    std::vector <const Hit2D*> &edgeHits,
    double frac) const
{

  // if hit list is empty get out of here!
  if (!(inputHits.size())) {
    throw LArUtilException("Provided empty hit list!");
    return;
  }

  // if the fraction is > 1 then use 1...should not be larger
  // frac is the fraction of charge in the hit list
  // than needs to be included in the Polygon
  if (frac > 1) { frac = 1; }

  // clear list of hits that define the edges of the polygon
  edgeHits.clear();

  // determine the plane for this cluster (assumes all hits from the same cluster)
  unsigned char plane = (inputHits[0]).plane;

  // Define subset of hits to define polygon
  std::map<double, const Hit2D*> hitmap;

  // define a parameter that stores the total charge in the cluster
  double qtotal = 0;
  for (auto const &h : inputHits) {
    hitmap.insert(std::pair<double, const Hit2D*>(h.charge, &h));
    qtotal += h.charge;
  }
  // define a parameter to store the charge that will be within the polygon
  double qintegral = 0;
  std::vector<const Hit2D*> ordered_hits;
  ordered_hits.reserve(inputHits.size());
  for (auto hiter = hitmap.rbegin(); qintegral <= qtotal * frac && hiter != hitmap.rend(); ++hiter) {
    qintegral += (*hiter).first;
    ordered_hits.push_back((*hiter).second);
  }

  // Define container to hold found polygon corner PxHit index & distance
  std::vector<size_t> hit_index(8, 0);
  std::vector<double> hit_distance(8, 1e9);

  // Loop over hits and find corner points in the plane view
  // Also fill corner edge points
  std::vector<larutil::Point2D> edges(4, Point2D(plane, 0, 0));
  double wire_max = geom->Nwires(plane) * fWireToCm;
  double time_max = (detp->NumberTimeSamples()) * fTimeToCm;

  for (size_t index = 0; index < ordered_hits.size(); ++index) {

    /* Deprecated: do not throw exceptions if out of TPC bounds...
    if (ordered_hits.at(index)->t < -detp -> TriggerOffset() ||
    ordered_hits.at(index)->w < 0 ||
    ordered_hits.at(index)->t > time_max ||
    ordered_hits.at(index)->w > wire_max ) {

    throw LArUtilException(Form("Invalid wire/time (%g,%g) for plane %i ... range is (0=>%g,0=>%g)",
          ordered_hits.at(index)->w,
          ordered_hits.at(index)->t,
          plane,
          wire_max,
          time_max)
           );
    return;
    }
    */

    double dist = 0;

    // First thing to do:
    // Find the hits that have the largest/smallest wire number and time
    // these will define the first (up to) 4 boundaries of our polygon

    // Comparison w/ (Wire,0)
    dist = ordered_hits.at(index)->t;
    if (dist < hit_distance.at(1)) {
      hit_distance.at(1) = dist;
      hit_index.at(1) = index;
      edges.at(0).t = ordered_hits.at(index)->t;
      edges.at(1).t = ordered_hits.at(index)->t;
    }

    // Comparison w/ (WireMax,Time)
    dist = wire_max - ordered_hits.at(index)->w;
    if (dist < hit_distance.at(3)) {
      hit_distance.at(3) = dist;
      hit_index.at(3) = index;
      edges.at(1).w = ordered_hits.at(index)->w;
      edges.at(2).w = ordered_hits.at(index)->w;
    }

    // Comparison w/ (Wire,TimeMax)
    dist = time_max - ordered_hits.at(index)->t;
    if (dist < hit_distance.at(5)) {
      hit_distance.at(5) = dist;
      hit_index.at(5) = index;
      edges.at(2).t = ordered_hits.at(index)->t;
      edges.at(3).t = ordered_hits.at(index)->t;
    }

    // Comparison w/ (0,Time)
    dist = ordered_hits.at(index)->w;
    if (dist < hit_distance.at(7)) {
      hit_distance.at(7) = dist;
      hit_index.at(7) = index;
      edges.at(0).w = ordered_hits.at(index)->w;
      edges.at(3).w = ordered_hits.at(index)->w;
    }
  }

  // next find the hits that are closest to the 3 corners of the rectangle
  for (size_t index = 0; index < ordered_hits.size(); ++index) {

    double dist = 0;
    // Comparison w/ (0,0)
    dist = pow((ordered_hits.at(index)->t - edges.at(0).t), 2) + pow((ordered_hits.at(index)->w - edges.at(0).w), 2);
    if (dist < hit_distance.at(0)) {
      hit_distance.at(0) = dist;
      hit_index.at(0) = index;
    }

    // Comparison w/ (WireMax,0)
    dist = pow((ordered_hits.at(index)->t - edges.at(1).t), 2) + pow((ordered_hits.at(index)->w - edges.at(1).w), 2);
    if (dist < hit_distance.at(2)) {
      hit_distance.at(2) = dist;
      hit_index.at(2) = index;
    }

    // Comparison w/ (WireMax,TimeMax)
    dist = pow((ordered_hits.at(index)->t - edges.at(2).t), 2) + pow((ordered_hits.at(index)->w - edges.at(2).w), 2);
    if (dist < hit_distance.at(4)) {
      hit_distance.at(4) = dist;
      hit_index.at(4) = index;
    }

    // Comparison w/ (0,TimeMax)
    dist = pow((ordered_hits.at(index)->t - edges.at(3).t), 2) + pow((ordered_hits.at(index)->w - edges.at(3).w), 2);
    if (dist < hit_distance.at(6)) {
      hit_distance.at(6) = dist;
      hit_index.at(6) = index;
    }

  }
  // Loop over the resulting hit indexes and append unique hits to define the polygon to the return hit list
  std::set<size_t> unique_index;
  std::vector<size_t> candidate_polygon;
  candidate_polygon.reserve(9);
  //    std::cout << "Original polygon: " << std::endl;
  for (auto &index : hit_index) {

    if (unique_index.find(index) == unique_index.end()) {
      //        hitlistlocal.push_back((const Hit2D*)(ordered_hits.at(index)));
      //std::cout << "(" << ordered_hits.at(index)->w << ", " << ordered_hits.at(index)->t << ")" << std::endl;
      unique_index.insert(index);
      candidate_polygon.push_back(index);
    }
  }
  for (auto &index : hit_index) {
    candidate_polygon.push_back(index);
    break;
  }

  // we should only have a maximum of 8 edges for the polygon!
  if (unique_index.size() > 8) throw LArUtilException("Size of the polygon > 8!");

  //Untangle Polygon
  candidate_polygon = OrderPolygonEdges( ordered_hits, candidate_polygon);

  edgeHits.clear();
  for ( unsigned int i = 0; i < (candidate_polygon.size() - 1); i++) {
    edgeHits.push_back((const Hit2D*)(ordered_hits.at(candidate_polygon.at(i))));
  }

  //check that polygon does not have more than 8 sides
  if (unique_index.size() > 8) throw LArUtilException("Size of the polygon > 8!");

  return;
}


std::vector<size_t>  GeometryHelper::OrderPolygonEdges( std::vector<const Hit2D*> ordered_hits ,
    std::vector<size_t> candidate_polygon) const
{

  //loop over edges
  for ( unsigned int i = 0; i < (candidate_polygon.size() - 1); i++) {
    double Ax = ordered_hits.at(candidate_polygon.at(i))->w;
    double Ay = ordered_hits.at(candidate_polygon.at(i))->t;
    double Bx = ordered_hits.at(candidate_polygon.at(i + 1))->w;
    double By = ordered_hits.at(candidate_polygon.at(i + 1))->t;
    //loop over edges that have not been checked yet...
    //only ones furhter down in polygon
    for ( unsigned int j = i + 2; j < (candidate_polygon.size() - 1); j++) {
      //avoid consecutive segments:
      if ( candidate_polygon.at(i) == candidate_polygon.at(j + 1) )
        continue;
      else {
        double Cx = ordered_hits.at(candidate_polygon.at(j))->w;
        double Cy = ordered_hits.at(candidate_polygon.at(j))->t;
        double Dx = ordered_hits.at(candidate_polygon.at(j + 1))->w;
        double Dy = ordered_hits.at(candidate_polygon.at(j + 1))->t;

        if ( (Clockwise(Ax, Ay, Cx, Cy, Dx, Dy) != Clockwise(Bx, By, Cx, Cy, Dx, Dy))
             and (Clockwise(Ax, Ay, Bx, By, Cx, Cy) != Clockwise(Ax, Ay, Bx, By, Dx, Dy)) ) {
          size_t tmp = candidate_polygon.at(i + 1);
          candidate_polygon.at(i + 1) = candidate_polygon.at(j);
          candidate_polygon.at(j) = tmp;
          //check that last element is still first (to close circle...)
          candidate_polygon.at(candidate_polygon.size() - 1) = candidate_polygon.at(0);
          //swapped polygon...now do recursion to make sure
          return OrderPolygonEdges( ordered_hits, candidate_polygon);
        }//if crossing
      }
    }//second loop
  }//first loop
  return candidate_polygon;
}

bool GeometryHelper::Clockwise(const double& Ax, const double& Ay,
                               const double& Bx, const double& By,
                               const double& Cx, const double& Cy) const
{

  return (Cy - Ay) * (Bx - Ax) > (By - Ay) * (Cx - Ax);
}

double GeometryHelper::PerpendicularDistance(const Point2D& pt,
    const double& slope,
    const double& intercept) const
{

  // line assumed to be of form :
  // y = slope * x + intercept

  if (slope == 0)
    return fabs ( pt.t - intercept );

  double d_perp = fabs ( pt.t - slope * pt.w - intercept ) / fabs(slope);

  return d_perp;
}

double GeometryHelper::PerpendicularDistance(const Point2D& pt,
    const double& slope,
    const Point2D& anchor) const
{

  double intercept = anchor.t - slope * anchor.w;

  return PerpendicularDistance(pt, slope, intercept);
}

bool GeometryHelper::Point_isInTPC(const TVector3 & pointIn3D) const {

  // Check against the 3 coordinates:
  if (pointIn3D.X() > geom -> DetHalfWidth() + detp -> TriggerOffset() * TimeToCm()
      || pointIn3D.X() < - geom -> DetHalfWidth() - detp -> TriggerOffset() * TimeToCm())
  {
    return false;
  }
  if (pointIn3D.Y() > geom -> DetHalfHeight() || pointIn3D.Y() < - geom -> DetHalfHeight() )
  {
    return false;
  }
  if (pointIn3D.Z() > geom -> DetLength() || pointIn3D.Z() < 0.0)
  {
    return false;
  }
  return true;
}


std::vector<unsigned int> GeometryHelper::SelectLocalPointList( const std::vector<Hit2D> & inputHits,
    const Hit2D & startingHit,
    const float & distanceAlongLine,
    const float & distancePerpToLine,
    const float & lineSlope,
    Hit2D & averageHit) const {

  std::vector<unsigned int> returnIndexes;
  returnIndexes.reserve(inputHits.size());

  // Turn the slope into a vector for the direction:
  TVector3 slope(1.0, lineSlope, 0.0);
  slope *= 1.0 / slope.Mag();

  // Figure out the starting point:
  TVector3 startPoint(startingHit.w, startingHit.t, 0.0);

  // keep track of the averages:
  float _average_w = 0.0, _average_t = 0.0;

  // Loop over the hits and figure out if they are in the box or not:
  unsigned int i = 0;
  for (auto & hit : inputHits) {
    // Make each hit a real point for simplicity:
    TVector3 thisHit(hit.w, hit.t, 0.0);
    // Linear distance is easy to find:
    float linearDisplacement = (thisHit - startPoint).Dot(slope);
    // Check the linear displacement:
    if (linearDisplacement > distanceAlongLine && linearDisplacement > 0.0) {
      // Then this hit fails, continue
      i++;
      continue;
    }
    // Perp distance isn't hard either:
    float perpDistance = DistanceToLine3D(startPoint, slope, thisHit);
    if (perpDistance > distancePerpToLine) {
      // Again, then this hit fails.
      i++;
      continue;
    }


    // Reached here, push back the index and move on with the loop:
    returnIndexes.push_back(i);
    _average_w += hit.w;
    _average_t += hit.t;
    i++;
  }

  if (returnIndexes.size() > 0) {
    _average_w /= returnIndexes.size();
    _average_t /= returnIndexes.size();
  }

  averageHit.w = _average_w;
  averageHit.t = _average_t;

  return returnIndexes;
}

int GeometryHelper::Get3DAxisN(const int& iplane0, const int& iplane1,
                               const double& omega0, const double& omega1,
                               double& phi, double& theta) const {

  // prepare vertical angle information for the various planes
  std::vector<double> vertangle;
  vertangle.resize(geom->Nplanes());
  for (UInt_t ip = 0; ip < geom->Nplanes(); ip++)
    vertangle[ip] = geom->WireAngleToVertical(geom->PlaneToView(ip)) - TMath::Pi() / 2; // wire angle

  // y, z, x coordinates
  Double_t ln(0), mn(0), nn(0);
  Double_t phis(0), thetan(0);

  // Pretend collection and induction planes.
  // "Collection" is the plane with the vertical angle equal to zero.
  // If both are non-zero, collection is the one with the negative angle.
  UInt_t Cplane = 0, Iplane = 1;

  // angleC and angleI are the respective angles to vertical in C/I
  // planes and slopeC, slopeI are the tangents of the track.
  Double_t angleC, angleI, slopeC, slopeI, omegaC, omegaI;
  omegaC = larlite::data::kINVALID_DOUBLE;
  omegaI = larlite::data::kINVALID_DOUBLE;

  // Don't know how to reconstruct these yet, so exit with error.
  // In
  if (omega0 == 0 || omega1 == 0) {
    phi = 0;
    theta = -999;
    return -1;
  }

  //////insert check for existence of planes.

  //check if backwards going track
  //Double_t backwards=0;
  Double_t alt_backwards = 0;

  if (fabs(omega0) > (TMath::Pi() / 2.0) || fabs(omega1) > (TMath::Pi() / 2.0) ) {
    alt_backwards = 1;
  }

  if (vertangle[iplane0] == 0) {
    // first plane is at 0 degrees
    Cplane = iplane0;
    Iplane = iplane1;
    omegaC = omega0;
    omegaI = omega1;
  }
  else if (vertangle[iplane1] == 0) {
    // second plane is at 0 degrees
    Cplane = iplane1;
    Iplane = iplane0;
    omegaC = omega1;
    omegaI = omega0;
  }
  else if (vertangle[iplane0] != 0 && vertangle[iplane1] != 0) {
    //both planes are at non zero degree - find the one with deg<0
    if (vertangle[iplane1] < vertangle[iplane0]) {
      Cplane = iplane1;
      Iplane = iplane0;
      omegaC = omega1;
      omegaI = omega0;
    }
    else if (vertangle[iplane1] > vertangle[iplane0]) {
      Cplane = iplane0;
      Iplane = iplane1;
      omegaC = omega0;
      omegaI = omega1;
    }
    else {
      //throw error - same plane.
      return -1;
    }

  }

  slopeC = tan(omegaC);
  slopeI = tan(omegaI);
  angleC = vertangle[Cplane];
  angleI = vertangle[Iplane];


  //0 -1 factor depending on if one of the planes is vertical.
  bool nfact = !(vertangle[Cplane]);

  //ln represents y, omega is 2d angle -- in first 2 quadrants y is positive.
  if (omegaC < TMath::Pi() && omegaC > 0 )
    ln = 1;
  else
    ln = -1;

  //calculate x and z using y ( ln )
  mn = (ln / (2 * sin(angleI))) * ((cos(angleI) / (slopeC * cos(angleC))) - (1 / slopeI)
                                   + nfact * (  cos(angleI) / (cos(angleC) * slopeC) - 1 / slopeI  )     );

  nn = (ln / (2 * cos(angleC))) * ((1 / slopeC) + (1 / slopeI) + nfact * ((1 / slopeC) - (1 / slopeI)));

  // Direction angles
  if (fabs(omegaC) > 0.01) // catch numeric error values
  {
    //phi=atan(ln/nn);
    phis = asin(ln / TMath::Sqrt(ln * ln + nn * nn));

    if (fabs(slopeC + slopeI) < 0.001)
      phis = 0;
    else if ( fabs(omegaI) > 0.01 && (omegaI / fabs(omegaI) == -omegaC / fabs(omegaC) )
              && ( fabs(omegaC) < 1 * TMath::Pi() / 180 || fabs(omegaC) > 179 * TMath::Pi() / 180 ) ) // angles have
      phis = (fabs(omegaC) > TMath::Pi() / 2) ? TMath::Pi() : 0;  //angles are


    if (nn < 0 && phis > 0 && !(!alt_backwards && fabs(phis) < TMath::Pi() / 4 ) ) // do not go back if track looks forward and phi is forward
      phis = (TMath::Pi()) - phis;
    else if (nn < 0 && phis < 0 && !(!alt_backwards && fabs(phis) < TMath::Pi() / 4 ) )
      phis = (-TMath::Pi()) - phis;


    phi = phis;

  }
  //If plane2 (collection), phi = 2d angle (omegaC in this case)
  else
  {
    phis = omegaC;
    phi = omegaC;
  }

  thetan = -asin ( mn / (sqrt(pow(ln, 2) + pow(mn, 2) + pow(nn, 2)) ) ) ;
  theta = thetan;

  return 0;
}


int GeometryHelper::GetXYZ(const Point2D *p0, const Point2D *p1, Double_t* xyz) const
{

  Double_t pos[3] = {0.};
  geom->PlaneOriginVtx(p0->plane, pos);
  Double_t x = (p0->t) - detp->TriggerOffset() * fTimeToCm + pos[0];
  double yz[2];

  GetYZ(p0, p1, yz);


  xyz[0] = x;
  xyz[1] = yz[0];
  xyz[2] = yz[1];

  return 0;
}


int GeometryHelper::GetYZ(const Point2D *p0, const Point2D *p1, Double_t* yz) const
{

  Double_t y, z;

  // Force to the closest wires if not in the range
  int z0 = p0->w / fWireToCm;
  int z1 = p1-> w / fWireToCm;
  if (z0 < 0) {
    std::cout << "\033[93mWarning\033[00m \033[95m<<GeometryHelper::GetYZ>>\033[00m" << std::endl
              << " 2D wire position " << p0->w << " [cm] corresponds to negative wire number." << std::endl
              << " Forcing it to wire=0..." << std::endl
              << "\033[93mWarning ends...\033[00m" << std::endl;
    z0 = 0;
  }
  else if (z0 >= (int)(geom->Nwires(p0->plane))) {
    std::cout << "\033[93mWarning\033[00m \033[95m<<GeometryHelper::GetYZ>>\033[00m" << std::endl
              << " 2D wire position " << p0->w << " [cm] exceeds max wire number " << (geom->Nwires(p0->plane) - 1) << std::endl
              << " Forcing it to the max wire number..." << std::endl
              << "\033[93mWarning ends...\033[00m" << std::endl;
    z0 = geom->Nwires(p0->plane) - 1;
  }
  if (z1 < 0) {
    std::cout << "\033[93mWarning\033[00m \033[95m<<GeometryHelper::GetYZ>>\033[00m" << std::endl
              << " 2D wire position " << p1->w << " [cm] corresponds to negative wire number." << std::endl
              << " Forcing it to wire=0..." << std::endl
              << "\033[93mWarning ends...\033[00m" << std::endl;
    z1 = 0;
  }
  if (z1 >= (int)(geom->Nwires(p1->plane))) {
    std::cout << "\033[93mWarning\033[00m \033[95m<<GeometryHelper::GetYZ>>\033[00m" << std::endl
              << " 2D wire position " << p1->w << " [cm] exceeds max wire number " << (geom->Nwires(p0->plane) - 1) << std::endl
              << " Forcing it to the max wire number..." << std::endl
              << "\033[93mWarning ends...\033[00m" << std::endl;
    z1 = geom->Nwires(p1->plane) - 1;
  }

  UInt_t chan1 = geom->PlaneWireToChannel(p0->plane, z0);
  UInt_t chan2 = geom->PlaneWireToChannel(p1->plane, z1);

  if (! geom->ChannelsIntersect(chan1, chan2, y, z) )
    return -1;


  yz[0] = y;
  yz[1] = z;

  return 0;
}

  bool GeometryHelper::ContainedYZ(const double& y, const double& z) const {

    // if y out of bounds
    if ( (y < -geom->DetHalfHeight()) or (y> geom->DetHalfHeight()) )
      return false;
    
    // if z out of bounds
    if ( (z < 0) or (z > geom->DetLength()) )
      return false;
    
    return true;
  }
  
  bool GeometryHelper::Contained(const double& x, const double& y, const double& z) const {

    // if x out of bounds
    if ( (x < 0) or (x > 2*geom->DetHalfWidth()) )
      return false;

    // if y out of bounds
    if ( (y < -geom->DetHalfHeight()) or (y> geom->DetHalfHeight()) )
      return false;

    // if z out of bounds
    if ( (z < 0) or (z > geom->DetLength()) )
      return false;

    return true;
  }
  

} // larutil

#endif
