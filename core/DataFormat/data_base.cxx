#ifndef LARLITE_DATA_BASE_CXX
#define LARLITE_DATA_BASE_CXX

#include "data_base.h"

namespace larlite {

  //
  // data_base
  //
  
  data_base::data_base(unsigned short type)
    : _type(type)
  { clear_data(); }

  const unsigned short& data_base::data_type() const
  {return _type; }  

  //
  // output_base
  //

  output_base::output_base(unsigned short type,
			   const std::string name)
    : data_base(type)
    , _id(_type,name)
  { clear_data(); }

  output_base::output_base(const output_base& orig)
    : data_base(orig)
    , _id(orig._id)
  {}

  void output_base::clear_data()
  { data_base::clear_data(); }

  const std::string& output_base::name() const
  { return _id.second; }

  const ::larlite::product_id output_base::id() const
  { return _id; }

  //
  // run_base
  //
  run_base::run_base(const unsigned short type,
		     const std::string name)
    : output_base(type,name)
  { clear_data(); }

  run_base::run_base(const run_base& origin) : output_base(origin)
					     , fRunNumber(origin.fRunNumber)
  {}

  void run_base::clear_data()
  { output_base::clear_data(); fRunNumber=kINVALID_RUN;}

  const unsigned int& run_base::run() const
  { return fRunNumber; }

  void run_base::set_run(unsigned int run)
  { fRunNumber    = run; }
  
  //
  // subrun_base
  //
  subrun_base::subrun_base(const unsigned short type,
			   const std::string name)
    : output_base(type,name)
  { clear_data(); }

  subrun_base::subrun_base(const subrun_base& origin)
  : output_base(origin)
      , fRunNumber(origin.fRunNumber)
      , fSubRunNumber(origin.fSubRunNumber)
  {}

  void subrun_base::clear_data()
  {
    output_base::clear_data(); 
    fRunNumber=kINVALID_RUN; 
    fSubRunNumber=kINVALID_SUBRUN;
  }

  const unsigned int& subrun_base::run() const
  { return fRunNumber; }

  const unsigned int& subrun_base::subrun() const
  { return fSubRunNumber; }  

  void subrun_base::set_run    (unsigned int run) { fRunNumber    = run; }

  void subrun_base::set_subrun (unsigned int run) { fSubRunNumber = run; }

  //
  // event_base
  //

  event_base::event_base(unsigned short    type,
			 const std::string name)
    : output_base(type,name)
  { clear_data(); }
  
  event_base::event_base(const event_base &original) 
    : output_base(original)
    , fRunNumber(original.fRunNumber)
    , fSubRunNumber(original.fSubRunNumber)
    , fEventID(original.fEventID)
  {}
  
  void event_base::clear_data()
  {
    output_base::clear_data();
    fRunNumber = kINVALID_RUN;
    fSubRunNumber = kINVALID_SUBRUN;
    fEventID = kINVALID_EVENT;
  }

  unsigned int event_base::run      () const { return fRunNumber;    }
  unsigned int event_base::subrun   () const { return fSubRunNumber; }
  unsigned int event_base::event_id () const { return fEventID;      }
  
  void event_base::set_run      (unsigned int run) { fRunNumber    = run; }
  void event_base::set_subrun   (unsigned int run) { fSubRunNumber = run; }
  void event_base::set_event_id (unsigned int id ) { fEventID      = id;  }

}

#endif
