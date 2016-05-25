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
#pragma link C++ class evd::Shower2D+;
#pragma link C++ class std::vector<::evd::Shower2D>+;
#pragma link C++ class evd::RecoBase<evd::Shower2D>+;
#pragma link C++ class evd::DrawShower+;

#pragma link C++ class evd::Hit2D+;
#pragma link C++ class std::vector<::evd::Hit2D>+;
#pragma link C++ class evd::RecoBase<evd::Hit2D>+;
#pragma link C++ class evd::DrawHit+;

#pragma link C++ class evd::Cluster2D+;
#pragma link C++ class std::vector<::evd::Cluster2D>+;
#pragma link C++ class evd::RecoBase<evd::Cluster2D>+;
#pragma link C++ class evd::DrawCluster+;

#pragma link C++ class evd::DrawMatch+;

#pragma link C++ class evd::Track2D+;
#pragma link C++ class std::vector<std::pair<float,float> >+;
#pragma link C++ class std::vector<::evd::Track2D>+;
#pragma link C++ class evd::RecoBase<evd::Track2D>+;
#pragma link C++ class evd::DrawTrack+;

#pragma link C++ class evd::Endpoint2D+;
#pragma link C++ class std::vector<::evd::Endpoint2D>+;
#pragma link C++ class evd::RecoBase<::evd::Endpoint2D>+;
#pragma link C++ class evd::DrawEndpoint+;


// Link the classes that are processors
#pragma link C++ class evd::DrawVertex+;
#pragma link C++ class evd::DrawSpacepoint+;

#pragma link C++ class evd::Neutrino2D+;
#pragma link C++ class std::vector<::evd::Neutrino2D>+;
#pragma link C++ class evd::RecoBase<evd::Neutrino2D>+;
#pragma link C++ class evd::DrawNeutrino+;


// #pragma link C++ class std::vector<std::vector<std::pair<float,float> > >+;
//ADD_NEW_CLASS ... do not change this line
#endif







