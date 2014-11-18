//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#if defined(__ROOTCINT__) || defined(__ROOTCLING__)
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace geoalgo ;
#pragma link C++ class geoalgo::DistToBoxWall+;
#pragma link C++ class geoalgo::GeoAlgoException+;
#pragma link C++ class geoalgo::PointToLineDist+;
#pragma link C++ class geoalgo::TwoLineIntersection+;
#pragma link C++ class geoalgo::TrajectoryInVolume+;
#pragma link C++ class geoalgo::SegmentPoCA+;
//ADD_NEW_CLASS ... do not change this line

#endif








