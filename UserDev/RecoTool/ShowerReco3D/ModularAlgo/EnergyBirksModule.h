/**
 * \file EnergyBirksModule.h
 *
 * \ingroup ModularAlgo
 * 
 * \brief Class def header for a class EnergyBirksModule
 *
 * @author david caratelli
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef ENERGYBIRKSMODULE_H
#define ENERGYBIRKSMODULE_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
#include "AnalysisAlg/CalorimetryAlg.h"

/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

  class EnergyBirksModule : ShowerRecoModuleBase{

  public:

    /// Default constructor
    EnergyBirksModule();

    /// Default destructor
    ~EnergyBirksModule(){}

    void do_reconstruction(const ShowerClusterSet_t &, Shower_t &);

    void SetUseArea(bool on) { _useArea = on; }

  private:

    /// Calorimetry algorithm
    ::calo::CalorimetryAlg *_caloAlg;

    /// flag for whether to decide if we should use the hit area or amplitude for energy calculations
    bool _useArea;

  };
  
} // showerreco

#endif
/** @} */ // end of doxygen group 
