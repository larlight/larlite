/**
 * \file ShowerRecoAlgModular.h
 *
 * \ingroup ModularAlgo
 * 
 * \brief Class def header for a class ShowerRecoAlgModular
 *
 * @author cadams
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef SHOWERRECOALGMODULAR_H
#define SHOWERRECOALGMODULAR_H

#include <iostream>
#include "ShowerReco3D/Base/ShowerRecoAlgBase.h"
#include "ShowerRecoModuleBase.h"
#include "TTree.h"
#include "TFile.h"
#include <TStopwatch.h>

/**
   \class ShowerRecoAlgModular
   User defined class ShowerRecoAlgModular ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {


  class ShowerRecoAlgModular : public ShowerRecoAlgBase {

  public:

    /// Default constructor
    ShowerRecoAlgModular(){_debug = false;}

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
     * @brief Insert a module after the index specifed
     * @details Allows the user to insert a module at a specific index. If the index is not present before this function call, 
     * the module will not be added and an error is generated.
     * 
     * @param module The module to be added
     * @param index The current index after which the module will be inserted
     */
    void InsertShowerRecoModule(ShowerRecoModuleBase * module, unsigned int index);

    /**
     * @brief Insert a module after a specified name
     * @details Allows the user to insert a module after a module of the name specified. If the name is not present before this function call, 
     * the module will not be added and an error is generated.
     * 
     * @param module The module to be added.
     * @param name The name of another module, after which the target module will be inserted.
     */
    void InsertShowerRecoModule(ShowerRecoModuleBase * module, std::string name);

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

    /**
     * @brief Prints the module list
     * @details Prints out the module list in the order in which they will run, nicely formatted.
     */
    void PrintModuleList();

    /**
     * @brief Set the debug option
     * @details Debug mode prints the changes in the shower_t object after each module is called.
     * Modules are expected to have their own debug mode that is activated separately.
     * 
     * @param b true or false to turn on or off debug mode.  Default for the whole class is off, default for this function is on
     */
    void SetDebug(bool b = true){_debug = b;}

    /**
     * @brief function to finalize the algorithm -> basically used to write each modular algo's trees
     */
    void Finalize(TFile* fout);

    /**
     * @brief function to initialize the algorithm -> basically used to create the tree
     */
    void Initialize();

  private:

    void printChanges(const Shower_t & localCopy, const Shower_t result, std::string moduleName);

    std::vector<ShowerRecoModuleBase * > _modules;

    bool _debug;

    // Time profilers
    TStopwatch _watch; ///< For profiling
    std::vector<double> _module_time_v; ///< Overall time for processing
    std::vector<size_t> _module_ctr_v;  ///< Overall number of clusters processed by algo

  };

} // showerreco

#endif
/** @} */ // end of doxygen group 

