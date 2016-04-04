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

#include "DataFormat/pfpart.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"
#include "DataFormat/spacepoint.h"
#include "DataFormat/seed.h"
#include "DataFormat/vertex.h"

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
    
    virtual void GenerateProtoShower(const std::vector<::larlite::cluster> &clus_v, 
				     const std::vector< std::vector<::larlite::hit> > &hit_v,
				     const std::vector<::larlite::spacepoint> &sps_v,
				     const std::vector<::larlite::vertex> &vtx,
				     showerreco::ProtoShower & proto_shower) = 0;
    
    
  };
  
}// namespace
    
#endif
/** @} */ // end of doxygen group 

