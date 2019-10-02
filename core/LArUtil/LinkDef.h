//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace larutil;

#pragma link C++ class std::vector<std::vector<Float_t> >+;
#pragma link C++ class std::vector<std::vector<std::vector<Double_t > > >+;
#pragma link C++ class larutil::LArUtilException+;
#pragma link C++ class larutil::InvalidWireError+;
//#pragma link C++ class larutil::LArUtilBase+;
#pragma link C++ class larutil::Geometry+;
#pragma link C++ class larutil::LArProperties+;
#pragma link C++ class larutil::DetectorProperties+;
#pragma link C++ class larutil::TimeService+;
#pragma link C++ class larutil::ElecClock+;
#pragma link C++ class larutil::PxPoint+;
#pragma link C++ class larutil::PxLine+;
#pragma link C++ class larutil::PxHit+;

#pragma link C++ class larutil::LArUtilConfig+;
#pragma link C++ class larutil::LArUtilManager+;
#pragma link C++ class larutil::GeometryHelper+;
#pragma link C++ class larutil::SpaceChargeMicroBooNEInterface+;
#pragma link C++ class larutil::SpaceChargeMicroBooNEMCC8+;
#pragma link C++ class larutil::SpaceChargeMicroBooNEMCC9+;
#pragma link C++ class larutil::SpaceChargeMicroBooNE+;
//ADD_NEW_CLASS ... do not change this line
#endif








