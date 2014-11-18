//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#if defined(__ROOTCINT__) || defined(__ROOTCLING__)
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class cmtool::CPAlgoArray+;
#pragma link C++ class cmtool::CPAlgoNHits+;
#pragma link C++ class cmtool::CPAlgoQSum+;
#pragma link C++ class cmtool::CPAlgoPolyArea+;
#pragma link C++ class cmtool::CPAlgoIgnoreTracks+;
//ADD_CPALGO_CLASS ... do not change this line
//ADD_NEW_CLASS ... do not change this line
#endif
