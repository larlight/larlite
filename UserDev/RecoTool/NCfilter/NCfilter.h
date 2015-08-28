/**
 * \file NCfilter.h
 *
 * \ingroup NCfilter
 * 
 * \brief Class def header for a class NCfilter
 *
 * @author ryan
 */

/** \addtogroup NCfilter

    @{*/

#ifndef RECOTOOL_NCFILTER_H
#define RECOTOOL_NCFILTER_H

#define PI 3.14159265

#include "Analysis/ana_base.h"
#include "ClusterRecoUtil/Base/ClusterParamsAlg.h"
#include "ClusterRecoUtil/Base/CRUHelper.h"
#include "LArUtil/LArUtilBase.h"

#include "DivReg.h"
#include "ForceRegions.h"
#include "Quality.h"
#include "PlaneCheckMatch.h"
#include "LookBack.h"

#include<algorithm> // For sort()

namespace larlite {
  /**
     \class NCfilter
     User custom analysis class made by ryan
   */
  class NCfilter : public ana_base{
  
  public:

    /// Default constructor
    NCfilter() : ana_base() { _name="NCfilter"; radius_of_interest=2.0;}

    /// Default destructor
    virtual ~NCfilter(){};

    /** IMPLEMENT in NCfilter.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in NCfilter.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in NCfilter.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    protected:

	::cluster::DivReg fDivReg;
        ::cluster::CRUHelper fCRUHelper;
        ::cluster::ForceRegions fForceRegions;
        ::cluster::Quality fQuality;
        ::cluster::PlaneCheckMatch fPlaneCheckMatch;
        ::cluster::LookBack fLookBack;

    double radius_of_interest;


    private: 
	std::pair<double,double> HighQSlope(std::vector<unsigned int> hitindex ,larlite::event_hit const* hits);
	std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>> CheckQuality(larlite::event_hit *const hits,std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>> protocluster, std::vector<bool> Flags);
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
