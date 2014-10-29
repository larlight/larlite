/**
 * \file ClusterParamsAlg.h
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class ClusterParamsExecutor
 *
 * @author andrzejs
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef RECOTOOL_CLUSTERPARAMSALG_H
#define RECOTOOL_CLUSTERPARAMSALG_H

#include <TMath.h>
#include <TH2F.h>
#include <TF1.h>
#include <TH1F.h>
#include <TPrincipal.h>
#include <TCanvas.h>
#include <TLegend.h>

#include "LArUtil/LArUtilManager.h"
#include "LArUtil/PxUtils.h"
#include "ClusterParams.h"
#include "CRUException.h"
#include "FANN/FANNService.h"

#include <vector>
#include <map>

#include "DataFormat/hit.h"
#include "LArUtil/LArUtil-TypeDef.h"
//#include "HoughBaseAlg.hh"
#include "TPrincipal.h"
#include "TStopwatch.h"

namespace cluster {
   
  class ClusterParamsAlg {

  public:

    /// Default constructor
    ClusterParamsAlg();

    /// Alternative constructor with larlight's hits
    ClusterParamsAlg(const std::vector<const larlite::hit*>&);

    /// Alternative constructor with larutil::PxHit vector
    ClusterParamsAlg(const std::vector<larutil::PxHit>&);

    ~ClusterParamsAlg(){};

    void Initialize();

    //void SetHits(const std::vector<larutil::PxHit*>&);

    void SetMinNHits(size_t nhit) { fMinNHits = nhit; }

    size_t MinNHits() const { return fMinNHits; }
    
    int SetHits(const std::vector<const larlite::hit*> &);

    int SetHits(const std::vector<larutil::PxHit> &);

    void SetRefineDirectionQMin(double qmin){ fQMinRefDir = qmin; }

    void SetVerbose(bool yes=true){ verbose = yes;}

    void SetArgoneutGeometry();

    void Report();

    void TimeReport() const;

    /**
     * This function returns a feature vector suitable for a neural net
     * This function uses the data from cluster_params but packages it
     * up in a different way, and so is inappropriate to include in 
     * clusterParams.hh.   That's why it's here.
     * @param  data  takes a reference to a vector< float>
     */
    void  GetFANNVector(std::vector<float> & data);
    // std::vector<float> & GetFANNVector();

    /**
     * For debugging purposes, prints the result of GetFANNVector
     * in a nicely formatted form.
     * @return [description]
     */
    void  PrintFANNVector();


    /**
      Runs all the functions which calculate cluster params                        
      and stashes the results in the private ClusterParams                         
      struct.                                                                      
                                                                                   
      @param override_DoGetAverages       force re-execution of GetAverages()      
      @param override_DoGetRoughAxis      force re-execution of GetRoughAxis()     
      @param override_DoGetProfileInfo    force re-execution of GetProfileInfo()   
      @param override_DoRefineStartPoints force re-execution of RefineStartPoints()
      @param override_DoGetFinalSlope     force re-execution of GetFinalSlope()    
    */
    void FillParams(bool override_DoGetAverages      =false,
                    bool override_DoGetRoughAxis     =false,
                    bool override_DoGetProfileInfo   =false,
                    bool override_DoRefineStartPointsAndDirection=false,
            		    // bool override_DoRefineDirection  =false,
                    bool override_DoGetFinalSlope    =false,
                    bool override_DoTrackShowerSep   =false );

    const cluster_params& GetParams() const
    { return fParams;}

    /**
       Calculates the following variables:
       mean_charge
       mean_x
       mean_y
       charge_wgt_x
       charge_wgt_y
       eigenvalue_principal
       eigenvalue_secondary
       multi_hit_wires
       N_Wires
       @param override force recalculation of variables
    */
    void GetAverages(bool override=false);


    /**
      Calculates the following variables:
      verticalness
      fRough2DSlope
      fRough2DIntercept
      @param override [description]
    */
    //void GetRoughAxis(bool override=false);
    void GetRoughAxis(bool override=false);


    /**
       Calculates the following variables:
       opening_angle
       opening_angle_highcharge
       closing_angle
       closing_angle_highcharge
       offaxis_hits
       @param override [description]
    */
    void GetProfileInfo(bool override=false);


    /**
       Calculates the following variables:
       length
       width
       @param override [description]
    */
    void RefineStartPoints(bool override=false);

    /**
       Calculates the following variables:
       hit_density_1D
       hit_density_2D
       angle_2d
       direction
       @param override [description]
    */
    void GetFinalSlope(bool override=false);

    void RefineDirection(bool override=false);

    void RefineStartPointAndDirection(bool override=false);

    void TrackShowerSeparation(bool override=false);

    void setNeuralNetPath(std::string s){fNeuralNetPath = s;}

    void FillPolygon();

    void GetOpeningAngle();

    const larutil::PxPoint& RoughStartPoint() {return fRoughBeginPoint;}
    const larutil::PxPoint& RoughEndPoint() {return fRoughEndPoint;}

    double RoughSlope() {return fRough2DSlope;}
    double RoughIntercept() {return fRough2DIntercept;}
    
    void EnableFANN();

    void DisableFANN(){enableFANN = false;}

    size_t GetNHits() const {return fHitVector.size();}
    const std::vector<larutil::PxHit>& GetHitVector() const {return fHitVector;}
    int Plane() const {return fPlane;}
    
  protected:
    
    larutil::GeometryUtilities  *fGSer;

    /// Cut value for # hits: below this value clusters are not evaluated
    size_t fMinNHits;

    /**
       This vector holds the pointer to hits. 
       This should be used for computation for speed.
    */
    std::vector<larutil::PxHit> fHitVector;

    // bool to control debug/verbose mode
    // defaults to off.
    bool verbose;

    //settable parameters:
    std::vector<double> fChargeCutoffThreshold;
    int fPlane;

    //this is required in RefineDirection
    double fQMinRefDir;
    
    std::vector< double > fChargeProfile;
    std::vector< double > fCoarseChargeProfile;
    
    std::vector< double > fChargeProfileNew;
   // double fMaxLinLength;
   // double fLinBins;
    
    int fCoarseNbins;
    int fProfileNbins;
    int fProfileMaximumBin;
    double fProfileIntegralForward;
    double fProfileIntegralBackward;
    double fProjectedLength;
    
    //extreme intercepts using the rough_2d_slope
   // double fInterHigh;
   // double fInterLow;
    double fBeginIntercept;
    double fEndIntercept;
    double fInterHigh_side;
    double fInterLow_side;
    
    // book keeping variables to validate completion of methods:
    bool fFinishedGetAverages;       
    bool fFinishedGetRoughAxis;      
    bool fFinishedGetProfileInfo;    
    bool fFinishedRefineStartPoints; 
    bool fFinishedRefineDirection;   
    bool fFinishedGetFinalSlope;     
    bool fFinishedRefineStartPointAndDirection;
    bool fFinishedTrackShowerSep;

    double fRough2DSlope;        // slope 
    double fRough2DIntercept;    // slope 
    larutil::PxPoint fRoughBeginPoint;
    larutil::PxPoint fRoughEndPoint;
    bool enableFANN;

    public:

    cluster::cluster_params fParams;

    std::string fNeuralNetPath;

    std::vector<std::string> fTimeRecord_ProcName;
    std::vector<double> fTimeRecord_ProcTime;

  }; //class ClusterParamsAlg
  
} //namespace cluster

/** @} */ // end of doxygen group

#endif
