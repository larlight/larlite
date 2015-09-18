/**
 * \file NPtFilter.h
 *
 * \ingroup Algorithms
 * 
 * \brief Class def header for a class NPtFilter
 *
 * @author kazuhiro
 */

/** \addtogroup Algorithms

    @{*/
#ifndef OPT0FINDER_NPTFILTER_H
#define OPT0FINDER_NPTFILTER_H

#include "OpT0Finder/Base/BaseTPCFilter.h"
namespace flashana {
  /**
     \class NPtFilter
     Implementation of flashana::BaseTPCFilter abstract algorithm class. \n
     It applies a _very_ simple filtering: excludes TPC objects (flashana::QCluster_t) \n
     that contains less than specified number of points. \n
  */
  class NPtFilter : public BaseTPCFilter{
    
  public:
    
    /// Default constructor
    NPtFilter();
    
    /// Default destructor
    ~NPtFilter(){}

    /// Implementation of virtualfunction
    IDArray_t Filter(const QClusterArray_t&);

  private:
    size_t _min_num_pt; ///< mininum number of QPoint_t to pass the filter
    
  };
}

#endif
/** @} */ // end of doxygen group 

