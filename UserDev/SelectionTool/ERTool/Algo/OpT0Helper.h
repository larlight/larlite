/**
 * \file OpT0Helper.h
 *
 * \ingroup Algo
 * 
 * \brief Class def header for a class OpT0Helper
 *
 * @author kazuhiro
 */

/** \addtogroup Algo

    @{*/
#ifndef ERTOOL_OPT0HELPER_H
#define ERTOOL_OPT0HELPER_H

#include "FhiclLite/PSet.h"
#include "ERTool/Base/EventData.h"
#include "OpT0Finder/Base/OpT0FinderTypes.h"
#include "OpT0Finder/Base/OpT0FinderConstants.h"

namespace ertool {
  /**
     \class OpT0Helper
  */
  class OpT0Helper{
    
  public:
    
    /// Default constructor
    OpT0Helper(const std::string name="OpT0Helper");
    
    /// Default destructor
    ~OpT0Helper(){}

    /// Name
    const std::string& Name() const
    { return _name; }

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

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
  };
}

#endif
/** @} */ // end of doxygen group 

