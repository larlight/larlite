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
namespace ertool {

  /**
     \class ERAlgoCROrphan
     User custom Algorithm class made by kazuhiro
   */
  class ERAlgoCROrphan : public AlgoBase {
  
  public:

    /// Default constructor
    ERAlgoCROrphan(const std::string& name="CROrphan");

    /// Default destructor
    virtual ~ERAlgoCROrphan(){};

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

    bool IsCROrphan(const ::geoalgo::HalfLine& shr) const;

    ::geoalgo::AABox _tpc_box;
    
    bool   _respect_trk_dir;
    double _min_dist_trk_def;
    double _dist_back_to_ceiling;
  };
}
#endif

/** @} */ // end of doxygen group 
