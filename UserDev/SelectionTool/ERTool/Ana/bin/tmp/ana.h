/**
 * \file Ana_Class_Name.h
 *
 * \ingroup Working_Package
 * 
 * \brief Class def header for a class Ana_Class_Name
 *
 * @author Shell_User_Name
 */

/** \addtogroup Working_Package

    @{*/

#ifndef ERTOOL_ANA_CLASS_NAME_H
#define ERTOOL_ANA_CLASS_NAME_H

#include "ERTool/Base/AnaBase.h"

namespace ertool {

  /**
     \class Ana_Class_Name
     User custom Analysis class made by kazuhiro
   */
  class Ana_Class_Name : public AnaBase {
  
  public:

    /// Default constructor
    Ana_Class_Name(const std::string& name="Ana_Class_Name");

    /// Default destructor
    virtual ~Ana_Class_Name(){}

    /// Reset function
    virtual void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    bool Analyze(const EventData &data, const ParticleGraph &ps);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);

  };
}
#endif

/** @} */ // end of doxygen group 
