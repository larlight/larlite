////////////////////////////////////////////////////////////////////////////
// \version $Id: PCAxis.cxx,v 1.5 2010/02/15 20:32:46 usher Exp $
//
// \brief Definition of Principal Components Axis object for LArSoft
//
// \author usher@slac.stanford.edu
//
////////////////////////////////////////////////////////////////////////////

#include "pcaxis.h"

namespace larlite
{

  //----------------------------------------------------------------------
  pcaxis::pcaxis() :
    data_base(data::kPCAxis),
    fSvdOK(false),
    fNumHitsUsed(0),
    fAveHitDoca(0),
    fID(0)
  {
    for (int i=0; i<3; i++) {
      fEigenValues[i] = 0;
      fAvePosition[i] = 0;
    }
  }
  
  //----------------------------------------------------------------------
  pcaxis::pcaxis(bool                               ok,
		 int                                nHits,
		 const double*                      eigenValues,
		 const larlite::pcaxis::EigenVectors& eigenVecs,
		 const double*                      avePos,
		 const double                       aveHitDoca,
		 size_t                             id) :
    data_base(data::kPCAxis),
    fSvdOK(ok),
    fNumHitsUsed(nHits),
    fEigenVectors(eigenVecs),
    fAveHitDoca(aveHitDoca),
    fID(id)
  {
    fEigenValues[0]     = eigenValues[0];
    fEigenValues[1]     = eigenValues[1];
    fEigenValues[2]     = eigenValues[2];
    fAvePosition[0]     = avePos[0];
    fAvePosition[1]     = avePos[1];
    fAvePosition[2]     = avePos[2];
  }
  
  //----------------------------------------------------------------------------
  // comparison operator
  //
  bool operator < (const pcaxis& a, const pcaxis& b)
  {
    if(a.getID() != b. getID())
      return a.getID() < b.getID();
    
    return false; //They are equal
  }

} // End of namespace

