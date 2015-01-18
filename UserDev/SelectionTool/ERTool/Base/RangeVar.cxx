#ifndef ERTOOL_RANGEVAR_CXX
#define ERTOOL_RANGEVAR_CXX

#include "RangeVar.h"
#include <TString.h>
namespace ertool {

  RangeVar::RangeVar(std::string name,
		     double value,
		     double min,
		     double max) 
    : _name("rvar")
    , _value(value)
    , _min(min)
    , _max(max)
  {
    if(!name.empty()) _name += "_" + name;
  }

  void RangeVar::Set( const double& value,
		      const double& min,
		      const double& max )
  {
    if(min<max && min<=value && value<=max) {
      _min = min;
      _max = max;
      _value = value;
    }else
      throw ERException(Form("Cannot set range %g=>%g with current value %g",
			     min,max,value));
  }

  void RangeVar::Set( const double& min, const double& max)
  {
    Set(_value,min,max);
  }
  
}

#endif
