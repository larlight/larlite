/**
 * \file RecoObjBase.h
 *
 * \ingroup Base
 * 
 * \brief Class def header for a class RecoObjBase
 *
 * @author kazuhiro
 */

/** \addtogroup Base

    @{*/
#ifndef ERTOOL_RECOOBJBASE_H
#define ERTOOL_RECOOBJBASE_H

#include <iostream>
#include "ERToolTypes.h"
#include "ERToolConstants.h"
namespace ertool {
  class EventData;

  /**
     \class RecoObjBase
     User defined class ertool::RecoObjBase ... these comments are used to generate
     doxygen documentation!
  */
  class RecoObjBase {

    friend class EventData;
    
  public:
    
    /// Default constructor
    RecoObjBase();
    
    /// Default destructor
    virtual ~RecoObjBase(){}

    /// Reset method (do nothing in the base class)
    virtual void Reset(){};
    
    /// ID query
    RecoID_t RecoID() const;

    /// Type query
    RecoType_t RecoType() const;

  protected:

    /// Function to set an id
    void SetRecoInfo(const RecoID_t id, const RecoType_t type);

    RecoID_t   _reco_id;   ///< Unique reco object identifier
    RecoType_t _reco_type; ///< Reco object type

  };
}

#endif
/** @} */ // end of doxygen group 

