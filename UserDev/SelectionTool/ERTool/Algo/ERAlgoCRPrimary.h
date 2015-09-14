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
    ERAlgoCRPrimary(const std::string& name="CRPrimary");

    /// Default destructor
    virtual ~ERAlgoCRPrimary(){};

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
    bool _respect_track_dir;
    bool _tag_entering_or_exiting_as_cosmics;
    ::geoalgo::AABox _tpc_box;
    ::geoalgo::AABox _cosmic_box;
  };
}
#endif

/** @} */ // end of doxygen group 
