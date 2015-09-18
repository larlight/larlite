//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace phot+;
#pragma link C++ namespace sim+;
#pragma link C++ class sim::PhotonVoxel+;
#pragma link C++ class sim::PhotonVoxelDef+;
#pragma link C++ class phot::PhotonVisibilityService+;
#pragma link C++ class phot::PhotonLibrary+;

//ADD_NEW_CLASS ... do not change this line
#endif
