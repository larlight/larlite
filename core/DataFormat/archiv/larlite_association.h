/**
 * \file larlite_association.h
 *
 * \ingroup DataFormat
 * 
 * \brief association class def
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/
#ifndef LARLITE_ASSOCIATION_H
#define LARLITE_ASSOCIATION_H

#include "data_base.h"
#include <vector>
#include <map>

namespace larlite{

  typedef std::vector<unsigned int> AssUnit_t;
  typedef std::vector<AssUnit_t > AssSet_t;

  /**
     \class association
     Hold association among various objects.
     Association is stored in terms of a vector of index numbers.
  */
  class association : public event_base {
    
  public:
    
    /// Default constructor
    association(const std::string name="noname") : event_base(data::kAssociation,name)
    { event_base::clear_data(); }
    
    /// Default copy constructor to avoid memory leak in ROOT streamer
    association(const association &original) : event_base(original),
					       _data(original._data)
    {}
    
    /// Default destructor
    virtual ~association(){}
    
    /// Clear method
    virtual void clear_data() { event_base::clear_data(); _data.clear();}

    /// Adder for an association
    void insert(std::string name, 
		const size_t index_source,
		const AssUnit_t& ass);

    /// Adder for a while set of association
    void insert(std::string name,
		const AssSet_t& ass);

    /// Getter for # of associated data product types
    size_t size(std::string name) const;

    /// Getter for # of associated data product instances
    size_t size(std::string name, size_t index_source) const;

    /// Getter for associated data product indecies for all objects
    const AssSet_t& get(std::string name) const;

    /// Getter for associated data product indecies from one object
    const AssUnit_t& get(std::string name, size_t index_source) const;

  protected:

    /// Association storage
    std::map<std::string,AssSet_t> _data;

    ////////////////////////
    ClassDef(association,1)
    ////////////////////////
      
  };

}
#endif
/** @} */ // end of doxygen group 
