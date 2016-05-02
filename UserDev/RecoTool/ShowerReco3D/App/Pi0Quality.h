/**
 * \file Pi0Quality.h
 *
 * \ingroup ShowerReco3D
 * 
 * \brief Class def header for a class Pi0Quality
 *
 * @author yuntse
 */

/** \addtogroup ShowerReco3D

    @{*/

#ifndef LARLITE_PI0QUALITY_H
#define LARLITE_PI0QUALITY_H

#include <TH1D.h>
#include <TH2D.h>
#include <map>
#include "Analysis/ana_base.h"
#include "MCComp/MCMatchAlg.h"
#include "ShowerReco3D/Base/ShowerRecoException.h"
#include "DataFormat/shower.h"
#include "DataFormat/mcshower.h"

namespace larlite {
  /**
     \class ShowerQuality_oldway
     User custom analysis class made by kazuhiro
   */
  class Pi0Quality : public ana_base{
  
  public:

    /// Default constructor
    Pi0Quality();

    /// Default destructor
    virtual ~Pi0Quality(){};

    /**
       Setter function for a shower producer name.
    */
    void SetShowerProducer(const std::string name)
    { fShowerProducer = name; }

    /// Set maximum energy for MCShowers to be considered
    void SetMaxEnergyCut(const double energy) { _mc_energy_max = energy; }

    /// Set minimum energy for MCShowers to be considered
    void SetMinEnergyCut(const double energy) { _mc_energy_min = energy; }

    /** IMPLEMENT in Pi0Quality.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in Pi0Quality.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in Pi0Quality.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    /// set whether to fill quality info per MC or per reco shower
    void setMCShowerQuality_multishowers(bool on) { _mcShowerQuality = on; }

    void setFillSinglePi0Tree( bool on ) { _fillSinglePi0Tree = on; }

    // use trigger time
    void setUseTrigger(bool on) { _use_trigger = on; }

  protected:

    // Function to fill TTree
    void FillQualityInfo(const shower& reco_shower,
			 const mcshower& mc_shower,
			 const size_t& shower_index, const size_t& best_index,
			 const double& mcq,
			 const AssSet_t& ass_cluster_v);

    // Function to fill TTree (when MCShower info only is available)
    void FillQualityInfo(const mcshower& mc_shower);

    // Function to fill SinglePi0Tree
    void FillSinglePi0Info( shower& reco_shower1, shower& reco_shower2,
                            const mcshower& mc_shower1, const mcshower& mc_shower2,
                            std::vector< size_t >& irecogamma, std::vector< size_t >& imcgamma,
                            std::vector< double >& imcq, const AssSet_t& ass_cluster_v );

    void FillSinglePi0Shower( shower& reco_shower, const mcshower& mc_shower,
                              size_t& shower_index, size_t& mcshower_index,
                              double& mcq, const AssSet_t& ass_cluster_v,
                              const unsigned int i );

    void FillSinglePi0Shower(const mcshower& mc_shower, const unsigned int i);

    double DCAPointToLine( const TVector3& p, const TVector3& c, const TVector3& m );

    // boolean to decide if to fill the tree once per MC shower
    // or once per RECO shower
    bool _mcShowerQuality;

    // whether to analyze single pi0 reconstruction
    bool _fillSinglePi0Tree;

    // use trigger offset?
    bool _use_trigger;

    /// Shower back tracking algorithm
    ::btutil::MCMatchAlg fBTAlg;

    /// Minimum MC shower energy cut
    double _mc_energy_min;

    /// Maximum MC shower energy cut
    double _mc_energy_max;

    /// Shower Producer's Name
    std::string fShowerProducer;

    /// Matching correctness
    TH1D *hMatchCorrectness;

    TH1D *hVtxDX; ///< X difference (reco-MC) in cm
    TH1D *hVtxDY; ///< Y difference (reco-MC) in cm
    TH1D *hVtxDZ; ///< Z difference (reco-MC) in cm
    TH1D *hVtxDR; ///< 3D vtx distance between reco to MC in cm

    TH1D *hDCosX; ///< Direction unit vector X component difference
    TH1D *hDCosY; ///< Direction unit vector Y component difference
    TH1D *hDCosZ; ///< Direction unit vector Z component difference
    TH1D *h3DAngleDiff; ///< Opening angle between reco & MC 3D direction

    TH2D *hEnergyCorr; ///< Energy correlation reco (x) vs. MC (y)

    TH1D *hEnergyAssym; ///< Energy assym. parameter: (reco E - MC E) / (reco E + MC E) * 2
    TH1D *hEnergyDiff;  ///< Energy difference: reco E - MC E
    
    TH1D *hMatchedClusterEff; ///< Matched 3D shower's cluster efficiency (combined across planes)
    TH1D *hMatchedClusterPur; ///< Matched 3D shower's cluster purity (combined across planes)

    /// dEdx per particle per PDG code
    std::map<int,TH1D*> mDEDX;

    /// Best plane id
    TH1D *hBestPlane;

    /// event variables
    int _event, _subrun, _run;

    /// For convenience: struct to define a set of parameters per shower to be stored in TTree
    struct TreeParams_t {

      double reco_x, reco_y, reco_z;
      double reco_dcosx, reco_dcosy, reco_dcosz;
      double reco_energy;
      double reco_energy_U;
      double reco_energy_V;
      double reco_energy_Y;
      double reco_dedx;
      double reco_dqdx;
      double reco_dqdx_U;
      double reco_dqdx_V;
      double reco_dqdx_Y;
      double reco_dedx_U;
      double reco_dedx_V;
      double reco_dedx_Y;
      int    best_plane_id;

      // is there a reconstructed match?
      int    match;

      double mc_x, mc_y, mc_z, mc_t;
      double mc_q, mc_q_bt;
      double mc_dcosx, mc_dcosy, mc_dcosz;
      double mc_energy;
      int    mc_pdgid;

      double mc_reco_anglediff;
      double mc_reco_dist;

      double mc_containment;
      double match_correctness;
      double cluster_eff;
      double cluster_pur;

    } fTreeParams;

    struct SinglePi0TreeParams_t {

      int n_mcshowers;
      int n_recoshowers;
      //Detprofile energy of the FIRST (for now, only) mcshower
      double MCPi0Mass;
      double DepERecoThetaPi0Mass;
      double RecoEMCThetaPi0Mass;
      double PerfectRecoPi0Mass;
      double MCCosTheta;
      double MCTheta;
      double RecoPi0Mass;
      double RecoCosTheta;
      double RecoTheta;
      double mc_start_dist;
      double mcv_reco_dca[2];
      double recov_mc_dca[2];

      double reco_x[2], reco_y[2], reco_z[2];
      double reco_dcosx[2], reco_dcosy[2], reco_dcosz[2];
      double reco_energy[2];
      double reco_energy_U[2];
      double reco_energy_V[2];
      double reco_energy_Y[2];
      double reco_dedx[2];
      double reco_dqdx[2];
      double reco_dqdx_U[2];
      double reco_dqdx_V[2];
      double reco_dqdx_Y[2];
      double reco_dedx_U[2];
      double reco_dedx_V[2];
      double reco_dedx_Y[2];
      int    best_plane_id[2];

      // is there a reconstructed match?
      int    match[2];

      double mc_x[2], mc_y[2], mc_z[2], mc_t[2];
      double dep_x[2], dep_y[2], dep_z[2], dep_t[2];
      double mc_q[2], mc_q_bt[2];
      double mc_dcosx[2], mc_dcosy[2], mc_dcosz[2];
      double dep_dcosx[2], dep_dcosy[2], dep_dcosz[2];
      double mc_energy[2];
      double dep_energy[2];
      int    mc_pdgid[2];

      double mc_reco_anglediff[2];
      double mc_reco_dist[2];
      double dep_reco_dist[2];

      double mc_containment[2];
      double match_correctness[2];
      double cluster_eff[2];
      double cluster_pur[2];

    } fSinglePi0TreeParams;

    /// Analysis TTree
    TTree *fTree;
    TTree *fSinglePi0Tree;

    /// Function to prepare TTree
    void InitializeAnaTree();
    void InitializeSinglePi0Tree();

    int _numRecoShowers = 0;

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
