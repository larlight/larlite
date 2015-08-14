/**
 * \file ParamsAlgBase.h
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class ParamsAlgBase
 *
 * @author cadams
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef PARAMSALGBASE_H
#define PARAMSALGBASE_H

#include <iostream>
#include <string>
#include "ClusterParams.h"

namespace cluster {


  /**
     \class ParamsAlgBase
     User defined class ParamsAlgBase ... these comments are used to generate
     doxygen documentation!
   */
  class ParamsAlgBase{

  public:

    /// Default constructor
    ParamsAlgBase(){}

    /// Default destructor
    virtual ~ParamsAlgBase(){}

    /**
     * @brief get the name of this module, used in helping organize order of modules and insertion/removal
     * @return name
     */
    std::string name(){return _name;}

    virtual void do_params_fill(cluster_params &, bool verbose) = 0;

  protected:
    std::string _name;

  };

} // cluster

#endif
/** @} */ // end of doxygen group 

