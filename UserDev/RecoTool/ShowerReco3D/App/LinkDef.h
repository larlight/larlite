//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class larlite::ShowerReco3D+;
#pragma link C++ class larlite::ComputePi0Mass+;
#pragma link C++ class larlite::Pi0Mass+;
#pragma link C++ class larlite::Pi0Quality+;
#pragma link C++ class larlite::ShowerQuality_singleshowers+;
#pragma link C++ class larlite::ShowerQuality_multishowers+;
#pragma link C++ class larlite::HitCalibration+;
#pragma link C++ class larlite::ToyShowerGenerator+;
#pragma link C++ class larlite::ShowerAna+;
#pragma link C++ class showerreco::ShowerClusterFinder+;
#pragma link C++ class showerreco::ColtonPandoraShower+;
#pragma link C++ class showerreco::ClusterInfo+;
//ADD_NEW_CLASS ... do not change this line
//ADD_NEW_ALG_CLASS ... do not change this line
#endif



