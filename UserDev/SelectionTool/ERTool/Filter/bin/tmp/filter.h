/**
 * \file Filter_Class_Name.h
 *
 * \ingroup Working_Package
 * 
 * \brief Class def header for a class Filter_Class_Name
 *
 * @author kazuhiro
 */

/** \addtogroup Working_Package

    @{*/

#ifndef ERTOOL_FILTER_CLASS_NAME_H
#define ERTOOL_FILTER_CLASS_NAME_H

#include "Base/FilterBase.h"

namespace ertool {

  /**
     \class Filter_Class_Name
     User custom SPAFilter class made by kazuhiro
   */
  class Filter_Class_Name : public FilterBase {
  
  public:

    /// Default constructor
    Filter_Class_Name();

    /// Default destructor
    virtual ~Filter_Class_Name(){};

    /// Select function
    virtual void Filter (EventData& data);

  };
}
#endif

/** @} */ // end of doxygen group 
