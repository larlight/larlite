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
#include <vector>
#include "ERToolConstants.h"
namespace ertool {
  /**
     \class Provenance
     User defined class Provenance ... these comments are used to generate
     doxygen documentation!
  */
  class Provenance{
    
  public:
    
    /// Default constructor
    Provenance(const std::string& name="noname");

    Provenance(const Provenance& input, const std::string& name="noname");
    
    /// Default destructor
    ~Provenance(){}

    void AddInfo(const std::string& key,
		 const std::string& value);

    void SetInfo(const std::string& key,
		 const std::string& value);

    bool HasInfo(const std::string& key) const;

    const ertool::ProcInfo_t& Info(const size_t proc_id=kINVALID_SIZE) const;

    const std::string& Name(const size_t proc_id=kINVALID_SIZE) const;

    const size_t ProcessID() const
    { return (_proc_name_v.size()-1); }
    
  private:
    std::vector<std::string> _proc_name_v;
    std::vector<ertool::ProcInfo_t>  _proc_info_v;
  };
}

#endif
/** @} */ // end of doxygen group 

