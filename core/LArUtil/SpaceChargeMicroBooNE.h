#ifndef __SPACECHARGEMICROBOONE_H__
#define __SPACECHARGEMICROBOONE_H__
////////////////////////////////////////////////////////////////////////
// \file SpaceChargeMicroBooNE.h
//
// \brief header of class for storing/accessing space charge distortions for MicroBooNE
//
// \author mrmooney@bnl.gov
// 
////////////////////////////////////////////////////////////////////////


// C/C++ standard libraries
#include "SpaceChargeMicroBooNEInterface.h"

namespace larutil{
  
  class SpaceChargeMicroBooNE {
 
  public:

    typedef enum { kMCC9_Forward=0, kMCC9_Backward, kMCC8_E273, kMCC8_E227, kMCC8_E500 } Version_t;
    

    SpaceChargeMicroBooNE( Version_t version=kMCC9_Forward, std::string filename="" );
    ~SpaceChargeMicroBooNE();

    bool Configure( Version_t version, std::string filename="");
    bool Configure( std::string filename ) { return Configure(kMCC9_Forward,filename); };  ///< [DEPRECATED] kept to be backwards compatible. will default to kMCC9_Forward.
    
    std::vector<double> GetPosOffsets(double xVal, double yVal, double zVal) const;
    std::vector<double> GetEfieldOffsets(double xVal, double yVal, double zVal) const;
    Version_t GetVersion() const { return _version; };
    float tickoffset_forward_hack( float tick ) const; // tick offset to add when going from SCE projected position to image
    float tickoffset_backward_hack( float tick ) const; // tick offset to add when going from SCE projected position to image    
    
  protected:


    Version_t _version;
    SpaceChargeMicroBooNEInterface* _sce;

  }; // class SpaceChargeMicroBooNE
}
#endif
