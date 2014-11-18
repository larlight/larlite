//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#if defined(__ROOTCINT__) || defined(__ROOTCLING__)
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace FANN;

#pragma link C++ class cluster::TrainingModule+;
#pragma link C++ class cluster::FANNModule+;
#pragma link C++ class FANN::neural_net+;
#pragma link C++ class FANN::training_data+;
//ADD_NEW_CLASS ... do not change this line
#endif
