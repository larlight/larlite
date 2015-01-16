/**
 * \file SPAHelper.h
 *
 * \ingroup ShowerPdfTool
 * 
 * \brief Class def header for a class SPAHelper
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerPdfTool

    @{*/
#ifndef SELECTIONTOOL_SPAHELPER_H
#define SELECTIONTOOL_SPAHELPER_H

#include <iostream>
#include <map>
#include <TRandom.h>

#include "DataFormat/mctruth.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/mctrack.h"

#include "DataFormat/shower.h"
#include "DataFormat/track.h"
#include "DataFormat/vertex.h"
#include "DataFormat/cosmictag.h"
#include "DataFormat/calorimetry.h"
#include "DataFormat/partid.h"
#include "SPAData.h"

namespace sptool {
  /**
     \class SPAHelper
     @brief A collection of helper functions to generate SPAData to be used in SPT fmwk.
  */
  class SPAHelper{
    
  public:
    
    /// Default constructor
    SPAHelper(){};
    
    /// Default destructor
    virtual ~SPAHelper(){};

    /// Fill Track Info from MC
    void FillTracks( const ::larlite::event_mctrack& mct_v,
		     SPAData& res ) const;
    
    /// Fill Track Info from RECO
    void FillTracks ( const ::larlite::event_track&       trk_v,
		      const ::larlite::event_cosmictag&   cos_trk_v,
		      const ::larlite::event_calorimetry& calo_trk_v,
		      const ::larlite::event_partid&      pid_trk_v,
		      SPAData& res ) const;

    /// Fill Shower Info from MC
    void FillShowers ( const ::larlite::event_mcshower& mcs_v,
		       SPAData& res ) const;

    /// Fill Shower Info from RECO
    void FillShowers ( const ::larlite::event_shower& shw_v,
		       const ::larlite::event_cosmictag& ctag_shw_v,
		       SPAData& res ) const;

    /// Fill Vertex Info from MC
    void FillVertices ( const ::larlite::event_vertex& vtx_v, 
			SPAData& res ) const;
    
    /// Fill Vertex Info from RECO
    void FillVertices ( const ::larlite::event_mctruth& mci_v,
			SPAData& res) const;

    /// Generate SPAData for full MC information
    SPAData Generate ( const ::larlite::event_mctruth&  mci_v,
		       const ::larlite::event_mctrack&  mct_v,
		       const ::larlite::event_mcshower& mcs_v ) const;

    /// Generate SPAData for MCShower/MCTrack information
    SPAData Generate ( const ::larlite::event_mctrack&  mct_v,
		       const ::larlite::event_mcshower& mcs_v ) const;

    /// Append MCTruth to input SPAData
    void Append ( const ::larlite::event_mctruth& mci_v,
		  SPAData& res) const;

    /// Generate SPAData only using shower information
    SPAData Generate ( const ::larlite::event_shower&    shw_v,
		       const ::larlite::event_cosmictag& cos_shw_v ) const;

    /// Generate SPAData using shower + track + vertex information
    SPAData Generate ( const ::larlite::event_vertex&      vtx_v,
		       const ::larlite::event_track&       trk_v,
		       const ::larlite::event_shower&      shw_v,
		       const ::larlite::event_cosmictag&   cos_trk_v,
		       const ::larlite::event_cosmictag&   cos_shw_v,
		       const ::larlite::event_calorimetry& calo_trk_v,
		       const ::larlite::event_partid&      pid_trk_v) const;

    /// Append vertex information to input SPAData
    void Append ( const ::larlite::event_vertex& vtx_v, 
		  SPAData& res) const;

    /// Append shower information to input SPAData
    void Append ( const ::larlite::event_shower&    shw_v,
		  const ::larlite::event_cosmictag& cos_shw_v,
		  SPAData& res) const;

    /// Append track information to input SPAData
    void Append ( const ::larlite::event_track&       trk_v,
		  const ::larlite::event_cosmictag&   cos_trk_v,
		  const ::larlite::event_calorimetry& calo_trk_v,
		  const ::larlite::event_partid&      pid_trk_v,
		  SPAData& res ) const;
    
  };
}

#endif
/** @} */ // end of doxygen group 

