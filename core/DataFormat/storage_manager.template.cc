#ifndef LARLITE_STORAGE_MANAGER_TEMPLATE_CXX
#define LARLITE_STORAGE_MANAGER_TEMPLATE_CXX
//#include "storage_manager.h"
//#include "storage_manager.h"

namespace larlite {

  template<> data::DataType_t storage_manager::data_type<potsummary> () const
  { return data::kPOTSummary; }

  template<> data::DataType_t storage_manager::data_type<event_gtruth> () const
  { return data::kGTruth; }

  template<> data::DataType_t storage_manager::data_type<event_mctruth> () const
  { return data::kMCTruth; }

  template<> data::DataType_t storage_manager::data_type<event_mcpart> () const
  { return data::kMCParticle; }

  template<> data::DataType_t storage_manager::data_type<event_mcflux> () const
  { return data::kMCFlux; }

  template<> data::DataType_t storage_manager::data_type<event_simch> () const
  { return data::kSimChannel; }

  template<> data::DataType_t storage_manager::data_type<event_mcshower> () const
  { return data::kMCShower; }

  template<> data::DataType_t storage_manager::data_type<event_rawdigit> () const
  { return data::kRawDigit; }

  template<> data::DataType_t storage_manager::data_type<event_wire> () const
  { return data::kWire; }

  template<> data::DataType_t storage_manager::data_type<event_hit> () const
  { return data::kHit; }

  template<> data::DataType_t storage_manager::data_type<event_ophit> () const
  { return data::kOpHit; }

  template<> data::DataType_t storage_manager::data_type<event_opflash> () const
  { return data::kOpFlash; }

  template<> data::DataType_t storage_manager::data_type<event_cluster> () const
  { return data::kCluster; }

  template<> data::DataType_t storage_manager::data_type<event_seed> () const
  { return data::kSeed; }

  template<> data::DataType_t storage_manager::data_type<event_spacepoint> () const
  { return data::kSpacePoint; }

  template<> data::DataType_t storage_manager::data_type<event_track> () const
  { return data::kTrack; }

  template<> data::DataType_t storage_manager::data_type<event_shower> () const
  { return data::kShower; }

  template<> data::DataType_t storage_manager::data_type<event_vertex> () const
  { return data::kVertex; }

  template<> data::DataType_t storage_manager::data_type<event_endpoint2d> () const
  { return data::kEndPoint2D; }

  template<> data::DataType_t storage_manager::data_type<event_calorimetry> () const
  { return data::kCalorimetry; }

  template<> data::DataType_t storage_manager::data_type<event_partid> () const
  { return data::kParticleID; }

  template<> data::DataType_t storage_manager::data_type<event_pfpart> () const
  { return data::kPFParticle; }

  template<> data::DataType_t storage_manager::data_type<event_user> () const
  { return data::kUserInfo; }

  template<> data::DataType_t storage_manager::data_type<event_mctrack> () const
  { return data::kMCTrack;  }

  template <class T>
  data::DataType_t storage_manager::data_type() const
  { 
    Message::send(msg::kERROR,
		  __PRETTY_FUNCTION__,
		  "No corresponding data::DataType_t enum value found!");
    throw std::exception();
    return data::kUndefined;
  }

  //template<> data::DataType_t storage_manager::data_type<event_trigger> () const
  //{ return data::kTrigger; }

  /*
  template <class T>
  T* storage_manager::get_data(std::string const name)
  {
    auto type = data_type<T>();
    return (T*)(get_data(type,name));
  }
  */
}
#endif
  
