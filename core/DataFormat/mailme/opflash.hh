////////////////////////////////////////////////////////////////////////////
// \version $Id: OpFlash.h,v 1.7 2010/03/01 21:32:44 bpage Exp $
//
// \brief Definition of OpFlash object for LArSoft
//
// \author bjpjones@mit.edu
//         cschiu@mit.edu
//
////////////////////////////////////////////////////////////////////////////
#ifndef OPFLASH_H
#define OPFLASH_H

#include <vector>

namespace recob {

  // subevents are groupings of OpDet pulse peaks. Each peak comes from an OpDet and
  // has a certain number of PE; each subevent has a time associated with it
  class OpFlash {
    public:
      OpFlash(); // Default constructor

private:
 
      double                fTime;         // Time relative to trigger
      double                fTimeWidth;    // Width of the flash in time
      double                fAbsTime;      // Time by PMT readout clock
      unsigned int          fFrame;        // Frame number 
      std::vector< double > fPEperOpDet;   // Number of PE on each PMT
      std::vector< double > fWireCenters;  // Geometric center in each view
      std::vector< double > fWireWidths;   // Geometric width in each view
      double                fYCenter;      // Geometric center in y
      double                fYWidth;       // Geometric width in y
      double                fZCenter;      // Geometric center in z
      double                fZWidth;       // Geometric width in z
      double                fFastToTotal;  // Fast to total light ratio
      bool                  fInBeamFrame;  // Is this in the beam frame?
      int                   fOnBeamTime;   // Is this in time with beam?
      


#ifndef __GCCXML__

  public:
      OpFlash(double time, double timewidth, double abstime, unsigned int frame,
	      std::vector< double > PEperOpDet,
	      bool InBeamFrame=0, int OnBeamTime=0, double FastToTotal=1,
	      double yCenter=0, double yWidth=0,
	      double zCenter=0, double zWidth=0,
	      std::vector<double> WireCenters = std::vector<double>(0),
	      std::vector<double> WireWidths  = std::vector<double>(0));

      // Get Methods
      double                Time()              const;
      double                TimeWidth()         const;
      double                AbsTime()           const;
      unsigned int          Frame()             const;
      double                PE(unsigned int i)  const;
      double                YCenter()           const;
      double                YWidth()            const;
      double                ZCenter()           const;
      double                ZWidth()            const; 
      
      bool                  InBeamFrame()       const;
      int                   OnBeamTime()        const;

      std::vector<double>   WireCenters()       const;
      std::vector<double>   WireWidths()        const;
     
      friend bool           operator <  (const OpFlash & a, const OpFlash & b);

      double                TotalPE()           const;
      double                FastToTotal()       const;

#endif

  };

}

#ifndef __GCCXML__

inline double recob::OpFlash::Time()              const { return fTime;        }
inline double recob::OpFlash::TimeWidth()         const { return fTimeWidth;   }
inline double recob::OpFlash::AbsTime()           const { return fAbsTime;     }
inline unsigned int recob::OpFlash::Frame()       const { return fFrame;       }
inline double recob::OpFlash::PE(unsigned int i)  const { return fPEperOpDet[i]; }
inline double recob::OpFlash::YCenter()           const { return fYCenter;     }
inline double recob::OpFlash::YWidth()            const { return fYWidth;      }
inline double recob::OpFlash::ZCenter()           const { return fZCenter;     }
inline double recob::OpFlash::ZWidth()            const { return fZWidth;      }
inline double recob::OpFlash::FastToTotal()            const { return fFastToTotal;      }
inline std::vector<double> recob::OpFlash::WireCenters()            const { return fWireCenters;      }
inline std::vector<double> recob::OpFlash::WireWidths()             const { return fWireWidths;      }
inline bool  recob::OpFlash::InBeamFrame()          const { return fInBeamFrame;     }
inline int  recob::OpFlash::OnBeamTime()          const { return fOnBeamTime;     }

namespace recob{
  struct OpFlashSortByTime {
    bool operator() (recob::OpFlash i, recob::OpFlash j){ return i.Time() < j.Time(); }
  };
}

#endif

#endif
