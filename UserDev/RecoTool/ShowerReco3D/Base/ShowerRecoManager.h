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
   * @brief Reconstruct showers
   */
  void Reconstruct (std::vector< ::showerreco::Shower_t>& showers);

  /// Finalize: provide TFile access so that anything that needs to be stored can be stored
  void Finalize(TFile* fout = nullptr);

  // initalize function
  void Initialize();

  /**
     @brief set clusters for the current event
   */
  void SetProtoShowers(const std::vector< ::showerreco::ProtoShower >& proto_showers)
  { _proto_showers = proto_showers; }


private:

  /// Shower reconstruction algorithm
  std::vector< ::showerreco::ShowerRecoAlgBase* > _alg_v;

  /// Shower analysis code
  std::vector< ::showerreco::ShowerAnaBase* > _ana_v;

  void Process(const ClusterAss_t& ass,
               std::vector< ::showerreco::Shower_t >& showers);

  // vector of input clusters to be used for reconstruction
  std::vector< ::showerreco::ProtoShower > _proto_showers;



};
}

#endif
/** @} */ // end of doxygen group

