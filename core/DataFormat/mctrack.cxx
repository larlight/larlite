#ifndef LARLITE_MCTRACK_CXX
#define LARLITE_MCTRACK_CXX

#include "mctrack.h"

namespace larlite {


  void mctrack::clear_data()
  {
    std::vector<mcstep>::clear();
    data_base::clear_data();

    fPDGCode         = data::kINVALID_INT;
    fCreationProcess = "";
    fAncestorPDGCode = data::kINVALID_INT;

    TLorentzVector invalid(data::kINVALID_DOUBLE,
			   data::kINVALID_DOUBLE,
			   data::kINVALID_DOUBLE,
			   data::kINVALID_DOUBLE);

    fG4Start.SetPosition(invalid);
    fG4Start.SetMomentum(invalid);
    fG4End.SetPosition(invalid);
    fG4End.SetMomentum(invalid);
    fAncestorG4Start.SetPosition(invalid);
    fAncestorG4Start.SetMomentum(invalid);
  }

}
#endif
  
