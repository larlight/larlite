/**
 * \file DrawPiZeroROI.h
 *
 * \ingroup RecoViewer
 *
 * \brief Class def header for a class DrawPiZeroROI
 *
 * @author jzennamo
 */

/** \addtogroup RecoViewer

    @{*/
#ifndef LARLITE_DRAWPIZEROROI_H
#define LARLITE_DRAWPIZEROROI_H

#include <iostream>
#include "Analysis/ana_base.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryUtilities.h"
#include "DataFormat/PiZeroROI.h"
#include "RecoBase.h"

/**
   \class DrawPiZeroROI
   User defined class DrawPiZeroROI ... these comments are used to generate
   doxygen documentation!
 */


namespace evd {

class roi2d  {

public:

    /// Default constructo
    roi2d() {}

    /// Default destructor
    ~roi2d() {}

    int plane() {return _plane;}
    larutil::Point2D maxmaxPoint() {return _maxmaxPoint;}
    larutil::Point2D minminPoint() {return _minminPoint;}
    larutil::Point2D vtxPoint() {return _vtxPoint;}
    bool is_good() {return _is_good;}

    // ALL OF THESE VARIABLES ARE THE PROJECTION INTO THE PLANE
    int _plane;                ///< The Plane of the shower
    larutil::Point2D _minminPoint;      ///< Wire time start point (units in cm)
    larutil::Point2D _maxmaxPoint;      ///< Wire time start point (units in cm)
    larutil::Point2D _vtxPoint;      ///< Wire time start point (units in cm)
    bool _is_good;             ///< Whether or not the projection succeeded
};


class DrawPiZeroROI : public larlite::ana_base, public RecoBase<roi2d> {

public:

    /// Default constructor
    DrawPiZeroROI();

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

private:

    // ROI get drawn as rectangle.  Needs the wire-tick range
    // Return these as roi2d objects
    // This shows how to handle abstract objects in the viewer

    roi2d getroi2d(larlite::PiZeroROI roi, unsigned int plane);

};

} // evd

#endif
/** @} */ // end of doxygen group

