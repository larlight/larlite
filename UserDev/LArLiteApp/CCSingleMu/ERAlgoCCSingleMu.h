/**
 * \file ERAlgoCCSingleMu.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class ERAlgoCCSingleMu
 *
 * @author kazuhiro
 */

/** \addtogroup ERTool

    @{*/

#ifndef ERTOOL_ERALGOCCSINGLEMU_H
#define ERTOOL_ERALGOCCSINGLEMU_H

#include "ERTool/Base/AlgoBase.h"
#include "ERTool/Algo/AlgoFindRelationship.h"

namespace ertool {

  /**
     \class ERAlgoCCSingleMu
     User custom Algorithm class made by kazuhiro
   */
  class ERAlgoCCSingleMu : public AlgoBase {
  
  public:

    /// Default constructor
    ERAlgoCCSingleMu();

    /// Default destructor
    virtual ~ERAlgoCCSingleMu(){};

    /// Reset function
    virtual void Reset();

    /// Function to evaluate input showers and determine a score
    virtual ParticleSet Reconstruct(const EventData &data);

  private:

    ::ertool::AlgoFindRelationship _relalg;
    double _min_trk_length;
    double _min_dist_common_origin;
  };
}
#endif

/** @} */ // end of doxygen group 
