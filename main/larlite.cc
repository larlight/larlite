#include <stdlib.h>
#include <stdio.h>

#include <TROOT.h>
#include <TRint.h>
#include <TStyle.h>
//int Error; 

extern void InitGui(); // Initializer for GUI
VoidFuncPtr_t initfuncs[] = { InitGui, 0 };

// Initialize the ROOT system
TROOT root("Rint", "The ROOT Interactive Interface", initfuncs);

int main(int argc, char **argv)
{
  TRint *theApp = new TRint("larlight", &argc, argv, NULL, 0, 0);

//  gStyle->SetPalette(1,0);
  
  // Run interactive interface
  theApp->Run();
  
 
  return(0);
};

