#ifndef LARLITE_ENDPOINT2D_CXX
#define LARLITE_ENDPOINT2D_CXX

#include "endpoint2d.h"

namespace larlite{

  //************************************************************
  endpoint2d::endpoint2d(Double_t    driftTime,
			 UInt_t      wire,
			 Double_t    strength,
			 Int_t       id,
			 geo::View_t view,
			 Double_t    totalQ)
    : data_base(data::kEndPoint2D),
      fDriftTime(driftTime),
      fWire(wire),
      fID(id),
      fStrength(strength),
      fView(view),
      fTotalCharge(totalQ)
  //************************************************************
  {}

  //************************************************************
  void endpoint2d::clear_data()
  //************************************************************
  {
    data_base::clear_data();
    fDriftTime=data::kINVALID_DOUBLE;
    fWire=data::kINVALID_UINT;
    fStrength=data::kINVALID_DOUBLE;
    fID=data::kINVALID_INT;
    fView=geo::kUnknown;
    fTotalCharge=data::kINVALID_DOUBLE;
  }

}

#endif
