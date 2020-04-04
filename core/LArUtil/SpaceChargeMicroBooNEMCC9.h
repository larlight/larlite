#ifndef __SPACECHARGEMICROBOONE_MCC9_H__
#define __SPACECHARGEMICROBOONE_MCC9_H__
////////////////////////////////////////////////////////////////////////
// \file SpaceChargeMicroBooNEMCC9.h
//
// \brief header of class for storing/accessing space charge distortions for MicroBooNE
//
// \author mrmooney@bnl.gov
// 
////////////////////////////////////////////////////////////////////////

// ROOT includes
#include "TGraph.h"
#include "TH3F.h"

// larlite
#include "SpaceChargeMicroBooNEInterface.h"

// C/C++ standard libraries
#include <string>
#include <vector>

namespace larutil{
  
  class SpaceChargeMicroBooNEMCC9 : public SpaceChargeMicroBooNEInterface {
 
  public:

    SpaceChargeMicroBooNEMCC9();
    SpaceChargeMicroBooNEMCC9(std::string filename);
    ~SpaceChargeMicroBooNEMCC9();
      
    bool Configure(std::string filename);
      
    std::vector<double> GetPosOffsets(double xVal, double yVal, double zVal) const;
    std::vector<double> GetEfieldOffsets(double xVal, double yVal, double zVal) const;
 
  private:

    TH3F* _hD[3];
    TH3F* _hE[3];
    TFile* _inputfile;

    double TransformX(double xVal) const;
    double TransformY(double yVal) const;
    double TransformZ(double zVal) const;
    bool IsInsideBoundaries(double xVal, double yVal, double zVal) const;

    double _Xmin;
    double _Xmax;
    double _Ymin;
    double _Ymax;
    double _Zmin;
    double _Zmax;
    

  }; // class SpaceChargeMicroBooNEMCC9
}
#endif
