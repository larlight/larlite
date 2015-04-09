/**
 * \file mctree.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for mctree data container
 *
 * @author David Caratelli
 */

/** \addtogroup DataFormat
    
@{*/

#ifndef LARLITE_MCTREE_H
#define LARLITE_MCTREE_H

#include "data_base.h"
#include "treenode.h"
#include <set>
#include <map>
#include <exception>

namespace larlite {
  /**
     \class mctree
     LArSoft Mctree data container
  */
  class mctree : public data_base, public treenode {
      
  public:
    
    /// Default constructor 
    mctree() : data_base(data::kMCTree), treenode::treenode() {clear_data();}
    
    /// Alternative constructor
    mctree(int trackId);

    /// Copy ctor
    mctree(const mctree& orig) 
      : data_base(orig)
      , treenode(orig)
    {}

    /// Default destructor
    virtual ~mctree(){}

    ///--- Utility ---///
    virtual void clear_data();

  private:

  };
  
  /**
     \class event_mctree
     A collection of Mctreeicles
  */
  class event_mctree : public std::vector<larlite::mctree>,
		       public event_base {
    
  public:
    
    /// Default constructor
    event_mctree(std::string name="noname") : event_base(data::kMCTree,name) {clear_data();}
    
    /// Copy constructor
    event_mctree(const event_mctree& original) : std::vector<larlite::mctree>(original),
						 event_base(original)
    {};
    
    /// Default destructor
    virtual ~event_mctree(){};
    
    /// Clear method
    virtual void clear_data() {event_base::clear_data(); clear(); _particleMap.clear(); }


    void clearMap() { _particleMap.clear(); }

    void AddMapEntry(int trackId, int pos) { _particleMap[trackId] = pos; }

    void setMap(std::map<int,int> m) { _particleMap = m; }

    // given trackId returns position in event_mcpart
    int searchParticleMap(std::set<int>::iterator it){ return _particleMap.find(*it)->second; }

    // given trackId returns position in event_mcpart
    int searchParticleMap(int trackId);

  private:

    // the <event_mcpart position, trackId> map should live here
    // one map for the entire event
    std::map<int,int> _particleMap;
    
  };
}
#endif

/** @} */ // end of doxygen group 
