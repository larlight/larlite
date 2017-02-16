//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace fcllite+;
#pragma link C++ class fcllite::FhiclLiteException+;
#pragma link C++ class fcllite::PSet+;
#pragma link C++ class fcllite::ConfigManager+;
#pragma link C++ function fcllite::PSet::get< string > (const string&)+;
#pragma link C++ function fcllite::CreatePSetFromFile(const string)+;
//ADD_NEW_CLASS ... do not change this line
#endif



