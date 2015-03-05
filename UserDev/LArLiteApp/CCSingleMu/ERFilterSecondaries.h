/**
 * \file ERFilterSecondaries.h
 *
 * \ingroup CCSingleMu
 * 
 * \brief Class def header for a class ERFilterSecondaries
 *
 * @author kazuhiro
 */

/** \addtogroup CCSingleMu

    @{*/

#ifndef ERTOOL_ERFILTERSECONDARIES_H
#define ERTOOL_ERFILTERSECONDARIES_H

#include "GeoAlgo/GeoAlgo.h"
#include "ERTool/Algo/AlgoFindRelationship.h"
#include "ERTool/Base/FilterBase.h"
#include <set>
#include <vector>
#include <deque>
namespace ertool {

  /**
     \class ERFilterSecondaries
     User custom SPAFilter class made by kazuhiro
   */
  class ERFilterSecondaries : public FilterBase {
  
  public:

    /// Default constructor
    ERFilterSecondaries();

    /// Default destructor
    virtual ~ERFilterSecondaries(){}

    /// Select function
    virtual void Filter (EventData& data);

    void RegisterSeed(const size_t trk_id);

  protected:

    ::geoalgo::GeoAlgo _geoalg;

    ::ertool::AlgoFindRelationship _relalg;

    double _min_dist_trk_trk;
    bool   _use_perpdist_trk_trk;
    double _min_dist_trk_shr;
    bool   _use_perpdist_trk_shr;

    std::set<size_t>   _seed_trk_s;
    std::deque<size_t> _remaining_trk_v;
    std::deque<size_t> _remaining_shr_v;
    std::vector<std::set<size_t> > _removed_trk_v;

  };
}
#endif

/** @} */ // end of doxygen group 
