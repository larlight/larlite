/**
 * \file FunctionFactory.h
 *
 * \ingroup RecoEmulator
 * 
 * \brief Class def header for a class FunctionFactory
 *
 * @author david caratelli
 */

/** \addtogroup RecoEmulator

    @{*/
#ifndef FUNCTIONFACTORY_H
#define FUNCTIONFACTORY_H

#include <iostream>
#include <vector>
#include <map>

/**
   \class Function
   * @brief class to define individual functions
*/
class Function{

 public:

  Function(){};

  ~Function(){}

  Function(std::string name=""){_name = name;}

  double Call() { return 0; }

  std::string Name() { return _name; }

 private:

  std::string _name;

};

/**
   \class FunctionFactory
   * @brief class to hold user-defined functions to be used for random drawing
 */
class FunctionFactory{

public:

  /// Default constructor
  FunctionFactory(){}

  /// Default destructor
  ~FunctionFactory(){}

  /**
   * @brief retrieve a specific function from the factory
   */
  Function* RetrieveFunction(std::string name) {
    auto iter = _function_map.find(name);
    if (iter == _function_map.end() || !((*iter).second) ){
      std::cerr << "\033[93m Found no registered function \033[00m" << name << std::endl;
      return nullptr;
    }
    return (*iter).second;
  }

 private:
  
  // list of functions stored as a map
  std::map<std::string, Function*> _function_map;

};

#endif
/** @} */ // end of doxygen group 

