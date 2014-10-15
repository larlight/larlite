/**
 * \file association.h
 *
 * \ingroup DataFormat
 * 
 * \brief association class def
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/
#ifndef ASSOCIATION_H
#define ASSOCIATION_H

#include "data_base.h"
#include <map>

namespace larlight{
  /**
     \class association
     Hold association among various objects.
     Association is stored in terms of a vector of index numbers.
  */

  class association : public std::map<std::string,std::vector<std::vector<unsigned int> > >,
		      public event_base {
    
  public:
    
    /// Default constructor
    association() : event_base(data::kAssociation)
    { event_base::clear_data(); }
    
    /// Default copy constructor to avoid memory leak in ROOT streamer
    association(const association &original) : std::map<std::string,std::vector<std::vector<unsigned int> > >(original),
					       event_base(original)
    {}
    
    /// Default destructor
    virtual ~association(){}
    
    /// Clear method
    virtual void clear_data() { event_base::clear_data(); clear();}

    /// Adder for an association
    void insert(std::string name, 
		const size_t index_source,
		const std::vector<unsigned int>& ass);

    /// Adder for a while set of association
    void insert(std::string name,
		const std::vector<std::vector<unsigned int> >& ass);

    /// Getter for # of associated data product types
    size_t size(std::string name) const;

    /// Getter for # of associated data product instances
    size_t size(std::string name, size_t index_source) const;

    /// Getter for associated data product indecies for all objects
    const std::vector<std::vector<unsigned int> > query(std::string name) const;

    /// Getter for associated data product indecies from one object
    const std::vector<unsigned int> query(std::string name, size_t index_source) const;

  protected:
    
    ////////////////////////
    ClassDef(association,1)
    ////////////////////////
      
  };

}
#endif
/** @} */ // end of doxygen group 
