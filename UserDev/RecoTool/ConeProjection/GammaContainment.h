/**
 * \file GammaContainment.h
 *
 * \ingroup GammaContainment
 *
 * \brief Class def header for a class GammaContainment
 *
 * @author ryan
 */

/** \addtogroup GammaContainment
 
 @{*/

#ifndef LARLITE_GAMMACONTAINMENT_H
#define LARLITE_GAMMACONTAINMENT_H

#include "Analysis/ana_base.h"
#include "LArUtil/LArUtilManager.h"
#include "LArUtil/PxUtils.h"
#include "LArUtil/LArUtilBase.h"
#include "LArUtil/LArUtil-TypeDef.h"
#include "LArUtil/TimeService.h"

#include "DataFormat/DataFormat-TypeDef.h"
#include "DataFormat/hit.h"
#include "DataFormat/cluster.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/data_base.h"

#include <math.h>       /* aTan */
#define PI 3.14159265358979323846264338327950288419
#include <stdlib.h>     /* srand, rand */

#include "TH1.h"
#include "TH2.h"


#include "geoconic.h"
//#include "ctools.h"
//#include "conicalprojection.h"
//#include "coneprofile.h"


#include "ClusterRecoUtil/Base/ClusterParamsAlg.h"
#include "ClusterRecoUtil/Base/CRUHelper.h"
#include "EMShowerTools/EMShowerProfile.h"

namespace larlite {
  /**
   \class GammaContainment
   User custom analysis class made by SHELL_USER_NAME
   */
  class GammaContainment : public ana_base{
    
  public:
    
    /// Default constructor
    GammaContainment(){ _name="GammaContainment"; _fout=0;}
    
    /// Default destructor
    virtual ~GammaContainment(){}
    
    /** IMPLEMENT in GammaContainment.cc!
     Initialization method to be called before the analysis event loop.
     */
    virtual bool initialize();
    
    /** IMPLEMENT in GammaContainment.cc!
     Analyze a data event-by-event
     */
    virtual bool analyze(storage_manager* storage);
    
    /** IMPLEMENT in GammaContainment.cc!
     Finalize method to be called after all events processed.
     */
    virtual bool finalize();
    
  protected:
    
    
    //::larlite::conicalprojection fconeproj;
    ::larlite::geoconic fgeoconic;
    //::larlite::ctools fctools;
    //::larlite::coneprofile fconeprofile;
    
    double angle = 60.0; // magic number place holder for now.
    double RadLength = 14.0;
    int smoothness = 16;// would be nice if this was even... but this gives the smoothness of the edge of the polygon cone
    double ConeLength = 52;
    
    TH1D *fA30_L3_P0;
    TH1D *fA30_L3_P1;
    TH1D *fA30_L3_P2;
    TH1D *fA30_L4_P0;
    TH1D *fA30_L4_P1;
    TH1D *fA30_L4_P2;
    TH1D *fA30_L5_P0;
    TH1D *fA30_L5_P1;
    TH1D *fA30_L5_P2;
    
    TH1D *fA45_L3_P0;
    TH1D *fA45_L3_P1;
    TH1D *fA45_L3_P2;
    TH1D *fA45_L4_P0;
    TH1D *fA45_L4_P1;
    TH1D *fA45_L4_P2;
    TH1D *fA45_L5_P0;
    TH1D *fA45_L5_P1;
    TH1D *fA45_L5_P2;
    
    TH1D *fA60_L3_P0;
    TH1D *fA60_L3_P1;
    TH1D *fA60_L3_P2;
    TH1D *fA60_L4_P0;
    TH1D *fA60_L4_P1;
    TH1D *fA60_L4_P2;
    TH1D *fA60_L5_P0;
    TH1D *fA60_L5_P1;
    TH1D *fA60_L5_P2;
    
    ////////
    TH2D *fA30_L3_P0E;
    TH2D *fA30_L3_P1E;
    TH2D *fA30_L3_P2E;
    TH2D *fA30_L4_P0E;
    TH2D *fA30_L4_P1E;
    TH2D *fA30_L4_P2E;
    TH2D *fA30_L5_P0E;
    TH2D *fA30_L5_P1E;
    TH2D *fA30_L5_P2E;
    
    TH2D *fA45_L3_P0E;
    TH2D *fA45_L3_P1E;
    TH2D *fA45_L3_P2E;
    TH2D *fA45_L4_P0E;
    TH2D *fA45_L4_P1E;
    TH2D *fA45_L4_P2E;
    TH2D *fA45_L5_P0E;
    TH2D *fA45_L5_P1E;
    TH2D *fA45_L5_P2E;
    
    TH2D *fA60_L3_P0E;
    TH2D *fA60_L3_P1E;
    TH2D *fA60_L3_P2E;
    TH2D *fA60_L4_P0E;
    TH2D *fA60_L4_P1E;
    TH2D *fA60_L4_P2E;
    TH2D *fA60_L5_P0E;
    TH2D *fA60_L5_P1E;
    TH2D *fA60_L5_P2E;
    
    
    void InitializeAnaTree();
    
    TTree *FullTree;
    
    double cp0;
    double cp1;
    double cp2;
    double mcsdeposit0;
    double mcsdeposit1;
    double mcsdeposit2;
    double cont0;
    double cont1;
    double cont2;
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
