/**
 * \file UnitBase.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class UnitBase
 *
 * @author kazuhiro
 */

/** \addtogroup ERTool

    @{*/
#ifndef ERTOOL_UNITBASE_H
#define ERTOOL_UNITBASE_H

#include <iostream>
#include <TFile.h>
#include "FhiclLite/PSet.h"
#include "ERToolTypes.h"
#include "MessageUtil.h"
namespace ertool {
  class Manager;
  /**
     \class UnitBase
     @brief Base class for ertool::AlgoBase and ertool::FilterBase
     This abstract base class introduces multiple common functions for children classes \n
     (ertool::AlgoBase and ertool::FilterBase) to fit in the ERTool fmwk.      \n

     As the class inherits from ertool::Record, it has a systemtaic procedure to store/load parameter \n
     sets (variable name & values) from larlite::user_info.
 */
  //class UnitBase : public Record {
  class UnitBase : public MessageUtil{
    friend class Manager;
  public:
    
    /// Default constructor
    UnitBase(const std::string& name="");
    
    /// Default destructor
    virtual ~UnitBase(){}

    /// Name getter
    const std::string& Name() const;

    /// Function to accept fclite::PSet
    virtual void AcceptPSet(const ::fcllite::PSet& cfg) = 0;
    
    /// Function to reset itself
    virtual void Reset() = 0;

    /// Called @ before processing the first event sample
    virtual void ProcessBegin() = 0;

    /// Called after processing the last event sample
    virtual void ProcessEnd(TFile* fout=nullptr) = 0;
    
    /// Boolean to set whether the use is in the training mode or not
    bool _training_mode;

  protected:

    ::fcllite::PSet& OutputPSet();

  private:
    /// Name
    std::string _name;

    /// Output configuration
    ::fcllite::PSet _out_conf;
    
  };
}

#endif
/** @} */ // end of doxygen group 

