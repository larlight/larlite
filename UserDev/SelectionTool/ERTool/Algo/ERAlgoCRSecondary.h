/**
 * \file ERAlgoCRSecondary.h
 *
 * \ingroup Algo
 * 
 * \brief Class def header for a class ERAlgoCRSecondary
 *
 * @author kazuhiro
 */

/** \addtogroup Algo

    @{*/

#ifndef ERTOOL_ERALGOCRSECONDARY_H
#define ERTOOL_ERALGOCRSECONDARY_H

#include "ERTool/Base/AlgoBase.h"

namespace ertool {

  /**
     \class ERAlgoCRSecondary
     User custom Algorithm class made by kazuhiro
   */
  class ERAlgoCRSecondary : public AlgoBase {
  
  public:

    /// Default constructor
    ERAlgoCRSecondary(const std::string& name="CRSecondary");

    /// Default destructor
    virtual ~ERAlgoCRSecondary(){};

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

  private:
    
    bool IsCRSecondary(const ::geoalgo::Trajectory_t seed,
		       const ::geoalgo::Trajectory_t cand) const;
    bool IsCRSecondary(const ::geoalgo::Trajectory_t seed,
		       const ::geoalgo::HalfLine_t cand) const;

    double _min_dist_trk_trk;
    double _min_dist_common_origin;
    double _min_dist_trk_def;
    bool   _use_perpdist_trk_trk;
    double _min_dist_trk_shr;
    bool   _use_perpdist_trk_shr;
  };
}
#endif

/** @} */ // end of doxygen group 
