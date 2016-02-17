/**
 * \file ERToolHelperUtil.h
 *
 * \ingroup LArLite_ERTool
 *
 * \brief Utility for ERToolHelper (and others) to use to centralize 
 *         cut values, xshift definitions, etc
 *
 * @author kaleko
 */

/** \addtogroup LArLite_ERTool

    @{*/
#ifndef LARLITE_ERTOOLHELPERUTIL_H
#define LARLITE_ERTOOLHELPERUTIL_H

#include <iostream>
#include <TRandom.h>
#include "DataFormat/DataFormat-TypeDef.h"
#include "LArUtil/Geometry.h"

namespace larlite {

/**
   \class ERToolHelperUtil
   @brief A collection of helper functions to generate SPAData to be used in SPT fmwk.
*/
class ERToolHelperUtil {

public:

    /// Default constructor
    ERToolHelperUtil() {
        _minEDep = 1.e-10;

        // get the detector width and total time of a singel fram
        _DetWidth = 2 * geom->DetHalfWidth();
        _DetFramePeriod = 1.6E6; //ns, TODO: Fill this not by hand but from a better source

        _disable_xshift = false;
    };

    /// Default destructor
    virtual ~ERToolHelperUtil() {};

    /// Set minimum EDep amount for shower to be added to EventData
    void SetMinEDep(double E) { if (E < 1.e-10) E = 1.e-10; _minEDep = E; }

    /// Calculates for each mc track, based on the time of the event, the corresponding shift in x-direction
    TLorentzVector getXShift(const mctrack& mct) const;

    /// Calculates for each mc shower, based on the time of the event, the corresponding shift in x-direction
    TLorentzVector getXShift(const mcshower& mcs) const;

    /// Calculates for each mc particle, based on the time of the event, the corresponding shift in x-direction
    TLorentzVector getXShift(const mcpart& mcp) const;

    /// Setter to disable xshift
    void setDisableXShift(bool flag) { _disable_xshift = flag; }

    bool isViable(const mcshower& mcs) const;

    double getMCShowerdEdX(const mcshower& mcs) const;
    
private:

    // Minimum deposited energy for showers in order to
    // be added to EventData
    double _minEDep;

    // detector width and time size of a frame used for calculations of shift
    double _DetWidth;  //cm
    double _DetFramePeriod;

    bool _disable_xshift;

    const ::larutil::Geometry *geom = ::larutil::Geometry::GetME();
};
}

#endif
/** @} */ // end of doxygen group

