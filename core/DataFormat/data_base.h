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

#include "Base/DataFormatConstants.h"
#include "larlite_dataformat_utils.h"
#include <TString.h>
#include "DataFormatException.h"
namespace larlite{

  class storage_manager;

  /**
     \class data_base
     A base coass for all data objects in LArLight
  */

  class data_base {
    
  public:
    
    /// Default constructor
    data_base(unsigned short type = data::kINVALID_USHORT);

    /// Default copy constructor to avoid memory leak in ROOT streamer
    data_base(const data_base &original) : _type(original._type) {}
    
    /// Default destructor
    virtual ~data_base(){}
    
    /// Clear method
    virtual void clear_data(){}
    
    /// data type getter
    const unsigned short& data_type() const;

  protected:

    /// Product type
    unsigned short _type;

  };

  /**
     \class output_base
     A base class for a producer output data product: it knows about who the hell made it. \n
     All input/output instance held directly by the fmwk IO (i.e. larlite::storage_manager)\n
     must inherit from this class.
   */
  class output_base : public data_base {
  public:
    /// Default ctor
    output_base(unsigned short type = data::kINVALID_USHORT,
		const std::string name = "noname");
    /// Copy ctor
    output_base(const output_base& orig);

    /// Default dtor
    ~output_base() {}

    /// Inherited method to clear
    virtual void clear_data();
    /// producer's name
    const std::string& name() const;
    /// product_id creator
    const ::larlite::product_id id() const;
    
  private:
    ::larlite::product_id _id;

  };

  /**
     \class run_base
     A base class for run-wise data products
   */
  class run_base : public output_base
  {
    friend class storage_manager;
  public:
    /// Default ctor
    run_base(const unsigned short type = data::kINVALID_USHORT,
	     const std::string name = "noname");
    /// Copy ctor
    run_base(const run_base& origin);

    /// Default dtor
    ~run_base() {}
    /// Inherited clear data
    virtual void clear_data();

    //
    // Simple type variable getters
    // 
    /// Run number getter
    const unsigned int& run() const;

  private:
    unsigned int fRunNumber; ///< run number

  private:
    /// run number setter
    void set_run (unsigned int run);

  };

  /**
     \class subrun_base
     A base class for subrun-wise data products
   */
  class subrun_base : public output_base
  {
    friend class storage_manager;
  public:
    /// Default ctor
    subrun_base(const unsigned short type = data::kINVALID_USHORT,
		const std::string name = "noname");

    /// Copy ctor
    subrun_base(const subrun_base& origin);

    /// Default dtor
    virtual ~subrun_base() {}
    /// Inherited clear method
    virtual void clear_data();

    //
    // Simple type variable getters
    // 
    /// Run number getter
    const unsigned int& run() const;
    /// Sub-Run number getter
    const unsigned int& subrun() const;

  private:
    unsigned int fRunNumber; ///< run number
    unsigned int fSubRunNumber; ///< sub run number

  private:
    /// run number setter
    void set_run      (unsigned int run);
    /// sub-run number setter
    void set_subrun   (unsigned int run);

  };

  /**
     \class event_base
     A base class for event-wise data holder class
  */
  class event_base : public output_base 
  {
    friend class storage_manager;
  public:
    
    /// Default constructor
    event_base(unsigned short    type = data::kINVALID_USHORT,
	       const std::string name = "noname");
    
    /// Default copy constructor to avoid memory leak in ROOT streamer
    event_base(const event_base &original);
    
    /// Default destructor
    virtual ~event_base(){}
    
    /// Clear method
    virtual void clear_data();

    //
    // Simple type variable getters
    //     
    /// run number getter
    unsigned int run      () const;
    /// sub-run number getter
    unsigned int subrun   () const;
    /// event-id getter
    unsigned int event_id () const;

  private:
    unsigned int fRunNumber;    ///< Run number
    unsigned int fSubRunNumber; ///< Sub-Run number
    unsigned int fEventID;      ///< Event ID

  private:
    /// run number setter
    void set_run      (unsigned int run);
    /// sub-run number setter
    void set_subrun   (unsigned int run);
    /// event-id setter
    void set_event_id (unsigned int id );

  };
}
#endif
/** @} */ // end of doxygen group 
