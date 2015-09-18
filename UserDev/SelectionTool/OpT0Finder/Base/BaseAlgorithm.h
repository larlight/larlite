/**
 * \file BaseAlgorithm.h
 *
 * \ingroup Base
 * 
 * \brief Class def header for a class BaseAlgorithm
 *
 * @author kazuhiro
 */

/** \addtogroup Base

    @{*/
#ifndef OPT0FINDER_BASEALGORITHM_H
#define OPT0FINDER_BASEALGORITHM_H

#include "OpT0FinderTypes.h"
#include "ColorPrint.h"
namespace flashana {
  /**
     \class BaseAlgorithm
  */
  class BaseAlgorithm : public ColorPrint {
    
  public:
    
    /// Default constructor
    BaseAlgorithm(const Algorithm_t type)
      : _type(type)
    {}
    
    /// Default destructor
    ~BaseAlgorithm(){}

    /// Algorithm type
    Algorithm_t AlgorithmType() const { return _type; }

  private:
    /// Algorithm type
    Algorithm_t _type;
  };
}
#endif
/** @} */ // end of doxygen group 

