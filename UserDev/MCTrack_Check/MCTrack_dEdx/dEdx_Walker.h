/**
 * \file dEdx_Walker.h
 *
 * \ingroup MCTrack_dEdx
 * 
 * \brief Class def header for a class dEdx_Walker
 *
 * @author jzennamo
 */

/** \addtogroup MCTrack_dEdx

    @{*/

#ifndef LARLITE_DEDX_WALKER_H
#define LARLITE_DEDX_WALKER_H

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
     \class dEdx_Walker
     User custom analysis class made by SHELL_USER_NAME
   */
  class dEdx_Walker : public ana_base{
  
  public:

    /// Default constructor
    dEdx_Walker(){ _name="dEdx_Walker"; _fout=0;}

    /// Default destructor
    virtual ~dEdx_Walker(){}

    /** IMPLEMENT in dEdx_Walker.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in dEdx_Walker.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in dEdx_Walker.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:
    // GeoAlgo Tool    
    ::geoalgo::GeoAlgo _geoAlgo;


    std::vector<double>  dEdx;
    std::vector< std::vector<double> >  dQdx;
    std::vector<int> step;

    TGraph* g;
    TH1D* dedx;

    int w = 0;

    double AVG = 0;
    int N = 0;

    ::geoalgo::AABox fTPC;

    
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
