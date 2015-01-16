/**
 * \file FilterECut.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class FilterECut
 *
 * @author david
 */

/** \addtogroup ERTool

    @{*/

#ifndef ERTOOL_FILTERECUT_H
#define ERTOOL_FILTERECUT_H

#include "Base/FilterBase.h"

namespace ertool {

  /**
     \class FilterECut
     User custom Filter class made by david
   */
  class FilterECut : public FilterBase {
  
  public:

    /// Default constructor
    FilterECut();

    /// Default destructor
    virtual ~FilterECut(){};

    /// Select function
    virtual void Filter(EventData& data);

    /// Set Energy Cut for this Filter
    void SetECut(double ecut) { _ECut = ecut; }

  private:
    
    // Energy cut value
    double _ECut;

  };
}
#endif

/** @} */ // end of doxygen group 
