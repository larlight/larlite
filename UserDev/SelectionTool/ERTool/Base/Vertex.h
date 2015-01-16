/**
 * \file Vertex.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class ertool::Vertex
 *
 * @author kazuhiro
 */

/** \addtogroup ERTool

    @{*/
#ifndef ERTOOL_VERTEX_H
#define ERTOOL_VERTEX_H

#include "GeoAlgo/GeoVector.h"
#include "RecoObjBase.h"
#include "ERException.h"
namespace ertool {

  /**
     \class Vertex
     @brief Data holder class to contain Vertex data. This is contained within ertool::EventData instance.
     This class instance is created per vertex, and stored within ertool::EventData.
     AlgoX and FilterX will use ertool::EventData instance for evaluation.
  */
  class Vertex : public RecoObjBase
	       , public ::geoalgo::Vector {

  public:

    /// default ctor
    Vertex(double x=kINVALID_DOUBLE,
	   double y=kINVALID_DOUBLE,
	   double z=kINVALID_DOUBLE);

    /// default dtor
    virtual ~Vertex(){}

    /// Reset function
    void Reset();

    ClassDef(Vertex,1)
  };
}
#endif
/** @} */ // end of doxygen group 

