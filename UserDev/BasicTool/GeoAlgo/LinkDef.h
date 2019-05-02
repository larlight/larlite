//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace larlite::geoalgo+;
#pragma link C++ class larlite::geoalgo::Vector+;
#pragma link C++ class std::vector<larlite::geoalgo::Vector>+;
#pragma link C++ class std::vector<std::vector<larlite::geoalgo::Vector> >+;
#pragma link C++ class std::map<larlite::geoalgo::Vector,string>+;
#pragma link C++ class larlite::geoalgo::Trajectory+;
#pragma link C++ class std::vector<larlite::geoalgo::Trajectory>+;
#pragma link C++ class larlite::geoalgo::HalfLine+;
#pragma link C++ class std::vector<larlite::geoalgo::HalfLine>+;
#pragma link C++ class larlite::geoalgo::Line+;
#pragma link C++ class std::vector<larlite::geoalgo::Line>+;
#pragma link C++ class larlite::geoalgo::DirectedLine+;
#pragma link C++ class std::vector<larlite::geoalgo::DirectedLine>+;
#pragma link C++ class larlite::geoalgo::LineSegment+;
#pragma link C++ class std::vector<larlite::geoalgo::LineSegment>+;
#pragma link C++ class larlite::geoalgo::AABox+;
#pragma link C++ class std::vector<larlite::geoalgo::AABox>+;
#pragma link C++ class larlite::geoalgo::Cylinder+;
#pragma link C++ class std::vector<larlite::geoalgo::Cylinder>+;
#pragma link C++ class larlite::geoalgo::Cone+;
#pragma link C++ class std::vector<larlite::geoalgo::Cone>+;
#pragma link C++ class larlite::geoalgo::Sphere+;
#pragma link C++ class std::vector<larlite::geoalgo::Sphere>+;
#pragma link C++ class std::pair<larlite::geoalgo::Vector,string>+;
#pragma link C++ class std::map<larlite::geoalgo::Vector,string>+;

#pragma link C++ class larlite::geoalgo::GeoAlgo+;
#pragma link C++ class larlite::geoalgo::GeoObjCollection+;
//ADD_NEW_CLASS ... do not change this line

#endif
















