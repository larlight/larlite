/**
 * \file NCfilter.h
 *
 * \ingroup NCfilter
 * 
 * \brief Class def header for a class Quality
 *
 * @author ryan
 */

/** \addtogroup NCfilter

    @{*/

#ifndef RECOTOOL_QUALITY_H
#define RECOTOOL_QUALITY_H

#include <iostream>
#include <vector>
#include "Analysis/ana_base.h"
#include "LArUtil/PxUtils.h"
#include "LArUtil/LArUtilBase.h"
#include "ClusterRecoUtil/Base/CRUHelper.h"
#include "ClusterRecoUtil/Base/ClusterParamsAlg.h"

// ROOT includes
#include <TF1.h>
#include <TH2F.h>
#include <TH1F.h>
#include <TPrincipal.h>
#include <TVectorD.h>
#include <TGraph.h>
#include <TMath.h>
#include <TH1D.h>
#include <TVirtualFitter.h>

namespace cluster {
  class Quality{

  public:

    /// Default constructor
    Quality(){};

    /// Default destructor
    virtual ~Quality(){};

    /**
       Clustering function. Returns a vector of clusters.
       Each cluster is represented as a group of hit via vector of hit indexes.
    */
   
	// what ever I want it to return 
  double  TimeProf(std::vector<larutil::PxHit> hita ,std::vector<larutil::PxHit> hitb);
 // std::pair<double,double>  ProtoVertex(std::vector<larutil::PxHit> hita ,std::vector<larutil::PxHit> hitb);
	std::pair<double,double>  ProtoVertexcw(std::vector<larutil::PxHit> hita ,std::vector<larutil::PxHit> hitb);
	std::vector<std::pair<double,double>> ProtoVertexCW( const std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>> Regions, const larlite::event_hit* hits ); 


 protected:
        cluster::CRUHelper fCRUHelper;

  };
}
#endif
/** @} */ // end of doxygen group 
