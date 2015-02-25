//
// Example C++ routine to run your analysis module, LArUtil.
//

#include "LArUtil/Geometry.h"
#include "LArUtil/DetectorProperties.h"
#include "LArUtil/LArProperties.h"
#include <iostream>

int main(int argc, char** argv){

  // Instantiate utilities:
  larutil::Geometry::GetME();
  larutil::DetectorProperties::GetME();
  larutil::LArProperties::GetME();

  std::cout
    << "This is LArUtil Simple Test Routine!" << std::endl
    << "# of channels : " << larutil::Geometry::GetME()->Nchannels() << std::endl
    << "Temperature   : " << larutil::LArProperties::GetME()->Temperature() << std::endl
    << "Sampling Rate : " << larutil::DetectorProperties::GetME()->SamplingRate() <<std::endl
    << std::endl;

  return 0;
}
