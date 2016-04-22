/**
 * \file DrawShower.h
 *
 * \ingroup RecoViewer
 *
 * \brief Class def header for a class DrawShower
 *
 * @author cadams
 */

/** \addtogroup RecoViewer

    @{*/
#ifndef LARLITE_DRAWSHOWER_H
#define LARLITE_DRAWSHOWER_H

#include <iostream>
#include "Analysis/ana_base.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryUtilities.h"
#include "DataFormat/shower.h"
#include "RecoBase.h"

/**
   \class DrawShower
   User defined class DrawShower ... these comments are used to generate
   doxygen documentation!
 */


namespace evd {

class Shower2d  {

public:

    /// Default constructo
    Shower2d() {}

    /// Default destructor
    ~Shower2d() {}

    int plane() {return _plane;}
    larutil::Point2D startPoint() {return _startPoint;}
    larutil::Point2D endPoint() {return _endPoint;}
    float angleInPlane() {return _angleInPlane;}
    float openingAngle() {return _openingAngle;}
    float length() {return _length;}
    bool is_good() {return _is_good;}
    float dedx() {return _dedx;}

    // ALL OF THESE VARIABLES ARE THE PROJECTION INTO THE PLANE
    int _plane;                ///< The Plane of the shower
    larutil::Point2D _startPoint;      ///< Wire time start point (units in cm)
    larutil::Point2D _endPoint;      ///< Wire time start point (units in cm)
    float _angleInPlane;       ///< Angle in the plane
    float _openingAngle;       ///< Opening angle
    float _length;             ///< Length in cm
    bool _is_good;             ///< Whether or not the projection succeeded
    float _dedx;                ///< dedx in collection plane, for printout
};


class DrawShower : public larlite::ana_base, public RecoBase<Shower2d> {

public:

    /// Default constructor
    DrawShower();

    /// Default destructor
    // ~DrawShower(){}

    /** IMPLEMENT in DrawCluster.cc!
        Initialization method to be called before the analysis event loop.
    */
    virtual bool initialize();

    /** IMPLEMENT in DrawCluster.cc!
        Analyze a data event-by-event
    */
    virtual bool analyze(larlite::storage_manager* storage);

    /** IMPLEMENT in DrawCluster.cc!
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();


    // void setProducer(std::string s){producer = s;}


    // const std::vector< ::evd::Shower2d >   & getShowersByPlane(unsigned int p) const;
    // const shower2d getShower(unsigned int plane, unsigned int index) const;

private:

    // Showers get drawn as cones.  Need a start point, and start direction
    // Also need an opening angle and the length
    // Return these as shower2d objects
    // This shows how to handle abstract objects in the viewer



    Shower2d getShower2d(larlite::shower shower, unsigned int plane);

};

} // evd

#endif
/** @} */ // end of doxygen group

