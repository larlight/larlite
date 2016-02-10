//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace showerreco;
#pragma link C++ namespace showerreco::energy;

//ADD_NEW_CLASS ... do not change this line
#pragma link C++ class showerreco::ShowerClusterSet_t+;
#pragma link C++ class showerreco::ProtoShower+;
#pragma link C++ class showerreco::Shower_t+;
#pragma link C++ class showerreco::ShowerRecoException+;
#pragma link C++ class showerreco::ShowerCalo+;
#pragma link C++ class showerreco::ShowerRecoAlgBase+;
#pragma link C++ class showerreco::ShowerAnaBase+;
#pragma link C++ class showerreco::ShowerRecoManager+;
#pragma link C++ class showerreco::ProtoShowerHelper+;
//ADD_NEW_ALG_CLASS ... do not change this line
#endif




