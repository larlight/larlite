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

namespace fclite {

  /**
     \class ConfigManager
     User defined class ConfigManager ... these comments are used to generate
     doxygen documentation!
  */
  class ConfigManager{
    
  public:
    
    /// Default constructor
    ConfigManager();
    
    /// Default destructor
    virtual ~ConfigManager(){};
    
    void AddCfgFile(std::string fname);
    
    const fclite::PSet& Config() const { return _data; }
    
    const fclite::PSet& SubConfig(const std::string& key)
    { return _data.get_pset(key); }
    
  protected:
    
    fclite::PSet _data;
  };
}
#endif
/** @} */ // end of doxygen group 

