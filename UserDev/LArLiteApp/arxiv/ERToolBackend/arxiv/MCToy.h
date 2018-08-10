/**
 * \file MCToy.h
 *
 * \ingroup ERToolBackend
 * 
 * \brief Class def header for a class MCToy
 *
 * @author kazuhiro
 */

/** \addtogroup ERToolBackend

    @{*/
#ifndef MCTOY_H
#define MCTOY_H

#include <iostream>
#include "ParticleID.h"
#include "DataFormat/DataFormat-TypeDef.h"
#include "ERTool/Base/EmptyInput.h"

#include "LArUtil/ElecClock.h"
#include "LArUtil/Geometry.h"
#include "EMShowerTools/EMShowerProfile.h"

namespace ertool {
  /**
     \class MCToy
     User defined class MCToy ... these comments are used to generate
     doxygen documentation!
  */
  class MCToy{
    
  public:
    
    /// Default constructor
    MCToy(){}
    
    /// Default destructor
    ~MCToy(){}

    void FillMCInfo( const event_mctruth&   mci_v,
		     const event_mcshower&  mcs_v,
		     const event_mctrack&   mct_v,
		     ::ertool::io::EmptyInput& strm) const;
    
  };
}

#endif
/** @} */ // end of doxygen group 

