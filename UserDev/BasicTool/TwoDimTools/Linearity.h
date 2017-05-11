/**
 * \file Linearity.h
 *
 * \ingroup Clusterer
 * 
 * \brief Class def header for a class Linearity
 *
 * @author david caratelli
 */

/** \addtogroup Clusterer

    @{*/
#ifndef TWODIM_LINEARITY_H
#define TWODIM_LINEARITY_H

#include <iostream>
#include <vector>
#include <cmath>
#include "TMath.h"

namespace twodimtools {
  
  /**
     \class Linearity
     User defined class Linearity ... these comments are used to generate
     doxygen documentation!
  */
  class Linearity{
    
  public:
    
    /// Default constructor
    Linearity();
    
    Linearity(const std::vector<double>& x_v, const std::vector<double>& y_v);
    
    /// Default destructor
    ~Linearity(){}

    void add_point(double x, double y) { _x_v.push_back(x); _y_v.push_back(y); }
    
    /// covariance, standard deviation, mean
    double cov (const std::vector<double>& data1,
		const std::vector<double>& data2,
		bool save=false);
  
    double stdev(const std::vector<double>& data);
    
    double mean (const std::vector<double>& data);
    
    double linearity(const std::vector<double>& data1,
		     const std::vector<double>& data2,
		     bool save=false);

    double linearity() { return linearity(_x_v,_y_v,true); }
    
    void local_linearity(const std::vector<double>& data1,
			 const std::vector<double>& data2);

    void local_linearity() { return local_linearity(_x_v,_y_v); }
    
    // local linearity radius
    double _r;
    
    double _slope;
    double _slope_err;
    double _intercept;
    double _covxx;
    double _covyy;
    double _covxy;
    double _pearson;
    double _lin;
    double _meanx;
    double _meany;
    double _stdx;
    double _stdy;
    int    _dof;
    std::vector<double> _lin_v; // from local linearity
    double _local_lin_avg;
    double _local_lin_truncated_avg;
    double _summed_square_variance;

    std::vector<double> _x_v;
    std::vector<double> _y_v;

  };

}

#endif
/** @} */ // end of doxygen group 

