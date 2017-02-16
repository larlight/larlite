/**
 * \file Linearity.h
 *
 * \ingroup Clusterer
 * 
 * \brief Class def header for a class Linearity
 *
 * @author david
 */

/** \addtogroup Clusterer

    @{*/
#ifndef LINEARITY_H
#define LINEARITY_H

#include <iostream>
#include <vector>
#include <cmath>

namespace twodim {
  
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
    
    /// covariance, standard deviation, mean
    double cov (const std::vector<double>& data1,
		const std::vector<double>& data2,
		bool save=false);
  
    double stdev(const std::vector<double>& data);
    
    double mean (const std::vector<double>& data);
    
    double linearity(const std::vector<double>& data1,
		     const std::vector<double>& data2,
		     bool save=false);
    
    void local_linearity(const std::vector<double>& data1,
			 const std::vector<double>& data2);
    
    // local linearity radius
    double _r;
    
    double _slope;
    double _intercept;
    double _covxx;
    double _covyy;
    double _covxy;
    double _lin;
    double _meanx;
    double _meany;
    double _stdx;
    double _stdy;
    int    _dof;
    std::vector<double> _lin_v; // from local linearity
    double _local_lin_avg;
    double _local_lin_truncated_avg;
    
    
  };

}

#endif
/** @} */ // end of doxygen group 

