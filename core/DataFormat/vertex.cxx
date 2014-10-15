#ifndef VERTEX_CXX
#define VERTEX_CXX

#include "vertex.h"

namespace larlite {

  //************************************************************
  vertex::vertex(Double_t* xyz,
		 Int_t      id)
    : data_base(data::kVertex),
      fID(id)
  //************************************************************
  {
    fXYZ[0] = xyz[0];
    fXYZ[1] = xyz[1];
    fXYZ[2] = xyz[2];
  }

  //************************************************************
  void vertex::XYZ(Double_t *xyz) const
  //************************************************************
  {
    xyz[0] = fXYZ[0];
    xyz[1] = fXYZ[1];
    xyz[2] = fXYZ[2];
  }

  //************************************************************
  void vertex::clear_data()
  //************************************************************
  {
    data_base::clear_data();
    fXYZ[0] = data::kINVALID_DOUBLE;
    fXYZ[1] = data::kINVALID_DOUBLE;
    fXYZ[2] = data::kINVALID_DOUBLE;
    fID = data::kINVALID_INT;
  }

}

#endif
