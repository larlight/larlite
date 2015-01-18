/**
 * \file RangeVar.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class RangeVar
 *
 * @author kazuhiro
 */

/** \addtogroup ERTool

    @{*/
#ifndef ERTOOL_RANGEVAR_H
#define ERTOOL_RANGEVAR_H

#include <string>
#include "ERException.h"
namespace ertool {
  /**
     \class RangeVar
     User defined class ertool::RangeVar ... these comments are used to generate
     doxygen documentation!
  */
  class RangeVar {
    
  public:
    
    /// Default constructor
    RangeVar(std::string name="",
	     double _value=0,
	     double _min=0,
	     double _max=0);
    
    /// Default destructor
    virtual ~RangeVar(){}

    bool Within(double v) const { return (_min <= v && v <= _max); }
    
    void Name  ( const std::string& name) { _name  = name; }
    void Value ( const double& v)         { _value = v;    }
    void Min   ( const double& v)         { _min   = v;    }
    void Max   ( const double& v)         { _max    = v;   }
    void Set   ( const double& vlaue,
		 const double& min,
		 const double& max );
    void Set   ( const double& min, const double& max);

    const std::string& Name()  const { return _name;  }
    const double&      Value() const { return _value; }
    const double&      Min()   const { return _min;   }
    const double&      Max()   const { return _max;   }

  protected:

    std::string _name;
    double _value;
    double _min;
    double _max;
  };
}

#endif
/** @} */ // end of doxygen group 

