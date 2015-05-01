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

#ifndef LARLITE_SPACEPOINT_H
#define LARLITE_SPACEPOINT_H

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
    
    spacepoint(int id, 
	       double x,  double y,  double z,
	       double ex, double ey, double ez,
	       double chi2);
    /// Copy constructor
    spacepoint(const spacepoint& original);

    /// Default destructor
    virtual ~spacepoint(){}
    
    int           ID()     const;
    const double* XYZ()    const;
    const double* ErrXYZ() const;
    double        Chisq()  const;
    
    /// Method to clear data member
    void clear_data();

  protected:
    
    int    fID;        ///< SpacePoint ID
    double fXYZ[3];    ///< position of SpacePoint in xyz
    double fErrXYZ[6]; ///< Error matrix (triangular).
    double fChisq;     ///< Chisquare. 

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
    
  };
}
#endif

/** @} */ // end of doxygen group 
