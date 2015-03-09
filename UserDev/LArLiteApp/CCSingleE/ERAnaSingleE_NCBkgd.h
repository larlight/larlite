/**
 * \file ERAnaSingleE_NCBkgd.h
 *
 * \ingroup CCSingleE
 * 
 * \brief Class def header for a class ERAnaSingleE_NCBkgd
 *
 * @author jzennamo
 */

/** \addtogroup CCSingleE

    @{*/

#ifndef ERTOOL_ERANASINGLEE_NCBKGD_H
#define ERTOOL_ERANASINGLEE_NCBKGD_H

#include "ERTool/Base/AnaBase.h"
#include "TTree.h"
#include "TH1D.h"

#include "DataFormat/mctruth.h"

namespace ertool {

  /**
     \class ERAnaSingleE_NCBkgd
     User custom Analysis class made by kazuhiro
   */
  class ERAnaSingleE_NCBkgd : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaSingleE_NCBkgd();

    /// Default destructor
    virtual ~ERAnaSingleE_NCBkgd(){}

    /// Reset function
    virtual void Reset();

    /// Called @ before processing the first event sample
    virtual void ProcessBegin(){}

    /// Function to evaluate input showers and determine a score
    virtual bool Analyze(const EventData &data, const ParticleSet &ps);

    /// Called after processing the last event sample
    virtual void ProcessEnd(TFile* fout=nullptr) {}

        /// Function to re-set TTree variables
    void ResetTreeVariables();

    /// Function to save efficiency vs. variable to histogram
    //    void MakeEffPlot(std::string varname,
    //		     int nbins, double xmin, double xmax);


  private:
    
    // Result tree comparison for reconstructed events 
    TTree* _result_tree;

    // True Event 
    int _n_showers;
    int _n_gamma;
    int _n_elec;

    int _n_track; 

    int _n_protons;
    int _n_neutrons;
    int _n_piplus;
    int _n_pi0;

    double _e_nu; 
    double _e_lep_candidate;
    double _e_mom_lep_candidate;

    int _pdg_mom_lep_candidate; 
    int _pdg_lep_candidate;

    double _vtxdis_lep_candidate;    
    double _showerL_lep_candidate;

    //Reco Event
    int _n_showers_RECO;
    int _n_gamma_RECO;
    int _n_elec_RECO;

    int _n_track_RECO; 

    int _n_protons_RECO;
    int _n_neutrons_RECO;
    int _n_piplus_RECO;
    int _n_pi0_RECO;

    double _e_nu_RECO; 
    double _e_lep_candidate_RECO;
    double _e_mom_lep_candidate_RECO;

    int _pdg_mom_lep_candidate_RECO; 
    int _pdg_lep_candidate_RECO;

    double _vtxdis_lep_candidate_RECO;    
    double _showerL_lep_candidate_RECO;

    


  };
}
#endif

/** @} */ // end of doxygen group 
