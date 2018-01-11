/**
 * \file CMatchBookKeeper.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CMatchBookKeeper
 *
 * @author kazuhiro
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CMATCHBOOKKEEPER_H
#define RECOTOOL_CMATCHBOOKKEEPER_H

#include <iostream>
#include <vector>
#include <map>
#include "CMTException.h"

namespace cmtool {
  /**
     \class CMatchBookKeeper
     User defined class CMatchBookKeeper ... these comments are used to generate
     doxygen documentation!
  */
  class CMatchBookKeeper {
    
  public:
    
    /// Default constructor
    CMatchBookKeeper();
    
    /// Default destructor
    virtual ~CMatchBookKeeper(){};
  
    /// Reset method
    void Reset();

    /// Method to register matched clusters
    void Match(const std::vector<unsigned int>& matched_indexes,
	       const float& score);

    /// Method to get result
    std::vector<std::vector<unsigned int> > GetResult() const;

    /// Method to pass result
    void PassResult(std::vector<std::vector<unsigned int> >& result) const;

  protected:

    std::multimap<float,std::vector<unsigned int> > _register;

  };
}
#endif
/** @} */ // end of doxygen group 

