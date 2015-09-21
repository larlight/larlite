/**
 * \file ShowerQuality.h
 *
 * \ingroup ShowerReco3D
 *
 * \brief Package to run on output of shower reconstruction and make a ttree with various parameters that
 * quantify how well shower reconstruction ran. It requires reco showers and MCshowers. If you are running
 * on single particle generated files, that's all you need. If you are running on events with multiple MCshowers,
 * you also need to include simchannel files because those are needed to match a reco shower with an MCshower.
 *
 * @author kaleko
 */

/** \addtogroup ShowerReco3D

    @{*/

#ifndef LARLITE_SHOWERQUALITY_H
#define LARLITE_SHOWERQUALITY_H

#include <TH1D.h>
#include <TGraphErrors.h>
// #include <TH2D.h>
#include <map>
#include "Analysis/ana_base.h"
//#include "MCComp/MCMatchAlg.h"
#include "ShowerReco3D/Base/ShowerRecoException.h"
#include "DataFormat/shower.h"
#include "DataFormat/mcshower.h"

namespace larlite {
/**
   \class ShowerQuality
   User custom analysis class made by kazuhiro/kaleko
 */
class ShowerQuality : public ana_base {

public:

  /// Default constructor
  ShowerQuality();

  /// Default destructor
  virtual ~ShowerQuality() {};

  /**
     Setter function for a shower producer name.
  */
  void SetShowerProducer(const std::string name)
  { fShowerProducer = name; }

  /// Set maximum energy for MCShowers to be considered
  void SetMaxEnergyCut(const double energy) { _mc_energy_max = energy; }

  /// Set minimum energy for MCShowers to be considered
  void SetMinEnergyCut(const double energy) { _mc_energy_min = energy; }

  /**
      Initialization method to be called before the analysis event loop.
  */
  virtual bool initialize();

  /**
      Analyze a data event-by-event
  */
  virtual bool analyze(storage_manager* storage);

  /**
      Finalize method to be called after all events processed.
  */
  virtual bool finalize();

  /// set whether to fill quality info per MC or per reco shower
  //    void setMCShowerQuality(bool on) { _mcShowerQuality = on; }

  /// Setter to toggle if you are running on a single particle file
  void SetSingleParticleQuality(bool flag) { _single_particle_quality = flag; }


protected:

  // Function to fill TTree
  void FillQualityInfo(const shower& reco_shower,
                       const mcshower& mc_shower);
  // ,
  //                      const size_t& shower_index, const size_t& best_index,
  //                      const AssSet_t& ass_cluster_v);


  /// Function to set all of once-per-shower tree parameters to default values
  void ResetShowerTreeParams();

  /// Function to set all of once-per-event tree parameters to default values
  void ResetEventTreeParams();

  // boolean to decide if to fill the tree once per MC shower
  // or once per RECO shower
  // bool _mcShowerQuality;

  /// Boolean to toggle if you are looking for shower quality on single generated particle (simple)
  /// or on a multi-particle realistic event (complicated, needs simchannel)
  bool _single_particle_quality;

  // /// Shower back tracking algorithm needed to match reco showers with mc showers.
  // ::btutil::MCMatchAlg fBTAlg;

  /// Minimum MC shower energy cut
  double _mc_energy_min;

  /// Maximum MC shower energy cut
  double _mc_energy_max;

  /// Shower Producer's Name
  std::string fShowerProducer;

  // /// Matching correctness
  // TH1D *hMatchCorrectness;

  // TH1D *hVtxDX; ///< X difference (reco-MC) in cm
  // TH1D *hVtxDY; ///< Y difference (reco-MC) in cm
  // TH1D *hVtxDZ; ///< Z difference (reco-MC) in cm
  // TH1D *hVtxDR; ///< 3D vtx distance between reco to MC in cm

  // TH1D *hDCosX; ///< Direction unit vector X component difference
  // TH1D *hDCosY; ///< Direction unit vector Y component difference
  // TH1D *hDCosZ; ///< Direction unit vector Z component difference
  // TH1D *h3DAngleDiff; ///< Opening angle between reco & MC 3D direction

  // TH2D *hEnergyCorr; ///< Energy correlation reco (x) vs. MC (y)

  // TH1D *hEnergyAssym; ///< Energy assym. parameter: (reco E - MC E) / (reco E + MC E) * 2
  // TH1D *hEnergyDiff;  ///< Energy difference: reco E - MC E

  // TH1D *hMatchedClusterEff; ///< Matched 3D shower's cluster efficiency (combined across planes)
  // TH1D *hMatchedClusterPur; ///< Matched 3D shower's cluster purity (combined across planes)

  // /// dEdx per particle per PDG code
  // std::map<int,TH1D*> mDEDX;

  // /// Best plane id
  // TH1D *hBestPlane;


  /// Analysis TTree. Filled once per reconstructed shower.
  TTree *fShowerTree;

  /// Analysis TTree. Filled once per event.
  TTree *fEventTree;

  /// For convenience: struct to define a set of parameters per shower to be stored in per-reconstructed-shower TTree
  struct ShowerTreeParams_t {

    double reco_x, reco_y, reco_z;
    double reco_dcosx, reco_dcosy, reco_dcosz;
    // double reco_energy;
    double reco_energy_U;
    double reco_energy_V;
    double reco_energy_Y;
    // double reco_dedx;
    double reco_dedx_U;
    double reco_dedx_V;
    double reco_dedx_Y;
    // int    best_plane_id;

    double mc_x, mc_y, mc_z;
    double mc_dcosx, mc_dcosy, mc_dcosz;
    double mc_energy;
    // int    mc_pdgid;

    double mc_reco_anglediff;
    double mc_reco_dist;

    // double mc_containment;
    // double match_correctness;
    // double cluster_eff;
    // double cluster_pur;

  } fShowerTreeParams;

  struct EventTreeParams_t {

    int n_mcshowers;
    int n_recoshowers;
    //Detprofile energy of the FIRST (for now, only) mcshower
    double mcs_E;

  } fEventTreeParams;

  /// Function to prepare TTrees
  void InitializeAnaTrees();

  /// Histograms used in efficiency calculations
  TH1F* _n_mcshowers_trueenergybins;
  TH1F* _n_recoshowers_trueenergybins;
  /// The actual (saved) TGraph of shower reconstruction efficiency vs true shower energy
  TGraphErrors* _reco_efficiency_trueenergybins;

  /// Function to compute reconstruction efficiencies (filling TGraphs, etc)
  void ComputeEfficiencies();
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
