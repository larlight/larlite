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
#include "Env.h"
#include "TObject.h"
namespace ertool {

  class EventData;

  /**
     \class RecoObjBase
     User defined class ertool::RecoObjBase ... these comments are used to generate
     doxygen documentation!
  */
  class RecoObjBase : public TObject{

    friend class EventData;
    
  public:
    
    /// Default constructor
    RecoObjBase(){Reset();}
    
    /// Default destructor
    virtual ~RecoObjBase(){}

    /// Reset variables
    void Reset();

    /// Cosmic score
    double _cosmogenic;

    /// Function to ask if this object is filtered
    bool IsFiltered() const { return _exclude; }

  protected:

    /// Function to set a filter flag
    void Filter(bool doit=true) { _exclude = doit; }

    /// Filter flag
    bool _exclude;

    ClassDef(RecoObjBase,1)
  };
}

#endif
/** @} */ // end of doxygen group 

