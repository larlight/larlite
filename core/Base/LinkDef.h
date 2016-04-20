//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace larlite+;
#pragma link C++ namespace larlite::simb+;
#pragma link C++ namespace larlite::anab+;
#pragma link C++ namespace larlite::msg+;
#pragma link C++ namespace larlite::larch+;
//#pragma link C++ namespace larlite::data+;
#pragma link C++ class larlite::data+;
#pragma link C++ namespace larlite::geo+;

#pragma link C++ enum larlite::geo::SigType_t+;
#pragma link C++ enum larlite::geo::View_t+;

#pragma link C++ struct larlite::geo::TPCID+;
#pragma link C++ struct larlite::geo::PlaneID+;
#pragma link C++ struct larlite::geo::WireID+;

#pragma link C++ class std::vector<larlite::geo::SigType_t>+;
#pragma link C++ class std::vector<larlite::geo::View_t>+;
#pragma link C++ class std::vector<std::string>+;
#pragma link C++ enum larlite::data::DataType_t+;

#pragma link C++ class larlite::Message+;
#pragma link C++ class larlite::larlite_base+;

#pragma link C++ function const std::string& GetProductName(const larlite::data::DataType_t)+;

#endif
