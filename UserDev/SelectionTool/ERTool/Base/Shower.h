/**
 * \file Shower.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class ertool::Shower
 *
 * @author kazuhiro
 */

/** \addtogroup ERTool

    @{*/
#ifndef ERTOOL_SHOWER_H
#define ERTOOL_SHOWER_H

#include "GeoAlgo/GeoHalfLine.h"
#include "RecoObjBase.h"
#include "ERException.h"
namespace ertool {

  /**
     \class Shower
     @brief Data holder class to contain Shower data. This is contained within ertool::EventData instance.
     This class instance is created per shower, and stored within ertool::EventData.
     AlgoX and FilterX will use ertool::EventData instance for evaluation.
  */
  class Shower : public RecoObjBase
	       , public ::geoalgo::HalfLine {

  public:

    /// default ctor
    Shower();

    /// alternative ctor
    Shower(const ::geoalgo::Vector& start,
	   const ::geoalgo::Vector& dir);

    /// default dtor
    virtual ~Shower(){}

    /// alternative ctor
    /*
    template <class T, class U>
    Shower(const T& pos, const U& dir) : HalfLine(pos,dir)
    {}
    */
    /// Reset function
    void Reset();

    /// Radius of shower, either read from data product or computed
    double _radius;

    /// Opening angle of a shower cone, either read from data product or computed
    double _cone_angle;

    /// dE/dX @ shower start
    double _dedx;

    /// Total deposited energy
    double _energy;

    ClassDef(Shower,1)
  };
}
#endif
/** @} */ // end of doxygen group 

