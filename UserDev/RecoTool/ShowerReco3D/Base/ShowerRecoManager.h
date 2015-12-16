/**
 * \file ShowerRecoManager.h
 *
 * \ingroup ShowerReco3D
 *
 * \brief Class def header for a class ShowerRecoManager
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerReco3D

    @{*/
#ifndef LARLITE_SHOWERRECOMANAGER_H
#define LARLITE_SHOWERRECOMANAGER_H

#include <iostream>
#include <TFile.h>
#include "CMTool/CMToolBase/CMatchManager.h"
#include "ClusterRecoUtil/Alg/DefaultParamsAlg.h"
#include "ShowerRecoException.h"
#include "ShowerRecoAlgBase.h"
#include "ShowerAnaBase.h"
#include "TStopwatch.h"

namespace showerreco {

typedef std::vector<std::vector<unsigned int> > ClusterAss_t;

typedef std::vector< ::Hit2D> PxHitSet_t;

/**
   \class ShowerRecoManager
   User defined class ShowerRecoManager ... these comments are used to generate
   doxygen documentation!
*/
class ShowerRecoManager {

public:

  /// Default constructor
  ShowerRecoManager();

  /// Default destructor
  ~ShowerRecoManager() {}

  /// Add shower reconstruction algorithm
  void AddAlgo(ShowerRecoAlgBase* alg) { _alg_v.push_back(alg); }

  /// Add shower analysis class
  void AddAna(ShowerAnaBase* ana) { _ana_v.push_back(ana); }

  /// Per event "Reset"
  void Reset();

  /**
   * @brief Reconstruct shower w/ matching: return is a matched cluster association
   */
  ClusterAss_t Reconstruct (const std::vector<::cluster::cluster_params >& clusters,
                            std::vector< ::showerreco::Shower_t>& showers);

  /**
   * @brief Reconstruct shower w/ matching
   */
  void Reconstruct (const std::vector<::cluster::cluster_params >& clusters,
                    const ClusterAss_t& ass,
                    std::vector< ::showerreco::Shower_t>& showers);

  /// Access to MatchManager
  ::cmtool::CMatchManager& MatchManager() { return *fMatchMgr; }

  /// Finalize: provide TFile access so that anything that needs to be stored can be stored
  void Finalize(TFile* fout = nullptr);

  // initalize function
  void Initialize();


private:

  /// Shower reconstruction algorithm
  std::vector< ::showerreco::ShowerRecoAlgBase* > _alg_v;

  /// Shower analysis code
  std::vector< ::showerreco::ShowerAnaBase* > _ana_v;

  /// Cluster matching code
  ::cmtool::CMatchManager *fMatchMgr;

  void Process(const ClusterAss_t& ass,
               std::vector< ::showerreco::Shower_t >& showers);

  ::cluster::DefaultParamsAlg _params_alg;

  // stopwatch for time-profiling
  TStopwatch _watch;
  // keep track of number of calls
  double _matching_times;
  double _matching_calls;
  double _CPAN_times;
  double _CPAN_calls;

  ::larlite::storage_manager * _storage;

};
}

#endif
/** @} */ // end of doxygen group

