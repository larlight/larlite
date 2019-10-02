#ifndef __SPACECHARGEMICROBOONE_INTERFACE_H__
#define __SPACECHARGEMICROBOONE_INTERFACE_H__
////////////////////////////////////////////////////////////////////////
// \file SpaceChargeMicroBooNEInterface.h
//
// \brief header of class for storing/accessing space charge distortions for MicroBooNE
//
// \author mrmooney@bnl.gov
// 
////////////////////////////////////////////////////////////////////////


// C/C++ standard libraries
#include <string>
#include <vector>

namespace larutil{
  
  class SpaceChargeMicroBooNEInterface {
 
  public:

    SpaceChargeMicroBooNEInterface() {};
    virtual ~SpaceChargeMicroBooNEInterface() {}
      
    virtual bool Configure(std::string filename) = 0;
      
    virtual std::vector<double> GetPosOffsets(double xVal, double yVal, double zVal) const = 0;
    virtual std::vector<double> GetEfieldOffsets(double xVal, double yVal, double zVal) const = 0;

 
  }; // class SpaceChargeMicroInterface
}

#endif
