/**
 * \file PSet.h
 *
 * \ingroup FhiclLite
 * 
 * \brief Class def header for a class PSet
 *
 * @author kazuhiro
 */

/** \addtogroup FhiclLite

    @{*/
#ifndef BASICTOOL_FHICLLITE_PSET_H
#define BASICTOOL_FHICLLITE_PSET_H

#include <iostream>
#include <string>
#include <map>
#include <TString.h>
#include "FhiclLiteException.h"
#include "UtilFunc.h"
namespace fclite {
  /**
     \class PSet
     User defined class PSet ... these comments are used to generate
     doxygen documentation!
  */
  class PSet {
    
  public:
    
    /// Default constructor
    PSet(const std::string name="",
	 const std::string data="");

    /// Default destructor
    virtual ~PSet(){};

    /// Copy ctor
    PSet(const PSet& orig) : _name       ( orig._name       )
			   , _data_value ( orig._data_value )
			   , _data_pset  ( orig._data_pset  )
    {}

    /// clear method
    void clear() 
    { _data_value.clear(); _data_pset.clear(); }

    /// Set data contents
    void add(const std::string& data);

    /// Insert method for a simple param
    void add_value(std::string key,
		   std::string value);
    
    /// Dump into a text format
    std::string dump(size_t indent_size=0) const;

    /// Template getter
    template <class T>
    T get(const std::string& key) const{
      auto iter = _data_value.find(key);
      if( iter == _data_value.end() ) {
	std::string msg;
	msg = "Key does not exist: \"" + key + "\"";
	throw FhiclLiteException(msg);
      }
      return FromString<T>((*iter).second);
    }

    const PSet& get_pset(const std::string& key) const;

    const std::vector<std::string> value_keys () const;
    const std::vector<std::string> pset_keys  () const;
    bool  contains_value (const std::string& key) const;
    bool  contains_pset  (const std::string& key) const;

  protected:

    enum KeyChar_t {
      kParamDef,
      kBlockStart,
      kBlockEnd,
      kNone
    };

    /// Insert method for a PSet rep
    void add_pset(std::string key,
		  std::string pset);

    std::pair<PSet::KeyChar_t,size_t> search(const std::string& txt, const size_t start) const;
    void strip(std::string& str, const std::string& key);
    void rstrip(std::string& str, const std::string& key);
    void no_space(std::string& txt);

    std::string _name;

    std::map<std::string,std::string> _data_value;
    std::map<std::string,::fclite::PSet> _data_pset;

  };

  template std::string PSet::get(const std::string& key) const;
  template float       PSet::get(const std::string& key) const;
  template double      PSet::get(const std::string& key) const;
  template short       PSet::get(const std::string& key) const;
  template int         PSet::get(const std::string& key) const;
  template long        PSet::get(const std::string& key) const;
  template unsigned short PSet::get(const std::string& key) const;
  template unsigned int   PSet::get(const std::string& key) const;
  template unsigned long  PSet::get(const std::string& key) const;

}

#endif
/** @} */ // end of doxygen group 

