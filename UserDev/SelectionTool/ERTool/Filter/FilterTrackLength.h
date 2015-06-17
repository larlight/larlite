/**
 * \file FilterTrackLength.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class FilterTrackLength
 *
 * @author david
 */

/** \addtogroup ERTool

    @{*/

#ifndef ERTOOL_FILTERTRACKLENGTH_H
#define ERTOOL_FILTERTRACKLENGTH_H

#include "ERTool/Base/FilterBase.h"

namespace ertool {

  /**
     \class FilterTrackLength
     User custom Filter class made by david
   */
  class FilterTrackLength : public FilterBase {
  
  public:

    /// Default constructor
    FilterTrackLength();

    /// Default destructor
    virtual ~FilterTrackLength(){};

    /// Select function
    void Filter(EventData& data);

    /// Set Length Cut value
    void setLengthCut(double l) { _lenCut = l; }

  private:

    /// Cut value for length: if track is < _lenCut long -> remove
    double _lenCut;
    
  };
}
#endif

/** @} */ // end of doxygen group 
