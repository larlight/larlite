/**
 * \file storage_store.h
 *
 * \ingroup DataFormat
 * 
 * \brief container for storage manager that derives from TObject
 *
 * @author Taritree - MIT 2016
 *
 * This exists for passing storage manager instances into python.  
 * There was issues for doing this directly for storage_manager instanes, likely
 * due to the multiple inherentance of storage_manager.  So,
 * this is a workaround until I can fix that.
 */
/** \addtogroup DataFormat

    @{*/

#ifndef LARLITE_STORAGE_STORE_H
#define LARLITE_STORAGE_STORE_H

#include <TObject.h>
#include "DataFormat/storage_manager.h"

#include <string>
#include <vector>
#include <map>
#include <set>

namespace larlite {

  class storage_store : public TObject {

  public:
    storage_store();
    ~storage_store();

  protected:
    static storage_store* gStorageStore;

    std::set< std::string > storage_names;
    std::map< std::string, storage_manager* > storage_map;
    

  public:
/*     // provide singleton? */
/*     static storage_store* getTheStorageStore() {  */
/*       if ( gStorageStore==NULL ) */
/* 	gStorageStore = new storage_store; */
/*       return gStorageStore;  */
/*     }; */

    void add_store( std::string name, storage_manager* storage_ptr );
    storage_manager* get_store( std::string name );

    ClassDef( storage_store, 1 );

  };


};

#endif

