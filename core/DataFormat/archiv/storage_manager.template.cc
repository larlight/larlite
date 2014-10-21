#ifndef LARLITE_STORAGE_MANAGER_TEMPLATE_CC
#define LARLITE_STORAGE_MANAGER_TEMPLATE_CC

namespace larlite{

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_gtruth> () const
  { return data::kGTruth; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_mctruth> () const
  { return data::kMCTruth; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_mcpart> () const
  { return data::kMCParticle; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_mcflux> () const
  { return data::kMCFlux; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_simch> () const
  { return data::kSimChannel; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_mcshower> () const
  { return data::kMCShower; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_rawdigit> () const
  { return data::kRawDigit; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_wire> () const
  { return data::kWire; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_hit> () const
  { return data::kHit; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_ophit> () const
  { return data::kOpHit; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_opflash> () const
  { return data::kOpFlash; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_cluster> () const
  { return data::kCluster; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_seed> () const
  { return data::kSeed; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_spacepoint> () const
  { return data::kSpacePoint; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_track> () const
  { return data::kTrack; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_shower> () const
  { return data::kShower; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_vertex> () const
  { return data::kVertex; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_endpoint2d> () const
  { return data::kEndPoint2D; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_calorimetry> () const
  { return data::kCalorimetry; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_partid> () const
  { return data::kParticleID; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_pfpart> () const
  { return data::kPFParticle; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_user> () const
  { return data::kUserInfo; }

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



}
#endif
