/**
 * \file ShowerQuality.h
 *
 * \ingroup ShowerReco3D
 * 
 * \brief Class def header for a class ShowerQuality
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerReco3D

    @{*/

#ifndef LARLITE_SHOWERQUALITY_H
#define LARLITE_SHOWERQUALITY_H

#include <TH1D.h>
#include <TH2D.h>
#include <map>
#include "Analysis/ana_base.h"
#include "MCComp/MCMatchAlg.h"
#include "ShowerRecoException.h"

namespace larlite {
  /**
     \class ShowerQuality
     User custom analysis class made by kazuhiro
   */
  class ShowerQuality : public ana_base{
  
  public:

    /// Default constructor
    ShowerQuality();

    /// Default destructor
    virtual ~ShowerQuality(){};

    /**
       Setter function for a shower producer name.
    */
    void SetShowerProducer(const std::string name)
    { fShowerProducer = name; }

    /// Set maximum energy for MCShowers to be considered
    void SetMaxEnergyCut(const double energy) { _mc_energy_max = energy; }

    /// Set minimum energy for MCShowers to be considered
    void SetMinEnergyCut(const double energy) { _mc_energy_min = energy; }

    /** IMPLEMENT in ShowerQuality.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ShowerQuality.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ShowerQuality.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:

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

    /// For convenience: struct to define a set of parameters per shower to be stored in TTree
    struct TreeParams_t {
      
      double reco_x, reco_y, reco_z;
      double reco_dcosx, reco_dcosy, reco_dcosz;
      double reco_energy;
      double reco_energy_U;
      double reco_energy_V;
      double reco_energy_Y;
      double reco_dedx;
      double reco_dedx_U;
      double reco_dedx_V;
      double reco_dedx_Y;
      int    best_plane_id;

      double mc_x, mc_y, mc_z;
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

    /// Analysis TTree
    TTree *fTree;

    /// Function to prepare TTree
    void InitializeAnaTree();

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
