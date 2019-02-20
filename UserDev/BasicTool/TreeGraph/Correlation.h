/**
 * \file Correlation.h
 *
 * \ingroup GeoTree
 * 
 * \brief Class def header for a class geotree::Correlation
 *
 * @author david caratelli
 */

/** \addtogroup GeoTree
    
    @{*/
#ifndef CORRELATION_H
#define CORRELATION_H

#include "GeoAlgo/GeoVector.h"

namespace geotree{

  enum RelationType_t {
    kParent,
    kChild,
    kSibling,
    kUnknown
  };
  
  /**
     \class geotree::Correlation
     User defined class geograph::Correlation
     A correlation is a simple structure
     that uniquely defines the correlation
     between two nodes.
     It is composed of:
     - a score (double)
     - a vertex (larlite::geoalgo::Point_t)
     - a type (::geotree::RelationType_t)
  */
  
  class Correlation{

  public:

    /// Default constructor
    Correlation() { _score = 0.; _vtx = larlite::geoalgo::Point_t(); _type = ::geotree::RelationType_t::kUnknown; }

    /// Constructor with information specified
    Correlation(double s, larlite::geoalgo::Point_t vtx, ::geotree::RelationType_t type);
    
    /// Getters
    double Score() const { return _score; }
    larlite::geoalgo::Point_t Vtx() const { return _vtx; }
    ::geotree::RelationType_t Relation() const { return _type; }

    /// Default destructor
    virtual ~Correlation(){}

    /// Correlation editing: score
    void EditCorrelation(double s) { _score = s; }

    /// Correlation editing: vtx
    void EditCorrelation(larlite::geoalgo::Point_t vtx) { _vtx = vtx; }

    /// Correlation editing: type
    void EditCorrelation(::geotree::RelationType_t type) { _type = type; }
    
    /// Correlation editing: edit all fields
    void EditCorrelation(double s, larlite::geoalgo::Point_t vtx, ::geotree::RelationType_t type);
    
  private: 
    
    double _score;
    larlite::geoalgo::Point_t _vtx;
    ::geotree::RelationType_t _type;

  };

}

#endif
/** @} */ // end of doxygen group 
