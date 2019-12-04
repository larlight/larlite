#ifndef __SPACECHARGEMICROBOONE_CXX__
#define __SPACECHARGEMICROBOONE_CXX__
////////////////////////////////////////////////////////////////////////
// \file SpaceChargeMicroBooNE.C
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
#include "SpaceChargeMicroBooNE.h"

#include "SpaceChargeMicroBooNEMCC8.h"
#include "SpaceChargeMicroBooNEMCC9.h"

namespace larutil{
  

  //-----------------------------------------------
  SpaceChargeMicroBooNE::SpaceChargeMicroBooNE( Version_t version, std::string filename)
    : _version(version)
  {
    Configure(version,filename);    
  }

  //-----------------------------------------------
  SpaceChargeMicroBooNE::~SpaceChargeMicroBooNE() 
  {
    delete _sce;
    _sce = nullptr;
  }  
    
  //-----------------------------------------------
  bool SpaceChargeMicroBooNE::Configure( Version_t version, std::string filename)
  {
    _version = version;
    
    std::string sce_file_path;

    if ( filename!="" ) {
      // override default
      sce_file_path = filename;
    }
    else {
      switch( _version ) {
      case kMCC8_E273:
        sce_file_path = Form("%s/LArUtil/dat/SCEoffsets_MicroBooNE_E273.root",getenv("LARLITE_COREDIR"));
        break;
      case kMCC8_E227:
        sce_file_path = Form("%s/LArUtil/dat/SCEoffsets_MicroBooNE_E227.root",getenv("LARLITE_COREDIR"));
        break;
      case kMCC8_E500:
        sce_file_path = Form("%s/LArUtil/dat/SCEoffsets_MicroBooNE_E500.root",getenv("LARLITE_COREDIR"));
        break;
      case kMCC9_Backward:
        sce_file_path = Form("%s/LArUtil/dat/SCEoffsets_dataDriven_combined_bkwd_Jan18.root",getenv("LARLITE_COREDIR"));
        break;
      case kMCC9_Forward:
      default:
        sce_file_path = Form("%s/LArUtil/dat/SCEoffsets_dataDriven_combined_fwd_Jan18.root",getenv("LARLITE_COREDIR"));
        break;
      }
    }

    switch (_version) {
    case kMCC8_E273:
    case kMCC8_E227:
    case kMCC8_E500:
      _sce = new SpaceChargeMicroBooNEMCC8(sce_file_path);
      break;
    case kMCC9_Forward:
    case kMCC9_Backward:
    default:
      _sce = new SpaceChargeMicroBooNEMCC9(sce_file_path);
      break;
    }
    
    return true;
  }

  //----------------------------------------------------------------------------
  /// Primary working method of service that provides position offsets to be
  /// used in ionization electron drift
  std::vector<double> SpaceChargeMicroBooNE::GetPosOffsets(double xVal, double yVal, double zVal) const
  {
    return _sce->GetPosOffsets(xVal,yVal,zVal);
  }

  //----------------------------------------------------------------------------
  /// Primary working method of service that provides position offsets to be
  /// used in ionization electron drift
  std::vector<double> SpaceChargeMicroBooNE::GetEfieldOffsets(double xVal, double yVal, double zVal) const
  {
    return _sce->GetEfieldOffsets(xVal,yVal,zVal);
  }
  
  float SpaceChargeMicroBooNE::tickoffset_forward_hack( float tick ) const {
    return 6 + (tick-3200)*0.014;
  }

  float SpaceChargeMicroBooNE::tickoffset_backward_hack( float tick ) const {
    return (tick-6+3200*0.014)/1.014 - tick;
  }
  
}

#endif
