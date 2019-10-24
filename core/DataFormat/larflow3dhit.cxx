#ifndef LARLITE_LARFLOW3DHIT_CXX
#define LARLITE_LARFLOW3DHIT_CXX

#include "larflow3dhit.h"

namespace larlite {

  //#################################################
  larflow3dhit::larflow3dhit() : data_base(data::kLArFlow3DHit)
  //#################################################
  {
    this->resize(3,0);
    targetwire.resize(2,0);
    tick = 0;        // row
    srcwire = 0;     // column in source image
    idxhit = 0;
    matchquality = kNoMatch;
    consistency3d = kNoValue;
    truthflag = kNoTruthMatch;
    center_y_dist = 4000;
    dy = 0;
    dz = 0;
    endpt_score = 0;
    track_score = 0;
    shower_score = 0;
    renormed_track_score = 0;
    renormed_shower_score = 0;
    src_infill = 0;
    tar_infill.resize(2,0);
    X_truth.resize(3,0);
    trackid = 0;
    dWall = 0;
    
    clear_data();
  }

  //##########################################################################
  void larflow3dhit::clear_data()
  //##########################################################################
  {
    data_base::clear_data();
  }
  
}
#endif
  
