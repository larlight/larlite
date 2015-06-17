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

#include "ERTool/Algo/AlgoEMPart.h"
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <RooGaussian.h>
#include "GeoAlgo/GeoAlgo.h"

namespace ertool {

  /**
     \class AlgoPi0
     @brief Algorithm to reconstruct Pi0 events using a Log-Likelihood function.
     ertool::AlgoPi0 aims at finding Pi0 events. Given a list of showers and tracks, \n
     the algorithm takes every combination of shower-pairs and calculates a          \n
     Log-Likelihood that the two showers come from a Pi0. If this is succesfull a Pi0\n
     ertool::Particle object is created.                                             \n
     The LL value is calculated as follows:                                          \n
     1) LL += likelihood shower A is gamma-like                                      \n
     2) LL += likelihood shower B is gamma-like                                      \n
     3) LL += likelihood Pi0 vertex is "correct"                                     \n
     4) LL += likelyhood distance between vertex and shower A start comes from an    \n
        exponential distribution with decay constant = radiation length              \n
     5) LL += likelyhood distance between vertex and shower B start comes from an    \n
        exponential distribution with decay constant = radiation length              \n
                                                                                     \n
     1) & 2) AlgoEMPart is used to calculate the likelyhood.                         \n
                                                                                     \n
     3) the Pi0 vertex is found by tracing back the two shower direction from their  \n
     start point and finding the Impact Parameter between these two lines. This is   \n
     the closest distance between the two segments. Because we are working with      \n
     reconstructed quantities we do not expect these two lines to perfectly          \n
     intersect, but, if we are dealing with an actual Pi0 event, they will come      \n
     within a few cm of each other. What that number actually is (and the statistical\n
     distribution followed by this reconstructed quantity for many events) depends on\n
     our reconstruction capabilities and position/angular resolution. Currently this \n
     distribution is modeled to be a gaussian with a few-cm width. Background events \n
     are modeled to follow a flat distribution. For low values of the Impact         \n
     Parameter the signal PDF is larger than that of the Background, and vice-versa  \n
     for large values. This model can definitely be improved but for now it          \n
     accomplishes the basic goal: the smaller the impact parameter the greater the   \n
     likelyhood we assign to the event being a Pi0.                                  \n
                                                                                     \n
    4) & 5) Done similarly to point 3). The signal here is modeled by an exponential \n
    decay with decay constant equal to the radiation length. The background is a     \n
    flat distribution. The goal of these two steps is to recognize that a shower that\n
    is found to be 5 meters from the reconstructed vertex is probably not from a Pi0.\n
    As above, the moel - especially for background - can be improved.                \n
   */
  class AlgoPi0 : public AlgoBase {
  
  public:

    /// Default constructor
    AlgoPi0(const std::string& name="Pi0");

    /// Default destructor
    ~AlgoPi0(){}

    /// Called only once by the constructor in its lifespan
    void SetDefaultParams();

    void Reset(){}

    /// What to do before event-loop begins
    void ProcessBegin();

    /// Configuration setter
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Function to evaluate input showers and determine a score
    bool Reconstruct(const EventData &data, ParticleGraph& graph);

    /// Function executed at end of process
    void ProcessEnd(TFile* fout);

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

    /// GeoAlgo instance for geometric computations
    geoalgo::GeoAlgo _geoAlgo;

    /// verobsity boolean for couts
    bool _verbose;

    /// pi0 lifetime constant
    double _tau;

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
    double _dot; ///< vtx = reco vertex. sA, sB = start points of showers. dA, dB direction of showers. _dot = [(sA-vtx)*dA] x [(sB-vtx)*dB] 

  };
}
#endif

/** @} */ // end of doxygen group 
