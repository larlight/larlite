////////////////////////////////////////////////////////////////////////////
// \version $Id: OpHit.h,v 1.7 2010/03/01 21:32:44 bpage Exp $
//
// \brief Definition of OpHit object for LArSoft
//
// \author bjpjones@mit.edu
//         cschiu@mit.edu
//
////////////////////////////////////////////////////////////////////////////
#ifndef LARLITE_OPHIT_H
#define LARLITE_OPHIT_H

#include "data_base.h"

namespace larlite {

  /// hits are 2D representations of charge deposited in the tdc/wire plane
  /// hits are assumed to be made from deconvoluted, unipolar signals
  class ophit : public data_base {

  public:
    ophit(); // Default constructor

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

  public:
    
    ophit(int opchannel, 
	  double peaktime, 
	  double peaktimeabs,
	  unsigned short frame,
	  double width, 
	  double area, 
	  double peakheight, 
	  double pe,
	  double fasttototal);
    
    // Get Methods
    int    OpChannel()      const;
    unsigned short Frame()  const;
    double PeakTime()       const;
    double PeakTimeAbs()    const;
    double Width()          const;
    double Area()           const;
    double Amplitude()      const;
    double PE()             const;
    double FastToTotal()    const;
    
    friend bool           operator <  (const ophit & a, const ophit & b);
    
  private:
  };

  class event_ophit : public std::vector<larlite::ophit>,
		      public event_base {

  public:

    /// default ctor
    event_ophit(std::string name="noname") : event_base(data::kOpHit,name) {clear_data();}

    /// Default copy constructor
    event_ophit(const event_ophit& original) : std::vector<larlite::ophit>(original),
					       event_base(original)
    {}

    ~event_ophit() {}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}

  private:

  };
		      
}

#endif
