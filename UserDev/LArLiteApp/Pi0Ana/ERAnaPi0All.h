/**
 * \file ERAnaPi0All.h
 *
 * \ingroup Pi0Ana
 * 
 * \brief Class def header for a class ERAnaPi0All
 *
 * @author ah673
 */

/** \addtogroup Pi0Ana

    @{*/

#ifndef ERTOOL_ERANAPI0ALL_H
#define ERTOOL_ERANAPI0ALL_H

#include "ERTool/Base/AnaBase.h"
#include "ERTool/Base/AnaBase.h"
#include "ERTool/Base/ParticleGraph.h"
#include "ERTool/Base/Particle.h"
#include "ERTool/Base/EventData.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2F.h"
#include <math.h>       /* cos */
#include "DataFormat/mctruth.h"

namespace ertool {

  /**
     \class ERAnaPi0All
     User custom Analysis class made by kazuhiro
   */
  class ERAnaPi0All : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaPi0All(const std::string& name="ERAnaPi0All");

    /// Default destructor
    virtual ~ERAnaPi0All(){}

    /// Reset function
    virtual void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    bool Analyze(const EventData &data, const ParticleGraph &ps);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);

    void PrepareTTree();

    void SetVerbose( bool verbose ) { _verbose = verbose ; }

 protected:
    
    TTree * _pi0_tree ;

    int _nPi0 	 ;
    int _nMeson	 ;
    int _nProton ;

    //Examine 3 cases: 
    //0) CC pi0s (pi0 + muon)
    //1) NC pi0 inclusive (pi0 + pi+/-)
    //2) NC single pi0 (pi0)
    int _nCCPi0    ; 
    int _nNCIncPi0 ;
    int _nNC1Pi0   ;
    int _nEvents   ;

    int _NC_ctr ;
    int _CC_ctr ;
    int _NC1pi0_ctr ;
    double _eff ;

    //Look at vertices of pi0 and track siblings
    double _x_pi0 ;
    double _y_pi0 ;
    double _z_pi0 ;
    double _vtx_dist ;
    double _x_track ;
    double _y_track ;
    double _z_track ;

    bool _verbose ;


  };
}
#endif

/** @} */ // end of doxygen group 
