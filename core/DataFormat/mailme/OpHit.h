////////////////////////////////////////////////////////////////////////////
// \version $Id: OpHit.h,v 1.7 2010/03/01 21:32:44 bpage Exp $
//
// \brief Definition of OpHit object for LArSoft
//
// \author bjpjones@mit.edu
//         cschiu@mit.edu
//
////////////////////////////////////////////////////////////////////////////
#ifndef OPHIT_H
#define OPHIT_H

namespace recob {

  /// hits are 2D representations of charge deposited in the tdc/wire plane
  /// hits are assumed to be made from deconvoluted, unipolar signals
  class OpHit {
    public:
      OpHit(); // Default constructor

private:

      int                   fOpChannel;
      unsigned short        fFrame;
      double                fPeakTime;
      double                fPeakTimeAbs;
      double                fWidth;
      double                fArea;
      double                fAmplitude;
      double                fPE;
      double                fFastToTotal;

#ifndef __GCCXML__

  public:

      OpHit(int opchannel, 
	    double peaktime, 
	    double peaktimeabs,
	    unsigned short frame,
	    double width, 
	    double area, 
	    double peakheight, 
	    double pe,
	    double fasttototal);

      // Get Methods
      int                   OpChannel()         const; 
      double                PeakTimeAbs()       const; 
      double                PeakTime()          const; 
      unsigned short        Frame()             const; 
      double                Width()             const; 
      double                Area()              const; 
      double                Amplitude()         const; 
      double                PE()                const; 
      double                FastToTotal()       const;
      friend bool           operator <  (const OpHit & a, const OpHit & b);

#endif

  };
}

#ifndef __GCCXML__

inline int    recob::OpHit::OpChannel()      const { return fOpChannel;      }
unsigned short recob::OpHit::Frame()         const { return fFrame;          }
inline double recob::OpHit::PeakTime()       const { return fPeakTime;       }
inline double recob::OpHit::PeakTimeAbs()    const { return fPeakTimeAbs;    }
inline double recob::OpHit::Width()          const { return fWidth;          }
inline double recob::OpHit::Area()           const { return fArea;           }
inline double recob::OpHit::Amplitude()      const { return fAmplitude;      }
inline double recob::OpHit::PE()             const { return fPE;             }
inline double recob::OpHit::FastToTotal()    const { return fFastToTotal;    }

#endif

#endif
