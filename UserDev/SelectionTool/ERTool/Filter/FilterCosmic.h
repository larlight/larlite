/**
 * \file FilterCosmic.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class FilterCosmic
 *
 * @author kazuhiro
 */

/** \addtogroup ERTool

    @{*/

#ifndef ERTOOL_FILTERCOSMIC_H
#define ERTOOL_FILTERCOSMIC_H

#include "Base/FilterBase.h"

namespace ertool {

  /**
     \class FilterCosmic
     User custom Filter class made by kazuhiro
   */
  class FilterCosmic : public FilterBase {
  
  public:

    /// Default constructor
    FilterCosmic();

    /// Default destructor
    virtual ~FilterCosmic(){};

    /// Select function
    virtual void Filter(EventData& data);

    void SetCosmicScoreCut (double score) { _score = score ; }	

  private:
    
    double _score ;
    
  };
}
#endif

/** @} */ // end of doxygen group 
