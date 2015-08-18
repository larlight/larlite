/**
 * \file FillPolygon.h
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class FillPolygon
 *
 * @author david caratelli
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef FILLPOLYGON_H
#define FILLPOLYGON_H

#include <iostream>
#include "ParamsAlgBase.h"

namespace cluster {

  /**
     \class FillPolygon
     User defined class FillPolygon ... these comments are used to generate
     doxygen documentation!
   */
  class FillPolygon : public ParamsAlgBase {

  public:

    /// Default constructor
    FillPolygon(){_name = "FillPolygon"; _frac = 0.95;}

    /// Default destructor
    ~FillPolygon(){}

    void do_params_fill(cluster_params &);

    /**
     * @brief Set fraction of charge that needs to be enclosed by polygon
     */
    void setFrac(const double f) { _frac = f; }

  private:

    /// Fraction of cluster charge that needs to be included in the polygon
    double _frac;
    
  };

} // cluster


#endif
/** @} */ // end of doxygen group 

