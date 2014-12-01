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
#pragma link C++ class sptool::RooRealVars_t+;

#pragma link C++ class sptool::SPTBase+;
#pragma link C++ class sptool::SPAShower+;
#pragma link C++ class sptool::SPAData+;
#pragma link C++ class sptool::SPAManager+;
#pragma link C++ class sptool::ShowerPdfFactory+;
//ADD_NEW_CLASS ... do not change this line
#pragma link C++ class larlite::ExampleSPSelection+;
#pragma link C++ class larlite::ExampleSPFit+;

#endif


