////////////////////////////////////////////////////////////////////////////
// \version $Id: OpFlash.h,v 1.7 2010/03/01 21:32:44 bpage Exp $
//
// \brief Definition of OpFlash object for LArSoft
//
// \author bjpjones@mit.edu
//         cschiu@mit.edu
//
////////////////////////////////////////////////////////////////////////////
#ifndef LARLITE_OPFLASH_H
#define LARLITE_OPFLASH_H

#include "data_base.h"
#include <vector>

namespace larlite {

  // subevents are groupings of OpDet pulse peaks. Each peak comes from an OpDet and
  // has a certain number of PE; each subevent has a time associated with it
  class opflash : public data_base {

  public:

      opflash(); // Default constructor

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
    
  public:

    opflash(double time, double timewidth, double abstime, unsigned int frame,
	    std::vector< double > PEperOpDet,
	    bool InBeamFrame=0, int OnBeamTime=0, double FastToTotal=1,
	    double yCenter=0, double yWidth=0,
	    double zCenter=0, double zWidth=0,
	    std::vector<double> WireCenters = std::vector<double>(0),
	    std::vector<double> WireWidths  = std::vector<double>(0));
    
    // Get Methods
    double Time()              const;
    double TimeWidth()         const;
    double AbsTime()           const;
    unsigned int Frame()       const;
    double PE(unsigned int i)  const;
    double YCenter()           const;
    double YWidth()            const;
    double ZCenter()           const;
    double ZWidth()            const;
    double FastToTotal()       const;
    const std::vector<double>& WireCenters() const;
    const std::vector<double>& WireWidths()  const;
    bool  InBeamFrame()        const;
    int  OnBeamTime()          const;
    double TotalPE() const;
    size_t nOpDets() const { return fPEperOpDet.size(); };

    friend bool           operator <  (const opflash & a, const opflash & b);

  private:

  };

  class event_opflash : public std::vector<larlite::opflash>,
			public event_base {
    
  public:

    /// Default ctor
    event_opflash(std::string name="noname") : event_base(data::kOpFlash,name) {clear_data();}

    /// Default copy constructor
    event_opflash(const event_opflash& original) : std::vector<larlite::opflash>(original),
						   event_base(original)
    {}

    ~event_opflash(){}

    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}

  private:

  };

}

namespace larlite{
  struct OpFlashSortByTime {
    bool operator() (opflash i, opflash j){ return i.Time() < j.Time(); }
  };
}

#endif
