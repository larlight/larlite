/**
 * \file ProtoShowerAlgBase.h
 *
 * \ingroup ProtoShower
 * 
 * \brief Class def header for a class ProtoShowerAlgBase
 *
 * @author david caratelli
 */

/** \addtogroup ProtoShower

    @{*/
#ifndef PROTOSHOWERALGBASE_H
#define PROTOSHOWERALGBASE_H

#include <iostream>
#include "DataFormat/storage_manager.h"


#include "ShowerReco3D/Base/ShowerRecoTypes.h"

/**
   \class ProtoShowerAlgBase
   User defined class ProtoShowerAlgBase ... these comments are used to generate
   doxygen documentation!
 */

namespace showerreco {  

  class ProtoShowerAlgBase{
    
  public:
    
    /// Default constructor
    ProtoShowerAlgBase(){}
    
    /// Default destructor
    ~ProtoShowerAlgBase(){}
    
    virtual void GenerateProtoShower(::larlite::storage_manager* storage,
				     ::larlite::event_pfpart* ev_pfpart,
				     const size_t proto_shower_pfpart,
				     showerreco::ProtoShower & proto_shower) = 0;
    
    
  };
  
}// namespace
    
#endif
/** @} */ // end of doxygen group 

