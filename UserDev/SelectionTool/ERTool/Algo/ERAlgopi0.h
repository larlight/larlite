/**
 * \file ERAlgopi0.h
 *
 * \ingroup Algo
 * 
 * \brief Class def header for a class ERAlgopi0
 *
 * @author Ryan Grosso
 */

/** \addtogroup Algo

    @{*/

#ifndef ERTOOL_ERALGOPI0_H
#define ERTOOL_ERALGOPI0_H

#include "ERTool/Base/AlgoBase.h"
#include "ERTool/Algo/AlgoEMPart.h"
#include "EMShowerTools/EMShowerProfile.h"

#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <RooGaussian.h>
#include "GeoAlgo/GeoAlgo.h"


namespace ertool {

  /**
     \class ERAlgopi0
     User custom Algorithm class made by kazuhiro
   */
  class ERAlgopi0 : public AlgoBase {
  
  public:

    /// Default constructor
    ERAlgopi0(const std::string& name="ERAlgopi0");

    /// Default destructor
    virtual ~ERAlgopi0(){};

    /// Reset function
    void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    bool Reconstruct(const EventData &data, ParticleGraph& graph);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);

    /// Likelihood of two showers being pi0 daughters
    void LL(const Shower& shower_a,
            const Shower& shower_b,
            double& ll,
            double& mass,
            geoalgo::Point_t& vertex,
            geoalgo::Vector_t& momentum);

    /// Set minimum shower energy
    void setMinShrEnergy(double E) { _energy_min = E; }

    /// Set maximum shower energy
    void setMaxShrEnergy(double E) { _energy_max = E; }



	protected:


    /// GeoAlgo instance for geometric computations
    geoalgo::GeoAlgo _geoAlgo;

    // algo em part for looking at shower type
    AlgoEMPart  _alg_emp;

    double _energy_min =0;
    double _energy_max =0;


    // TTree info 
    TTree *_ll_tree;
    double _dedx_A, _dedx_B;
    double _vtxDist_A, _vtxDist_B;
    double _vtx_IP;

    double _ll_photon_A, _ll_photon_B;

    double _ll;
    double _angle;
    double _E_A, _E_B;
    double _mass;
    double _dot;
	

  };
}
#endif

/** @} */ // end of doxygen group 
