/**
 * \file SPABookKeeper.h
 *
 * \ingroup SPTBase
 * 
 * \brief Class def header for a class SPABookKeeper
 *
 * @author kazuhiro
 */

/** \addtogroup SPTBase

    @{*/
#ifndef SELECTIONTOOL_SPABOOKKEEPER_H
#define SELECTIONTOOL_SPABOOKKEEPER_H

#include "SPAData.h"
namespace sptool {

  /**
     \class SPABookKeeper
     User defined class SPABookKeeper ... these comments are used to generate
     doxygen documentation!
  */
  class SPABookKeeper {
    
  public:
    
    /// Default constructor
    SPABookKeeper(){}
    
    /// Default destructor
    virtual ~SPABookKeeper(){}

    void book(const double& score,
	      const SPAComb_t& comb)
    { _list.insert(std::make_pair(score,comb)); }

    
    const std::multimap<double,sptool::SPAComb_t>& list() const
    { return _list; }

    std::multimap<double,sptool::SPAComb_t> unique_list() const;
    
  protected:
    
    std::multimap<double,sptool::SPAComb_t> _list;
  };
}

#endif
/** @} */ // end of doxygen group 

