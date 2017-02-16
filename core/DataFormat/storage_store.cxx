#include "DataFormat/storage_store.h"

ClassImp(larlite::storage_store);

namespace larlite {

  //storage_store* storage_store::gStorageStore = NULL;

  storage_store::storage_store() {}
  
  storage_store::~storage_store() {}

  void storage_store::add_store( std::string name, storage_manager* storage_ptr ) {
    storage_map[name] = storage_ptr;
    storage_names.insert( name );
  }

  storage_manager* storage_store::get_store( std::string name ) {
    auto it = storage_names.find(name);
    if ( it==storage_names.end() ) return NULL;
    return storage_map[name];
  }

};
