#ifndef __SPACECHARGEMICROBOONE_MCC9_CXX__
#define __SPACECHARGEMICROBOONE_MCC9_CXX__
////////////////////////////////////////////////////////////////////////
// \file SpaceChargeMicroBooNEMCC9.C
//
// \brief implementation of class for storing/accessing space charge distortions for MicroBooNE
//
// \author mrmooney@bnl.gov
// 
////////////////////////////////////////////////////////////////////////

// C++ language includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "math.h"
#include "stdio.h"

#include "TFile.h"

// LArSoft includes
#include "SpaceChargeMicroBooNEMCC9.h"

namespace larutil{
  
  //-----------------------------------------------
  SpaceChargeMicroBooNEMCC9::SpaceChargeMicroBooNEMCC9()
  {
    std::string sce_file_path = Form("%s/LArUtil/dat/SCEoffsets_dataDriven_combined_fwd_Jan18.root",getenv("LARLITE_COREDIR"));
    Configure(sce_file_path);
  }

  //-----------------------------------------------
  SpaceChargeMicroBooNEMCC9::SpaceChargeMicroBooNEMCC9(std::string filename)
  {
    Configure(filename);
  }

  //-----------------------------------------------
  SpaceChargeMicroBooNEMCC9::~SpaceChargeMicroBooNEMCC9()
  {
    _inputfile->Close();
  }

  //------------------------------------------------
  bool SpaceChargeMicroBooNEMCC9::Configure(std::string filename)
  {  
    _inputfile = new TFile(filename.c_str(), "READ");
    _hD[0] = (TH3F*)_inputfile->Get("hDx");
    _hD[1] = (TH3F*)_inputfile->Get("hDy");
    _hD[2] = (TH3F*)_inputfile->Get("hDz");
    _hE[0] = (TH3F*)_inputfile->Get("hEx");
    _hE[1] = (TH3F*)_inputfile->Get("hEy");
    _hE[2] = (TH3F*)_inputfile->Get("hEz");

    _Xmin = _hD[0]->GetXaxis()->GetBinCenter(1);
    _Xmax = _hD[0]->GetXaxis()->GetBinCenter( _hD[0]->GetXaxis()->GetNbins() );
    _Ymin = _hD[0]->GetYaxis()->GetBinCenter(1);
    _Ymax = _hD[0]->GetYaxis()->GetBinCenter( _hD[0]->GetYaxis()->GetNbins() );
    _Zmin = _hD[0]->GetZaxis()->GetBinCenter(1);
    _Zmax = _hD[0]->GetZaxis()->GetBinCenter( _hD[0]->GetZaxis()->GetNbins() );
    
    return true;
  }

  //----------------------------------------------------------------------------
  /// Primary working method of service that provides position offsets to be
  /// used in ionization electron drift
  std::vector<double> SpaceChargeMicroBooNEMCC9::GetPosOffsets(double xVal, double yVal, double zVal) const
  {
    std::vector<double> thePosOffsets(3,0);

    if( IsInsideBoundaries(xVal,yVal,zVal) ) {
      
      double xValNew = TransformX(xVal);
      double yValNew = TransformY(yVal);
      double zValNew = TransformZ(zVal);
      // std::cout << "[" <<  __FILE__ <<  ":L" << __LINE__ << "] getting offset for "
      //           << "input=(" << xVal << "," << yVal << "," << zVal << ") "
      //           << "transformed=(" << xValNew << "," << yValNew << "," << zValNew << ") "
      //           << std::endl;
      
      for (int i=0; i<3; i++ )  {
        try {
          thePosOffsets[i] = _hD[i]->Interpolate( xValNew, yValNew, zValNew );
        }
        catch ( std::exception& e ) {
          std::stringstream ss;
          ss << "[" <<  __FILE__ <<  ":L" << __LINE__ << "] error getting offset for "
             << "input=(" << xVal << "," << yVal << "," << zVal << ") "
             << "transformed=(" << xValNew << "," << yValNew << "," << zValNew << ") "
             << "bounds "
             << " x=[" << _hD[i]->GetXaxis()->GetXmin() << "," << _hD[i]->GetXaxis()->GetXmax() << "] "
             << " y=[" << _hD[i]->GetYaxis()->GetXmin() << "," << _hD[i]->GetYaxis()->GetXmax() << "] "
             << " z=[" << _hD[i]->GetZaxis()->GetXmin() << "," << _hD[i]->GetZaxis()->GetXmax() << "] "
             << " error=" << e.what()
             << std::endl;
          throw std::runtime_error( ss.str() );
        }

        // std::cout << "bounds "
        //           << " x=[" << _hD[i]->GetXaxis()->GetXmin() << "," << _hD[i]->GetXaxis()->GetXmax() << "] "
        //           << " y=[" << _hD[i]->GetYaxis()->GetXmin() << "," << _hD[i]->GetYaxis()->GetXmax() << "] "
        //           << " z=[" << _hD[i]->GetZaxis()->GetXmin() << "," << _hD[i]->GetZaxis()->GetXmax() << "] "
        //           << std::endl;

      }
      
    }

    return thePosOffsets;
  }

  //----------------------------------------------------------------------------
  /// Primary working method of service that provides E field offsets to be
  /// used in charge/light yield calculation (e.g.)
  std::vector<double> SpaceChargeMicroBooNEMCC9::GetEfieldOffsets(double xVal, double yVal, double zVal) const
  {
    
    std::vector<double> theEfieldOffsets(3,0);

    if(IsInsideBoundaries(xVal,yVal,zVal)) {

      double xValNew = TransformX(xVal);
      double yValNew = TransformY(yVal);
      double zValNew = TransformZ(zVal);      
      
      for (int i=0; i<3; i++)
        theEfieldOffsets[i] = -1.0*_hE[i]->Interpolate( xValNew, yValNew, zValNew );
    }

    return theEfieldOffsets;
  }

  //----------------------------------------------------------------------------
  /// Transform X to SCE X coordinate:  [2.56,0.0] --> [0.0,2.50]
  double SpaceChargeMicroBooNEMCC9::TransformX(double xVal) const
  {
    double xValNew;
    xValNew = 2.50 - (2.50/2.56)*(xVal/100.0);
    //xValNew -= 1.25;

    return xValNew;
  }

  //----------------------------------------------------------------------------
  /// Transform Y to SCE Y coordinate:  [-1.165,1.165] --> [0.0,2.50]
  double SpaceChargeMicroBooNEMCC9::TransformY(double yVal) const
  {
    double yValNew;
    yValNew = (2.50/2.33)*((yVal/100.0)+1.165);
    //yValNew -= 1.25;
    return yValNew;
  }

  //----------------------------------------------------------------------------
  /// Transform Z to SCE Z coordinate:  [0.0,10.37] --> [0.0,10.0]
  double SpaceChargeMicroBooNEMCC9::TransformZ(double zVal) const
  {
    double zValNew;
    zValNew = (10.0/10.37)*(zVal/100.0);

    return zValNew;
  }

  //----------------------------------------------------------------------------
  /// Check to see if point is inside boundaries of map (allow to go slightly out of range)
  bool SpaceChargeMicroBooNEMCC9::IsInsideBoundaries(double xVal, double yVal, double zVal) const
  {
    bool isInside = true;
    double xValNew = TransformX(xVal);
    double yValNew = TransformY(yVal);
    double zValNew = TransformZ(zVal);      

    //if((xVal < 0.0) || (xVal > 260.0) || (yVal < -120.0) || (yVal > 120.0) || (zVal < 0.0) || (zVal > 1050.0))
    if( (   xValNew <= _Xmin || xValNew >= _Xmax )
        || (yValNew <= _Ymin || yValNew >= _Ymax )
        || (zValNew <= _Zmin || zValNew >= _Zmax ) )
      {
	isInside = false;
      }
    
    return isInside;
  }
  
}

#endif
