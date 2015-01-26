/**
 * \file MakeParticleTree.h
 *
 * \ingroup ParticleTree
 * 
 * \brief Class def header for a class MakeParticleTree
 *
 * @author David Caratelli
 */

/** \addtogroup ParticleTree

    @{*/
#ifndef MAKEPARTICLETREE_H
#define MAKEPARTICLETREE_H

#include <iostream>
#include "Analysis/ana_base.h"
#include "DataFormat/treenode.h"

namespace larlite {
  /**
     \class MakeParticleTree
     User defined class MakeParticleTree ... these comments are used to generate
     doxygen documentation!
  */
  class MakeParticleTree{
    
    
    void setParticleMap(event_mctree *event_tree, event_mcpart *event_part);
    
    int searchParticleMap(std::set<int>::iterator it){ return _ParticleMap.find(*it)->second; }
    
    int searchParticleMap(const int trackID);
    
    void setTrees(event_mctree *event_tree, event_mcpart *event_part);
    
    void AddNodes(mcpart part, treenode& parentnode, int ancestor, event_mcpart *event_part);
    
  public:
    
    /// Default constructor
    MakeParticleTree(){};
    
    /// Default destructor
    virtual ~MakeParticleTree(){};

  private:
    
    std::map<int, int> _ParticleMap;

    
  };

}
  
#endif
/** @} */ // end of doxygen group 

