//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class cmtool::CBAlgoMergeSmallToTrack+;
#pragma link C++ class cmtool::CBAlgoMergeSingleToBig+;
#pragma link C++ class cmtool::CBAlgoMergeInline+;
#pragma link C++ class cmtool::CBAlgoMergeDiffuse+;
#pragma link C++ class cmtool::CBAlgoMergeShortestDistance+;
#pragma link C++ class cmtool::CBAlgoProhibitBigToBig+;
#pragma link C++ class cmtool::CBAlgoProhibitStartToStart+;
#pragma link C++ class cmtool::CBAlgoProhibitTrackToTrack+;
#pragma link C++ class cmtool::CBAlgoMergeWithinBoundary+;

//#pragma link C++ class larlite::HitToCluster+;
//#pragma link C++ class larlite::DropSingles+;
//ADD_NEW_CLASS ... do not change this line
#endif


