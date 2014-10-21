#ifndef LARLITE_STORAGE_MANAGER_TEMPLATE_CC
#define LARLITE_STORAGE_MANAGER_TEMPLATE_CC

namespace larlite{

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_gtruth> ()
  { return data::kGTruth; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_mctruth> ()
  { return data::kMCTruth; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_mcpart> ()
  { return data::kMCParticle; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_mcflux> ()
  { return data::kMCFlux; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_simch> ()
  { return data::kSimChannel; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_mcshower> ()
  { return data::kMCShower; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_rawdigit> ()
  { return data::kRawDigit; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_wire> ()
  { return data::kWire; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_hit> ()
  { return data::kHit; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_ophit> ()
  { return data::kOpHit; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_opflash> ()
  { return data::kOpFlash; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_cluster> ()
  { return data::kCluster; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_seed> ()
  { return data::kSeed; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_spacepoint> ()
  { return data::kSpacePoint; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_track> ()
  { return data::kTrack; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_shower> ()
  { return data::kShower; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_vertex> ()
  { return data::kVertex; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_endpoint2d> ()
  { return data::kEndPoint2D; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_calorimetry> ()
  { return data::kCalorimetry; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_partid> ()
  { return data::kParticleID; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_pfpart> ()
  { return data::kPFParticle; }

  template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_user> ()
  { return data::kUserInfo; }

  //template<> const ::larlite::data::DataType_t storage_manager::data_type<::larlite::event_trigger> ()
  //{ return data::kTrigger; }

}
#endif
