//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#if defined(__ROOTCINT__) || defined(__ROOTCLING__)
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class larlite::ana_base+;
#pragma link C++ class std::vector<larlite::ana_base*>+;
#pragma link C++ class larlite::ana_processor+;
#pragma link C++ class std::vector<TH3D*>+;
#pragma link C++ class std::vector<TH2D*>+;
#pragma link C++ class std::vector<TH1D*>+;
#pragma link C++ class std::vector<TGraph*>+;
#pragma link C++ class std::vector<TGraphErrors*>+;
//ADD_NEW_CLASS ... do not change this line
#endif













