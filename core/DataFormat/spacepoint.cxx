#ifndef LARLITE_SPACEPOINT_CXX
#define LARLITE_SPACEPOINT_CXX

#include "spacepoint.h"

namespace larlite{

  spacepoint::spacepoint(int id, 
			 double x,  double y,  double z,
			 double ex, double ey, double ez,
			 double chi2) : data_base(data::kSpacePoint)
  {
    clear_data();
    fID=id;
    fXYZ[0]=x;
    fXYZ[1]=y;
    fXYZ[2]=z;
    fErrXYZ[0]=ex;
    fErrXYZ[1]=ey;
    fErrXYZ[2]=ez;
    fChisq=chi2;
  }

  spacepoint::spacepoint(const spacepoint& original) : data_base(original),
						       fID(original.fID),
						       fChisq(original.fChisq)
  {
    fXYZ[0]=original.fXYZ[0];
    fXYZ[1]=original.fXYZ[1];
    fXYZ[2]=original.fXYZ[2];
    fErrXYZ[0]=original.fErrXYZ[0];
    fErrXYZ[1]=original.fErrXYZ[1];
    fErrXYZ[2]=original.fErrXYZ[2];
  }
    
  int           spacepoint::ID()     const { return fID;     }
  const double* spacepoint::XYZ()    const { return fXYZ;    }
  const double* spacepoint::ErrXYZ() const { return fErrXYZ; }
  double        spacepoint::Chisq()  const { return fChisq;  }
    
  /// Method to clear data member
  void spacepoint::clear_data() {
    data_base::clear_data();
    fID = -1;
    fXYZ[0]=-99999;
    fXYZ[1]=-99999;
    fXYZ[2]=-99999;
    fErrXYZ[0]=-99999;
    fErrXYZ[1]=-99999;
    fErrXYZ[2]=-99999;
    fChisq=-1;
  }

}
#endif
  
