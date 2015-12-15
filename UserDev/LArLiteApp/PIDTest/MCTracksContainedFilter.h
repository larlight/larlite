/**
 * \file MCTracksContainedFilter.h
 *
 * \ingroup TrackStudy
 * 
 * \brief Class def header for a class MCTracksContainedFilter
 *
 * @author davidkaleko
 */

/** \addtogroup TrackStudy

    @{*/

#ifndef LARLITE_MCTRACKSCONTAINEDFILTER_H
#define LARLITE_MCTRACKSCONTAINEDFILTER_H

#include "Analysis/ana_base.h"
#include "GeoAlgo/GeoAABox.h"
#include "DataFormat/mctrack.h"
#include "LArUtil/Geometry.h"

namespace larlite {
  /**
     \class MCTracksContainedFilter
     User custom analysis class made by davidkaleko
   */
  class MCTracksContainedFilter : public ana_base{
  
  public:
    
    /// Default constructor
    MCTracksContainedFilter(){ 
      _name="MCTracksContainedFilter"; 
      _fout=0;
    };
    
    /// Default destructor
    virtual ~MCTracksContainedFilter(){};

    /** IMPLEMENT in MCTracksContainedFilter.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MCTracksContainedFilter.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in MCTracksContainedFilter.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:
    
    bool isFullyContained(larlite::mctrack const &mytrack);

    //Geometry algo instance
    geoalgo::AABox _myGeoAABox;

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
