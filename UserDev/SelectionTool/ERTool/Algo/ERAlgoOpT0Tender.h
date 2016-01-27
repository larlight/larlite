/**
 * \file ERAlgoOpT0Tender.h
 *
 * \ingroup Algo
 * 
 * \brief Class def header for a class ERAlgoOpT0Tender
 *
 * @author Rui
 */

/** \addtogroup Algo

    @{*/

#ifndef ERTOOL_ERALGOOPT0TENDER_H
#define ERTOOL_ERALGOOPT0TENDER_H

#include "ERTool/Base/AlgoBase.h"
#include "FhiclLite/PSet.h"
#include "ERTool/Base/EventData.h"
#include "OpT0Finder/Base/OpT0FinderTypes.h"
#include "OpT0Finder/Base/OpT0FinderConstants.h"
#include "OpT0Finder/Algorithms/LightPath.h"

namespace ertool {

  /**
     \class ERAlgoOpT0Tender
     User custom Algorithm class made by kazuhiro
   */
  class ERAlgoOpT0Tender : public AlgoBase {
  
  public:

    /// Default constructor
    ERAlgoOpT0Tender(const std::string& name="ERAlgoOpT0Tender");

    /// Default destructor
    virtual ~ERAlgoOpT0Tender(){};
    
    /// Name
    const std::string& Name() const
    { return _name; }
    
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
    
    ::flashana::QCluster_t GetQCluster(const EventData& data,
				       const std::vector<RecoID_t>& shower_id_v,
				       const std::vector<RecoID_t>& track_id_v) const;
    
    void AppendGeoSteps(::flashana::QCluster_t& cluster,
			const ::geoalgo::Point_t& start,
			const ::geoalgo::Vector_t& dir,
			const double length) const;
    
  private:
    
    std::string _name;
    bool _use_energy;
    double _step_size;
    ::flashana::LightPath LP;
    
    
  };
}
#endif

/** @} */ // end of doxygen group 
