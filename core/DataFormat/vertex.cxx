#ifndef LARLITE_VERTEX_CXX
#define LARLITE_VERTEX_CXX

#include "vertex.h"

namespace larlite {

  vertex::vertex()
    : data_base(data::kVertex)
  {clear_data();}

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

  int vertex::ID() const   { return fID;     }
  double vertex::X() const { return fXYZ[0]; }
  double vertex::Y() const { return fXYZ[1]; }
  double vertex::Z() const { return fXYZ[2]; }

  event_vertex::event_vertex(std::string name)
    : event_base(data::kVertex,name)
  {clear_data();}
}

#endif
