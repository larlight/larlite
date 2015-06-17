/**
 * \file AlgoBase.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class AlgoBase
 *
 * @author kazuhiro
 */

/** \addtogroup ERTool

    @{*/
#ifndef ERTOOL_ALGOBASE_H
#define ERTOOL_ALGOBASE_H

#include "UnitBase.h"
#include "EventData.h"
#include "Particle.h"
namespace ertool {

  /**
     \class AlgoBase
     @brief Base class for Shower Pdf Algorithms (Algo).
     This abstract base class introduces multiple common functions for children classes (AlgoX)
     to fit in the framework. 

     All AlgoX classes need to implement AlgoBase::Reconstruct function which reconstructs a\n
     ertool::ParticleSet object which is a set of ertool::Particle and is meant to represent  \n
     reconstructed particles. Given a muon track, one may simply populate one ertool::Particle \n
     of a muon type. Or, if you find 2 gamma showers that come from pi0, for instance, you may  \n
     fill ParticleSet w/ a single pi0. Refer to ertool::Particle and ertool::ParticleSet for \n
     details of how to store information.

     As the class inherits from ertool::UnitBase, it has a systemtaic procedure to store/load parameter \n
     sets (variable name & values) from larlite::user_info.
 */
  class AlgoBase : public UnitBase {

  public:
    
    /// Default constructor
    AlgoBase() : UnitBase() { _name = "AlgoBase"; _training_mode = false; }
    
    /// Default destructor
    virtual ~AlgoBase(){}
    
    /// Function to evaluate input showers and determine a score
    virtual ParticleSet Reconstruct(const EventData &data) {return ParticleSet(); }

  };
}

#endif
/** @} */ // end of doxygen group 

