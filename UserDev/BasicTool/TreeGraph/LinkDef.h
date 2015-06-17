//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace geotree+;
#pragma link C++ class geotree::Correlation+;
#pragma link C++ class geotree::NodeCollection+;
#pragma link C++ class geotree::Node+;
#pragma link C++ class geotree::Manager+;
#pragma link C++ class geotree::Relation+;
#pragma link C++ class geotree::AlgoBase+;
#pragma link C++ class geotree::AlgoMultipleParentsBase+;
#pragma link C++ class geotree::AlgoMultipleParentsHighScore+;
#pragma link C++ class geotree::AlgoParentIsSiblingsSiblingBase+;
#pragma link C++ class geotree::AlgoParentIsSiblingsSibling+;
#pragma link C++ class geotree::AlgoGenericConflictBase+;
#pragma link C++ class geotree::AlgoGenericConflictRemoveSibling+;
#pragma link C++ class geotree::AlgoGenericConflictFindHighestScore+;
#pragma link C++ class geotree::AlgoSibHasDiffParent+;
#pragma link C++ class std::vector<geotree::Node>+;
//ADD_NEW_CLASS ... do not change this line
#endif
