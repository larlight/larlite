/**
 * \file SPAlgoPi0.h
 *
 * \ingroup ShowerPdfTool
 * 
 * \brief Class def header for a class SPAlgoPi0
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerPdfTool

    @{*/

#ifndef SELECTIONTOOL_SPALGOPI0_H
#define SELECTIONTOOL_SPALGOPI0_H

#include "SPAlgo/SPAlgoEMPart.h"
#include <TH1D.h>
#include <TH2D.h>
namespace sptool {

  /**
     \class SPAlgoPi0
     User custom SPAFilter class made by kazuhiro
   */
  class SPAlgoPi0 : public SPAlgoBase {
  
  public:

    /// Default constructor
    SPAlgoPi0();

    /// Default destructor
    virtual ~SPAlgoPi0(){}

    /// What to do before event-loop begins
    virtual void ProcessBegin();

    /// Override the sptool::SPTBase::LoadParams function
    virtual void LoadParams(std::string fname="",size_t version=kINVALID_SIZE);

    /// Function to evaluate input showers and determine a score
    virtual SPArticleSet Reconstruct(const SPAData &data);

    /// Function executed at end of process
    virtual void ProcessEnd(TFile* fout);

    /// Likelihood of two showers being pi0 daughters
    void LL(const SPAShower& shower_a,
	    const SPAShower& shower_b,
	    double& ll,
	    double& mass);

    /// Select verbosity mode
    void setVerbose(bool on) { _verbose = on; }

    /// Set maximum Impact Parameter for two showers
    void setIPMax(double ip) { _ip_max = ip; }

    /// Set maximum Opening Angle for two showers
    void setAngleMax(double angle) { _angle_max = angle; }

    /// Set minimum shower energy
    void setMinShrEnergy(double E) { _energy_min = E; }

    /// Set maximum shower energy
    void setMaxShrEnergy(double E) { _energy_max = E; }

    /// Set minimum fit value
    void setMinFitMass(double m) { _fit_min = m; }

    /// Set maximum fit value
    void setMaxFitMass(double m) { _fit_max = m; }

  protected:

    /// verobsity boolean for couts
    bool _verbose;

    // Variables for Fit range [MeV/c]
    double _fit_min, _fit_max;

    SPAlgoEMPart  _alg_emp;
    // Info for Mass Peak PDF
    RooAbsPdf   *_pi0_pdf;
    RooRealVar  *_pi0_massVar, *_pi0_massMean, *_pi0_massSigma;
    RooDataSet  *_pi0_massData;
    RooGaussian *_pi0_massPdf;
    double _pi0_mean, _pi0_mean_min, _pi0_mean_max; // [MeV/c]
    double _pi0_sigma, _pi0_sigma_min, _pi0_sigma_max; // [MeV/c]
    // Info for RadLen Correlation PDF
    RooAbsPdf   *_shrCorrelation_pdf;
    RooRealVar  *_radLenVar, *_radLenVal;
    RooDataSet  *_radLenData;
    double _x, _x_min, _x_max; // [CM]
    double _radLen, _radLen_min, _radLen_max; // [ CM-1]
    

    double _energy_min,   _energy_max;
    double _angle_min,    _angle_max;
    double _ip_max;
 

    TH2D *_hMass_vs_LL;
    TH1D *_hMass;
    TH1D *_hBestMass;

    // Tree for candidate pi0 events
    TTree* _candidate_tree;
    double _shr1_E;
    double _shr2_E;
    double _angle;
    double _mass;
    double _shr1_dEdx;
    double _shr2_dEdx;
    double _dist_1;   // distance between the vertex candidate & shr1 start
    double _dist_2;   // distance between the vertex candidate & shr2 start
    double _vtx_dist; // Distance between two points on shr1 & shr2 axes that define the IP
    double _ll;

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
