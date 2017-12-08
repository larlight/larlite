/**
 * \file GammaProfile.h
 *
 * \ingroup GammaProfile
 * 
 * \brief Class def header for a class GammaProfile
 *
 * @author ryan
 */

/** \addtogroup GammaProfile

    @{*/

#ifndef LARLITE_GAMMAPROFILE_H
#define LARLITE_GAMMAPROFILE_H

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
#include "TH3.h"



#include "geoconic.h"
//#include "ctools.h"
//#include "conicalprojection.h"
//#include "coneprofile.h"


#include "ClusterRecoUtil/Base/ClusterParamsAlg.h"
#include "ClusterRecoUtil/Base/CRUHelper.h"
#include "EMShowerTools/EMShowerProfile.h"

namespace larlite {
  /**
     \class GammaProfile
     User custom analysis class made by SHELL_USER_NAME
   */
  class GammaProfile : public ana_base{
  
  public:

    /// Default constructor
    GammaProfile(){ _name="GammaProfile"; _fout=0;}

    /// Default destructor
    virtual ~GammaProfile(){}

    /** IMPLEMENT in GammaProfile.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in GammaProfile.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in GammaProfile.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:


        //::larlite::conicalprojection fconeproj;
        ::larlite::geoconic fgeoconic;
        //::larlite::ctools fctools;
        //::larlite::coneprofile fconeprofile;

//	double AxisLength = 200;// Setting this here... This is the length of the cone
        //double openingangle = 50.0; // magic number place holder for now. 
        double openingangle ; // magic number place holder for now. 
        int smoothness = 16;// would be nice if this was even... but this gives the smoothness of the edge of the polygon cone
	double energy= -999;
	//double angle = 60;// magic number place holder for now. 
	double angle = 60;// magic number place holder for now. 
                bool coneintpc = true;
                double ConeLength = 5*14;
	

	TH2D *fFracL0;
	TH2D *fFracL1;
	TH2D *fFracL2;

	TH3D *f3FracL0;
	TH3D *f3FracL1;
	TH3D *f3FracL2;

	TH1D *fmcEn;
	TH1D *fmcEnGood;
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
