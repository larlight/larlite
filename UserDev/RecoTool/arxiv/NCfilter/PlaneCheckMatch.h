/**
 * \file NCfilter.h
 *
 * \ingroup NCfilter
 * 
 * \brief Class def header for a class PlaneCheckMatch
 *
 * @author ryan
 */

/** \addtogroup NCfilter

    @{*/

#ifndef RECOTOOL_PLANECHECKMATCH_H
#define RECOTOOL_PLANECHECKMATCH_H

#include <iostream>
#include <vector>
#include "Analysis/ana_base.h"
#include "LArUtil/PxUtils.h"
#include "LArUtil/LArUtilBase.h"
#include "ClusterRecoUtil/Base/CRUHelper.h"
#include "ClusterRecoUtil/Base/ClusterParamsAlg.h"
#include "Quality.h"

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
  /**
     \class PlaneCheckMatch
     Describe me
   */
  class PlaneCheckMatch{

  public:

    /// Default constructor
    PlaneCheckMatch(){};

    /// Default destructor
    virtual ~PlaneCheckMatch(){};

    /**
       Clustering function. Returns a vector of clusters.
       Each cluster is represented as a group of hit via vector of hit indexes.
    */
   
	// what ever I want it to return 
  std::vector<bool> PlaneCheck(const std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>> Regions, const larlite::event_hit* hits  );



 protected:
    cluster::CRUHelper fCRUHelper;
    cluster::Quality fQuality;

  };
}
#endif
/** @} */ // end of doxygen group 
