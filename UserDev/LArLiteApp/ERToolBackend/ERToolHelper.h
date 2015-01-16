/**
 * \file ERToolHelper.h
 *
 * \ingroup LArLite_ERTool
 * 
 * \brief Class def header for a class ERToolHelper
 *
 * @author kazuhiro
 */

/** \addtogroup LArLite_ERTool

    @{*/
#ifndef LARLITE_ERTOOLHELPER_H
#define LARLITE_ERTOOLHELPER_H

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
#include "ERTool/Base/EventData.h"

namespace larlite {
  /**
     \class ERToolHelper
     @brief A collection of helper functions to generate SPAData to be used in SPT fmwk.
  */
  class ERToolHelper{
    
  public:
    
    /// Default constructor
    ERToolHelper(){};
    
    /// Default destructor
    virtual ~ERToolHelper(){};

    /// Fill Track Info from MC
    void FillTracks( const event_mctrack& mct_v,
		     ::ertool::EventData& res ) const;
    
    /// Fill Track Info from RECO
    void FillTracks ( const event_track&       trk_v,
		      const event_cosmictag&   cos_trk_v,
		      const event_calorimetry& calo_trk_v,
		      const event_partid&      pid_trk_v,
		      ::ertool::EventData& res ) const;

    /// Fill Shower Info from MC
    void FillShowers ( const event_mcshower& mcs_v,
		       ::ertool::EventData& res ) const;

    /// Fill Shower Info from RECO
    void FillShowers ( const event_shower& shw_v,
		       const event_cosmictag& ctag_shw_v,
		       ::ertool::EventData& res ) const;

    /// Fill Vertex Info from MC
    void FillVertices ( const event_vertex& vtx_v, 
			::ertool::EventData& res ) const;
    
    /// Fill Vertex Info from RECO
    void FillVertices ( const event_mctruth& mci_v,
			::ertool::EventData& res) const;

    /// Generate ::ertool::EventData for full MC information
    ::ertool::EventData Generate ( const event_mctruth&  mci_v,
				   const event_mctrack&  mct_v,
				   const event_mcshower& mcs_v ) const;

    /// Generate ::ertool::EventData for MCShower/MCTrack information
    ::ertool::EventData Generate ( const event_mctrack&  mct_v,
				   const event_mcshower& mcs_v ) const;

    /// Append MCTruth to input ::ertool::EventData
    void Append ( const event_mctruth& mci_v,
		  ::ertool::EventData& res) const;

    /// Generate ::ertool::EventData only using shower information
    ::ertool::EventData Generate ( const event_shower&    shw_v,
				   const event_cosmictag& cos_shw_v ) const;

    /// Generate ::ertool::EventData using shower + track + vertex information
    ::ertool::EventData Generate ( const event_vertex&      vtx_v,
				   const event_track&       trk_v,
				   const event_shower&      shw_v,
				   const event_cosmictag&   cos_trk_v,
				   const event_cosmictag&   cos_shw_v,
				   const event_calorimetry& calo_trk_v,
				   const event_partid&      pid_trk_v) const;

    /// Append vertex information to input ::ertool::EventData
    void Append ( const event_vertex& vtx_v, 
		  ::ertool::EventData& res) const;

    /// Append shower information to input ::ertool::EventData
    void Append ( const event_shower&    shw_v,
		  const event_cosmictag& cos_shw_v,
		  ::ertool::EventData& res) const;

    /// Append track information to input ::ertool::EventData
    void Append ( const event_track&       trk_v,
		  const event_cosmictag&   cos_trk_v,
		  const event_calorimetry& calo_trk_v,
		  const event_partid&      pid_trk_v,
		  ::ertool::EventData& res ) const;
    
  };
}

#endif
/** @} */ // end of doxygen group 

