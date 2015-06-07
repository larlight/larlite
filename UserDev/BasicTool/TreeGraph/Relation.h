/**
 * \file Relation.h
 *
 * \ingroup GeoTree
 * 
 * \brief Class def header for a class geotree::Relation
 *
 * @author david caratelli
 */

/** \addtogroup GeoTree
    
    @{*/
#ifndef RELATION_H
#define RELATION_H

#include "Node.h"

namespace geotree{

  class Relation{
    
  public:

    Relation(){};

    RelationType_t Child() { return geotree::RelationType_t::kChild; }

    RelationType_t Sibling() { return geotree::RelationType_t::kSibling; }

    RelationType_t Parent() { return geotree::RelationType_t::kParent; }

  };

}

#endif
/** @} */ // end of doxygen group 
