//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

// #pragma link C++ class evd::RecoBase3D+;
// Link all the classes that are "interactive" - for use in evd
// #pragma link C++ class evd::Shower2d+;
// #pragma link C++ class std::vector<::evd::Shower2d>+;
// #pragma link C++ class evd::RecoBase<evd::Shower2d>+;
// #pragma link C++ class evd::DrawShower+;

// #pragma link C++ class evd::Hit+;
// #pragma link C++ class std::vector<::evd::Hit>+;
// #pragma link C++ class evd::RecoBase<evd::Hit>+;
// #pragma link C++ class evd::DrawHit+;

// #pragma link C++ class evd::Cluster2d+;
// #pragma link C++ class std::vector<::evd::Cluster2d>+;
// #pragma link C++ class evd::RecoBase<evd::Cluster2d>+;
// #pragma link C++ class evd::DrawCluster+;

// #pragma link C++ class evd::DrawMatch+;

#pragma link C++ class evd::Track3D+;
// #pragma link C++ class std::vector<std::pair<float,float> >+;
#pragma link C++ class std::vector<evd::Track3D>+;
#pragma link C++ class evd::RecoBase3D<evd::Track3D>+;
#pragma link C++ class evd::DrawTrack3D+;

#pragma link C++ class evd::MCTrack3D+;
#pragma link C++ class std::vector<evd::MCTrack3D>+;
#pragma link C++ class evd::RecoBase3D<evd::MCTrack3D>+;
#pragma link C++ class evd::DrawMCTrack3D+;


// Link the classes that are processors
// #pragma link C++ class larlite::DrawVertex+;
// #pragma link C++ class larlite::DrawEndpoint2d+;
// #pragma link C++ class std::vector<std::vector<std::pair<float,float> > >+;
//ADD_NEW_CLASS ... do not change this line
#endif







