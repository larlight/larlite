/**
 * \file Provenance.h
 *
 * \ingroup Base
 * 
 * \brief Class def header for a class Provenance
 *
 * @author kazuhiro
 */

/** \addtogroup Base

    @{*/
#ifndef ERTOOL_PROVENANCE_H
#define ERTOOL_PROVENANCE_H

#include <iostream>
#include <map>
#include <string>
namespace ertool {
  /**
     \class Provenance
     User defined class Provenance ... these comments are used to generate
     doxygen documentation!
  */
  class Provenance{

  public:
    typedef std::map<std::string,std::string> ProcInfo_t;
    
  public:
    
    /// Default constructor
    Provenance(const std::string& name="noname");
    
    /// Default destructor
    ~Provenance(){}

    void SetInput(const Provenance& input);

    void AddInfo(const std::string& key,
		 const std::string& value);

    const ProcInfo_t& InputInfo() const
    { return _input_info; }

    const ProcInfo_t& Info() const
    { return _info; }

    const std::string& ProcessName() const
    { return _proc_name; }

    const size_t ProcessID() const
    { return _proc_id; }
    
  private:

    size_t      _proc_id;
    std::string _proc_name;
    ProcInfo_t  _info;
    
    std::string _input_name;
    ProcInfo_t  _input_info;
  };
}

#endif
/** @} */ // end of doxygen group 

