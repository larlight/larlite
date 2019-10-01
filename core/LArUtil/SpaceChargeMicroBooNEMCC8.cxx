#ifndef __SPACECHARGEMICROBOONE_MCC8_CXX__
#define __SPACECHARGEMICROBOONE_MCC8_CXX__
////////////////////////////////////////////////////////////////////////
// \file SpaceChargeMicroBooNEMCC8.C
//
// \brief implementation of class for storing/accessing space charge distortions for MicroBooNE
//
// \author mrmooney@bnl.gov
// 
////////////////////////////////////////////////////////////////////////

// C++ language includes
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "math.h"
#include "stdio.h"

#include "TFile.h"

// LArSoft includes
#include "SpaceChargeMicroBooNEMCC8.h"

namespace larutil{
  
  //-----------------------------------------------
  SpaceChargeMicroBooNEMCC8::SpaceChargeMicroBooNEMCC8()
  {
    f1_x = nullptr;
    f2_x = nullptr;
    f3_x = nullptr;
    f4_x = nullptr;
    f5_x = nullptr;
    fFinal_x = nullptr;
      
    f1_y = nullptr;
    f2_y = nullptr;
    f3_y = nullptr;
    f4_y = nullptr;
    f5_y = nullptr;
    f6_y = nullptr;
    fFinal_y = nullptr;
      
    f1_z = nullptr;
    f2_z = nullptr;
    f3_z = nullptr;
    f4_z = nullptr;
    fFinal_z = nullptr;
      
    f1_Ex = nullptr;
    f2_Ex = nullptr;
    f3_Ex = nullptr;
    f4_Ex = nullptr;
    f5_Ex = nullptr;
    fFinal_Ex = nullptr;
      
    f1_Ey = nullptr;
    f2_Ey = nullptr;
    f3_Ey = nullptr;
    f4_Ey = nullptr;
    f5_Ey = nullptr;
    f6_Ey = nullptr;
    fFinal_Ey = nullptr;
      
    f1_Ez = nullptr;
    f2_Ez = nullptr;
    f3_Ez = nullptr;
    f4_Ez = nullptr;
    fFinal_Ez = nullptr;

    f1_x = nullptr;
    f2_x = nullptr;
    f3_x = nullptr;
    f4_x = nullptr;
    f5_x = nullptr;
    fFinal_x = nullptr;
      
    f1_y = nullptr;
    f2_y = nullptr;
    f3_y = nullptr;
    f4_y = nullptr;
    f5_y = nullptr;
    f6_y = nullptr;
    fFinal_y = nullptr;
      
    f1_z = nullptr;
    f2_z = nullptr;
    f3_z = nullptr;
    f4_z = nullptr;
    fFinal_z = nullptr;
      
    f1_Ex = nullptr;
    f2_Ex = nullptr;
    f3_Ex = nullptr;
    f4_Ex = nullptr;
    f5_Ex = nullptr;
    fFinal_Ex = nullptr;
      
    f1_Ey = nullptr;
    f2_Ey = nullptr;
    f3_Ey = nullptr;
    f4_Ey = nullptr;
    f5_Ey = nullptr;
    f6_Ey = nullptr;
    fFinal_Ey = nullptr;
      
    f1_Ez = nullptr;
    f2_Ez = nullptr;
    f3_Ez = nullptr;
    f4_Ez = nullptr;
    fFinal_Ez = nullptr;

    f1_x = new TF1("f1_x","pol6");
    f2_x = new TF1("f2_x","pol6");
    f3_x = new TF1("f3_x","pol6");
    f4_x = new TF1("f4_x","pol6");
    f5_x = new TF1("f5_x","pol6");
    fFinal_x = new TF1("fFinal_x","pol4");
      
    f1_y = new TF1("f1_y","pol5");
    f2_y = new TF1("f2_y","pol5");
    f3_y = new TF1("f3_y","pol5");
    f4_y = new TF1("f4_y","pol5");
    f5_y = new TF1("f5_y","pol5");
    f6_y = new TF1("f6_y","pol5");
    fFinal_y = new TF1("fFinal_y","pol5");
      
    f1_z = new TF1("f1_z","pol4");
    f2_z = new TF1("f2_z","pol4");
    f3_z = new TF1("f3_z","pol4");
    f4_z = new TF1("f4_z","pol4");
    fFinal_z = new TF1("fFinal_z","pol3");
      
    f1_Ex = new TF1("f1_Ex","pol6");
    f2_Ex = new TF1("f2_Ex","pol6");
    f3_Ex = new TF1("f3_Ex","pol6");
    f4_Ex = new TF1("f4_Ex","pol6");
    f5_Ex = new TF1("f5_Ex","pol6");
    fFinal_Ex = new TF1("fFinal_Ex","pol4");
      
    f1_Ey = new TF1("f1_Ey","pol5");
    f2_Ey = new TF1("f2_Ey","pol5");
    f3_Ey = new TF1("f3_Ey","pol5");
    f4_Ey = new TF1("f4_Ey","pol5");
    f5_Ey = new TF1("f5_Ey","pol5");
    f6_Ey = new TF1("f6_Ey","pol5");
    fFinal_Ey = new TF1("fFinal_Ey","pol5");
      
    f1_Ez = new TF1("f1_Ez","pol4");
    f2_Ez = new TF1("f2_Ez","pol4");
    f3_Ez = new TF1("f3_Ez","pol4");
    f4_Ez = new TF1("f4_Ez","pol4");
    fFinal_Ez = new TF1("fFinal_Ez","pol3");

    inputfile = nullptr;

    for(int i = 0; i < 7; i++) {
      g1_x[i] =  nullptr;
      g2_x[i] =  nullptr;
      g3_x[i] =  nullptr;
      g4_x[i] =  nullptr;
      g5_x[i] =  nullptr;

      g1_y[i] =  nullptr;
      g2_y[i] =  nullptr;
      g3_y[i] =  nullptr;
      g4_y[i] =  nullptr;
      g5_y[i] =  nullptr;
      g6_y[i] =  nullptr;

      g1_z[i] =  nullptr;
      g2_z[i] =  nullptr;
      g3_z[i] =  nullptr;
      g4_z[i] =  nullptr;

      g1_Ex[i] = nullptr;
      g2_Ex[i] = nullptr;
      g3_Ex[i] = nullptr;
      g4_Ex[i] = nullptr;
      g5_Ex[i] = nullptr;

      g1_Ey[i] = nullptr;
      g2_Ey[i] = nullptr;
      g3_Ey[i] = nullptr;
      g4_Ey[i] = nullptr;
      g5_Ey[i] = nullptr;
      g6_Ey[i] = nullptr;

      g1_Ez[i] = nullptr;
      g2_Ez[i] = nullptr;
      g3_Ez[i] = nullptr;
      g4_Ez[i] = nullptr;
    }
    std::string sce_file_path = Form("%s/LArUtil/dat/SCEoffsets_MicroBooNE_E273.root",getenv("LARLITE_COREDIR"));
    Configure(sce_file_path);
  }

  //-----------------------------------------------
  SpaceChargeMicroBooNEMCC8::SpaceChargeMicroBooNEMCC8(std::string filename)
  {
    Configure(filename);
  }

  //-----------------------------------------------
  SpaceChargeMicroBooNEMCC8::~SpaceChargeMicroBooNEMCC8()
  {
    inputfile->Close();
  }

  //------------------------------------------------
  bool SpaceChargeMicroBooNEMCC8::Configure(std::string filename)
  {  
    //TFile inputfile(filename.c_str(), "READ");
    inputfile = new TFile(filename.c_str(), "READ");

    // std::cout << "@filename=" << filename << std::endl;
    // std::cout << "@filename=" << filename << std::endl;
    // std::cout << "@filename=" << filename << std::endl;
    // std::cout << "@filename=" << filename << std::endl;
    // for(int i = 0; i < 5; i++)
    //   {
    g1_x[0] = (TGraph*)inputfile->Get("deltaX/g1_0");
    g2_x[0] = (TGraph*)inputfile->Get("deltaX/g2_0");
    g3_x[0] = (TGraph*)inputfile->Get("deltaX/g3_0");
    g4_x[0] = (TGraph*)inputfile->Get("deltaX/g4_0");
    g5_x[0] = (TGraph*)inputfile->Get("deltaX/g5_0");

    g1_y[0] = (TGraph*)inputfile->Get("deltaY/g1_0");
    g2_y[0] = (TGraph*)inputfile->Get("deltaY/g2_0");
    g3_y[0] = (TGraph*)inputfile->Get("deltaY/g3_0");
    g4_y[0] = (TGraph*)inputfile->Get("deltaY/g4_0");
    g5_y[0] = (TGraph*)inputfile->Get("deltaY/g5_0");
    g6_y[0] = (TGraph*)inputfile->Get("deltaY/g6_0");

    g1_z[0] = (TGraph*)inputfile->Get("deltaZ/g1_0");
    g2_z[0] = (TGraph*)inputfile->Get("deltaZ/g2_0");
    g3_z[0] = (TGraph*)inputfile->Get("deltaZ/g3_0");
    g4_z[0] = (TGraph*)inputfile->Get("deltaZ/g4_0");

    g1_Ex[0] = (TGraph*)inputfile->Get("deltaExOverE/g1_0");
    g2_Ex[0] = (TGraph*)inputfile->Get("deltaExOverE/g2_0");
    g3_Ex[0] = (TGraph*)inputfile->Get("deltaExOverE/g3_0");
    g4_Ex[0] = (TGraph*)inputfile->Get("deltaExOverE/g4_0");
    g5_Ex[0] = (TGraph*)inputfile->Get("deltaExOverE/g5_0");

    g1_Ey[0] = (TGraph*)inputfile->Get("deltaEyOverE/g1_0");
    g2_Ey[0] = (TGraph*)inputfile->Get("deltaEyOverE/g2_0");
    g3_Ey[0] = (TGraph*)inputfile->Get("deltaEyOverE/g3_0");
    g4_Ey[0] = (TGraph*)inputfile->Get("deltaEyOverE/g4_0");
    g5_Ey[0] = (TGraph*)inputfile->Get("deltaEyOverE/g5_0");
    g6_Ey[0] = (TGraph*)inputfile->Get("deltaEyOverE/g6_0");

    g1_Ez[0] = (TGraph*)inputfile->Get("deltaEzOverE/g1_0");
    g2_Ez[0] = (TGraph*)inputfile->Get("deltaEzOverE/g2_0");
    g3_Ez[0] = (TGraph*)inputfile->Get("deltaEzOverE/g3_0");
    g4_Ez[0] = (TGraph*)inputfile->Get("deltaEzOverE/g4_0");
    // }

    // for(int i = 0; i < 5; i++)
    //   {
    g1_x[1] = (TGraph*)inputfile->Get("deltaX/g1_1");
    g2_x[1] = (TGraph*)inputfile->Get("deltaX/g2_1");
    g3_x[1] = (TGraph*)inputfile->Get("deltaX/g3_1");
    g4_x[1] = (TGraph*)inputfile->Get("deltaX/g4_1");
    g5_x[1] = (TGraph*)inputfile->Get("deltaX/g5_1");

    g1_y[1] = (TGraph*)inputfile->Get("deltaY/g1_1");
    g2_y[1] = (TGraph*)inputfile->Get("deltaY/g2_1");
    g3_y[1] = (TGraph*)inputfile->Get("deltaY/g3_1");
    g4_y[1] = (TGraph*)inputfile->Get("deltaY/g4_1");
    g5_y[1] = (TGraph*)inputfile->Get("deltaY/g5_1");
    g6_y[1] = (TGraph*)inputfile->Get("deltaY/g6_1");

    g1_z[1] = (TGraph*)inputfile->Get("deltaZ/g1_1");
    g2_z[1] = (TGraph*)inputfile->Get("deltaZ/g2_1");
    g3_z[1] = (TGraph*)inputfile->Get("deltaZ/g3_1");
    g4_z[1] = (TGraph*)inputfile->Get("deltaZ/g4_1");

    g1_Ex[1] = (TGraph*)inputfile->Get("deltaExOverE/g1_1");
    g2_Ex[1] = (TGraph*)inputfile->Get("deltaExOverE/g2_1");
    g3_Ex[1] = (TGraph*)inputfile->Get("deltaExOverE/g3_1");
    g4_Ex[1] = (TGraph*)inputfile->Get("deltaExOverE/g4_1");
    g5_Ex[1] = (TGraph*)inputfile->Get("deltaExOverE/g5_1");

    g1_Ey[1] = (TGraph*)inputfile->Get("deltaEyOverE/g1_1");
    g2_Ey[1] = (TGraph*)inputfile->Get("deltaEyOverE/g2_1");
    g3_Ey[1] = (TGraph*)inputfile->Get("deltaEyOverE/g3_1");
    g4_Ey[1] = (TGraph*)inputfile->Get("deltaEyOverE/g4_1");
    g5_Ey[1] = (TGraph*)inputfile->Get("deltaEyOverE/g5_1");
    g6_Ey[1] = (TGraph*)inputfile->Get("deltaEyOverE/g6_1");

    g1_Ez[1] = (TGraph*)inputfile->Get("deltaEzOverE/g1_1");
    g2_Ez[1] = (TGraph*)inputfile->Get("deltaEzOverE/g2_1");
    g3_Ez[1] = (TGraph*)inputfile->Get("deltaEzOverE/g3_1");
    g4_Ez[1] = (TGraph*)inputfile->Get("deltaEzOverE/g4_1");
    // }

    // for(int i = 0; i < 5; i++)
    //   {
    g1_x[2] = (TGraph*)inputfile->Get("deltaX/g1_2");
    g2_x[2] = (TGraph*)inputfile->Get("deltaX/g2_2");
    g3_x[2] = (TGraph*)inputfile->Get("deltaX/g3_2");
    g4_x[2] = (TGraph*)inputfile->Get("deltaX/g4_2");
    g5_x[2] = (TGraph*)inputfile->Get("deltaX/g5_2");

    g1_y[2] = (TGraph*)inputfile->Get("deltaY/g1_2");
    g2_y[2] = (TGraph*)inputfile->Get("deltaY/g2_2");
    g3_y[2] = (TGraph*)inputfile->Get("deltaY/g3_2");
    g4_y[2] = (TGraph*)inputfile->Get("deltaY/g4_2");
    g5_y[2] = (TGraph*)inputfile->Get("deltaY/g5_2");
    g6_y[2] = (TGraph*)inputfile->Get("deltaY/g6_2");

    g1_z[2] = (TGraph*)inputfile->Get("deltaZ/g1_2");
    g2_z[2] = (TGraph*)inputfile->Get("deltaZ/g2_2");
    g3_z[2] = (TGraph*)inputfile->Get("deltaZ/g3_2");
    g4_z[2] = (TGraph*)inputfile->Get("deltaZ/g4_2");

    g1_Ex[2] = (TGraph*)inputfile->Get("deltaExOverE/g1_2");
    g2_Ex[2] = (TGraph*)inputfile->Get("deltaExOverE/g2_2");
    g3_Ex[2] = (TGraph*)inputfile->Get("deltaExOverE/g3_2");
    g4_Ex[2] = (TGraph*)inputfile->Get("deltaExOverE/g4_2");
    g5_Ex[2] = (TGraph*)inputfile->Get("deltaExOverE/g5_2");

    g1_Ey[2] = (TGraph*)inputfile->Get("deltaEyOverE/g1_2");
    g2_Ey[2] = (TGraph*)inputfile->Get("deltaEyOverE/g2_2");
    g3_Ey[2] = (TGraph*)inputfile->Get("deltaEyOverE/g3_2");
    g4_Ey[2] = (TGraph*)inputfile->Get("deltaEyOverE/g4_2");
    g5_Ey[2] = (TGraph*)inputfile->Get("deltaEyOverE/g5_2");
    g6_Ey[2] = (TGraph*)inputfile->Get("deltaEyOverE/g6_2");

    g1_Ez[2] = (TGraph*)inputfile->Get("deltaEzOverE/g1_2");
    g2_Ez[2] = (TGraph*)inputfile->Get("deltaEzOverE/g2_2");
    g3_Ez[2] = (TGraph*)inputfile->Get("deltaEzOverE/g3_2");
    g4_Ez[2] = (TGraph*)inputfile->Get("deltaEzOverE/g4_2");
    // }
    // for(int i = 0; i < 5; i++)
    //   {
    g1_x[3] = (TGraph*)inputfile->Get("deltaX/g1_3");
    g2_x[3] = (TGraph*)inputfile->Get("deltaX/g2_3");
    g3_x[3] = (TGraph*)inputfile->Get("deltaX/g3_3");
    g4_x[3] = (TGraph*)inputfile->Get("deltaX/g4_3");
    g5_x[3] = (TGraph*)inputfile->Get("deltaX/g5_3");

    g1_y[3] = (TGraph*)inputfile->Get("deltaY/g1_3");
    g2_y[3] = (TGraph*)inputfile->Get("deltaY/g2_3");
    g3_y[3] = (TGraph*)inputfile->Get("deltaY/g3_3");
    g4_y[3] = (TGraph*)inputfile->Get("deltaY/g4_3");
    g5_y[3] = (TGraph*)inputfile->Get("deltaY/g5_3");
    g6_y[3] = (TGraph*)inputfile->Get("deltaY/g6_3");

    g1_z[3] = (TGraph*)inputfile->Get("deltaZ/g1_3");
    g2_z[3] = (TGraph*)inputfile->Get("deltaZ/g2_3");
    g3_z[3] = (TGraph*)inputfile->Get("deltaZ/g3_3");
    g4_z[3] = (TGraph*)inputfile->Get("deltaZ/g4_3");

    g1_Ex[3] = (TGraph*)inputfile->Get("deltaExOverE/g1_3");
    g2_Ex[3] = (TGraph*)inputfile->Get("deltaExOverE/g2_3");
    g3_Ex[3] = (TGraph*)inputfile->Get("deltaExOverE/g3_3");
    g4_Ex[3] = (TGraph*)inputfile->Get("deltaExOverE/g4_3");
    g5_Ex[3] = (TGraph*)inputfile->Get("deltaExOverE/g5_3");

    g1_Ey[3] = (TGraph*)inputfile->Get("deltaEyOverE/g1_3");
    g2_Ey[3] = (TGraph*)inputfile->Get("deltaEyOverE/g2_3");
    g3_Ey[3] = (TGraph*)inputfile->Get("deltaEyOverE/g3_3");
    g4_Ey[3] = (TGraph*)inputfile->Get("deltaEyOverE/g4_3");
    g5_Ey[3] = (TGraph*)inputfile->Get("deltaEyOverE/g5_3");
    g6_Ey[3] = (TGraph*)inputfile->Get("deltaEyOverE/g6_3");

    g1_Ez[3] = (TGraph*)inputfile->Get("deltaEzOverE/g1_3");
    g2_Ez[3] = (TGraph*)inputfile->Get("deltaEzOverE/g2_3");
    g3_Ez[3] = (TGraph*)inputfile->Get("deltaEzOverE/g3_3");
    g4_Ez[3] = (TGraph*)inputfile->Get("deltaEzOverE/g4_3");
    // }

    // for(int i = 0; i < 5; i++)
    //   {
    g1_x[4] = (TGraph*)inputfile->Get("deltaX/g1_4");
    g2_x[4] = (TGraph*)inputfile->Get("deltaX/g2_4");
    g3_x[4] = (TGraph*)inputfile->Get("deltaX/g3_4");
    g4_x[4] = (TGraph*)inputfile->Get("deltaX/g4_4");
    g5_x[4] = (TGraph*)inputfile->Get("deltaX/g5_4");

    g1_y[4] = (TGraph*)inputfile->Get("deltaY/g1_4");
    g2_y[4] = (TGraph*)inputfile->Get("deltaY/g2_4");
    g3_y[4] = (TGraph*)inputfile->Get("deltaY/g3_4");
    g4_y[4] = (TGraph*)inputfile->Get("deltaY/g4_4");
    g5_y[4] = (TGraph*)inputfile->Get("deltaY/g5_4");
    g6_y[4] = (TGraph*)inputfile->Get("deltaY/g6_4");

    g1_z[4] = (TGraph*)inputfile->Get("deltaZ/g1_4");
    g2_z[4] = (TGraph*)inputfile->Get("deltaZ/g2_4");
    g3_z[4] = (TGraph*)inputfile->Get("deltaZ/g3_4");
    g4_z[4] = (TGraph*)inputfile->Get("deltaZ/g4_4");

    g1_Ex[4] = (TGraph*)inputfile->Get("deltaExOverE/g1_4");
    g2_Ex[4] = (TGraph*)inputfile->Get("deltaExOverE/g2_4");
    g3_Ex[4] = (TGraph*)inputfile->Get("deltaExOverE/g3_4");
    g4_Ex[4] = (TGraph*)inputfile->Get("deltaExOverE/g4_4");
    g5_Ex[4] = (TGraph*)inputfile->Get("deltaExOverE/g5_4");

    g1_Ey[4] = (TGraph*)inputfile->Get("deltaEyOverE/g1_4");
    g2_Ey[4] = (TGraph*)inputfile->Get("deltaEyOverE/g2_4");
    g3_Ey[4] = (TGraph*)inputfile->Get("deltaEyOverE/g3_4");
    g4_Ey[4] = (TGraph*)inputfile->Get("deltaEyOverE/g4_4");
    g5_Ey[4] = (TGraph*)inputfile->Get("deltaEyOverE/g5_4");
    g6_Ey[4] = (TGraph*)inputfile->Get("deltaEyOverE/g6_4");

    g1_Ez[4] = (TGraph*)inputfile->Get("deltaEzOverE/g1_4");
    g2_Ez[4] = (TGraph*)inputfile->Get("deltaEzOverE/g2_4");
    g3_Ez[4] = (TGraph*)inputfile->Get("deltaEzOverE/g3_4");
    g4_Ez[4] = (TGraph*)inputfile->Get("deltaEzOverE/g4_4");
    // }

    g1_x[5] = (TGraph*)inputfile->Get("deltaX/g1_5");
    g2_x[5] = (TGraph*)inputfile->Get("deltaX/g2_5");
    g3_x[5] = (TGraph*)inputfile->Get("deltaX/g3_5");   
    g4_x[5] = (TGraph*)inputfile->Get("deltaX/g4_5");
    g5_x[5] = (TGraph*)inputfile->Get("deltaX/g5_5");

    g1_y[5] = (TGraph*)inputfile->Get("deltaY/g1_5");
    g2_y[5] = (TGraph*)inputfile->Get("deltaY/g2_5");
    g3_y[5] = (TGraph*)inputfile->Get("deltaY/g3_5");   
    g4_y[5] = (TGraph*)inputfile->Get("deltaY/g4_5");
    g5_y[5] = (TGraph*)inputfile->Get("deltaY/g5_5");
    g6_y[5] = (TGraph*)inputfile->Get("deltaY/g6_5");
      
    g1_x[6] = (TGraph*)inputfile->Get("deltaX/g1_6");
    g2_x[6] = (TGraph*)inputfile->Get("deltaX/g2_6");
    g3_x[6] = (TGraph*)inputfile->Get("deltaX/g3_6");
    //std::cout << g4_x[6] << std::endl;
    //std::cout << (TGraph*)inputfile->Get("deltaX/g4_6") << std::endl;
    g4_x[6] = (TGraph*)inputfile->Get("deltaX/g4_6");
    //std::cout << g4_x[6] << std::endl;
    g5_x[6] = (TGraph*)inputfile->Get("deltaX/g5_6");

    g1_Ex[5] = (TGraph*)inputfile->Get("deltaExOverE/g1_5");
    g2_Ex[5] = (TGraph*)inputfile->Get("deltaExOverE/g2_5");
    g3_Ex[5] = (TGraph*)inputfile->Get("deltaExOverE/g3_5");
    g4_Ex[5] = (TGraph*)inputfile->Get("deltaExOverE/g4_5");
    g5_Ex[5] = (TGraph*)inputfile->Get("deltaExOverE/g5_5");

    g1_Ey[5] = (TGraph*)inputfile->Get("deltaEyOverE/g1_5");
    g2_Ey[5] = (TGraph*)inputfile->Get("deltaEyOverE/g2_5");
    g3_Ey[5] = (TGraph*)inputfile->Get("deltaEyOverE/g3_5");
    g4_Ey[5] = (TGraph*)inputfile->Get("deltaEyOverE/g4_5");
    g5_Ey[5] = (TGraph*)inputfile->Get("deltaEyOverE/g5_5");
    g6_Ey[5] = (TGraph*)inputfile->Get("deltaEyOverE/g6_5");

    g1_Ex[6] = (TGraph*)inputfile->Get("deltaExOverE/g1_6");
    g2_Ex[6] = (TGraph*)inputfile->Get("deltaExOverE/g2_6");
    g3_Ex[6] = (TGraph*)inputfile->Get("deltaExOverE/g3_6");
    g4_Ex[6] = (TGraph*)inputfile->Get("deltaExOverE/g4_6");
    g5_Ex[6] = (TGraph*)inputfile->Get("deltaExOverE/g5_6");

    //inputfile.Close();

    return true;
  }

  //----------------------------------------------------------------------------
  /// Primary working method of service that provides position offsets to be
  /// used in ionization electron drift
  std::vector<double> SpaceChargeMicroBooNEMCC8::GetPosOffsets(double xVal, double yVal, double zVal) const
  {
    std::vector<double> thePosOffsets;

    if(IsInsideBoundaries(xVal,yVal,zVal) == false)
      {
	thePosOffsets.resize(3,0.0);
      }
    else
      {
	thePosOffsets = GetPosOffsetsParametric(xVal,yVal,zVal);
      }

    return thePosOffsets;
  }

  //----------------------------------------------------------------------------
  /// Provides position offsets using a parametric representation
  std::vector<double> SpaceChargeMicroBooNEMCC8::GetPosOffsetsParametric(double xVal, double yVal, double zVal) const
  {
    std::vector<double> thePosOffsetsParametric;

    double xValNew = TransformX(xVal);
    double yValNew = TransformY(yVal);
    double zValNew = TransformZ(zVal);

    thePosOffsetsParametric.push_back(GetOnePosOffsetParametric(xValNew,yValNew,zValNew,"X"));
    thePosOffsetsParametric.push_back(GetOnePosOffsetParametric(xValNew,yValNew,zValNew,"Y"));
    thePosOffsetsParametric.push_back(GetOnePosOffsetParametric(xValNew,yValNew,zValNew,"Z"));

    return thePosOffsetsParametric;
  }

  //----------------------------------------------------------------------------
  /// Provides one position offset using a parametric representation, for a given
  /// axis
  double SpaceChargeMicroBooNEMCC8::GetOnePosOffsetParametric(double xValNew, double yValNew, double zValNew, std::string axis) const
  {      
    double parA[6][7];
    double parB[6];
  
    for(int j = 0; j < 6; j++)
      {
	for(int i = 0; i < 7; i++)
	  parA[j][i] = 0.0;
  
	parB[j] = 0.0;
      }
  
    if(axis == "X")
      {
	for(int j = 0; j < 7; j++)
	  {
	    //std::cout << "@j=" << j << " & zValNew=" << zValNew << std::endl;
	    //std::cout << g1_x[j] << std::endl;
	    fflush(stdout);
	    parA[0][j] = g1_x[j]->Eval(zValNew);
	    parA[1][j] = g2_x[j]->Eval(zValNew);
	    parA[2][j] = g3_x[j]->Eval(zValNew);
	    parA[3][j] = g4_x[j]->Eval(zValNew);
	    parA[4][j] = g5_x[j]->Eval(zValNew);
	  }
  
	f1_x->SetParameters(parA[0]);
	f2_x->SetParameters(parA[1]);
	f3_x->SetParameters(parA[2]);
	f4_x->SetParameters(parA[3]);
	f5_x->SetParameters(parA[4]);
      }
    else if(axis == "Y")
      {
	for(int j = 0; j < 6; j++)
	  {
	    parA[0][j] = g1_y[j]->Eval(zValNew);
	    parA[1][j] = g2_y[j]->Eval(zValNew);
	    parA[2][j] = g3_y[j]->Eval(zValNew);
	    parA[3][j] = g4_y[j]->Eval(zValNew);
	    parA[4][j] = g5_y[j]->Eval(zValNew);
	    parA[5][j] = g6_y[j]->Eval(zValNew);
	  }
  
	f1_y->SetParameters(parA[0]);
	f2_y->SetParameters(parA[1]);
	f3_y->SetParameters(parA[2]);
	f4_y->SetParameters(parA[3]);
	f5_y->SetParameters(parA[4]);
	f6_y->SetParameters(parA[5]);
      }
    else if(axis == "Z")
      {
	for(int j = 0; j < 5; j++)
	  {
	    parA[0][j] = g1_z[j]->Eval(zValNew);
	    parA[1][j] = g2_z[j]->Eval(zValNew);
	    parA[2][j] = g3_z[j]->Eval(zValNew);
	    parA[3][j] = g4_z[j]->Eval(zValNew);
	  }
  
	f1_z->SetParameters(parA[0]);
	f2_z->SetParameters(parA[1]);
	f3_z->SetParameters(parA[2]);
	f4_z->SetParameters(parA[3]);
      }
  
    double aValNew;
    double bValNew;
  
    if(axis == "Y")
      {
	aValNew = xValNew;
	bValNew = yValNew;
      }
    else
      {
	aValNew = yValNew;
	bValNew = xValNew;
      }
  
    double offsetValNew = 0.0;
    if(axis == "X")
      {
	parB[0] = f1_x->Eval(aValNew);
	parB[1] = f2_x->Eval(aValNew);
	parB[2] = f3_x->Eval(aValNew);
	parB[3] = f4_x->Eval(aValNew);
	parB[4] = f5_x->Eval(aValNew);
  
	fFinal_x->SetParameters(parB);
	offsetValNew = 100.0*fFinal_x->Eval(bValNew);
      }
    else if(axis == "Y")
      {
	parB[0] = f1_y->Eval(aValNew);
	parB[1] = f2_y->Eval(aValNew);
	parB[2] = f3_y->Eval(aValNew);
	parB[3] = f4_y->Eval(aValNew);
	parB[4] = f5_y->Eval(aValNew);
	parB[5] = f6_y->Eval(aValNew);
  
	fFinal_y->SetParameters(parB);
	offsetValNew = 100.0*fFinal_y->Eval(bValNew);
      }
    else if(axis == "Z")
      {
	parB[0] = f1_z->Eval(aValNew);
	parB[1] = f2_z->Eval(aValNew);
	parB[2] = f3_z->Eval(aValNew);
	parB[3] = f4_z->Eval(aValNew);
  
	fFinal_z->SetParameters(parB);
	offsetValNew = 100.0*fFinal_z->Eval(bValNew);
      }
  
    return offsetValNew;
  }

  //----------------------------------------------------------------------------
  /// Primary working method of service that provides E field offsets to be
  /// used in charge/light yield calculation (e.g.)
  std::vector<double> SpaceChargeMicroBooNEMCC8::GetEfieldOffsets(double xVal, double yVal, double zVal) const
  {
    std::vector<double> theEfieldOffsets;

    if(IsInsideBoundaries(xVal,yVal,zVal) == false)
      {
	theEfieldOffsets.resize(3,0.0);
      }
    else
      {
	theEfieldOffsets = GetEfieldOffsetsParametric(xVal,yVal,zVal);
      }

    theEfieldOffsets.at(0) = -1.0*theEfieldOffsets.at(0);
    theEfieldOffsets.at(1) = -1.0*theEfieldOffsets.at(1);
    theEfieldOffsets.at(2) = -1.0*theEfieldOffsets.at(2);

    return theEfieldOffsets;
  }

  //----------------------------------------------------------------------------
  /// Provides E field offsets using a parametric representation
  std::vector<double> SpaceChargeMicroBooNEMCC8::GetEfieldOffsetsParametric(double xVal, double yVal, double zVal) const
  {
    std::vector<double> theEfieldOffsetsParametric;

    double xValNew = TransformX(xVal);
    double yValNew = TransformY(yVal);
    double zValNew = TransformZ(zVal);

    theEfieldOffsetsParametric.push_back(GetOneEfieldOffsetParametric(xValNew,yValNew,zValNew,"X"));
    theEfieldOffsetsParametric.push_back(GetOneEfieldOffsetParametric(xValNew,yValNew,zValNew,"Y"));
    theEfieldOffsetsParametric.push_back(GetOneEfieldOffsetParametric(xValNew,yValNew,zValNew,"Z"));

    return theEfieldOffsetsParametric;
  }

  //----------------------------------------------------------------------------
  /// Provides one E field offset using a parametric representation, for a given
  /// axis
  double SpaceChargeMicroBooNEMCC8::GetOneEfieldOffsetParametric(double xValNew, double yValNew, double zValNew, std::string axis) const
  {      
    double parA[6][7];
    double parB[6];
  
    for(int j = 0; j < 6; j++)
      {
	for(int i = 0; i < 7; i++)
	  parA[j][i] = 0.0;
  
	parB[j] = 0.0;
      }
  
    if(axis == "X")
      {
	for(int j = 0; j < 7; j++)
	  {
	    parA[0][j] = g1_Ex[j]->Eval(zValNew);
	    parA[1][j] = g2_Ex[j]->Eval(zValNew);
	    parA[2][j] = g3_Ex[j]->Eval(zValNew);
	    parA[3][j] = g4_Ex[j]->Eval(zValNew);
	    parA[4][j] = g5_Ex[j]->Eval(zValNew);
	  }
  
	f1_Ex->SetParameters(parA[0]);
	f2_Ex->SetParameters(parA[1]);
	f3_Ex->SetParameters(parA[2]);
	f4_Ex->SetParameters(parA[3]);
	f5_Ex->SetParameters(parA[4]);
      }
    else if(axis == "Y")
      {
	for(int j = 0; j < 6; j++)
	  {
	    parA[0][j] = g1_Ey[j]->Eval(zValNew);
	    parA[1][j] = g2_Ey[j]->Eval(zValNew);
	    parA[2][j] = g3_Ey[j]->Eval(zValNew);
	    parA[3][j] = g4_Ey[j]->Eval(zValNew);
	    parA[4][j] = g5_Ey[j]->Eval(zValNew);
	    parA[5][j] = g6_Ey[j]->Eval(zValNew);
	  }
  
	f1_Ey->SetParameters(parA[0]);
	f2_Ey->SetParameters(parA[1]);
	f3_Ey->SetParameters(parA[2]);
	f4_Ey->SetParameters(parA[3]);
	f5_Ey->SetParameters(parA[4]);
	f6_Ey->SetParameters(parA[5]);
      }
    else if(axis == "Z")
      {
	for(int j = 0; j < 5; j++)
	  {
	    parA[0][j] = g1_Ez[j]->Eval(zValNew);
	    parA[1][j] = g2_Ez[j]->Eval(zValNew);
	    parA[2][j] = g3_Ez[j]->Eval(zValNew);
	    parA[3][j] = g4_Ez[j]->Eval(zValNew);
	  }
  
	f1_Ez->SetParameters(parA[0]);
	f2_Ez->SetParameters(parA[1]);
	f3_Ez->SetParameters(parA[2]);
	f4_Ez->SetParameters(parA[3]);
      }
  
    double aValNew;
    double bValNew;
  
    if(axis == "Y")
      {
	aValNew = xValNew;
	bValNew = yValNew;
      }
    else
      {
	aValNew = yValNew;
	bValNew = xValNew;
      }
  
    double offsetValNew = 0.0;
    if(axis == "X")
      {
	parB[0] = f1_Ex->Eval(aValNew);
	parB[1] = f2_Ex->Eval(aValNew);
	parB[2] = f3_Ex->Eval(aValNew);
	parB[3] = f4_Ex->Eval(aValNew);
	parB[4] = f5_Ex->Eval(aValNew);
  
	fFinal_Ex->SetParameters(parB);
	offsetValNew = fFinal_Ex->Eval(bValNew);
      }
    else if(axis == "Y")
      {
	parB[0] = f1_Ey->Eval(aValNew);
	parB[1] = f2_Ey->Eval(aValNew);
	parB[2] = f3_Ey->Eval(aValNew);
	parB[3] = f4_Ey->Eval(aValNew);
	parB[4] = f5_Ey->Eval(aValNew);
	parB[5] = f6_Ey->Eval(aValNew);
  
	fFinal_Ey->SetParameters(parB);
	offsetValNew = fFinal_Ey->Eval(bValNew);
      }
    else if(axis == "Z")
      {
	parB[0] = f1_Ez->Eval(aValNew);
	parB[1] = f2_Ez->Eval(aValNew);
	parB[2] = f3_Ez->Eval(aValNew);
	parB[3] = f4_Ez->Eval(aValNew);
  
	fFinal_Ez->SetParameters(parB);
	offsetValNew = fFinal_Ez->Eval(bValNew);
      }
  
    return offsetValNew;
  }

  //----------------------------------------------------------------------------
  /// Transform X to SCE X coordinate:  [2.56,0.0] --> [0.0,2.50]
  double SpaceChargeMicroBooNEMCC8::TransformX(double xVal) const
  {
    double xValNew;
    xValNew = 2.50 - (2.50/2.56)*(xVal/100.0);
    xValNew -= 1.25;

    return xValNew;
  }

  //----------------------------------------------------------------------------
  /// Transform Y to SCE Y coordinate:  [-1.165,1.165] --> [0.0,2.50]
  double SpaceChargeMicroBooNEMCC8::TransformY(double yVal) const
  {
    double yValNew;
    yValNew = (2.50/2.33)*((yVal/100.0)+1.165);
    yValNew -= 1.25;

    return yValNew;
  }

  //----------------------------------------------------------------------------
  /// Transform Z to SCE Z coordinate:  [0.0,10.37] --> [0.0,10.0]
  double SpaceChargeMicroBooNEMCC8::TransformZ(double zVal) const
  {
    double zValNew;
    zValNew = (10.0/10.37)*(zVal/100.0);

    return zValNew;
  }

  //----------------------------------------------------------------------------
  /// Check to see if point is inside boundaries of map (allow to go slightly out of range)
  bool SpaceChargeMicroBooNEMCC8::IsInsideBoundaries(double xVal, double yVal, double zVal) const
  {
    bool isInside = true;

    if((xVal < 0.0) || (xVal > 260.0) || (yVal < -120.0) || (yVal > 120.0) || (zVal < 0.0) || (zVal > 1050.0))
      {
	isInside = false;
      }

    return isInside;
  }
  
}

#endif
