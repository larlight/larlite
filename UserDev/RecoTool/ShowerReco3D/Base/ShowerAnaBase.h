/**
 * \file ShowerAnaBase.h
 *
 * \ingroup Base
 * 
 * \brief Class def header for a class ShowerAnaBase
 *
 * @author kazuhiro
 */

/** \addtogroup Base

    @{*/
#ifndef SHOWERANABASE_H
#define SHOWERANABASE_H

#include <iostream>
#include "ShowerRecoTypes.h"
#include "TTree.h"


namespace showerreco {
  /**
     \class ShowerAna
     User defined class ShowerAna ... these comments are used to generate
     doxygen documentation!
  */
  class ShowerAnaBase {
    
  public:
    
    /// Default constructor
    ShowerAnaBase();
    
    /// Default destructor
    virtual ~ShowerAnaBase(){}

    virtual void Analyze( const Shower_t& shower,
			  const ShowerClusterSet_t& clusters ) = 0;

    TTree* GetTree();

  protected:

    TTree *_tree;
    
  };
}

#endif
/** @} */ // end of doxygen group 

