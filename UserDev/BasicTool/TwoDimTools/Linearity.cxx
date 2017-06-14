#ifndef TWODIM_LINEARITY_CXX
#define TWODIM_LINEARITY_CXX

#include "Linearity.h"
#include <algorithm>

namespace twodimtools {
  
  Linearity::Linearity()
  {
    _x_v.clear();
    _y_v.clear();
    _r = 3.; // cm
  }
  
  Linearity::Linearity(const std::vector<double>& x_v, const std::vector<double>& y_v)
    :  Linearity()
  {

    _x_v = x_v;
    _y_v = y_v;
    
    linearity(x_v,y_v,true);
    
    local_linearity(x_v,y_v);
    
  }
  
  // covariance
  double Linearity::cov (const std::vector<double>& datax,
			 const std::vector<double>& datay,
			 bool save)
  {
    if(datax.size() == 0) { std::cout << "zero-vector!" << std::endl; return 0; }
    if(datay.size() == 0) { std::cout << "zero-vector!" << std::endl; return 0; }
    
    double result = 0.0;
    
    auto meanx = mean(datax);
    auto meany = mean(datay);
    
    if (save){
      _meanx = meanx;
      _meany = meany;
    }
    
    for(size_t i = 0; i < datax.size(); ++i)
      result += (datax[i] - meanx)*(datay[i] - meany);
    
    return result/((double)datax.size());
    
  }
  
  double Linearity::stdev(const std::vector<double>& data)
  {
    if(data.size() == 0) { std::cout << "zero-vector!" << std::endl; return 0; }
    
    double result = 0.0;
    auto    avg   = mean(data);
    for(const auto& d: data)
      result += (d - avg)*(d - avg);
    
    return sqrt(result/((double)data.size()));
  }
  
  double Linearity::mean(const std::vector<double>& data)
  {
    if(data.size() == 0) { std::cout << "zero-vector!" << std::endl; return 0; }
    
    double result = 0.0;
    
    for(const auto& d : data) 
      result += d;
    
    return (result / ((double)data.size()));
  }
  
  double Linearity::linearity(const std::vector<double>& datax,
			      const std::vector<double>& datay,
			      bool save)
  {
    
    if (datax.size() < 2)
      return 1.;
    
    auto covxy = cov(datax,datay,save);
    auto covxx = cov(datax,datax,save);
    auto covyy = cov(datay,datay,save);
    
    if (save){
      _covxy = covxy;
      _covxx = covxx;
      _covyy = covyy;
    }
    
    double r_num = covxy;
    double r_den = sqrt( covxx * covyy );
    double r = 0.;
    
    if (r_den == 0)
      r = 0.;
    else
      r = r_num / r_den;
    if (r > 1.) r = 1.;
    if (r < -1) r = -1;

    if (save)
    _pearson = r;
    
    auto dof = datax.size() - 2;
    if (save)
      _dof = dof;
    
    auto slope = r_num / covxx;
    if (save)
      _slope = slope;
    
    auto lin = sqrt( (1-r*r) * covyy / covxx / dof );
    if (save)
      _lin = lin;
    
    auto intercept = mean(datay) - slope * mean(datax);
    if (save)
      _intercept = intercept;
    
    // calculate sum of squared
    // factor to offset vertical square disance
    // instad of perpendicular one
    // also calculate uncertainty on slope (from: https://en.wikipedia.org/wiki/Simple_linear_regression)
    double f = cos( atan( slope ) );
    double sq = 0.;
    double slope_err = 0.;
    double ressum = 0.;
    double xvarsum = 0.;
    double xavg = 0.;
    
    for (auto const& x : datax) xavg += x;
    xavg /= datax.size();
    
    for (size_t i=0; i < datax.size(); i++){
      auto x = datax[i];
      auto y = datay[i];
      double res = y - (intercept + slope * x);
      xvarsum += (x-xavg)*(x-xavg);
      ressum += res*res;
      auto sqval = res * res * f * f;
      sq += sqval;
    }
    slope_err = sqrt( 1 / (datax.size() - 2.) * ressum / xvarsum );
    slope_err *= ::TMath::StudentQuantile(0.95,datax.size());
    if (save){
      _summed_square_variance = sqrt( sq / dof );
      _slope_err = slope_err;
    }

    /*
    // truncate SSV vector
    //std::sort( _sq_v.begin(), _sq_v.end() );
    _ssv_truncated = 0;
    size_t n = _sq_v.size();
    int ctr = 0;
    for (size_t i = (size_t)(n * 0.5); i < _sq_v.size(); i++){
      std::cout << "val = " << _sq_v[i] << std::endl;
      _ssv_truncated += _sq_v[i];
      ctr += 1;
    }
    _ssv_truncated = sqrt( _ssv_truncated / ctr );
    */

    return sqrt( sq / dof );
  }
  
  
  void Linearity::local_linearity(const std::vector<double>& data1,
				  const std::vector<double>& data2)
  {
    
    _lin_v.clear();
    
    _local_lin_avg = 0;
    
    for (size_t i=0; i < data1.size(); i++){
      
      std::vector<double> local_x_v;
      std::vector<double> local_y_v;
      
      double x = data1[i];
      double y = data2[i];
      
      for (size_t j=0; j < data1.size(); j++){
	
	if (i == j) continue;
	
	double d = (data2[j] - y) * (data2[j] - y) + (data1[j] - x) * (data1[j] - x);
	
	if (d < _r*_r){
	  local_x_v.push_back( data1[j] );
	  local_y_v.push_back( data2[j] );
	}
	
      }// second loop through hits
      
      if (local_x_v.size() < 4)
	continue;
      
      auto lin = linearity(local_x_v,local_y_v,false);
      
      _lin_v.push_back( lin );
      
      _local_lin_avg += lin;
      
    }// 1st loop through hits
    
    _local_lin_avg /= _lin_v.size();
    
    std::sort( _lin_v.begin(), _lin_v.end() );
    
    // truncate first and last quarters
    _local_lin_truncated_avg = 0;
    size_t n = _lin_v.size();
    int ctr = 0;
    for (size_t i = (size_t)(n * 0.25); i < (size_t)(n * 0.75); i++){
      if ( std::isnan(_lin_v[i]) ) continue;
      _local_lin_truncated_avg += _lin_v[i];
      ctr += 1;
    }

    _local_lin_truncated_avg /= ctr;

    if (ctr == 0)
      _local_lin_truncated_avg = 1.0;

    return;
  }

}// namespace

#endif
