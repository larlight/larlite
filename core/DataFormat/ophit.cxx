////////////////////////////////////////////////////////////////////////
// \version $Id: OpHit.cxx,v 1.7 2010/02/15 20:32:46 brebel Exp $
//
// \brief Definition of OpHit reconstruction object
//
// \author bjpjones@mit.edu
//         cschiu@mit.edu
//
////////////////////////////////////////////////////////////////////////
#ifndef OPHIT_CXX
#define OPHIT_CXX

//#include "RecoBase/OpHit.h"
//#include "messagefacility/MessageLogger/MessageLogger.h"

#include "ophit.h"

namespace larlite {

  //----------------------------------------------------------------------
  ophit::ophit()  
    : data_base(data::kOpHit)
    , fOpChannel     (-1)
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
  ophit::ophit(int opchannel, 
	       double peaktime, 
	       double peaktimeabs,
	       unsigned short frame, 
	       double width, 
	       double area, 
	       double amplitude, 
	       double pe,
	       double fasttototal)
    : data_base(data::kOpHit)
    , fOpChannel     (opchannel     )
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
  
  int    ophit::OpChannel()      const { return fOpChannel;      }
  unsigned short ophit::Frame()  const { return fFrame;          }
  double ophit::PeakTime()       const { return fPeakTime;       }
  double ophit::PeakTimeAbs()    const { return fPeakTimeAbs;    }
  double ophit::Width()          const { return fWidth;          }
  double ophit::Area()           const { return fArea;           }
  double ophit::Amplitude()      const { return fAmplitude;      }
  double ophit::PE()             const { return fPE;             }
  double ophit::FastToTotal()    const { return fFastToTotal;    }


  //----------------------------------------------------------------------
  bool operator < (const ophit & a, const ophit & b)
  {
    return a.PE() < b.PE();
  }

}
#endif
