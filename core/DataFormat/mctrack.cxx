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
    fMotherProcess   = "";

    fAncestorPDGCode   = data::kINVALID_INT;
    fAncestorG4TrackID = data::kINVALID_UINT;
    fAncestorProcess   = "";

    TLorentzVector invalid(data::kINVALID_DOUBLE,
			   data::kINVALID_DOUBLE,
			   data::kINVALID_DOUBLE,
			   data::kINVALID_DOUBLE);

    mcstep invalid_step(invalid,invalid);

    fG4Start = invalid_step;
    fG4End   = invalid_step;

    fMotherG4Start = invalid_step;
    fMotherG4End   = invalid_step;
    
    fAncestorG4Start = invalid_step;
    fAncestorG4End   = invalid_step;

  }

}
#endif
  
