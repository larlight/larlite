////////////////////////////////////////////////////////////////////////
// \version $Id: OpHit.cxx,v 1.7 2010/02/15 20:32:46 brebel Exp $
//
// \brief Definition of OpHit reconstruction object
//
// \author bjpjones@mit.edu
//         cschiu@mit.edu
//
////////////////////////////////////////////////////////////////////////

#include "RecoBase/OpHit.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

namespace recob{

  //----------------------------------------------------------------------
  OpHit::OpHit()  
    : fOpChannel     (-1)
    , fFrame         (0 )
    , fPeakTime      (0 )
    , fPeakTimeAbs   (0 )
    , fWidth         (0 )
    , fArea          (0 )
    , fAmplitude     (0.)
    , fPE            (0.)
    , fFastToTotal   (0.)
  {

  }

  //----------------------------------------------------------------------
  OpHit::OpHit(int opchannel, 
	       double peaktime, 
	       double peaktimeabs,
	       unsigned short frame, 
	       double width, 
	       double area, 
	       double amplitude, 
	       double pe,
	       double fasttototal)
    : fOpChannel     (opchannel     )
    , fFrame         (frame         )
    , fPeakTime      (peaktime      )
    , fPeakTimeAbs   (peaktimeabs   )
    , fWidth         (width         )
    , fArea          (area          )
    , fAmplitude     (amplitude     )
    , fPE            (pe            )
    , fFastToTotal   (fasttototal   ) 
  {
    
  }

  //----------------------------------------------------------------------
  bool operator < (const OpHit & a, const OpHit & b)
  {
    return a.PE() < b.PE();
  }

}
