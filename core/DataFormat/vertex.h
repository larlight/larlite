/**
 * \file vertex.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for vertex data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef LARLITE_VERTEX_H
#define LARLITE_VERTEX_H

#include "data_base.h"

namespace larlite {
  /**
     \class vertex
     LArSoft Vertex class equivalent data container
  */
  class vertex : public data_base {
    
  public:
    
    /// Default constructor
    vertex();

    /// Alternative constructor
    vertex(double* xyz,
	   int      id=data::kINVALID_INT);
    
    /// Copy constructor
    vertex(const vertex &original) : data_base(original),
				     fID(original.fID)
    {original.XYZ(fXYZ);}
    
    /// Default destructor
    virtual ~vertex(){}

    /// Clear method
    virtual void clear_data();

    ///--- Getters ---///
    void XYZ(double *xyz) const;
    int ID() const;
    double X() const;
    double Y() const;
    double Z() const;
    
  private:

    double fXYZ[3];    ///< location of vertex
    int    fID;        ///< id number for vertex
    
  };
  
  /**
     \class event_vertex
     A collection storage class of multiple vertex points
  */
  class event_vertex : public std::vector<larlite::vertex>,
		       public event_base {
    
  public:
    
    /// Default constructor
    event_vertex(std::string name="noname");
    
    /// Default copy constructor
    event_vertex(const event_vertex& original) : std::vector<larlite::vertex>(original),
						 event_base(original)
    {}
    
    /// Default destructor
    ~event_vertex(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}
    
  private:
    
  };
}
#endif

/** @} */ // end of doxygen group 
