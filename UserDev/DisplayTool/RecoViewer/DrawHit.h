/**
 * \file DrawHit.h
 *
 * \ingroup RecoViewer
 *
 * \brief Class def header for a class DrawHit
 *
 * @author cadams
 */

/** \addtogroup RecoViewer

    @{*/
#ifndef LARLITE_DRAWHIT_H
#define LARLITE_DRAWHIT_H

#include <iostream>
#include "Analysis/ana_base.h"
#include "RecoBase.h"

/**
   \class DrawHit
   User defined class DrawHit ... these comments are used to generate
   doxygen documentation!
 */

namespace evd {

class Hit {

public:
    Hit(float w, float t, float c, float r) :
    _wire(w),
    _time(t),
    _charge(c),
    _rms(r)
    {}
    ~Hit() {}
    float _wire;
    float _time;
    float _charge;
    float _rms;

    float wire()   {return _wire;}
    float time()   {return _time;}
    float charge() {return _charge;}
    float rms()    {return _rms;}
};

class DrawHit : public larlite::ana_base, public RecoBase<Hit> {

public:

    /// Default constructor
    DrawHit();

    /// Default destructor
    // ~DrawHit(){}

    /**
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

    float maxCharge(size_t plane);

private:

    std::vector <float> _maxCharge;

};

} // evd

#endif
/** @} */ // end of doxygen group

