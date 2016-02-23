//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class showerreco::ShowerRecoModuleBase+;
#pragma link C++ class showerreco::Axis3DModule+;
#pragma link C++ class showerreco::StartPoint3DModule+;
#pragma link C++ class showerreco::OtherStartPoint3D+;
#pragma link C++ class showerreco::dQdx2DModule+;
#pragma link C++ class showerreco::dQdxModule+;
#pragma link C++ class showerreco::dEdxFromdQdx+;
#pragma link C++ class showerreco::EnergyModule+;
#pragma link C++ class showerreco::LinearEnergy+;
#pragma link C++ class showerreco::GeoModule+;
#pragma link C++ class showerreco::Angle3DFormula+;
#pragma link C++ class showerreco::PCA3DAxis+;
#pragma link C++ class showerreco::StartPointPandora+;
#pragma link C++ class showerreco::EmptyModule+;
#pragma link C++ class showerreco::ToyGeoModule+;
//ADD_NEW_CLASS ... do not change this line

#pragma link C++ class showerreco::ShowerRecoAlgModular+;
//ADD_NEW_ALG_CLASS ... do not change this line
#endif




