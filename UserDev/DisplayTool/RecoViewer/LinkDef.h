//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

// #pragma link C++ class evd::RecoBase+;
// Link all the classes that are "interactive" - for use in evd
#pragma link C++ class evd::Shower2d+;
#pragma link C++ class std::vector<::evd::Shower2d>+;
#pragma link C++ class evd::RecoBase<evd::Shower2d>+;
#pragma link C++ class evd::DrawShower+;

#pragma link C++ class evd::Hit+;
#pragma link C++ class std::vector<::evd::Hit>+;
#pragma link C++ class evd::RecoBase<evd::Hit>+;
#pragma link C++ class evd::DrawHit+;

#pragma link C++ class evd::Cluster2d+;
#pragma link C++ class std::vector<::evd::Cluster2d>+;
#pragma link C++ class evd::RecoBase<evd::Cluster2d>+;
#pragma link C++ class evd::DrawCluster+;

#pragma link C++ class evd::DrawMatch+;

// Link the classes that are processors
#pragma link C++ class evd::DrawTrack+;
#pragma link C++ class larlite::DrawVertex+;
#pragma link C++ class larlite::DrawEndpoint2d+;
#pragma link C++ class std::vector<std::vector<std::pair<float,float> > >+;
#pragma link C++ class std::vector<std::pair<float,float> >+;
//ADD_NEW_CLASS ... do not change this line
#endif







