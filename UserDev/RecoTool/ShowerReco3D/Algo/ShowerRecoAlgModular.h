/**
 * \file ShowerRecoAlgModular.h
 *
 * \ingroup Algo
 * 
 * \brief Class def header for a class ShowerRecoAlgModular
 *
 * @author cadams
 */

/** \addtogroup Algo

    @{*/
#ifndef SHOWERRECOALGMODULAR_H
#define SHOWERRECOALGMODULAR_H

#include <iostream>
#include "ShowerReco3D/Base/ShowerRecoAlgBase.h"
#include "ShowerRecoModuleBase.h"


/**
   \class ShowerRecoAlgModular
   User defined class ShowerRecoAlgModular ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {


  class ShowerRecoAlgModular : public ShowerRecoAlgBase {

  public:

    /// Default constructor
    ShowerRecoAlgModular(){}

    /// Default destructor
    ~ShowerRecoAlgModular(){}


    /// Function to reset algorithm, to be called @ beginning of each event
    void Reset() { ShowerRecoAlgBase::Reset(); }


    /// Function to reconstruct a shower
    Shower_t RecoOneShower(const ShowerClusterSet_t& clusters);

    /**
     * @brief Add a module to the list of modules that run shower reconstruction
     * @details The function RecoOneShower will run all of the modules in order and eventually return
     * the completed shower.  Modules will run *in the order that they are added* and can be removed by either knowing
     * the index in the list or the name of the module.  Note that ShowerRecoModuleBase has a "name" member and that is what
     * is used for keeping track of the name.
     * 
     * @param module The module to be added, must at least inherit from ShowerRecoModuleBase
     */
    void AddShowerRecoModule(ShowerRecoModuleBase * module);

    /**
     * @brief Replace a module from the list of modules
     * @details The reason for having module removal ability is to allow users to easily override a module. 
     * Instead of having to know the whole process of shower reconstruction, they can just take the default 
     * setup, replace a module with their own version and move forward.
     * 
     * @param module The module to be added, must at least inherit from ShowerRecoModuleBase
     * @param int The index of the module to be replaced
     */
    void ReplaceShowerRecoModule(ShowerRecoModuleBase * module, unsigned int index);

    /**
     * @brief Replace a module from the list of modules
     * @details The reason for having module removal ability is to allow users to easily override a module. 
     * Instead of having to know the whole process of shower reconstruction, they can just take the default 
     * setup, replace a module with their own version and move forward.
     * 
     * @param module The module to be added, must at least inherit from ShowerRecoModuleBase
     * @param int The name of the module to be replaced
     */
    void ReplaceShowerRecoModule(ShowerRecoModuleBase * module, std::string name);

  private:

    std::vector<ShowerRecoModuleBase * > _modules;


  };

} // showerreco

#endif
/** @} */ // end of doxygen group 

