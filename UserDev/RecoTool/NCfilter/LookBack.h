/**
 * \file LookBack.h
 *
 * \ingroup NCfilter
 * 
 * \brief Class def header for a class LookBack
 *
 * @author ryan
 */

/** \addtogroup NCfilter

    @{*/

#ifndef RECOTOOL_LOOKBACK_H
#define RECOTOOL_LOOKBACK_H

#define PI 3.14159265

#include <iostream>
#include <vector>
#include "Analysis/ana_base.h"
#include "LArUtil/PxUtils.h"
#include "LArUtil/LArUtilBase.h"
#include "ClusterRecoUtil/CRUHelper.h"
#include "ClusterRecoUtil/ClusterParamsAlg.h"

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
     \class LookBack
     Describe me!
  */
  class LookBack{

  public:

    /// Default constructor
    LookBack(){};

    /// Default destructor
    virtual ~LookBack(){};

    /**
       Clustering function. Returns a vector of clusters.
       Each cluster is represented as a group of hit via vector of hit indexes.
    */
   
	// what ever I want it to return 
//  std::vector<bool> PlaneCheck(const std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>> Regions, const larlite::event_hit* hits  );
  std::pair<double,double> CWAngle(std::vector<unsigned int> hitindex, std::vector<larlite::hit> hitsvect);

  std::pair<std::vector<unsigned int>,std::vector<unsigned int>> ClusterRefine(std::pair<std::vector<unsigned int>,std::vector<unsigned int>> hitpair, std::vector<larlite::hit> hitsvect);


 protected:
        //cluster::CRUHelper fCRUHelper;
        //cluster::Quality fQuality;

	double radius_of_interest=2; 





  };
}
#endif
/** @} */ // end of doxygen group 
