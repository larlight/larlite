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

#pragma link C++ class showerreco::ProtoShowerHelper+;
#pragma link C++ class showerreco::ProtoShowerAlgBase+;

#pragma link C++ class showerreco::ProtoShowerAlgClusterParams+;
#pragma link C++ class showerreco::ProtoShowerAlgOpenCV+;
//ADD_NEW_CLASS ... do not change this line
//ADD_NEW_ALG_CLASS ... do not change this line
#endif





