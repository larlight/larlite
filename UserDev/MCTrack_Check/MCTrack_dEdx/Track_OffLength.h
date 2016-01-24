/**
 * \file Track_OffLength.h
 *
 * \ingroup MCTrack_dEdx
 * 
 * \brief Class def header for a class Track_OffLength
 *
 * @author jzennamo
 */

/** \addtogroup MCTrack_dEdx

    @{*/

#ifndef LARLITE_TRACK_OFFLENGTH_H
#define LARLITE_TRACK_OFFLENGTH_H

#include "Analysis/ana_base.h"
#include "DataFormat/mctrack.h"
#include "GeoAlgo/GeoAlgo.h"
#include "GeoAlgo/GeoAABox.h"
#include "GeoAlgo/GeoHalfLine.h"
#include <algorithm>
#include <utility>
#include "TGraph.h"
#include "TH1D.h"

namespace larlite {
  /**
     \class Track_OffLength
     User custom analysis class made by SHELL_USER_NAME
   */
  class Track_OffLength : public ana_base{
  
  public:

    /// Default constructor
    Track_OffLength(){ _name="Track_OffLength"; _fout=0;}

    /// Default destructor
    virtual ~Track_OffLength(){}

    /** IMPLEMENT in Track_OffLength.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in Track_OffLength.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in Track_OffLength.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:

    TH1D* OL;
    TH1D* dedx;
    std::vector<double> dEdx;
    std::vector< std::vector<double> > of;

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
