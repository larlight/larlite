/**
 * \file spacepoint.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for spacepoint data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef SPACEPOINT_H
#define SPACEPOINT_H

#include "data_base.h"
#include "TVector3.h"
#include "TMatrixD.h"

namespace larlite {
  /**
     \class spacepoint
     LArSoft SpacePoint class equivalent data container
  */
  class spacepoint : public data_base {
    
  public:
    
    /// Default constructor
    spacepoint() : data_base(data::kSpacePoint) {clear_data();}
    
    spacepoint(Int_t id, 
	       Double_t x,  Double_t y,  Double_t z,
	       Double_t ex, Double_t ey, Double_t ez,
	       Double_t chi2) : data_base(data::kSpacePoint)
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
    
    /// Default destructor
    virtual ~spacepoint(){}
    
    Int_t           ID()     const { return fID;     }
    const Double_t* XYZ()    const { return fXYZ;    }
    const Double_t* ErrXYZ() const { return fErrXYZ; }
    Double_t        Chisq()  const { return fChisq;  }
    
    /// Method to clear data member
    void clear_data() {
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
    
  protected:
    
    Int_t                        fID;        ///< SpacePoint ID
    Double_t                     fXYZ[3];    ///< position of SpacePoint in xyz
    Double_t                     fErrXYZ[6]; ///< Error matrix (triangular).
    Double_t                     fChisq;     ///< Chisquare. 
    
  private:
    
    ////////////////////////
    ClassDef(spacepoint,1)
    ////////////////////////
      
  };
  
  /**
     \class event_spacepoint
     A collection storage class of multiple spacepoints
  */
  class event_spacepoint : public std::vector<larlite::spacepoint>,
		    public event_base {
    
  public:
    
    /// Default constructor
    event_spacepoint(std::string name="noname") : event_base(data::kSpacePoint,name) {clear_data();}
    
    /// Default copy constructor
    event_spacepoint(const event_spacepoint& original) : std::vector<larlite::spacepoint>(original),
					   event_base(original)
    {}
    
    /// Default destructor
    ~event_spacepoint(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}
    
  private:
    
    ////////////////////////
    ClassDef(event_spacepoint,3)
    ////////////////////////
  };
}
#endif

/** @} */ // end of doxygen group 
