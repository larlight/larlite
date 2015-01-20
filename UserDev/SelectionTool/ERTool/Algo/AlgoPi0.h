/**
 * \file AlgoPi0.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class AlgoPi0
 *
 * @author kazuhiro
 */

/** \addtogroup ERTool

    @{*/

#ifndef ERTOOL_ALGOPI0_H
#define ERTOOL_ALGOPI0_H

#include "Algo/AlgoEMPart.h"
#include <TH1D.h>
#include <TH2D.h>
#include <RooGaussian.h>

namespace ertool {

  /**
     \class AlgoPi0
     User custom Algorithm class made by kazuhiro
   */
  class AlgoPi0 : public AlgoBase {
  
  public:

    /// Default constructor
    AlgoPi0();

    /// Default destructor
    virtual ~AlgoPi0(){}

    /// Called only once by the constructor in its lifespan
    void SetDefaultParams();

    /// What to do before event-loop begins
    virtual void ProcessBegin();

    /// Override the sptool::SPTBase::LoadParams function
    virtual void LoadParams(std::string fname="",size_t version=kINVALID_SIZE);

    /// Function to evaluate input showers and determine a score
    virtual ParticleSet Reconstruct(const EventData &data);

    /// Function executed at end of process
    virtual void ProcessEnd(TFile* fout);

    /// Likelihood of two showers being pi0 daughters
    void LL(const Shower& shower_a,
	    const Shower& shower_b,
	    double& ll,
	    double& mass,
	    geoalgo::Point_t& vertex,
	    geoalgo::Vector_t& momentum);

    /// Select verbosity mode
    void setVerbose(bool on) { _verbose = on; }

    /// Set maximum Impact Parameter for two showers
    void setIPMax(double ip) { _IPMax = ip; }

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

    PdfFactory _factory; ///< P.D.F. factory class instance

    /// verobsity boolean for couts
    bool _verbose;

    // Variables for Fit range [MeV/c]
    double _fit_min, _fit_max;

    AlgoEMPart  _alg_emp;

    // Info for RadLen Correlation PDF
    RooRealVar  *_radLenVar;
    RooAbsPdf   *_radLenSigl;
    RooAbsPdf   *_radLenBkgd;
    RooDataSet  *_radLenData;
    double _x, _x_min, _x_max; // [CM]

    // Info for Vertex Impact Parameter PDF
    RooRealVar *_vtxVar;
    RooAbsPdf  *_vtxSigl;
    RooAbsPdf  *_vtxBkgd;
    double     _vtx_err;

    double _energy_min,   _energy_max;
    double _angle_min,    _angle_max;
    double _vtxDistMax;
    double _IPMax;
 
    // Histograms
    TH2D *_hMass_vs_LL;
    TH1D *_hMass;
    TH1D *_hBestMass;

    // Tree for LL return values
    TTree *_ll_tree;
    double _dedx_A, _dedx_B;
    double _ll_vtx, _ll_dedx_A, _ll_dedx_B;
    double _vtx_IP;
    double _vtxDist_A, _vtxDist_B, _ll_vtxDist_A, _ll_vtxDist_B;
    double _ll;
    double _angle;
    double _E_A, _E_B;
    double _mass;

  };
}
#endif

/** @} */ // end of doxygen group 
