//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class vector<RooRealVar>+;

#pragma link C++ namespace ertool+;
#pragma link C++ class ertool::Params+;
#pragma link C++ class ertool::Record+;
#pragma link C++ class ertool::RecoObjBase+;
#pragma link C++ class ertool::Vertex+;
#pragma link C++ class ertool::Shower+;
#pragma link C++ class ertool::Track+;
#pragma link C++ class std::vector<ertool::Vertex>+;
#pragma link C++ class std::vector<ertool::Shower>+;
#pragma link C++ class std::vector<ertool::Track>+;
#pragma link C++ class std::vector<ertool::Vertex*>+;
#pragma link C++ class std::vector<ertool::Shower*>+;
#pragma link C++ class std::vector<ertool::Track*>+;
#pragma link C++ class ertool::EventData+;
#pragma link C++ class ertool::Particle+;
#pragma link C++ class ertool::BookKeeper+;

#pragma link C++ class ertool::UnitBase+;
#pragma link C++ class ertool::AlgoBase+;
#pragma link C++ class ertool::FilterBase+;

#pragma link C++ class ertool::Manager+;
#pragma link C++ class ertool::PdfFactory+;

#pragma link C++ function ertool::Combination(const size_t,const size_t)+;

#pragma link C++ class ertool::RecoObjBase+;
#pragma link C++ class ertool::RangeVar+;
//ADD_NEW_CLASS ... do not change this line

#endif











