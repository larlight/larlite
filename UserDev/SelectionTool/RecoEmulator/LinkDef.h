//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

// Using factory design, none of these need to be exposed to Python but feel free to change it --kazu
#pragma link C++ namespace recoemu+;
#pragma link C++ class recoemu::RecoEmulatorBase+;
#pragma link C++ class recoemu::TrackEmulatorBase+;
#pragma link C++ class recoemu::ShowerEmulatorBase+;
#pragma link C++ class recoemu::TrackEmuToy+;
#pragma link C++ class recoemu::ShowerEmuToy+;
#pragma link C++ class recoemu::ShowerEmuSmearingRecoBased+;
#pragma link C++ class recoemu::ShowerEmuSmearingParametrized+;
#pragma link C++ class recoemu::TrackEmuApplyLengthEff+;
#pragma link C++ class recoemu::TrackEmuTrackDeviation+;
#pragma link C++ class recoemu::TrackEmuTrackFlip+;
#pragma link C++ class recoemu::TrackEmuMaster+;
#pragma link C++ class RandomDrawTool+;
#pragma link C++ class FunctionFactory+;
#pragma link C++ class recoemu::RecoEmulatorException+;
//ADD_NEW_CLASS ... do not change this line
#endif
























