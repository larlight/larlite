/**
 * \file ClusterInfo.h
 *
 * \ingroup ModularAlgo
 * 
 * \brief Class def header for a class ClusterInfo
 *
 * @author jhewes
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef CLUSTERINFO_H
#define CLUSTERINFO_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

  class ClusterInfo : ShowerRecoModuleBase{

  public:

    /// Default constructor
    ClusterInfo(){_name = "ClusterInfo";}

    /// Default destructor
    ~ClusterInfo(){}


    void do_reconstruction(const ShowerClusterSet_t &, Shower_t &);

  private:
    
    void initialize();

  };

} // showerreco

#endif
/** @} */ // end of doxygen group 

