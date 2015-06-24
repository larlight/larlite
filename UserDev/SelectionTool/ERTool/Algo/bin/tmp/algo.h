/**
 * \file Algo_Class_Name.h
 *
 * \ingroup Working_Package
 * 
 * \brief Class def header for a class Algo_Class_Name
 *
 * @author Shell_User_Name
 */

/** \addtogroup Working_Package

    @{*/

#ifndef ERTOOL_ALGO_CLASS_NAME_H
#define ERTOOL_ALGO_CLASS_NAME_H

#include "ERTool/Base/AlgoBase.h"

namespace ertool {

  /**
     \class Algo_Class_Name
     User custom Algorithm class made by kazuhiro
   */
  class Algo_Class_Name : public AlgoBase {
  
  public:

    /// Default constructor
    Algo_Class_Name(const std::string& name="Algo_Class_Name");

    /// Default destructor
    virtual ~Algo_Class_Name(){};

    /// Reset function
    void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    bool Reconstruct(const EventData &data, ParticleGraph& graph);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);

  };
}
#endif

/** @} */ // end of doxygen group 
