//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace geoalgo+;
#pragma link C++ class geoalgo::Vector+;
#pragma link C++ class std::vector<geoalgo::Vector>+;
#pragma link C++ class std::vector<std::vector<geoalgo::Vector> >+;
#pragma link C++ class geoalgo::Trajectory+;
#pragma link C++ class geoalgo::HalfLine+;
#pragma link C++ class geoalgo::Line+;
#pragma link C++ class geoalgo::DirectedLine+;
#pragma link C++ class geoalgo::LineSegment+;
#pragma link C++ class geoalgo::AABox+;
#pragma link C++ class geoalgo::IntersectAlgo+;
#pragma link C++ class geoalgo::DistanceAlgo+;

#pragma link C++ class geoalgo::DistToBoxWall+;
#pragma link C++ class geoalgo::GeoAlgoException+;
#pragma link C++ class geoalgo::PointToLineDist+;
#pragma link C++ class geoalgo::TwoLineIntersection+;
#pragma link C++ class geoalgo::TrajectoryInVolume+;
#pragma link C++ class geoalgo::SegmentPoCA+;

#pragma link C++ namespace mpl+;
#pragma link C++ class mpl::GeoAlgoMPLBackEnd+;
//ADD_NEW_CLASS ... do not change this line

#endif
















