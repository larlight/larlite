/**
 * \file ERAna1PI0.h
 *
 * \ingroup Pi0Ana
 * 
 * \brief Class def header for a class ERAna1PI0
 *
 * @author ryan
 */

/** \addtogroup Pi0Ana

    @{*/

#ifndef ERTOOL_ERANA1PI0_H
#define ERTOOL_ERANA1PI0_H

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
     \class ERAna1PI0
     User custom Analysis class made by kazuhiro
   */
  class ERAna1PI0 : public AnaBase {
  
  public:

    /// Default constructor
    ERAna1PI0();

    /// Default destructor
    virtual ~ERAna1PI0(){};

    /// Reset function
    virtual void Reset();

    /// Called @ before processing the first event sample
//    virtual void ProcessBegin(){}
    void SetDebug(bool on) { _debug = on; }

    /// Function to evaluate input showers and determine a score
    //virtual bool Analyze(const EventData &data, const ParticleSet &ps);
    virtual bool Analyze(const EventData &data,  const ParticleGraph &graph);

    /// Called after processing the last event sample
     void ProcessEnd(TFile* fout) ;
    /// Function to re-set TTree variables
    void ResetTreeVariables();


    // Tree that has reco pi0 in it.
	// If reco fill out the info. 
		//If there is an actual pi0 then compare it
	
   private:

    bool _debug;

    TTree* _pi0_reco_tree;
    TTree* _pi0_tree;
    TTree* _1pi0_tree;
    TTree* _mpi0_tree;
	
    double _x_pi0_Reco, _y_pi0_Reco, _z_pi0_Reco; /// pi0 x,y,z start point
    double _px_pi0_Reco, _py_pi0_Reco, _pz_pi0_Reco , _p_pi0_Reco; /// pi0 px,py,pz,pmag
    double _mass_pi0_Reco; // reco mass of pi0

    double _x_shra_Reco, _y_shra_Reco, _z_shra_Reco; /// showera x,y,z start point
    double _px_shra_Reco, _py_shra_Reco, _pz_shra_Reco, _p_shra_Reco; /// showera momentum
    double _e_shra_Reco;
    double _x_shrb_Reco, _y_shrb_Reco, _z_shrb_Reco; /// showera x,y,z start point
    double _px_shrb_Reco, _py_shrb_Reco, _pz_shrb_Reco, _p_shrb_Reco; /// showera momentum
    double _e_shrb_Reco;

    double _shr_a_dot_b_Reco; // cos of showers











    double _x_gamma_Reco, _y_gamma_Reco, _z_gamma_Reco; /// lepton x,y,z start point
    double _x_gamma_MC, _y_gamma_MC, _z_gamma_MC; /// lepton x,y,z start point

    double _x_pi0_Reco, _y_pi0_Reco, _z_pi0_Reco; /// lepton x,y,z start point
    double _x_pi0_MC, _y_pi0_MC, _z_pi0_MC; /// lepton x,y,z start point

    double _e_gamma_Reco;
    double _e_gamma_MC ;

    double _e_pi0_Reco ;
    double _e_pi0_MC ;

    double _px_gamma_Reco, _py_gamma_Reco, _pz_gamma_Reco; /// lepton x,y,z start point
    double _px_gamma_MC, _py_gamma_MC, _pz_gamma_MC; /// lepton x,y,z start point

    double _theta_gamma_MC, _phi_gamma_MC;
    double _theta_gamma_Reco, _phi_gamma_Reco;

    double _mass_pi0_Reco;
    double _mass_pi0_MC ;

    double _vtx_dist;


    int ct ;

  };
}
#endif

/** @} */ // end of doxygen group 
