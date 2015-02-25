
#include "Base/larlite_base.h"
#include "Base/FrameworkConstants.h"
#include <iostream>
int main(){

  std::cout 
    << std::endl
    << "This is \"Base\" package simple test routine." << std::endl
    << std::endl;

  // Test routine for Base package.
  
  // (1) simple: create larlite_base

  larlite::larlite_base me;
  me.set_verbosity(larlite::msg::kDEBUG);
  
  // (2) Send messages
  
  me.print(larlite::msg::kDEBUG,"... sending DEBUG message");
  me.print(larlite::msg::kINFO,"... sending INFO message");
  me.print(larlite::msg::kNORMAL,"... sending NORMAL message");
  me.print(larlite::msg::kWARNING,"... sending WARNING message");
  me.print(larlite::msg::kERROR,"... sending ERROR message");

  std::cout
    << std::endl
    << "End of test routine." << std::endl
    << std::endl;

  return 0;

}
