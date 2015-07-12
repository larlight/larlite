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
#include "DataFormat/DataFormat-TypeDef.h"
#include "ERTool/Base/Manager.h"

#include "LArUtil/ElecClock.h"
#include "LArUtil/Geometry.h"

//#include "ERTool/Base/EventData.h"
//#include "ERTool/Base/ParticleGraph.h"

//Use ShowerProfile to calculate Height & Radius from Energy
#include "EMShowerTools/EMShowerProfile.h"

namespace larlite {

  /**
     \class ERToolHelper
     @brief A collection of helper functions to generate SPAData to be used in SPT fmwk.
  */
  class ERToolHelper{
    
  public:
    
    /// Default constructor
    ERToolHelper(){
    	_minEDep = 1.e-10;

    	// get the detector width and total time of a singel fram
    	auto geom = ::larutil::Geometry::GetME();
    	_DetWidth = 2 * geom->DetHalfWidth();
    	_DetFramePeriod = 1.6E6; //ns, TODO: Fill this not by hand but from a better source
    };
    
    /// Default destructor
    virtual ~ERToolHelper(){};
    
    /// Create MC EventData and ParticleSet
    void FillMCInfo( const event_mctruth&   mci_v,
		     const event_mcshower&  mcs_v,
		     const event_mctrack&   mct_v,
		     ::ertool::Manager&     mgr) const;


    /// Fill Track Info from MC
    void FillTracks( const event_mctrack&   mct_v,
		     ::ertool::Manager&     res) const;

    /// Fill Track Info from RECO
    void FillTracks ( const event_track&       trk_v,
		      const event_cosmictag&   cos_trk_v,
		      const event_calorimetry& calo_trk_v,
		      const event_partid&      pid_trk_v,
		      const event_ass&         ass_v,
		      ::ertool::Manager&       mgr) const;

    /// Fill Shower Info from MC
    void FillShowers ( const event_mcshower&  mcs_v,
		       ::ertool::Manager&     mgr) const;

    /// Fill Shower Info from RECO
    void FillShowers ( const event_shower&    shw_v,
		       const event_cosmictag& ctag_shw_v,
		       const event_ass&       ass_v,
		       ::ertool::Manager&     mgr) const;


    /// MC Cheater for single showers
    void SingleShowerCheater(const event_mcshower& mcs_v,
			     ::ertool::Manager&    mgr ) const;

    /// Set minimum EDep amount for shower to be added to EventData
    void SetMinEDep(double E) { if(E<1.e-10) E=1.e-10; _minEDep = E; }

	/// Calculates for each mc track, based on the time of the event, the corresponding shift in x-direction
    TLorentzVector getXShift(const mctrack& mct) const;

	/// Calculates for each mc shower, based on the time of the event, the corresponding shift in x-direction
    TLorentzVector getXShift(const mcshower& mct) const;


  private:

    // Minimum deposited energy for showers in order to
    // be added to EventData
    double _minEDep;
    
    // detector width and time size of a frame used for calculations of shift
    double _DetWidth;  //cm
    double _DetFramePeriod;

    EMShowerProfile _shrProfiler;

    struct PartID_t {
      ::geoalgo::Vector _pos, _mom;
      int _pdg;

      PartID_t() : _pos(3), _mom(3), _pdg(0) 
      {}

      PartID_t(const ::geoalgo::Vector& pos,
	       const ::geoalgo::Vector& mom,
	       int pdg)
	: _pos(pos), _mom(mom), _pdg(pdg)
      { 
	for(auto& v : _mom) {
	  v = double(int(v*1.e9))/1.e9;
	}
	for(auto& v : _pos) {
	  v = double(int(v*1.e9))/1.e9;
	}
      }

      bool same(const PartID_t& obj)
      { return (_pdg == obj._pdg && _pos == obj._pos && _mom == obj._mom); }

    };

  };
}

#endif
/** @} */ // end of doxygen group 

