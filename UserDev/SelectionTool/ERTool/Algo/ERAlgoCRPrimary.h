/**
 * \file ERAlgoCRPrimary.h
 *
 * \ingroup Algo
 *
 * \brief Class def header for a class ERAlgoCRPrimary
 *
 * @author kazuhiro
 */

/** \addtogroup Algo

    @{*/

#ifndef ERTOOL_ERALGOCRPRIMARY_H
#define ERTOOL_ERALGOCRPRIMARY_H

#include "ERTool/Base/AlgoBase.h"
#include "GeoAlgo/GeoAABox.h"
namespace ertool {

/**
   \class ERAlgoCRPrimary
   User custom Algorithm class made by kazuhiro
 */
class ERAlgoCRPrimary : public AlgoBase {

public:

  /// Default constructor
  ERAlgoCRPrimary(const std::string& name = "CRPrimary");

  /// Default destructor
  virtual ~ERAlgoCRPrimary() {};

  /// Reset function
  void Reset();

  /// Function to accept fclite::PSet
  void AcceptPSet(const ::fcllite::PSet& cfg);

  /// Called @ before processing the first event sample
  void ProcessBegin();

  /// Function to evaluate input showers and determine a score
  bool Reconstruct(const EventData &data, ParticleGraph& graph);

  /// Called after processing the last event sample
  void ProcessEnd(TFile* fout = nullptr);

private:
  bool _respect_track_dir;
  bool _tag_entering_or_exiting_as_cosmics;

  /// The size of these boxes can generally be found in a cfg file, often in UserDev/SelectionTool/ERTool/dat/*.cfg
  /// Default values for TPC box are x: [0.3, 256], y: [-115,115], z: [0.5,1036] (roughly the size of the TPC)
  ::geoalgo::AABox _tpc_box;
  /// Default values for the cosmic box are x: [-1.5,258], y: [-119,118], z: [-1.5,1038] (slightly larger than TPC)
  ::geoalgo::AABox _cosmic_box;
};
}
#endif

/** @} */ // end of doxygen group
