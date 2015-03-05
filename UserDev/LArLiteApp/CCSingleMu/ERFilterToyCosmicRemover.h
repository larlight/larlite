/**
 * \file ERFilterToyCosmicRemover.h
 *
 * \ingroup CCSingleMu
 * 
 * \brief Class def header for a class ERFilterToyCosmicRemover
 *
 * @author kazuhiro
 */

/** \addtogroup CCSingleMu

    @{*/

#ifndef ERTOOL_ERFILTERTOYCOSMICREMOVER_H
#define ERTOOL_ERFILTERTOYCOSMICREMOVER_H

#include "ERTool/Base/FilterBase.h"
#include "ERFilterSecondaries.h"
#include "GeoAlgo/GeoAABox.h"
namespace ertool {

  /**
     \class ERFilterToyCosmicRemover
     User custom SPAFilter class made by kazuhiro
   */
  class ERFilterToyCosmicRemover : public FilterBase {
  
  public:

    /// Default constructor
    ERFilterToyCosmicRemover();

    /// Default destructor
    virtual ~ERFilterToyCosmicRemover(){};

    /// Select function
    virtual void Filter (EventData& data);

  private:

    ::ertool::ERFilterSecondaries _sec_filter;
    ::geoalgo::AABox fTPC;
  };
}
#endif

/** @} */ // end of doxygen group 
