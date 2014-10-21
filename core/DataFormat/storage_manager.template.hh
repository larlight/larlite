#ifndef LARLITE_STORAGE_MANAGER_TEMPLATE_H
#define LARLITE_STORAGE_MANAGER_TEMPLATE_H

namespace larlite{

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_gtruth> () const;
  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_mctruth> () const;
  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_mcpart> () const;
  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_mcflux> () const;
  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_simch> () const;
  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_mcshower> () const;
  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_rawdigit> () const;
  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_wire> () const;
  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_hit> () const;
  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_ophit> () const;
  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_opflash> () const;
  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_cluster> () const;
  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_seed> () const;
  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_spacepoint> () const;
  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_track> () const;
  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_shower> () const;
  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_vertex> () const;
  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_endpoint2d> () const;
  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_calorimetry> () const;
  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_partid> () const;
  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_pfpart> () const;
  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_user> () const;

  /*
  template <class T>
  const ::larlite::data::DataType_t storage_manager::data_type() const
  { 
    Message::send(msg::kERROR,
		  __PRETTY_FUNCTION__,
		  "No corresponding data::DataType_t enum value found!");
    throw std::exception();
    return data::kUndefined;
  }

  //template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_trigger> ()
  //{ return data::kTrigger; }

  template <class T>
  T* storage_manager::get_data(std::string const name)
  {
    auto type = data_type<T>();
    return (T*)(get_data(type,name));
  }
  */
}
#endif
