//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace protoshower;

#pragma link C++ class protoshower::ProtoShower+;
#pragma link C++ class protoshower::ProtoShowerHelper+;
#pragma link C++ class protoshower::ProtoShowerAlgBase+;

#pragma link C++ class protoshower::ProtoShowerAlgArgoNeuT+;
#pragma link C++ class protoshower::ProtoShowerAlgClusterParams+;
#pragma link C++ class protoshower::ProtoShowerAlgOpenCV+;
#pragma link C++ class protoshower::ProtoShowerAlgLoadVertex+;
//ADD_NEW_CLASS ... do not change this line
//ADD_NEW_ALG_CLASS ... do not change this line
#endif





