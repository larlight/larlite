//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace btutil+;
#pragma link C++ struct btutil::WireRange_t+;
#pragma link C++ class btutil::MCBTException+;
#pragma link C++ class btutil::MCBTAlg+;
#pragma link C++ class btutil::MCMatchAlg+;
#pragma link C++ class larlite::MCSingleShowerCompAlg+;
//ADD_NEW_CLASS ... do not change this line
#endif

