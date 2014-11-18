//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#if defined(__ROOTCINT__) || defined(__ROOTCLING__)
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

//#pragma link C++ class std::vector<unsigned int>+;
#pragma link C++ class std::vector<std::vector<unsigned int> >+;
//#pragma link C++ class std::vector<unsigned short>+;
#pragma link C++ class std::vector<std::vector<unsigned short> >+;

#pragma link C++ class cmtool::CMTException+;
#pragma link C++ class cmtool::CMergeBookKeeper+;
#pragma link C++ class cmtool::CMatchBookKeeper+;
#pragma link C++ class cmtool::CMAlgoBase+;
#pragma link C++ class cmtool::CBoolAlgoBase+;
#pragma link C++ class cmtool::CFloatAlgoBase+;
#pragma link C++ class cmtool::CPriorityAlgoBase+;
#pragma link C++ class cmtool::CMManagerBase;
#pragma link C++ class cmtool::CMergeManager+;
#pragma link C++ class cmtool::CMatchManager+;

//ADD_NEW_CLASS ... do not change this line
#endif










































