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
#include <TObject.h>
#include <TString.h>

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
					     _name(original._name)
    {}

    
    /// Default destructor
    virtual ~event_base(){}
    
    /// Clear method
    virtual void clear_data();
    
    /// run number setter
    void set_run      (UInt_t run) { fRunNumber    = run; }
    /// sub-run number setter
    void set_subrun   (UInt_t run) { fSubRunNumber = run; }
    /// event-id setter
    void set_event_id (UInt_t id ) { fEventID      = id;  }
    
    /// run number getter
    UInt_t run      () const { return fRunNumber;    }
    /// sub-run number getter
    UInt_t subrun   () const { return fSubRunNumber; }
    /// event-id getter
    UInt_t event_id () const { return fEventID;      }
    /// data type getter
    data::DataType_t data_type() const {return _type; }
    /// producer's name
    const std::string& name() { return _name; }

  protected:
    
    /// Run number
    UInt_t fRunNumber;
    
    /// Sub-Run number
    UInt_t fSubRunNumber;
    
    /// Event ID
    UInt_t fEventID;

    /// Producer's name
    std::string _name;

    ////////////////////////
    ClassDef(event_base,1)
    ////////////////////////
      
  };
}
#endif
/** @} */ // end of doxygen group 
