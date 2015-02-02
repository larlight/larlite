/**
 * \file Algo_Class_Name.h
 *
 * \ingroup Working_Package
 * 
 * \brief Class def header for a class Algo_Class_Name
 *
 * @author Shell_User_Name
 */

/** \addtogroup Working_Package

    @{*/

#ifndef ERTOOL_ALGO_CLASS_NAME_H
#define ERTOOL_ALGO_CLASS_NAME_H

#include "ERTool/Base/AlgoBase.h"

namespace ertool {

  /**
     \class Algo_Class_Name
     User custom Algorithm class made by kazuhiro
   */
  class Algo_Class_Name : public AlgoBase {
  
  public:

    /// Default constructor
    Algo_Class_Name();

    /// Default destructor
    virtual ~Algo_Class_Name(){};

    /// Reset function
    virtual void Reset();

    /// Function to evaluate input showers and determine a score
    virtual ParticleSet Reconstruct(const EventData &data);

  };
}
#endif

/** @} */ // end of doxygen group 
