#ifndef LARLITE_STORAGE_MANAGER_TEMPLATE_CXX
#define LARLITE_STORAGE_MANAGER_TEMPLATE_CXX
//#include "storage_manager.h"
//#include "storage_manager.h"
#include "event_ass.h"

namespace larlite {
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
  
