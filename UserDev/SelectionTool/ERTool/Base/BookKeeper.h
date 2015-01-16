/**
 * \file BookKeeper.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class BookKeeper
 *
 * @author kazuhiro
 */

/** \addtogroup ERTool

    @{*/
#ifndef ERTOOL_BOOKKEEPER_H
#define ERTOOL_BOOKKEEPER_H

#include <vector>
#include <map>
#include <set>
#include "Env.h"
namespace ertool {

  /**
     \class BookKeeper
     User defined class BookKeeper ... these comments are used to generate
     doxygen documentation!
  */
  class BookKeeper {
    
  public:
    
    /// Default constructor
    BookKeeper(){}
    
    /// Default destructor
    virtual ~BookKeeper(){}

    void book(const double& score,
	      const Combination_t& comb)
    { _list.insert(std::make_pair(score,comb)); }

    const std::multimap<double,ertool::Combination_t>& list() const
    { return _list; }

    std::multimap<double,ertool::Combination_t> unique_list() const;
    
  protected:
    
    std::multimap<double,ertool::Combination_t> _list;
  };
}

#endif
/** @} */ // end of doxygen group 

