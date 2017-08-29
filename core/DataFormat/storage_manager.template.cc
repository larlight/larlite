#ifndef LARLITE_STORAGE_MANAGER_TEMPLATE_CXX
#define LARLITE_STORAGE_MANAGER_TEMPLATE_CXX
//#include "storage_manager.h"
//#include "storage_manager.h"
#include "event_ass.h"
namespace larlite {

  template<> data::DataType_t storage_manager::data_type<trigger> () const
  { return data::kTrigger; }

  template<> data::DataType_t storage_manager::data_type<swtrigger> () const
  { return data::kSWTrigger; }

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

  template<> data::DataType_t storage_manager::data_type<event_auxsimch> () const
  { return data::kAuxDetSimChannel; }

  template<> data::DataType_t storage_manager::data_type<event_mcshower> () const
  { return data::kMCShower; }

  template<> data::DataType_t storage_manager::data_type<event_rawdigit> () const
  { return data::kRawDigit;}

  template<> data::DataType_t storage_manager::data_type<event_opdetwaveform> () const
  { return data::kOpDetWaveform; }

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

  template<> data::DataType_t storage_manager::data_type<event_trackmom> () const
  { return data::kTrackMomentum; }

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

  template<> data::DataType_t storage_manager::data_type<event_mctree> () const
  { return data::kMCTree;  }

  template<> data::DataType_t storage_manager::data_type<event_cosmictag> () const
  { return data::kCosmicTag; }

  template<> data::DataType_t storage_manager::data_type<event_minos> () const
  { return data::kMinos; }

  template<> data::DataType_t storage_manager::data_type<event_ass> () const
  { return data::kAssociation; }

  template<> data::DataType_t storage_manager::data_type<event_fifo> () const
  { return data::kFIFO; }

  template<> data::DataType_t storage_manager::data_type<event_pcaxis> () const
  { return data::kPCAxis; }

  template<> data::DataType_t storage_manager::data_type<event_flashmatch> () const
  { return data::kFlashMatch; }

  template<> data::DataType_t storage_manager::data_type<event_simphotons> () const
  { return data::kSimPhotons; }

  template<> data::DataType_t storage_manager::data_type<event_mucsdata> () const
  { return data::kMuCSData; }

  template<> data::DataType_t storage_manager::data_type<event_mucsreco> () const
  { return data::kMuCSReco; }

  template<> data::DataType_t storage_manager::data_type<event_roi> () const
    { return data::kROI; }

  template<> data::DataType_t storage_manager::data_type<event_chstatus> () const
  { return data::kChStatus; }

  template<> data::DataType_t storage_manager::data_type<event_mceventweight> () const
    { return data::kMCEventWeight; }

  template <class T>
  data::DataType_t storage_manager::data_type() const
  { 
    Message::send(msg::kERROR,
		  __PRETTY_FUNCTION__,
		  "No corresponding data::DataType_t enum value found!");
    throw std::exception();
    return data::kUndefined;
  }

  template<> data::SubRunDataType_t storage_manager::subrundata_type<potsummary>() const
  { return data::kPOTSummary; }

  template <class T>
  data::SubRunDataType_t storage_manager::subrundata_type() const
  { 
    Message::send(msg::kERROR,
		  __PRETTY_FUNCTION__,
		  "No corresponding data::SubRunDataType_t enum value found!");
    throw std::exception();
    return data::kSUBRUNDATA_Undefined;
  }

  template <class T>
  data::RunDataType_t storage_manager::rundata_type() const
  { 
    Message::send(msg::kERROR,
		  __PRETTY_FUNCTION__,
		  "No corresponding data::RunDataType_t enum value found!");
    throw std::exception();
    return data::kRUNDATA_Undefined;
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
  /*
  template <class T, class U>
  const storage_manager::AssInfo_t storage_manager::find_one_assid(const T a, const U b) const
  {
    auto const& ev_ass_m = _ptr_data_array[data::kAssociation];
    for(auto const& ev_ass_p : ev_ass_m) {
      auto const& ev_ass = (larlite::event_ass*)(ev_ass_p.second);
      auto id = ev_ass->find_one_assid(a,b);
      if( id != kINVALID_ASS )
	return std::make_pair((const larlite::event_ass*)(ev_ass),id);
    }
    return std::make_pair((const larlite::event_ass*)nullptr,kINVALID_ASS);
  }
  */

  /*
  template <class T>
  const std::vector<std::vector<const T*> >
  storage_manager::create_ass_ptr(const product_id& id) const
  {
    std::vector<std::vector<const T*> > res;
    auto type_b = this->data_type<T>();
    auto const& data_coll_b = _ptr_data_array[type_b];
    if(data_coll_b.size()<1)
      Message::send(msg::kERROR,
		    __PRETTY_FUNCTION__,
		    Form("No data available for associated type %d (%s)",
			 type_b,data::kDATA_TREE_NAME[type_b].c_str())
		    );
    
    auto ass_set = this->find_all_assid(id,type_b);
    for(auto const& ass_info : ass_set) {

      auto const& ev_ass = ass_info.first;
      auto const& ev_ass_id = ass_info.second;
      auto const& id_b = ev_ass->association_keys(ev_ass_id).second;
      auto const& ass = ev_ass->association(ev_ass_id);
      
      if(res.size()<ev_ass.size()) res.resize(ass.size());

      for(size_t index_a=0; index_a<ass.size(); ++index_a) {

	for(auto const& index_b : ass[index_a])
	  res[index_a].push_back( (const T*)(data_coll_b->at(index_b)) );
      }
    }
    return res;
  }
  */
}
#endif
  
