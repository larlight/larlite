/**
 * \file Ana_Class_Name.h
 *
 * \ingroup Working_Package
 * 
 * \brief Class def header for a class Ana_Class_Name
 *
 * @author kazuhiro
 */

/** \addtogroup Working_Package

    @{*/

#ifndef ERTOOL_ANA_CLASS_NAME_H
#define ERTOOL_ANA_CLASS_NAME_H

#include "ERTool/Base/AnaBase.h"

namespace ertool {

  /**
     \class Ana_Class_Name
     User custom Analysis class made by kazuhiro
   */
  class Ana_Class_Name : public AnaBase {
  
  public:

    /// Default constructor
    Ana_Class_Name();

    /// Default destructor
    virtual ~Ana_Class_Name(){};

    /// Reset function
    virtual void Reset();

    /// Function to evaluate input showers and determine a score
    virtual bool Analyze(const EventData &data, const ParticleSet &ps);

  };
}
#endif

/** @} */ // end of doxygen group 
