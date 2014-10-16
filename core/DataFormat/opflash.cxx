////////////////////////////////////////////////////////////////////////
// \version $Id: OpFlash.cxx,v 1.7 2010/02/15 20:32:46 brebel Exp $
//
// \brief Definition of OpFlash reconstruction object
//
// \author bjpjones@mit.edu
//         cschiu@mit.edu
//
////////////////////////////////////////////////////////////////////////

#ifndef LARLITE_OPFLASH_CXX
#define LARLITE_OPFLASH_CXX

//#include "RecoBase/OpFlash.h"
//#include "messagefacility/MessageLogger/MessageLogger.h"
//#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "opflash.h"

namespace larlite {

  //----------------------------------------------------------------------
  opflash::opflash()  :
    data_base(data::kOpFlash),
    fTime(0.)
  {
  }

  //----------------------------------------------------------------------
  opflash::opflash(double time, double timewidth, double abstime, unsigned int frame,
		   std::vector<double> PEperOpDet, 
		   bool InBeamFrame, int onBeamTime, double FastToTotal,
		   double yCenter, double yWidth, 
		   double zCenter, double zWidth,
		   std::vector<double> WireCenters,
		   std::vector<double> WireWidths)
    : data_base(data::kOpFlash)
  {
    for (unsigned int i = 0; i < PEperOpDet.size(); ++i)
      fPEperOpDet.push_back(PEperOpDet[i]);
    fFrame       = frame;
    fAbsTime     = abstime;
    fTimeWidth   = timewidth;
    fTime        = time;
    fYCenter     = yCenter;
    fYWidth      = yWidth;
    fZCenter     = zCenter;
    fZWidth      = zWidth;
    fWireWidths  = WireWidths;
    fWireCenters = WireCenters;
    fInBeamFrame = InBeamFrame;
    fFastToTotal = FastToTotal;
    fOnBeamTime  = onBeamTime;
  }

  //----------------------------------------------------------------------
  bool operator < (const opflash & a, const opflash & b)
  {
    return a.Time() < b.Time();
  }

  //----------------------------------------------------------------------
  double opflash::TotalPE() const
  {
    double theTotalPE=0;
    for(size_t i=0; i!=fPEperOpDet.size(); ++i)
      theTotalPE+=fPEperOpDet.at(i);
    return theTotalPE;
  }

}

#endif
