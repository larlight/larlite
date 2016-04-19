/**
 * \file ShowerQuality_singleshowers.h
 *
 * \ingroup ShowerReco3D
 *
 * \brief Package to run on output of shower reconstruction and make a ttree with various parameters that
 * quantify how well shower reconstruction ran. It requires reco showers and MCshowers. If you are running
 * on single particle generated files, that's all you need. If you are running on events with multiple MCshowers,
 * you also need to include simchannel files because those are needed to match a reco shower with an MCshower.
 * Descriptions of definitions of efficiency/purity and other non-obvious parameters can be found in
 * DocDB 3771, or my personal drawing at http://www.nevis.columbia.edu/~kaleko/drawing_explaning_kazus_eff_pur.jpeg
 *
 * @author kaleko
 */

/** \addtogroup ShowerReco3D

    @{*/

#ifndef LARLITE_SHOWERQUALITY_SINGLESHOWERS_H
#define LARLITE_SHOWERQUALITY_SINGLESHOWERS_H

#include <map>
#include "Analysis/ana_base.h"
// #include "MCComp/MCMatchAlg.h"
#include "ShowerReco3D/Base/ShowerRecoException.h"
#include "DataFormat/shower.h"
#include "DataFormat/mcshower.h"
// #include "DataFormat/cluster.h"

#include "MCSingleShowerCompAlg.h"



namespace larlite {
/**
   \class ShowerQuality_singleshowers
   User custom analysis class made by kazuhiro/kaleko
 */
class ShowerQuality_singleshowers : public ana_base {

public:

  /// Default constructor
  ShowerQuality_singleshowers();

  /// Default destructor
  virtual ~ShowerQuality_singleshowers() {};

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

  /// Setter to toggle if you are running on a single particle file
  void SetSingleParticleQuality(bool flag) { _single_particle_quality = flag; }


protected:

  // Function to fill TTree
  void FillQualityInfo(const shower& reco_shower,
                       const mcshower& mc_shower,
                       size_t shower_index);
  // const std::vector< const larlite::cluster * > associated_clusters);

  /// Function to set all of once-per-shower tree parameters to default values
  void ResetShowerTreeParams();

  /// Function to set all of once-per-event tree parameters to default values
  void ResetEventTreeParams();

  /// Boolean to toggle if you are looking for shower quality on single generated particle (simple)
  /// or on a multi-particle realistic event (complicated, needs simchannel)
  bool _single_particle_quality;

  /// Minimum MC shower energy cut
  double _mc_energy_min;

  /// Maximum MC shower energy cut
  double _mc_energy_max;

  /// Shower Producer's Name
  std::string fShowerProducer;

  // /// Matching correctness
  // All "MatchCorrectness" histograms depend on simchannel.
  // TH1D *hMatchCorrectness;

  // "cluster efficiency" is the cluster's charge divided by all hit charge associated with that mcshower
  // (IE the Q summed over all hits in the event, for single particle events)
  // TH1D *hMatchedClusterEff; ///< Matched 3D shower's cluster efficiency (combined across planes)

  // "cluster purity" is the fraction of that cluster's Q belonging to the MCShower
  // for single-particle events, purity is always == 1 for every cluster.
  // TH1D *hMatchedClusterPur; ///< Matched 3D shower's cluster purity (combined across planes)

  /// Analysis TTree. Filled once per reconstructed shower.
  TTree *fShowerTree;

  /// Analysis TTree. Filled once per event.
  TTree *fEventTree;

  int _event, _subrun, _run;

  /// For convenience: struct to define a set of parameters per shower to be stored in per-reconstructed-shower TTree
  struct ShowerTreeParams_t {

    double reco_x, reco_y, reco_z;
    double reco_dcosx, reco_dcosy, reco_dcosz;
    double reco_energy_U;
    double reco_energy_V;
    double reco_energy_Y;
    double reco_dedx;
    double reco_dedx_U;
    double reco_dedx_V;
    double reco_dedx_Y;
    double reco_dqdx;
    double reco_dqdx_U;
    double reco_dqdx_V;
    double reco_dqdx_Y;
    double mc_x, mc_y, mc_z;
    double mc_dcosx, mc_dcosy, mc_dcosz;
    double mc_energy;
    double mc_reco_anglediff;
    double mc_reco_dist;
    double cluster_eff_U;
    double cluster_eff_V;
    double cluster_eff_Y;
    double cluster_pur_U;
    double cluster_pur_V;
    double cluster_pur_Y;
    double mc_containment;
    double reco_length;
    double mc_length;
    double mc_wildlength;

  } fShowerTreeParams;

  struct EventTreeParams_t {

    int n_mcshowers;
    int n_recoshowers;
    //Detprofile energy of the FIRST (for now, only) mcshower
    double mcs_E;
    double mc_containment;

  } fEventTreeParams;

  /// Function to prepare TTrees
  void InitializeAnaTrees();

  /// Algorithm to compute cluster efficiency/purity WITHOUT simchannel usage
  /// (only usable on single-mcshower events (single e, single gamma))
  MCSingleShowerCompAlg _alg;

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
