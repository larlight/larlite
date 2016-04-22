//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class evd::RawBase+;
#pragma link C++ class evd::DrawRaw+;
#pragma link C++ class evd::DrawWire+;
#pragma link C++ class evd::DrawLariatDaq+;
#pragma link C++ class evd::DrawRawDigit+;
#pragma link C++ class evd::TreeElementReader+;
#pragma link C++ class evd::DataFetcher+;
#pragma link C++ class evd::DrawUbSwiz+;
#pragma link C++ class ub_noise_filter::NoiseFilter+;
#pragma link C++ class ub_noise_filter::ChirpFilter+;
#pragma link C++ class CorrelatedNoiseFilter+;
//ADD_NEW_CLASS ... do not change this line
#endif








