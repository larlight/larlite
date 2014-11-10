#ifndef LARLITE_MCTRACK_CXX
#define LARLITE_MCTRACK_CXX

#include "mctrack.h"

namespace larlite {


  void mctrack::clear_data()
  {
    std::vector<mcstep>::clear();
    data_base::clear_data();

    fOrigin  = simb::kUnknown;
    fProcess = "";
    fPDGCode         = data::kINVALID_INT;
    fG4TrackID       = data::kINVALID_UINT;

    fMotherPDGCode   = data::kINVALID_INT;
    fMotherG4TrackID = data::kINVALID_UINT;

    fAncestorPDGCode   = data::kINVALID_INT;
    fAncestorG4TrackID = data::kINVALID_UINT;

    TLorentzVector invalid(data::kINVALID_DOUBLE,
			   data::kINVALID_DOUBLE,
			   data::kINVALID_DOUBLE,
			   data::kINVALID_DOUBLE);

    fG4Start.SetPosition(invalid);
    fG4Start.SetMomentum(invalid);
    fG4End.SetPosition(invalid);
    fG4End.SetMomentum(invalid);

    fMotherG4Start.SetPosition(invalid);
    fMotherG4Start.SetMomentum(invalid);
    fMotherG4End.SetPosition(invalid);
    fMotherG4End.SetMomentum(invalid);

    fAncestorG4Start.SetPosition(invalid);
    fAncestorG4Start.SetMomentum(invalid);
    fAncestorG4End.SetPosition(invalid);
    fAncestorG4End.SetMomentum(invalid);

  }

}
#endif
  
