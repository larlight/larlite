/**
 * \file DrawEndpoint.h
 *
 * \ingroup RecoViewer
 *
 * \brief Class def header for a class DrawEndpoint
 *
 * @author cadams
 */

/** \addtogroup RecoViewer

    @{*/

#ifndef LARLITE_DRAWENDPOINT_H
#define LARLITE_DRAWENDPOINT_H

#include "Analysis/ana_base.h"
#include "RecoBase.h"

namespace evd {

class Endpoint2D {

public:
  Endpoint2D() {}
  Endpoint2D(float w, float t, float c, float s) :
    _wire(w),
    _time(t),
    _charge(c),
    _strength(s)
  {};

  float wire()     {return _wire;}
  float time()     {return _time;}
  float charge()   {return _charge;}
  float strength() {return _strength;}

private:
  float _wire;
  float _time;
  float _charge;
  float _strength;

};

/**
   \class DrawEndpoint
   User custom analysis class made by SHELL_USER_NAME
 */
class DrawEndpoint : public larlite::ana_base, public RecoBase<Endpoint2D> {

public:

  /// Default constructor
  DrawEndpoint();

  // / Default destructor
  // virtual ~DrawEndpoint();

  /** IMPLEMENT in DrawEndpoint.cc!
      Initialization method to be called before the analysis event loop.
  */
  virtual bool initialize();

  /** IMPLEMENT in DrawEndpoint.cc!
      Analyze a data event-by-event
  */
  virtual bool analyze(larlite::storage_manager* storage);

  /** IMPLEMENT in DrawEndpoint.cc!
      Finalize method to be called after all events processed.
  */
  virtual bool finalize();

protected:

private:

};
}
#endif

//**************************************************************************
//
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group
