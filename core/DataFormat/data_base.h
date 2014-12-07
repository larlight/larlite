/**
 * \file data_base.h
 *
 * \ingroup DataFormat
 * 
 * \brief Base class def for data class
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef LARLITE_DATA_BASE_H
#define LARLITE_DATA_BASE_H

#include "Base/Base-TypeDef.h"
#include "larlite_dataformat_utils.h"
#include <TObject.h>
#include <TString.h>
#include "DataFormatException.h"
namespace larlite{

  /**
     \class data_base
     A base coass for all data objects in LArLight
  */

  class data_base : public TObject {
    
  public:
    
    /// Default constructor
    data_base(data::DataType_t type = data::kDATA_TYPE_MAX) : 
      TObject(),
      _type(type)
    { clear_data(); }
    
    /// Default copy constructor to avoid memory leak in ROOT streamer
    data_base(const data_base &original) : TObject(original),
					   _type(original._type)
    {}
    
    /// Default destructor
    virtual ~data_base(){}
    
    /// Clear method
    virtual void clear_data() {}
    
    /// data type getter
    data::DataType_t data_type() const {return _type; }

  protected:
    
    /// DATA_TYPE
    data::DataType_t _type;

    ////////////////////////
    ClassDef(data_base,1)
    ////////////////////////
      
  };

  /**
     \class event_base
     A base coass for event-wise data holder class
  */
  class event_base : public data_base 
  {
  public:
    
    /// Default constructor
    event_base(data::DataType_t  type = data::kDATA_TYPE_MAX,
	       const std::string name = "noname") :
      data_base(type),
      _name(name)
    { clear_data(); }
    
    /// Default copy constructor to avoid memory leak in ROOT streamer
    event_base(const event_base &original) : data_base(original),
					     fRunNumber(original.fRunNumber),
					     fSubRunNumber(original.fSubRunNumber),
					     fEventID(original.fEventID),
					     _name(original._name),
					     fAssociation(original.fAssociation)
    {}

    
    /// Default destructor
    virtual ~event_base(){}
    
    /// Clear method
    virtual void clear_data();
    
    //
    // Simple type variable setters
    // 
    /// run number setter
    void set_run      (unsigned int run) { fRunNumber    = run; }
    /// sub-run number setter
    void set_subrun   (unsigned int run) { fSubRunNumber = run; }
    /// event-id setter
    void set_event_id (unsigned int id ) { fEventID      = id;  }

    //
    // Simple type variable getters
    //     
    /// run number getter
    unsigned int run      () const { return fRunNumber;    }
    /// sub-run number getter
    unsigned int subrun   () const { return fSubRunNumber; }
    /// event-id getter
    unsigned int event_id () const { return fEventID;      }
    /// data type getter
    data::DataType_t data_type() const {return _type; }
    /// producer's name
    const std::string& name() const { return _name; }
    /// Product ID
    ::larlite::product_id id() const { return ::larlite::product_id(_type,_name); }

    //
    // Association adders
    //
    /// Adder for an association
    void set_association(const product_id type,
			 const size_t index_source,
			 const AssUnit_t& ass);

    /// Adder for a while set of association
    void set_association(const product_id type,
			 const AssSet_t& ass);

    /// Adder for an association
    void set_association(const data::DataType_t data_type,
			 const std::string data_name,
			 const size_t index_source,
			 const AssUnit_t& ass)
    { set_association(product_id(data_type,data_name),index_source,ass); }

    /// Adder for a while set of association
    void set_association(const data::DataType_t data_type,
			 const std::string data_name,
			 const AssSet_t& ass)
    { set_association(product_id(data_type,data_name),ass); }

    /// Getter for # of associated data product types
    size_t size_association(const product_id type) const;

    /// Getter for # of associated data product instances
    size_t size_association(const product_id type, const size_t index_source) const;

    /// Getter for associated data product indecies for all objects
    const AssSet_t& association(const product_id type) const;

    /// Getter for associated data product indecies from one object
    const AssUnit_t& association(const product_id type, const size_t index_source) const;

    /// Getter for associated data product indecies for all objects
    const AssSet_t& association(const data::DataType_t type, const std::string name) const
    { return association(product_id(type,name)); }

    /// Getter for associated data product indecies from one object
    const AssUnit_t& association(const data::DataType_t type, const std::string name,
				 const size_t index_source) const
    { return association(product_id(type,name),index_source); }

    /// Getter for associated data products' key info (product_id)
    const std::vector<larlite::product_id> association_keys() const;

    /// Getter for associated data products' key info (product_id)
    const std::vector<std::string> association_keys(const data::DataType_t type) const;

    /// Getter for associated data products' key info (product_id)
    const std::vector<std::string> association_keys(const data::DataType_t type,
						    const size_t index_source) const;
    
    /// List association
    void list_association() const;

  protected:
    
    /// Run number
    unsigned int fRunNumber;
    
    /// Sub-Run number
    unsigned int fSubRunNumber;
    
    /// Event ID
    unsigned int fEventID;

    /// Producer's name
    std::string _name;
    
    /// Association data holder
    AssMap_t fAssociation;

    ////////////////////////
    ClassDef(event_base,1)
    ////////////////////////
      
  };
}
#endif
/** @} */ // end of doxygen group 
