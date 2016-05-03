/**
 * \file Pi0Mass.h
 *
 * \ingroup ShowerReco3D
 *
 * \brief 
 *
 * @author yuntse
 */

/** \addtogroup ShowerReco3D

    @{*/

#ifndef LARLITE_PIZEROMASS_H
#define LARLITE_PIZEROMASS_H

#include <map>
#include "Analysis/ana_base.h"
#include "ShowerReco3D/Base/ShowerRecoException.h"
#include "DataFormat/shower.h"
#include "DataFormat/mcshower.h"
// #include "DataFormat/cluster.h"
#include "MCSingleShowerCompAlg.h"

namespace larlite {
/**
   \class Pi0Mass
 */
class Pi0Mass : public ana_base {

public:

  /// Default constructor
  Pi0Mass();

  /// Default destructor
  virtual ~Pi0Mass() {};

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


protected:

  // Function to fill TTree
  // void FillQualityInfo(const shower& reco_shower,
  //                      const mcshower& mc_shower,
  //                      size_t shower_index);
  // const std::vector< const larlite::cluster * > associated_clusters);

  /// Function to set all of once-per-shower tree parameters to default values
  // void ResetShowerTreeParams();

  /// Function to set all of once-per-event tree parameters to default values
  void ResetEventTreeParams();

  /// Minimum MC shower energy cut
  double _mc_energy_min;

  /// Maximum MC shower energy cut
  double _mc_energy_max;

  /// Shower Producer's Name
  std::string fShowerProducer;

  /// Analysis TTree. Filled once per reconstructed shower.
  // TTree *fShowerTree;

  /// Analysis TTree. Filled once per event.
  TTree *fEventTree;

  int _event, _subrun, _run;

  /// For convenience: struct to define a set of parameters per shower to be stored in per-reconstructed-shower TTree
/*  struct ShowerTreeParams_t {

    double reco_x, reco_y, reco_z;
    double reco_dcosx, reco_dcosy, reco_dcosz;
    double reco_energy_U;
    double reco_energy_V;
    double reco_energy_Y;
    double reco_dedx_U;
    double reco_dedx_V;
    double reco_dedx_Y;
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

  } fShowerTreeParams;
*/
  struct EventTreeParams_t {

    int n_mcshowers;
    int n_recoshowers;
    //Detprofile energy of the FIRST (for now, only) mcshower
    double mcs_E1;
    double mcs_E2;
    double mc_containment1;
    double mc_containment2;
    double PerfectRecoERecoThetaPi0Mass;
    double RecoEPerfectRecoThetaPi0Mass;
    double PerfectRecoPi0Mass;
    double PerfectRecoE1;
    double PerfectRecoE2;
    double PerfectRecoCosTheta;
    double PerfectRecoTheta;
    double RecoPi0Mass;
    double RecoE1;
    double RecoE2;
    double RecoCosTheta;
    double RecoTheta;
    double mc_reco_anglediff1;
    double mc_reco_anglediff2;
    double mc_start_dist;
    double mc_reco_start_dist1;
    double mc_reco_start_dist2;
    double mc_reco_dca1;
    double mc_reco_dca2;
    // double shower1MatchedDir;
    // double shower2MatchedDir;
    // int notMatchedShower1;
    // int notMatchedShower2;

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
