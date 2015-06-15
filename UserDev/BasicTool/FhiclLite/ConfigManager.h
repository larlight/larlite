/**
 * \file ConfigManager.h
 *
 * \ingroup FhiclLite
 * 
 * \brief Class def header for a class ConfigManager
 *
 * @author kazuhiro
 */

/** \addtogroup FhiclLite

    @{*/
#ifndef BASICTOOL_FHICLLITE_CONFIGMANAGER_H
#define BASICTOOL_FHICLLITE_CONFIGMANAGER_H

#include "PSet.h"

namespace fcllite {

  /**
     \class ConfigManager
     User defined class ConfigManager ... these comments are used to generate
     doxygen documentation!
  */
  class ConfigManager{
    
  public:
    
    /// Default constructor
    ConfigManager(const std::string& name="MainConfig");
    
    /// Default destructor
    virtual ~ConfigManager(){};

    void Reset();
    
    void AddCfgFile(std::string fname);
    
    const fcllite::PSet& Config() const { return _data; }
    
    const fcllite::PSet& SubConfig(const std::string& key)
    { return _data.get_pset(key); }
    
  protected:
    
    fcllite::PSet _data;
  };
}
#endif
/** @} */ // end of doxygen group 

