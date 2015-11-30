/**
 * \file ERAlgoCROrphan.h
 *
 * \ingroup Algo
 *
 * \brief Class def header for a class ERAlgoCROrphan
 *
 * @author kazuhiro
 */

/** \addtogroup Algo

    @{*/

#ifndef ERTOOL_ERALGOCRORPHAN_H
#define ERTOOL_ERALGOCRORPHAN_H

#include "ERTool/Base/AlgoBase.h"
#include "GeoAlgo/GeoAABox.h"
#include "ERTool/Algo/AlgoFindRelationship.h"

namespace ertool {

  /**
     \class ERAlgoCROrphan
     User custom Algorithm class made by kazuhiro
   */
  class ERAlgoCROrphan : public AlgoBase {

  public:

    /// Default constructor
    ERAlgoCROrphan(const std::string& name = "CROrphan");

    /// Default destructor
    virtual ~ERAlgoCROrphan() {};

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

    /// Function that takes in a shower (or, a short track that is converted to into a shower)
    /// and decides if it looks like a cosmic ray orphan (all alone near top of detector, pointing downwards)
    bool IsCROrphan(const ::geoalgo::HalfLine& shr) const;

    /// Function that takes in a particle and decides if it looks like it's neutron-induced from a cosmic
    /// (all alone, short track length, etc)
    /// Returns -1 if the particle is not neutron-induced, otherwise returns nodeID of closest cosmic primary
    int IsNeutronInduced(const ertool::Particle& p, const EventData &data, const ParticleGraph& ps) const;

    ::geoalgo::AABox _tpc_box;
    ertool::AlgoFindRelationship _findRel;

    bool   _respect_trk_dir;
    double _min_dist_trk_def;
    double _dist_back_to_ceiling;
    bool _tag_protons_from_neutrons;
    //Minimum distance [cm] b/t lone track and cosmic primary for it to be considered neutron-induced
    double _min_neutron_dist = 5.;
    //Maximum distance [cm] of lone track for it to be considered neutron-induced
    double _max_neutron_trklength = 1.;
  };
}
#endif

/** @} */ // end of doxygen group
