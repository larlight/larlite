#ifndef __SPACECHARGEMICROBOONE_MCC8_H__
#define __SPACECHARGEMICROBOONE_MCC8_H__
////////////////////////////////////////////////////////////////////////
// \file SpaceChargeMicroBooNEMCC8.h
//
// \brief header of class for storing/accessing space charge distortions for MicroBooNE
//
// \author mrmooney@bnl.gov
// 
////////////////////////////////////////////////////////////////////////

// ROOT includes
#include "TGraph.h"
#include "TF1.h"
#include "TFile.h"

// larlite
#include "SpaceChargeMicroBooNEInterface.h"

// C/C++ standard libraries
#include <string>
#include <vector>

namespace larutil{
  
  class SpaceChargeMicroBooNEMCC8 : public SpaceChargeMicroBooNEInterface {
 
  public:

    SpaceChargeMicroBooNEMCC8();
    SpaceChargeMicroBooNEMCC8(std::string filename);
    ~SpaceChargeMicroBooNEMCC8();
      
    bool Configure(std::string filename);
      
    std::vector<double> GetPosOffsets(double xVal, double yVal, double zVal) const;
    std::vector<double> GetEfieldOffsets(double xVal, double yVal, double zVal) const;
 
  private:

    std::vector<double> GetPosOffsetsParametric(double xVal, double yVal, double zVal) const;
    double GetOnePosOffsetParametric(double xVal, double yVal, double zVal, std::string axis) const;
    std::vector<double> GetEfieldOffsetsParametric(double xVal, double yVal, double zVal) const;
    double GetOneEfieldOffsetParametric(double xVal, double yVal, double zVal, std::string axis) const;
    double TransformX(double xVal) const;
    double TransformY(double yVal) const;
    double TransformZ(double zVal) const;
    bool IsInsideBoundaries(double xVal, double yVal, double zVal) const;

    bool fEnableSimSpatialSCE;
    bool fEnableSimEfieldSCE;
    bool fEnableCorrSCE;
      
    TGraph* g1_x[7];
    TGraph* g2_x[7];
    TGraph* g3_x[7];
    TGraph* g4_x[7];
    TGraph* g5_x[7];
      
    TGraph* g1_y[7];
    TGraph* g2_y[7];
    TGraph* g3_y[7];
    TGraph* g4_y[7];
    TGraph* g5_y[7];
    TGraph* g6_y[7];
      
    TGraph* g1_z[7];
    TGraph* g2_z[7];
    TGraph* g3_z[7];
    TGraph* g4_z[7];
      
    TF1 *f1_x;
    TF1 *f2_x;
    TF1 *f3_x;
    TF1 *f4_x;
    TF1 *f5_x;
    TF1 *fFinal_x;
      
    TF1 *f1_y;
    TF1 *f2_y;
    TF1 *f3_y;
    TF1 *f4_y;
    TF1 *f5_y;
    TF1 *f6_y;
    TF1 *fFinal_y;
      
    TF1 *f1_z;
    TF1 *f2_z;
    TF1 *f3_z;
    TF1 *f4_z;
    TF1 *fFinal_z;

    TGraph *g1_Ex[7];
    TGraph *g2_Ex[7];
    TGraph *g3_Ex[7];
    TGraph *g4_Ex[7];
    TGraph *g5_Ex[7];
      
    TGraph *g1_Ey[7];
    TGraph *g2_Ey[7];
    TGraph *g3_Ey[7];
    TGraph *g4_Ey[7];
    TGraph *g5_Ey[7];
    TGraph *g6_Ey[7];
      
    TGraph *g1_Ez[7];
    TGraph *g2_Ez[7];
    TGraph *g3_Ez[7];
    TGraph *g4_Ez[7];
      
    TF1 *f1_Ex;
    TF1 *f2_Ex;
    TF1 *f3_Ex;
    TF1 *f4_Ex;
    TF1 *f5_Ex;
    TF1 *fFinal_Ex;
      
    TF1 *f1_Ey;
    TF1 *f2_Ey;
    TF1 *f3_Ey;
    TF1 *f4_Ey;
    TF1 *f5_Ey;
    TF1 *f6_Ey;
    TF1 *fFinal_Ey;
      
    TF1 *f1_Ez;
    TF1 *f2_Ez;
    TF1 *f3_Ez;
    TF1 *f4_Ez;
    TF1 *fFinal_Ez;

    TFile* inputfile;

  }; // class SpaceChargeMicroBooNEMCC8
}
#endif
