//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace flashana;
#pragma link C++ namespace flashana::msg;
#pragma link C++ class flashana::QPoint_t+;
#pragma link C++ class flashana::QCluster_t+;
#pragma link C++ class flashana::Flash_t+;
#pragma link C++ class flashana::QPoint_t+;
#pragma link C++ class flashana::QCluster_t+;
#pragma link C++ class flashana::FlashMatch_t+;
#pragma link C++ class std::vector<flashana::Flash_t>+;
#pragma link C++ class std::vector<flashana::QPoint_t>+;
#pragma link C++ class std::vector<flashana::QCluster_t>+;
#pragma link C++ class std::vector<flashana::FlashMatch_t>+;
#pragma link C++ class flashana::FlashMatchManager+;
#pragma link C++ class flashana::BaseAlgorithm+;
#pragma link C++ class flashana::BaseProhibitAlgo+;
#pragma link C++ class flashana::BaseTPCFilter+;
#pragma link C++ class flashana::BaseFlashFilter+;
#pragma link C++ class flashana::BaseFlashMatch+;
#pragma link C++ class flashana::BaseFlashHypothesis+;
//ADD_NEW_CLASS ... do not change this line
#endif










