/**
 * \file RecoEmulatorFactory.h
 *
 * \ingroup RecoEmulator
 * 
 * \brief Class def header for a class RecoEmulatorFactory
 *
 * @author kazuhiro
 */

/** \addtogroup RecoEmulator

    @{*/
#ifndef RECOEMULATORFACTORY_H
#define RECOEMULATORFACTORY_H

#include <iostream>
#include <map>

namespace recoemu {

  class RecoEmulatorBase;

  /**
     \class RecoEmulatorFactoryBase
     \brief Abstract base class for factory (to be implemented per algorithm)
   */
  class RecoEmulatorFactoryBase {
  public:
    /// Default ctor
    RecoEmulatorFactoryBase(){}
    /// Default dtor (virtual)
    virtual ~RecoEmulatorFactoryBase(){}
    /// Abstract constructor method
    virtual RecoEmulatorBase* create() = 0;
  };

  /**
     \class RecoEmulatorFactory
     \brief Factory class that can generate appropriate reco emulation algorithm class instance for a given name key
     Factory class that can generate appropriate reco emulation algorithm class instance for a given name key.\n
     Registration of each algorithm is needed per such algorithm class.
  */
  class RecoEmulatorFactory {

  public:
    /// Default ctor, shouldn't be used
    RecoEmulatorFactory() {}

    /// Default dtor
    ~RecoEmulatorFactory() {_factory_map.clear();}
    /// Static sharable instance getter
    static RecoEmulatorFactory& get()
    { if(!_me) _me = new RecoEmulatorFactory; return *_me; }
    /// Factory registration method (should be called by global factory instance in algorithm header)
    void add_factory(const std::string name, recoemu::RecoEmulatorFactoryBase* factory)
    { _factory_map[name] = factory; }
    /// Factory creation method (should be called by clients, possibly you!)
    RecoEmulatorBase* create(const std::string name) {
      auto iter = _factory_map.find(name);
      if(iter == _factory_map.end() || !((*iter).second)) {
	std::cerr << "\033[93mFound no registered class\033[00m" << name << std::endl;
	return nullptr;
      }
      return (*iter).second->create();
    }
  private:
    /// Static factory container
    std::map<std::string,recoemu::RecoEmulatorFactoryBase*> _factory_map;
    /// Static self
    static RecoEmulatorFactory* _me;
  };
}
#endif
/** @} */ // end of doxygen group 

