/**
 * \file FilterFidVolume.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class FilterFidVolume
 *
 * @author kazuhiro
 */

/** \addtogroup ERTool

    @{*/

#ifndef ERTOOL_FILTERFIDVOLUME_H
#define ERTOOL_FILTERFIDVOLUME_H

#include "Base/FilterBase.h"
#include "GeoAlgo/GeoAABox.h"
#include "GeoAlgo/GeoAlgo.h"
#include "GeoAlgo/GeoHalfLine.h"
#include <TH1D.h>
#include <TH2D.h>
namespace ertool {

  /**
     \class FilterFidVolume
     User custom SPAFilter class made by kazuhiro
   */
  class FilterFidVolume : public FilterBase {
  
  public:

    /// Default constructor
    FilterFidVolume();

    /// Default destructor
    virtual ~FilterFidVolume(){};

    virtual void ProcessBegin();

    /// Select function
    virtual void Filter (EventData& data);

    virtual void ProcessEnd(TFile* fout);
    
  protected:

    TH1D *hPerpDist, *hProjDist, *hPerpDistXBoundary, *hPerpDistYBoundary, *hPerpDistZBoundary, *hTrackLength, *hTrackPhi, *hTrackTheta, *hLongestTrackLength, *hMuonTrackLength, *hMuonTrackPhi;
    TH2D *hPerpDistVsTrackLength, *hProjDistVsTrackLength;
    ::geoalgo::AABox fTPC;
    ::geoalgo::GeoAlgo fGeoAlgo;
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
