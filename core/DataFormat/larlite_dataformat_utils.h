/**
 * \file larlite_dataformat_utils.h
 *
 * \ingroup DataFormat
 * 
 * \brief DataFormat utility typedefs
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/
#ifndef LARLITE_DATAFORMAT_UTILS_H
#define LARLITE_DATAFORMAT_UTILS_H

#include <vector>
#include <map>

#include "Base/DataFormatConstants.h"

namespace larlite{

  /// Represents one => X (1 or many) association in terms of vector index number
  typedef std::vector<unsigned int> AssUnit_t;
  /// Represents a set of AssUnit_t ... association @ event_X data product level
  typedef std::vector<AssUnit_t > AssSet_t;

  /**
     \class product_id 
     A very simple std::pair that has (a) sorting feature and (b) simple constrution
     w/o invoking template spcialization for easy usage in python
   */
  class product_id : public std::pair<larlite::data::DataType_t,std::string> {
  public:

    /// default ctor
    product_id()
    { this->first=data::kUndefined; this->second="noname"; }

    /// alternative ctor
    product_id(data::DataType_t const type,
	       std::string const name)
    { this->first=type; this->second=name; }

    /// virtual dtor
    virtual ~product_id(){}

    /// For sorting
    inline bool operator< ( const product_id& rhs ) const 
    { 
      if(this->first < rhs.first) return true;
      else if(this->first > rhs.first) return false;
      else if(this->second < rhs.second) return true;
      return false;
    }
  };

  /// Association data type meant to be stored within event_X data product
  typedef std::map<larlite::product_id,larlite::AssSet_t> AssMap_t;

}
#endif
/** @} */ // end of doxygen group 
