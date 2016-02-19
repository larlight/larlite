//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;


#pragma link C++ class evd::Track3D+;
#pragma link C++ class std::vector<evd::Track3D>+;
#pragma link C++ class evd::RecoBase3D<evd::Track3D>+;
#pragma link C++ class evd::DrawTrack3D+;

#pragma link C++ class evd::MCTrack3D+;
#pragma link C++ class std::vector<evd::MCTrack3D>+;
#pragma link C++ class evd::RecoBase3D<evd::MCTrack3D>+;
#pragma link C++ class evd::DrawMCTrack3D+;

#pragma link C++ class evd::Shower3D+;
#pragma link C++ class std::vector<evd::Shower3D>+;
#pragma link C++ class evd::RecoBase3D<evd::Shower3D>+;
#pragma link C++ class evd::DrawShower3D+;

#pragma link C++ class evd::MCShower3D+;
#pragma link C++ class std::vector<evd::MCShower3D>+;
#pragma link C++ class evd::RecoBase3D<evd::MCShower3D>+;
#pragma link C++ class evd::DrawMCShower3D+;

// #pragma link C++ class evd::Spacepoint3D+;
#pragma link C++ class std::vector<evd::Spacepoint3D>+;
#pragma link C++ class evd::RecoBase3D<evd::Spacepoint3D>+;
#pragma link C++ class evd::DrawSpacepoint3D+;

#pragma link C++ class evd::SimChannel3D+;
#pragma link C++ class std::vector<evd::SimChannel3D>+;
#pragma link C++ class evd::RecoBase3D<evd::SimChannel3D>+;
#pragma link C++ class evd::DrawSimChannel3D+;

#pragma link C++ class evd::Opflash3D+;
#pragma link C++ class std::vector<evd::Opflash3D>+;
#pragma link C++ class evd::RecoBase3D<evd::Opflash3D>+;
#pragma link C++ class evd::DrawOpflash3D+;

#pragma link C++ class evd::Seed3D+;
#pragma link C++ class std::vector<evd::Seed3D>+;
#pragma link C++ class evd::RecoBase3D<evd::Seed3D>+;
#pragma link C++ class evd::DrawSeed3D+;

#pragma link C++ class evd::Vertex3D+;
#pragma link C++ class std::vector<evd::Vertex3D>+;
#pragma link C++ class evd::RecoBase3D<evd::Vertex3D>+;
#pragma link C++ class evd::DrawVertex3D+;

#pragma link C++ class evd::PFPart3D+;
#pragma link C++ class std::vector<evd::PFPart3D>+;
#pragma link C++ class evd::RecoBase3D<evd::PFPart3D>+;
#pragma link C++ class evd::DrawPFParticle3D+;

// Link the classes that are processors

//ADD_NEW_CLASS ... do not change this line
#endif







